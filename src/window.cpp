/**
 * @file window.cpp
 * @author James Bennion-Pedley (james@bojit.org)
 * @brief Core controller class
 * @version 0.1
 * @date 2022-04-02
 *
 * @copyright Copyright James Bennion-Pedley (c) 2022
 *
 */

/*-------------------------------- Includes ----------------------------------*/

#include "window.h"

#include <Arduino.h>

extern HardwareSerial debug;

/*----------------------------- Public Methods -------------------------------*/


/**
 * @brief Initialise power window controller
 *
 * @param pinmap Pin names for each window controller
 */
void WindowController::Init(pinmap_t* pinmap)
{
    m_pinmap = *pinmap;

    pinMode(m_pinmap.current, INPUT_ANALOG);

    pinMode(m_pinmap.fet_down, OUTPUT);
    pinMode(m_pinmap.fet_up, OUTPUT);

    pinMode(m_pinmap.btn_down, INPUT_PULLUP);
    pinMode(m_pinmap.btn_up, INPUT_PULLUP);

    ToState(STATE_STOPPED);
}


/**
 * @brief Run state machine
 */
void WindowController::DoState(void)
{
    state_t target_state = ReadSwitch();
    uint32_t t_now = millis();

    // Check if at end of travel
    if((t_now - m_state_ts > CURRENT_DEBOUNCE) && AtTravelLimit())
        ToState(STATE_STOPPED);

    // Change state, unless in auto mode (return to stop in window)
    if(!m_auto_lockout && (target_state != m_state)) {
        if(target_state == STATE_STOPPED) {
            if(((t_now - m_state_ts) > HOLD_START) && ((t_now - m_state_ts) < HOLD_STOP)) {
                m_auto_lockout = true;
                return;
            }
        }

        ToState(target_state);
    }

    // If have been moving for more than 10 seconds, assume failed current sensor
    if((m_state != STATE_STOPPED) && ((t_now - m_state_ts) > FAILURE_TIME))
        ToState(STATE_STOPPED);
}


/*---------------------------- Private Methods -------------------------------*/


/**
 * @brief Transition to new operation state
 *
 * @param state Target state
 */
void WindowController::ToState(state_t state)
{
    m_state = state;

    switch(m_state) {
        case(STATE_STOPPED):
            m_switch_lockout = true;   // Switch must return to zero to change state
            m_auto_lockout = false;
            Stop();
            break;

        case(STATE_OPENING):
            Open();
            break;

        case(STATE_CLOSING):
            Close();
            break;
    }

    m_state_ts = millis();
}


/**
 * @brief Stop motor movement
 */
void WindowController::Stop(void)
{
    debug.printf("%p - STOP\n", this);
    digitalWrite(m_pinmap.fet_down, LOW);
    digitalWrite(m_pinmap.fet_up, LOW);
}


/**
 * @brief Set motor to open window
 */
void WindowController::Open(void)
{
    debug.printf("%p - OPEN\n", this);
    digitalWrite(m_pinmap.fet_down, HIGH);
    digitalWrite(m_pinmap.fet_up, LOW);
}


/**
 * @brief Set motor to close window
 */
void WindowController::Close(void)
{
    debug.printf("%p - CLOSE\n", this);
    digitalWrite(m_pinmap.fet_down, LOW);
    digitalWrite(m_pinmap.fet_up, HIGH);
}


/**
 * @brief Read state of switch: with built-in debouncing
 *
 * @return WindowController::state_t
 */
WindowController::state_t WindowController::ReadSwitch(void)
{
    state_t target_state = STATE_STOPPED;
    uint32_t ts_now = millis();

    if(digitalRead(m_pinmap.btn_down) == LOW)
        target_state = STATE_OPENING;
    else if(digitalRead(m_pinmap.btn_up) == LOW)
        target_state = STATE_CLOSING;
    else
        m_switch_lockout = false;

    // Lockout means that the switch-state has to return to stopped to clear
    if(m_switch_lockout)
        return STATE_STOPPED;

    if((m_switch_prev_state != target_state) && (ts_now - m_switch_ts) > SWITCH_DEBOUNCE) {
        m_switch_prev_state = target_state;
        m_switch_ts = ts_now;
    }

    return m_switch_prev_state;
}


/**
 * @brief Read current pin, translate into positive current, check if threshold
 * has been reached.
 *
 * @return true if limit reached
 * @return false if limit not reached
 */
bool WindowController::AtTravelLimit(void)
{
    m_current = analogRead(m_pinmap.current);

    if(m_current == 1023)
        m_max_count++;
    else
        m_max_count = 0;

    // If ADC maxxed out for more than 3 samples, cut power
    if(m_max_count > 3)
        return true;
    else
        return false;
}
