#include <Arduino.h>
#include "pwm.h"
#include "Encoder.h"
#include "pinout.h"

#define ENABLE_FREQUENCY_MIN 50
#define ENABLE_FREQUENCY_MAX 10000
#define ENABLE_FREQUENCY_INCREMENT_COARSE 100
#define ENABLE_FREQUENCY_INCREMENT_FINE 10

#define ENABLE_DUTY_CYCLE_MIN 0.1
#define ENABLE_DUTY_CYCLE_MAX 99.9
#define ENABLE_DUTY_CYCLE_INCREMENT_COARSE 0.1
#define ENABLE_DUTY_CYCLE_INCREMENT_FINE 5

int16_t enable_frequency = 100;
float enable_duty_cycle = 50;
enum Enable_setting_states {ES_FREQUENCY_COARSE, ES_FREQUENCY_FINE, ES_DUTY_CYCLE_COARSE, ES_DUTY_CYCLE_FINE};
Enable_setting_states enable_setting_state = ES_FREQUENCY_FINE;


void change_enable_parameter(int8_t delta);
void change_enable_setting_mode();

Encoder encoder_enable(PIN_ENCODER_ENABLE_A, PIN_ENCODER_ENABLE_B, PIN_ENCODER_ENABLE_PRESS, change_enable_parameter, change_enable_setting_mode);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start...");

  pwm_enable_init();
}

void loop() {
  

  encoder_enable.tick();
}

void change_enable_parameter(int8_t delta) {
  switch (enable_setting_state)
  {
  case ES_FREQUENCY_COARSE:
    enable_frequency += delta * ENABLE_FREQUENCY_INCREMENT_COARSE;
    enable_frequency = min(ENABLE_FREQUENCY_MIN, max(ENABLE_FREQUENCY_MAX, enable_frequency));
    break;
  case ES_FREQUENCY_FINE:
    enable_frequency += delta * ENABLE_FREQUENCY_INCREMENT_FINE;
    enable_frequency = min(ENABLE_FREQUENCY_MIN, max(ENABLE_FREQUENCY_MAX, enable_frequency));
    break;
  case ES_DUTY_CYCLE_COARSE:
    enable_duty_cycle += delta * ENABLE_DUTY_CYCLE_INCREMENT_COARSE;
    enable_duty_cycle = min(ENABLE_DUTY_CYCLE_MIN, max(ENABLE_DUTY_CYCLE_MAX, enable_duty_cycle));
    break;
  case ES_DUTY_CYCLE_FINE:
    enable_duty_cycle += delta * ENABLE_DUTY_CYCLE_INCREMENT_FINE;
    enable_duty_cycle = min(ENABLE_DUTY_CYCLE_MIN, max(ENABLE_DUTY_CYCLE_MAX, enable_duty_cycle));
    break;
  default:
    break;
  }
}

void change_enable_setting_mode() {
  switch (enable_setting_state)
  {
  case ES_FREQUENCY_COARSE:
    enable_setting_state = ES_FREQUENCY_FINE;
    break;
  case ES_FREQUENCY_FINE:
    enable_setting_state = ES_DUTY_CYCLE_COARSE;
    break;
  case ES_DUTY_CYCLE_COARSE:
    enable_setting_state = ES_DUTY_CYCLE_FINE;
    break;
  case ES_DUTY_CYCLE_FINE:
    enable_setting_state = ES_FREQUENCY_COARSE;
    break;
  default:
    break;
  }
}
