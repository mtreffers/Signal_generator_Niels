#include <Arduino.h>
#include "pwm.h"

char incomingBytes[32];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start...");

  pwm_enable_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (Serial.available() > 0) {
  //   size_t result = Serial.readBytesUntil('\n', incomingBytes, sizeof(incomingBytes) / sizeof(char) );
    
  //   if(result > 0) {
  //     char buffer[33];
  //     for(uint8_t i = 0; i < result; i++) {
  //       buffer[i] = incomingBytes[i + 1];
  //     }
    
  //     if(incomingBytes[0] == 'f') {
  //       pwm_enable_set_frequency((uint16_t)atoi(buffer));
  //     } else if(incomingBytes[0] == 'd') {
  //       pwm_enable_set_duty_cycle((uint16_t)atoi(buffer));
  //     }

  //   }
  // }

  uint16_t val = min((uint16_t)analogRead(A0), 100);
  pwm_enable_set_duty_cycle(val);
  Serial.println(val);
  delay(250);
}