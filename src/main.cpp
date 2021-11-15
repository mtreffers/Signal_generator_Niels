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

#define OUTPUT_FREQUENCY_MIN 100000
#define OUTPUT_FREQUENCY_MAX 1000000
#define OUTPUT_FREQUENCY_INCREMENT_SMALL 100
#define OUTPUT_FREQUENCY_INCREMENT_FINE 1000
#define OUTPUT_FREQUENCY_INCREMENT_COARSE 10000
#define OUTPUT_FREQUENCY_INCREMENT_LARGE 100000

int16_t enable_frequency = ENABLE_FREQUENCY_MIN;
float enable_duty_cycle = ENABLE_DUTY_CYCLE_MIN;
enum Enable_setting_states {ES_FREQUENCY_COARSE, ES_FREQUENCY_FINE, ES_DUTY_CYCLE_COARSE, ES_DUTY_CYCLE_FINE};
Enable_setting_states enable_setting_state = ES_FREQUENCY_FINE;

int32_t output_frequency = OUTPUT_FREQUENCY_MIN;
enum Output_setting_states {OUT_FREQUENCY_SMALL, OUT_FREQUENCY_FINE, OUT_FREQUENCY_COARSE, OUT_FREQUENCY_LARGE};
Output_setting_states output_setting_state = OUT_FREQUENCY_FINE;

void change_enable_parameter(int8_t delta);
void change_enable_setting_mode();

void change_output_parameter(int8_t delta);
void change_output_setting_mode();

Encoder encoder_enable(PIN_ENCODER_ENABLE_A, PIN_ENCODER_ENABLE_B, PIN_ENCODER_ENABLE_PRESS, change_enable_parameter, change_enable_setting_mode);
Encoder encoder_output(PIN_ENCODER_FREQUENCY_A, PIN_ENCODER_FREQUENCY_B, PIN_ENCODER_FREQUENCY_PRESS, change_output_parameter, change_output_setting_mode);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start...");

  pwm_enable_init();
}

void loop() {
  

  encoder_enable.tick();
  encoder_output.tick();
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
  pwm_enable_set_frequency(enable_frequency);
  pwm_enable_set_duty_cycle(enable_duty_cycle);
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


void change_output_parameter(int8_t delta) {
  switch (output_setting_state)
  {
  case OUT_FREQUENCY_SMALL:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_SMALL;
    output_frequency = min(OUTPUT_FREQUENCY_MIN, max(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  case OUT_FREQUENCY_FINE:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_FINE;
    output_frequency = min(OUTPUT_FREQUENCY_MIN, max(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  case OUT_FREQUENCY_COARSE:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_COARSE;
    output_frequency = min(OUTPUT_FREQUENCY_MIN, max(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  case OUT_FREQUENCY_LARGE:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_LARGE;
    output_frequency = min(OUTPUT_FREQUENCY_MIN, max(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  default:
    break;
  }
}

void change_output_setting_mode() {
  switch (enable_setting_state)
  {
  case OUT_FREQUENCY_SMALL:
    output_setting_state = OUT_FREQUENCY_FINE;
    break;
  case OUT_FREQUENCY_FINE:
    output_setting_state = OUT_FREQUENCY_COARSE;
    break;
  case OUT_FREQUENCY_COARSE:
    output_setting_state = OUT_FREQUENCY_LARGE;
    break;
  case OUT_FREQUENCY_LARGE:
    output_setting_state = OUT_FREQUENCY_SMALL;
    break;
  default:
    break;
  }
}