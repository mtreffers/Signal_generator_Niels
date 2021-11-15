#include "Led.h"

#define PERIOD_BLINK_SLOW 500
#define PERIOD_BLINK_FAST 1000

Led::Led(uint8_t _pin) {
    this->pin = _pin;

    pinMode(this->pin, OUTPUT);
}

void Led::tick() {
    bool next_status = false;
    unsigned long current_time = millis();

    switch (this->mode)
    {
    case OFF:
        next_status = false;
        break;
    case BLINK_SLOW:
        if(current_time - this->previous_time > PERIOD_BLINK_SLOW) {
            next_status = !this->status;
        }
        break;
    case BLINK_FAST:
        if(current_time - this->previous_time > PERIOD_BLINK_FAST) {
            next_status = !this->status;
        }
        break;
    case ON:
        next_status = true;
        break;
    default:
        next_status = false;
        break;
    }
    digitalWrite(this->pin, next_status);
    this->status = next_status;
}

void Led::set_mode(Mode _mode) {

}
