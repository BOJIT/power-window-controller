
#ifndef __THREAD_CPP__
#define __THREAD_CPP__

/*--------------------------------- Includes ---------------------------------*/

#include "cmsis_os.h"

/*------------------------------- Primary Class ------------------------------*/

namespace THREAD {

class Thread
{
public:
    ///< @note This class deliberately has no constructor.

    bool Ready(void)
    {
        // ATOMIC READ
        return this->m_ready;

        // TODO maybe implement this as a flag/semaphore, so it blocks
    }

    /**
     * @brief Set flags on this thread instance. Can be called by other
     * threads to control execution flow
     * @note THREAD-SAFE!
     *
     *
     * @param flag
     * @param timeout
     */
    void SetFlag(uint32_t flag, uint32_t timeout)
    {
        osSetFlag(this->m_handle, flag, timeout);
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

        // Set params in Thread Attributes
        this->m_attr.name = name;
        this->m_attr.cb_mem = &(this->m_cb);
        this->m_attr.cb_size = sizeof(this->m_cb);
        this->m_attr.stack_mem = NULL;
        this->m_attr.stack_size = stack_size;

        // Create thread and store handle
        this->m_handle = osThreadNew(THREAD::Thread::global_entry, (void *)this, &(this->m_attr));
        return this->m_handle;
    }

protected:
    osThreadAttr_t m_attr;  ///< OS Thread Attributes. Must stay in scope!
    osThreadId_t m_handle;  ///< Internal handle pointing to Thread ID
    StaticTask_t m_cb;      ///< Internal static allocation of Control Block

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
    /**
     * @brief Internal flag that is used for waiting for ready state
     */
    bool m_ready = false;

    /**
     * @brief global entry. This function is identical for EVERY task!
     * This function will then route to the corresponding init and entry functions
     *
     * @param ptr pointer to object (this)
     */
    static void global_entry(void *ptr)
    {
        THREAD::Thread *t = static_cast<THREAD::Thread*>(ptr);

        t->Init();
        t->m_ready = true;
        t->Entry();

        // Catch thread that hasn't properly been terminated
        osThreadExit();
    }
};

} /* namespace THREAD */

/*----------------------------------------------------------------------------*/

#endif /* __THREAD_CPP__ */
