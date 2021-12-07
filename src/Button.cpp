#include "Button.h"

#define BOUNCE_TIME 30

Button::Button(uint8_t _pin, void (*_func_pressed)()) {
    this->pin = _pin;
    this->func_pressed = _func_pressed;

    pinMode(this->pin, INPUT_PULLUP);
}


// Based on: https://forum.arduino.cc/t/how-to-use-a-rotary-encoder/678250/2
void Button::tick() {
    unsigned long current_time = millis();
    bool s = digitalRead(this->pin);
    if (current_time - this->last_pressed >= BOUNCE_TIME) {
        if (this->last_state != s) {
            this->last_pressed = current_time;
            this->last_state = s;
            if(!s) {
                if(this->func_pressed != nullptr) {
                    this->func_pressed();
                }
            }
        }
  }
}