#ifndef _BUTTON_H
#define _BUTTON_H

#include <Arduino.h>
#include <stdint.h>

class Button {
    public:
        Button(uint8_t _pin, void (*_func_pressed)());
        void tick();

    private:
        uint8_t pin;
        void (*func_pressed)();

        unsigned long last_pressed = 0;
        bool last_state = false;

};

#endif /* _BUTTON_H */
