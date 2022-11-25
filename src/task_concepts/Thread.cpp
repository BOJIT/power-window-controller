/**
 * @file Thread.cpp
 * @author James Bennion-Pedley
 * @brief CMSIS-OS C++ Abstraction
 * @note Full support for Static Allocation
 * @date 25/11/2022
 *
 * @copyright Copyright (c) 2022
 *
 */

/*--------------------------------- Includes ---------------------------------*/

#include "Thread.h"

/*------------------------------ Public Methods ------------------------------*/

bool OS::BaseThread::AddDependency(OS::BaseThread *t)
{
    if(m_deps_count < MAX_DEPS) {
        m_deps[m_deps_count] = t;
        m_deps_count++;
        return true;
    } else {
        return false;
    }
}

bool OS::BaseThread::Ready(void)
{
    uint32_t flags = osEventFlagsWait(
                                        this->m_evt_handle,
                                        READY_FLAG,
                                        osFlagsWaitAny,
                                        osWaitForever
                                    );
    return true;    ///< Cannot return if not ready
}

/*------------------------------ Private Methods -----------------------------*/

void OS::BaseThread::CheckDependencies(void)
{
    for(size_t i = 0; i < m_deps_count; i++)
    {
        if(m_deps[i] != NULL)
            m_deps[i]->Ready();
    }
}

void OS::BaseThread::global_entry(void *ptr)
{
    OS::BaseThread *t = static_cast<OS::BaseThread*>(ptr);

    // Check our dependencies, init, then notify dependents
    t->CheckDependencies();
    t->Init();
    osEventFlagsSet(t->m_evt_handle, READY_FLAG);
    t->Entry();

    // Catch thread that hasn't properly been terminated
    osThreadExit();
}
