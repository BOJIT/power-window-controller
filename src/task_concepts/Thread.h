
#ifndef __THREAD_CPP__
#define __THREAD_CPP__

/*--------------------------------- Includes ---------------------------------*/

#include "cmsis_os.h"

/*------------------------------- Primary Class ------------------------------*/

namespace THREAD {

class Thread
{
public:
    // Note this class deliberately has no constructor

    bool Ready(void)
    {
        // ATOMIC READ
        return this->m_ready;

        // TODO maybe implement this as a flag/semaphore, so it blocks
    }

    void SetFlag(uint32_t flag, uint32_t timeout)
    {
        osSetFlag(this->m_handle, flag, timeout);
    }

    osThreadId_t Register(const char *name, osThreadPriority_t priority)
    {
        // TODO get pointer to thread stack and size

        // Set params in Thread Attributes
        this->m_cb.val1 = 1;
        this->m_cb.val2 = 1;
        this->m_handle = osRegisterThread(&(this->m_cb), THREAD::Thread::global_entry);

        return this->m_handle;
    }

protected:
    // Internal State
    osThreadAttr_t m_cb;
    osThreadId_t m_handle;

    void Init(void)
    {
        // Custom Init function can be added. This is optional
    }

    virtual void Entry(void);   // Entry function is mandatory!

private:
    bool m_ready = false;

    static void global_entry(void *ptr)
    {
        THREAD::Thread *t = static_cast<THREAD::Thread*>(ptr);

        t->Init();
        t->m_ready = true;
        t->Entry();

        // Catch thread that hasn't properly been terminated
        osThreadTerminate(t->m_handle);    // Or NULL?
    }
};

} /* namespace THREAD */

/*----------------------------------------------------------------------------*/

#endif /* __THREAD_CPP__ */
