#ifndef _LED_H
#define _LED_H

#include <Arduino.h>

class Led
{
    public:
        enum Mode {
            OFF,
            BLINK_SLOW,
            BLINK_FAST,
            ON
        };
        
        Led(uint8_t _pin);
        void tick();
        void set_mode(Mode _mode);

    private:
        uint8_t pin = 0;
        bool status = false;
        Mode mode = OFF;
        unsigned long previous_time = 0;
};


#endif /* _LED_H */
