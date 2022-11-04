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

/*---------------------------- Macros & Constants ----------------------------*/

/*----------------------------------- State ----------------------------------*/

/*------------------------------ Private Functions ---------------------------*/

/*------------------------------- Public Functions ---------------------------*/

osThreadId_t osRegisterThread(osThreadAttr_t *t, void(*fn)(void *))
{
    return NULL;
}

void osSetFlag(osThreadId_t t, uint32_t flag, uint32_t timeout)
{

}

void osDelay(uint32_t d)
{

}

void osThreadTerminate(osThreadId_t t)
{

}

/*----------------------------------------------------------------------------*/
