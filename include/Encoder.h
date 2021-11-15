#ifndef _ENCODER_H
#define _ENCODER_H

#include <Arduino.h>

class Encoder
{
    private:
        uint8_t pin_A = 0;
        uint8_t pin_B = 0;
        uint8_t pin_press = 0;
        void (*func_rotated)(int8_t) = nullptr;
        void (*func_pressed)() = nullptr;
    public:
        Encoder(uint8_t _pin_A, uint8_t _pin_B, uint8_t _pin_press, void (*_func_rotated)(int8_t), void (*_func_pressed)());
        void tick();
};

#endif /* _ENCODER_H */
