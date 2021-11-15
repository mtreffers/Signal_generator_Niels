#include <Arduino.h>
#include "pwm.h"
#include "Encoder.h"
#include "pinout.h"

#define ENABLE_FREQUENCY_MIN 50
#define ENABLE_FREQUENCY_MAX 10000
#define ENABLE_FREQUENCY_INCREMENT_COARSE 100
#define ENABLE_FREQUENCY_INCREMENT_FINE 10

#define ENABLE_DUTY_CYCLE_MIN 1
#define ENABLE_DUTY_CYCLE_MAX 100
#define ENABLE_DUTY_CYCLE_INCREMENT 1

int16_t enable_frequency = 100;
int8_t enable_duty_cycle = 50;
enum Enable_setting_states {ES_COARSE, ES_FINE, ES_DUTY_CYCLE, ES_NUMBER_STATES};
Enable_setting_states enable_setting_state = ES_FINE;


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
  case ES_COARSE:
    enable_frequency += delta * ENABLE_FREQUENCY_INCREMENT_COARSE;
    enable_frequency = min(ENABLE_FREQUENCY_MIN, max(ENABLE_FREQUENCY_MAX, enable_frequency));
    break;
  case ES_FINE:
    enable_frequency += delta * ENABLE_FREQUENCY_INCREMENT_FINE;
    enable_frequency = min(ENABLE_FREQUENCY_MIN, max(ENABLE_FREQUENCY_MAX, enable_frequency));
    break;
  case ES_DUTY_CYCLE:
    enable_duty_cycle += delta * ENABLE_DUTY_CYCLE_INCREMENT;
    enable_duty_cycle = min(ENABLE_DUTY_CYCLE_MIN, max(ENABLE_DUTY_CYCLE_MAX, enable_duty_cycle));
  default:
    break;
  }
}

void change_enable_setting_mode() {
  switch (enable_setting_state)
  {
  case ES_COARSE:
    enable_setting_state = ES_FINE;
    break;
  case ES_FINE:
    enable_setting_state = ES_DUTY_CYCLE;
    break;
  case ES_DUTY_CYCLE:
    enable_setting_state = ES_COARSE;
  default:
    break;
  }
}
