/**
 * @file Thread.h
 * @author James Bennion-Pedley
 * @brief CMSIS-OS C++ Abstraction
 * @note Full support for Static Allocation
 * @date 04/11/2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __THREAD_H__
#define __THREAD_H__

/*--------------------------------- Includes ---------------------------------*/

#include "cmsis_os.h"

/*------------------------------- Primary Class ------------------------------*/

namespace THREAD {

///< @note This class deliberately has no constructor.
class Thread
{
public:
    /**
     * @brief Add dependent task: The task here will not initialise until
     * all depdencies have initialised
     * @param t
     * @return true - if added successully
     * @return false - if MAX_DEPS has been exceeded!
     */
    bool AddDependency(THREAD::Thread *t)
    {
        if(m_deps_count < MAX_DEPS) {
            m_deps[m_deps_count] = t;
            m_deps_count++;
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief Poll if the task is ready (i.e. has initialised)
     * @note THREAD-SAFE!
     *
     * @return true
     */
    bool Ready(void)
    {
        uint32_t flags = osEventFlagsWait(
                                            this->m_evt_handle,
                                            READY_FLAG,
                                            osFlagsWaitAny,
                                            osWaitForever
                                        );
        return true;    ///< Cannot return if not ready
    }

    /**
     * @brief Register thread instance with scheduler.
     * This is typically done at initialisation.
     * @note THREAD-SAFE!
     *
     * @param name Name of Thread Instance
     * @param priority Priority of Thread Instance
     * @return osThreadId_t
     */
    osThreadId_t Register(const char *name, osPriority_t priority)
    {
        void *stack = NULL;
        uint32_t stack_size = this->GetStack(&stack);

        // Set params in Event Flag Attributes
        this->m_evt_attr.name = NULL;
        this->m_evt_attr.attr_bits = 0;
        this->m_evt_attr.cb_mem = &(this->m_evt_cb);
        this->m_evt_attr.cb_size = sizeof(this->m_evt_cb);

        // Set params in Thread Attributes
        this->m_attr.name = name;
        this->m_attr.cb_mem = &(this->m_cb);
        this->m_attr.cb_size = sizeof(this->m_cb);
        this->m_attr.stack_mem = NULL;
        this->m_attr.stack_size = stack_size;

        // Create RTOS objects
        this->m_handle = osThreadNew(THREAD::Thread::global_entry, (void *)this, &(this->m_attr));
        this->m_evt_handle = osEventFlagsNew(&(this->m_evt_attr));

        return this->m_handle;
    }

protected:
    osThreadId_t m_handle;  ///< Internal handle pointing to Thread ID

    /**
     * @brief Initialise Function. This is optional, but allows the dependency
     * of tasks on each other to be defined. Once Init() returns, the Thread
     * instance is marked as 'Ready', so a signal to dependent Threads will be sent.
     */
    void Init(void) {}

    /**
     * @brief Get Statically Allocated Stack. User implemented function.
     * If not overridden, stack will be allocated from heap.
     *
     * @param stack pointer to variable that contains stack pointer
     * @return uint32_t size of stack IN BYTES!
     */
    uint32_t GetStack(void **stack) { return 0; }

    /**
     * @brief Main Entry function. This must be provided in implementation!
     */
    virtual void Entry(void);

private:
    osThreadAttr_t m_attr;          ///< OS Thread Attributes. Must stay in scope!
    StaticTask_t m_cb;              ///< Internal static allocation of Control Block

    osEventFlagsAttr_t m_evt_attr;  ///< Event flag used for signalling 'readiness'
    StaticEvent_t m_evt_cb;         ///< Internal static allocation of Event Block
    osEventFlagsId_t m_evt_handle;  ///< Handle pointing to control block
    static constexpr uint32_t READY_FLAG = 0x01UL;   ///< Mask for Event Flags

    static constexpr size_t MAX_DEPS = 5;   ///< Max number of tasks to wait on (plenty)
    THREAD::Thread *m_deps[MAX_DEPS];       ///< Array of dependencies to check
    size_t m_deps_count = 0;                ///< Depth of array

    /**
     * @brief Iterate through table of added dependencies and wait on other
     * task event flags.
     */
    void CheckDependencies(void)
    {
        for(size_t i = 0; i < m_deps_count; i++)
        {
            if(m_deps[i] != NULL)
                m_deps[i]->Ready();
        }
    }

    /**
     * @brief global entry. This function is identical for EVERY task!
     * This function will then route to the corresponding init and entry functions
     *
     * @param ptr pointer to object (this)
     */
    static void global_entry(void *ptr)
    {
        THREAD::Thread *t = static_cast<THREAD::Thread*>(ptr);

        // Check our dependencies, init, then notify dependents
        t->CheckDependencies();
        t->Init();
        osEventFlagsSet(t->m_evt_handle, READY_FLAG);
        t->Entry();

        // Catch thread that hasn't properly been terminated
        osThreadExit();
    }
};

} /* namespace THREAD */

/*----------------------------------------------------------------------------*/

#endif /* __THREAD_H__ */
