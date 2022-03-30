#ifndef _BUTTON_H
#define _BUTTON_H

#include <Arduino.h>
#include <stdint.h>

class Button {
    public:
        Button(uint8_t _pin);
        void tick();
        
        bool last_state = false;
        
    private:
        uint8_t pin;

        unsigned long last_pressed = 0;
};

#endif /* _BUTTON_H */
