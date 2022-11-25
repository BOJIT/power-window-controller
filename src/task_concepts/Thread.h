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

/*-------------------------------- Base Thread -------------------------------*/

namespace OS {

class BaseThread
{
   public:
    bool AddDependency(OS::BaseThread *t);
    bool Ready(void);

    virtual osThreadId_t Register(const char *name, osPriority_t priority);

   protected:
    osThreadId_t m_handle;  ///< Internal handle pointing to Thread ID
    virtual void Init(void);
    virtual void Entry(void);

    osThreadAttr_t m_attr;          ///< OS Thread Attributes. Must stay in scope!
    StaticTask_t m_cb;              ///< Internal static allocation of Control Block

    osEventFlagsAttr_t m_evt_attr;  ///< Event flag used for signalling 'readiness'
    StaticEvent_t m_evt_cb;         ///< Internal static allocation of Event Block
    osEventFlagsId_t m_evt_handle;  ///< Handle pointing to control block
    static constexpr uint32_t READY_FLAG = 0x01UL;   ///< Mask for Event Flags

    static constexpr size_t MAX_DEPS = 5;   ///< Max number of tasks to wait on (plenty)
    OS::BaseThread *m_deps[MAX_DEPS];       ///< Array of dependencies to check

    size_t m_deps_count = 0;                ///< Depth of array

    void CheckDependencies(void);
    static void global_entry(void *ptr);
};

/*------------------------------ Thread Template -----------------------------*/

template<size_t stack>
class Thread: public BaseThread
{
   public:
    osThreadId_t Register(const char *name, osPriority_t priority)
    {
        // Set params in Event Flag Attributes
        this->m_evt_attr.name = NULL;
        this->m_evt_attr.attr_bits = 0;
        this->m_evt_attr.cb_mem = &(this->m_evt_cb);
        this->m_evt_attr.cb_size = sizeof(this->m_evt_cb);

        // Set params in Thread Attributes
        this->m_attr.name = name;
        this->m_attr.cb_mem = &(this->m_cb);
        this->m_attr.cb_size = sizeof(this->m_cb);
        this->m_attr.stack_mem = m_stack;
        this->m_attr.stack_size = stack;

        // Create RTOS objects
        this->m_handle = osThreadNew(global_entry, (void *)this, &(this->m_attr));
        this->m_evt_handle = osEventFlagsNew(&(this->m_evt_attr));

        return this->m_handle;
    }

    void Init(void) {}

   private:
    uint8_t m_stack[stack];

    // Make protected members private to thread implementations
    using BaseThread::m_attr;
    using BaseThread::m_cb;
    using BaseThread::m_evt_attr;
    using BaseThread::m_evt_cb;
    using BaseThread::m_evt_handle;
    using BaseThread::READY_FLAG;
    using BaseThread::MAX_DEPS;
    using BaseThread::m_deps;
    using BaseThread::m_deps_count;
    using BaseThread::CheckDependencies;
    using BaseThread::global_entry;
};

} /* namespace OS */

/*----------------------------------------------------------------------------*/

#endif /* __THREAD_H__ */
