#include <Arduino.h>
#include "pinout.h"

#define CLK_FREQ 16000000

uint16_t m_frequency = 10;
uint8_t m_duty_cycle = 50;
uint8_t m_prescaler = 0;

void _pwm_enable_set_prescaler(uint16_t frequency) {
    // Reset prescaler
    TCCR1B &= ~(_BV(CS00) | _BV(CS01) |_BV(CS02));

    if(frequency < 245) {
        // Set prescaler 8
        m_prescaler = 8;
        TCCR1B |= _BV(CS11);
    } else {
        // Set prescaler 1
        m_prescaler = 1;
        TCCR1B |= _BV(CS10);
    }
}

void _pwm_enable_set_frequency(uint16_t frequency) {
    m_frequency = frequency;
    _pwm_enable_set_prescaler(m_frequency);

    OCR1A = CLK_FREQ / (m_prescaler * m_frequency);
}

void _pwm_enable_set_duty_cycle(float duty_cycle) {
    m_duty_cycle = duty_cycle;
    OCR1B = ((uint16_t)(OCR1A * duty_cycle)) / 100;
}

void _pwm_enable_stop() {
    cli();
}

void _pwm_enable_start() {
    sei();
}

void _pwm_enable_reset() {
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
}

void pwm_enable_init() {
    pinMode(PIN_ENABLE_PWM_A, OUTPUT);
    pinMode(PIN_ENABLE_PWM_B, OUTPUT);

    _pwm_enable_stop();
    _pwm_enable_reset();

    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(WGM12) | _BV(WGM13);
    _pwm_enable_set_frequency(m_frequency);
    _pwm_enable_set_duty_cycle(m_duty_cycle);
    _pwm_enable_start();
}


void pwm_enable_set_frequency(uint16_t frequency) {
    _pwm_enable_stop();
    _pwm_enable_set_frequency(frequency);
    _pwm_enable_start();
}

void pwm_enable_set_duty_cycle(float duty_cycle) {
    _pwm_enable_stop();
    _pwm_enable_set_duty_cycle(duty_cycle);
    _pwm_enable_start();
}