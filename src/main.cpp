/**
 * @file main.cpp
 * @author James Bennion-Pedley (james@bojit.org)
 * @brief Controller for power windows using STM32 blue pill
 * @version 0.1
 * @date 2022-04-02
 *
 * @copyright Copyright James Bennion-Pedley (c) 2022
 *
 */

/*-------------------------------- Includes ----------------------------------*/

#include <Arduino.h>

#include "window.h"

/*--------------------------------- Macros -----------------------------------*/

#define LEFT_CURRENT            PA1
#define LEFT_FET_DOWN           PB10
#define LEFT_FET_UP             PB11
#define LEFT_BTN_DOWN           PA4
#define LEFT_BTN_UP             PA3

#define RIGHT_CURRENT           PA2
#define RIGHT_FET_DOWN          PB0
#define RIGHT_FET_UP            PB1
#define RIGHT_BTN_DOWN          PA6
#define RIGHT_BTN_UP            PA5

#define BAUD_RATE               115200
#define PROCESS_RATE            200

/*-------------------------------- Constants ---------------------------------*/

HardwareSerial debug(PB7, PB6);

static WindowController leftWindow;
static WindowController rightWindow;

/*------------------------------- Entry Point --------------------------------*/


void setup(void)
{
    debug.begin(BAUD_RATE);

    debug.println("Begin DEBUG");

    WindowController::pinmap_t l_pins = {
        LEFT_CURRENT,
        LEFT_FET_DOWN,
        LEFT_FET_UP,
        LEFT_BTN_DOWN,
        LEFT_BTN_UP
    };

    WindowController::pinmap_t r_pins = {
        RIGHT_CURRENT,
        RIGHT_FET_DOWN,
        RIGHT_FET_UP,
        RIGHT_BTN_DOWN,
        RIGHT_BTN_UP
    };

    leftWindow.Init(&l_pins, 500);
    rightWindow.Init(&r_pins, 500);
}


void loop(void)
{
    static uint32_t t_prev = 0;
    uint32_t t_now = millis();

    if((t_now - t_prev) > PROCESS_RATE) {
        leftWindow.DoState();
        rightWindow.DoState();

        t_prev = t_now;
    }
}
