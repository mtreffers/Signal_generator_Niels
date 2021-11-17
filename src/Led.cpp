#include "Led.h"

#define PERIOD_BLINK_SLOW 200
#define PERIOD_BLINK_FAST 1000

Led::Led(uint8_t _pin) {
    this->pin = _pin;

    pinMode(this->pin, OUTPUT);
}

void Led::tick() {
    bool next_status = this->status;
    unsigned long current_time = millis();

    switch (this->mode)
    {
    case OFF:
        next_status = false;
        break;
    case BLINK_SLOW:
        if(current_time - this->previous_time > PERIOD_BLINK_SLOW) {
            this->previous_time = current_time;
            next_status = !this->status;
        }
        break;
    case BLINK_FAST:
        if(current_time - this->previous_time > PERIOD_BLINK_FAST) {
            this->previous_time = current_time;
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

    if(this->status != next_status) {
        digitalWrite(this->pin, next_status);
        this->status = next_status;
    }
}

void Led::set_mode(Mode _mode) {
    this->mode = _mode;
}
