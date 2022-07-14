#include <Arduino.h>
#include <stdint.h>
#include "pwm.h"
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "Encoder.h"
#include "pinout.h"
#include <MD_AD9833.h>
#include <SPI.h>
#include "Led.h"
#include "Button.h"
#include "button_state.h"
#include <EEPROM.h>

#define ENABLE_FREQUENCY_MIN 50
#define ENABLE_FREQUENCY_MAX 15000
#define ENABLE_FREQUENCY_START 1000
#define ENABLE_FREQUENCY_INCREMENT_COARSE 100
#define ENABLE_FREQUENCY_INCREMENT_FINE 10

#define ENABLE_DUTY_CYCLE_MIN 1.0
#define ENABLE_DUTY_CYCLE_MAX 50.0
#define ENABLE_DUTY_CYCLE_INCREMENT_COARSE 5
#define ENABLE_DUTY_CYCLE_INCREMENT_FINE 0.1

#define OUTPUT_FREQUENCY_MIN 100000
#define OUTPUT_FREQUENCY_MAX 1200000
#define OUTPUT_FREQUENCY_INCREMENT_SMALL 100
#define OUTPUT_FREQUENCY_INCREMENT_FINE 1000
#define OUTPUT_FREQUENCY_INCREMENT_COARSE 50000
#define OUTPUT_FREQUENCY_INCREMENT_LARGE 250000

int16_t enable_frequency = ENABLE_FREQUENCY_MIN;
float enable_duty_cycle = ENABLE_DUTY_CYCLE_MIN;
enum Enable_setting_states {ES_FREQUENCY_COARSE, ES_FREQUENCY_FINE, ES_DUTY_CYCLE_COARSE, ES_DUTY_CYCLE_FINE};
Enable_setting_states enable_setting_state = ES_FREQUENCY_FINE;
bool enable_skip_next_encoder_tick = false;

float output_frequency = OUTPUT_FREQUENCY_MIN;
enum Output_setting_states {OUT_FREQUENCY_SMALL, OUT_FREQUENCY_FINE, OUT_FREQUENCY_COARSE, OUT_FREQUENCY_LARGE};
Output_setting_states output_setting_state = OUT_FREQUENCY_FINE;
bool output_skip_next_encoder_tick = false;

void change_enable_parameter(int8_t delta);
void change_enable_setting_mode();

void change_output_parameter(int8_t delta);
void change_output_setting_mode();

void save_settings();

Encoder encoder_enable(PIN_ENCODER_ENABLE_A, PIN_ENCODER_ENABLE_B);
long encoder_enable_position = 0;
Button button_enable(PIN_ENCODER_ENABLE_PRESS);

Encoder encoder_output(PIN_ENCODER_FREQUENCY_A, PIN_ENCODER_FREQUENCY_B);
long encoder_output_position = 0;
Button button_output(PIN_ENCODER_FREQUENCY_PRESS);

ButtonState button_state(&button_enable, &button_output, change_enable_setting_mode, change_output_setting_mode, save_settings);

MD_AD9833 AD(PIN_AD9833_FSYNC);
Led output_led(PIN_LED_FREQUENCY);
Led enable_led(PIN_LED_ENABLE);

typedef struct stored_settings{
  int16_t enable_frequency;
  float enable_duty_cycle;
  float output_frequency;
} Stored_settings;

Stored_settings eeprom_settings;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start...");

  if(digitalRead(PIN_ENCODER_ENABLE_PRESS) == false && digitalRead(PIN_ENCODER_FREQUENCY_PRESS) == false){
    Serial.println("Resetting initial values ...");
    save_settings();
  }
  else{
    Serial.println("Retrieving saved settings ...");
    EEPROM.get(0, eeprom_settings);

    if (eeprom_settings.enable_frequency >= ENABLE_FREQUENCY_MIN && eeprom_settings.enable_frequency <= ENABLE_FREQUENCY_MAX &&
      eeprom_settings.enable_duty_cycle >= ENABLE_DUTY_CYCLE_MIN && eeprom_settings.enable_duty_cycle <= ENABLE_DUTY_CYCLE_MAX &&
      eeprom_settings.output_frequency >= OUTPUT_FREQUENCY_MIN && eeprom_settings.output_frequency <= OUTPUT_FREQUENCY_MAX)
    {
      enable_frequency = eeprom_settings.enable_frequency;
      enable_duty_cycle = eeprom_settings.enable_duty_cycle;
      output_frequency = eeprom_settings.output_frequency;

      Serial.println("Saved settings retrieved");
    }
    else{
      Serial.println("No (valid) saved settings found");
    }
  }

  pwm_enable_init();
  AD.begin();
  AD.setMode(MD_AD9833::MODE_SQUARE1);
  
  change_enable_setting_mode();
  change_output_setting_mode();

  pwm_enable_set_frequency(enable_frequency);
  pwm_enable_set_duty_cycle(enable_duty_cycle);

  AD.setFrequency(MD_AD9833::CHAN_0, output_frequency);

}

void loop() {
  long position;
  position = encoder_enable.read();
  if(position != encoder_enable_position) {
    change_enable_parameter(encoder_enable_position - position);
    encoder_enable_position = position;
  }

  position = encoder_output.read();
  if(position != encoder_output_position) {
    change_output_parameter(encoder_output_position - position);
    encoder_output_position = position;
  }

  button_state.tick();

  output_led.tick();
  enable_led.tick();
}

void change_enable_parameter(int8_t delta) {
  // For a undetermined reason this function is triggered twice per encoder tick, looks like a hardware "issue/mismatch"
  // since in both instances the correct input level is detected.
  // This is a quick fix to only get one call per tick
  if(enable_skip_next_encoder_tick) {
    enable_skip_next_encoder_tick = false;
    return;
  } else {
    enable_skip_next_encoder_tick = true;
  }

  switch (enable_setting_state)
  {
  case ES_FREQUENCY_COARSE:
    enable_frequency += delta * ENABLE_FREQUENCY_INCREMENT_COARSE;
    enable_frequency = max(ENABLE_FREQUENCY_MIN, min(ENABLE_FREQUENCY_MAX, enable_frequency));
    break;
  case ES_FREQUENCY_FINE:
    enable_frequency += delta * ENABLE_FREQUENCY_INCREMENT_FINE;
    enable_frequency = max(ENABLE_FREQUENCY_MIN, min(ENABLE_FREQUENCY_MAX, enable_frequency));
    break;
  case ES_DUTY_CYCLE_COARSE:
    enable_duty_cycle += delta * ENABLE_DUTY_CYCLE_INCREMENT_COARSE;
    enable_duty_cycle = max(ENABLE_DUTY_CYCLE_MIN, min(ENABLE_DUTY_CYCLE_MAX, enable_duty_cycle));
    break;
  case ES_DUTY_CYCLE_FINE:
    enable_duty_cycle += delta * ENABLE_DUTY_CYCLE_INCREMENT_FINE;
    enable_duty_cycle = max(ENABLE_DUTY_CYCLE_MIN, min(ENABLE_DUTY_CYCLE_MAX, enable_duty_cycle));
    break;
  default:
    break;
  }
  pwm_enable_set_frequency(enable_frequency);
  pwm_enable_set_duty_cycle(enable_duty_cycle);

  Serial.print("Enable freq: ");
  Serial.print(enable_frequency);
  Serial.print(", Duty cycle: ");
  Serial.println(enable_duty_cycle, 1);
}

void change_enable_setting_mode() {
  switch (enable_setting_state)
  {
  case ES_FREQUENCY_COARSE:
    enable_setting_state = ES_FREQUENCY_FINE;
    enable_led.set_mode(Led::BLINK_SLOW);
    break;
  case ES_FREQUENCY_FINE:
    enable_setting_state = ES_DUTY_CYCLE_COARSE;
    enable_led.set_mode(Led::ON);
    break;
  case ES_DUTY_CYCLE_COARSE:
    enable_setting_state = ES_DUTY_CYCLE_FINE;
    enable_led.set_mode(Led::OFF);
    break;
  case ES_DUTY_CYCLE_FINE:
    enable_setting_state = ES_FREQUENCY_COARSE;
    enable_led.set_mode(Led::BLINK_FAST);
    break;
  default:
    break;
  }
}


void change_output_parameter(int8_t delta) {
  // For a undetermined reason this function is triggered twice per encoder tick, looks like a hardware "issue/mismatch"
  // since in both instances the correct input level is detected.
  // This is a quick fix to only get one call per tick
  if(output_skip_next_encoder_tick) {
    output_skip_next_encoder_tick = false;
    return;
  } else {
    output_skip_next_encoder_tick = true;
  }

  switch (output_setting_state)
  {
  case OUT_FREQUENCY_SMALL:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_SMALL;
    output_frequency = max(OUTPUT_FREQUENCY_MIN, min(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  case OUT_FREQUENCY_FINE:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_FINE;
    output_frequency = max(OUTPUT_FREQUENCY_MIN, min(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  case OUT_FREQUENCY_COARSE:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_COARSE;
    output_frequency = max(OUTPUT_FREQUENCY_MIN, min(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  case OUT_FREQUENCY_LARGE:
    output_frequency += delta * OUTPUT_FREQUENCY_INCREMENT_LARGE;
    output_frequency = max(OUTPUT_FREQUENCY_MIN, min(OUTPUT_FREQUENCY_MAX, output_frequency));
    break;
  default:
    break;
  }
  AD.setFrequency(MD_AD9833::CHAN_0, output_frequency);
  Serial.print("Output freq: ");
  Serial.println(output_frequency);
}

void change_output_setting_mode() {
  switch (output_setting_state)
  {
  case OUT_FREQUENCY_SMALL:
    output_setting_state = OUT_FREQUENCY_FINE;
    output_led.set_mode(Led::BLINK_SLOW);
    break;
  case OUT_FREQUENCY_FINE:
    output_setting_state = OUT_FREQUENCY_COARSE;
    output_led.set_mode(Led::BLINK_FAST);
    break;
  case OUT_FREQUENCY_COARSE:
    output_setting_state = OUT_FREQUENCY_LARGE;
    output_led.set_mode(Led::ON);
    break;
  case OUT_FREQUENCY_LARGE:
    output_setting_state = OUT_FREQUENCY_SMALL;
    output_led.set_mode(Led::OFF);
    break;
  default:
    break;
  }
}

void save_settings(){
  Serial.println("save settings");
  eeprom_settings.enable_frequency = enable_frequency;
  eeprom_settings.enable_duty_cycle = enable_duty_cycle;
  eeprom_settings.output_frequency = output_frequency;

  EEPROM.put(0, eeprom_settings);
}