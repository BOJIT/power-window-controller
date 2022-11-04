/**
 * @file cmsis_os.c
 * @author James Bennion-Pedley
 * @brief Brief summary here
 * @date 04/11/2022
 *
 * @copyright Copyright (c) 2022
 *
 */

/*--------------------------------- Includes ---------------------------------*/

#include "cmsis_os.h"

/*------------------------------- Public Functions ---------------------------*/

void osThreadFlagsSet(osThreadId_t t, uint32_t flags)
{

}

osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr)
{

}

uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags)
{

}

uint32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{

}

void osDelay(uint32_t d)
{

}

osThreadId_t osThreadNew(osThreadFunc_t func, void *arg, const osThreadAttr_t *attr)
{

}

void osThreadExit(void)
{

}

/*----------------------------------------------------------------------------*/
