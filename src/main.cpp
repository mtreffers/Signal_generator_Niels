#include <Arduino.h>
#include "pwm.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start...");

  pwm_enable_init();
}

void loop() {
  
}