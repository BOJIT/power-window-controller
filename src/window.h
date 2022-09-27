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
        uint16_t m_consecutive_samples = 0;

        void Init(pinmap_t* pinmap);
        void DoState(void);

    private:
        /* Config */
        static constexpr uint32_t HOLD_START = 600;
        static constexpr uint32_t HOLD_STOP = 1400;
        static constexpr uint32_t SWITCH_DEBOUNCE = 100;
        static constexpr uint32_t CURRENT_THRESHOLD = 700;
        static constexpr uint32_t CURRENT_DEBOUNCE = 100;
        static constexpr uint32_t FAILURE_TIME = 5000;

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

        bool m_switch_lockout = false;
        bool m_auto_lockout = false;

        void ToState(state_t state);

        void Stop(void);
        void Open(void);
        void Close(void);

        state_t ReadSwitch(void);
        bool AtTravelLimit(void);
};

/*----------------------------------------------------------------------------*/

#endif /* __WINDOW_H__ */
