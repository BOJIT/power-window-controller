/**
 * @file cmsis_os.h
 * @author James Bennion-Pedley
 * @brief Brief summary here
 * @date 04/11/2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SRC_TASK_CONCEPTS_CMSIS_OS_H__
#define __SRC_TASK_CONCEPTS_CMSIS_OS_H__

/*--------------------------------- Includes ---------------------------------*/

#include <stdint.h>
#include <stddef.h>

/*--------------------------------- Datatypes --------------------------------*/

// For sake of demo
typedef struct {
    uint32_t val1;
    uint32_t val2;
} osThreadAttr_t;
typedef void* osThreadId_t;
typedef uint32_t osThreadPriority_t;

/*--------------------------------- Functions --------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

osThreadId_t osRegisterThread(osThreadAttr_t *t, void(*fn)(void *));
void osSetFlag(osThreadId_t t, uint32_t flag, uint32_t timeout);
void osDelay(uint32_t d);
void osThreadTerminate(osThreadId_t t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*----------------------------------------------------------------------------*/

#endif /* __SRC_TASK_CONCEPTS_CMSIS_OS_H__ */
