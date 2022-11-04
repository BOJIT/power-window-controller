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

/*---------------------------------- Macros ----------------------------------*/


#define osFlagsWaitAny   0x00000000U
#define osWaitForever   0xFFFFFFFFU

/*--------------------------------- Datatypes --------------------------------*/

// For sake of demo
typedef struct {
    uint32_t val1;
    uint32_t val2;
} StaticTask_t;

typedef struct {
    uint32_t val1;
    uint32_t val2;
} StaticEvent_t;

typedef uint32_t osPriority_t;

typedef struct {
    const char *name;
    void *cb_mem;
    uint32_t cb_size;
    void *stack_mem;
    uint32_t stack_size;
    osPriority_t priority;
} osThreadAttr_t;

typedef struct {
    const char *name;
    uint32_t attr_bits;
    void * cb_mem;
    uint32_t cb_size;
} osEventFlagsAttr_t;

typedef void* osThreadId_t;
typedef void* osEventFlagsId_t;

typedef void ( *osThreadFunc_t)(void *argument);

/*--------------------------------- Functions --------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void osThreadFlagsSet(osThreadId_t t, uint32_t flags);
osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr);
uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags);
uint32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);
void osDelay(uint32_t d);
osThreadId_t osThreadNew(osThreadFunc_t func, void *arg, const osThreadAttr_t *attr);
void osThreadExit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*----------------------------------------------------------------------------*/

#endif /* __SRC_TASK_CONCEPTS_CMSIS_OS_H__ */
