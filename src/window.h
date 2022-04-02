/**
 * @file window.h
 * @author James Bennion-Pedley (james@bojit.org)
 * @brief Core controller class
 * @version 0.1
 * @date 2022-04-02
 *
 * @copyright Copyright James Bennion-Pedley (c) 2022
 *
 */

#ifndef __WINDOW_H__
#define __WINDOW_H__

/*-------------------------------- Includes ----------------------------------*/

#include <stdint.h>

/*------------------------------ Primary Class -------------------------------*/

class WindowController
{
    public:
        typedef struct {
            uint32_t current;
            uint32_t fet_down;
            uint32_t fet_up;
            uint32_t btn_down;
            uint32_t btn_up;
        } pinmap_t;

        uint16_t m_current;
        uint16_t m_stall_threshold;

        void Init(pinmap_t* pinmap, uint16_t stall_threshold);
        void DoState(void);

    private:
        /* Config */
        static constexpr uint32_t HOLD_START = 1000;
        static constexpr uint32_t HOLD_STOP = 2000;
        static constexpr uint32_t SWITCH_DEBOUNCE = 100;
        static constexpr uint32_t CURRENT_DEBOUNCE = 100;

        typedef enum {
            STATE_STOPPED,
            STATE_OPENING,
            STATE_CLOSING
        } state_t;

        pinmap_t m_pinmap;
        state_t m_state = STATE_STOPPED;
        uint32_t m_state_ts = 0;

        state_t m_switch_prev_state = STATE_STOPPED;
        uint32_t m_switch_ts = 0;

        uint32_t m_switch_lockout = false;
        uint32_t m_auto_lockout = false;

        void ToState(state_t state);

        void Stop(void);
        void Open(void);
        void Close(void);

        state_t ReadSwitch(void);
        bool AtTravelLimit(void);
};

/*----------------------------------------------------------------------------*/

#endif /* __WINDOW_H__ */
