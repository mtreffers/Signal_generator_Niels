#include "Button.h"

#define BOUNCE_TIME 30

Button::Button(uint8_t _pin) {
    this->pin = _pin;

    pinMode(this->pin, INPUT_PULLUP);
}


// Based on: https://forum.arduino.cc/t/how-to-use-a-rotary-encoder/678250/2
void Button::tick() {
    unsigned long current_time = millis();
    bool s = digitalRead(this->pin);
    // Serial.print("button HIGH/LOW: ");
    // Serial.println(s);
    if (current_time - this->last_pressed >= BOUNCE_TIME) {
        if (this->last_state != s) {
            this->last_pressed = current_time;
            this->last_state = s;
        }
  }
}