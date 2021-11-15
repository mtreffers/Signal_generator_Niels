#include "Encoder.h"

Encoder::Encoder(uint8_t _pin_A, uint8_t _pin_B, uint8_t _pin_press, void (*_func_rotated)(int8_t), void (*_func_pressed)()) {
    this->pin_A = _pin_A;
    this->pin_B = _pin_B;
    this->pin_press = _pin_press;
    this->func_rotated = _func_rotated;
    this->func_rotated = _func_rotated;
        
    pinMode(this->pin_A, INPUT);
    pinMode(this->pin_B, INPUT);
    pinMode(this->pin_press, INPUT_PULLUP);
}

void Encoder::tick() {
  int8_t delta = 0;
  enum {STATE_LOCKED, STATE_TURN_RIGHT_START, STATE_TURN_RIGHT_MIDDLE, STATE_TURN_RIGHT_END, STATE_TURN_LEFT_START, STATE_TURN_LEFT_MIDDLE, STATE_TURN_LEFT_END, STATE_UNDECIDED};
  static uint8_t encoderState = STATE_LOCKED;
  bool a = !digitalRead(this->pin_A);
  bool b = !digitalRead(this->pin_B);
  bool s = !digitalRead(this->pin_press);
  static bool switchState = s;
  switch (encoderState) {
    case STATE_LOCKED:
      if (a && b) {
        encoderState = STATE_UNDECIDED;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_LEFT_START;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_RIGHT_START;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
    case STATE_TURN_RIGHT_START:
      if (a && b) {
        encoderState = STATE_TURN_RIGHT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_RIGHT_END;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_RIGHT_START;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
    case STATE_TURN_RIGHT_MIDDLE:
    case STATE_TURN_RIGHT_END:
      if (a && b) {
        encoderState = STATE_TURN_RIGHT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_RIGHT_END;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_RIGHT_START;
      }
      else {
        encoderState = STATE_LOCKED;
        delta = -1;
      };
      break;
    case STATE_TURN_LEFT_START:
      if (a && b) {
        encoderState = STATE_TURN_LEFT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_LEFT_START;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_LEFT_END;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
    case STATE_TURN_LEFT_MIDDLE:
    case STATE_TURN_LEFT_END:
      if (a && b) {
        encoderState = STATE_TURN_LEFT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_LEFT_START;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_LEFT_END;
      }
      else {
        encoderState = STATE_LOCKED;
        delta = 1;
      };
      break;
    case STATE_UNDECIDED:
      if (a && b) {
        encoderState = STATE_UNDECIDED;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_RIGHT_END;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_LEFT_END;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
  }

  uint32_t current_time = millis();
  static uint32_t switch_time = 0;
  const uint32_t bounce_time = 30;
  bool back = false;
  if (current_time - switch_time >= bounce_time) {
    if (switchState != s) {
      switch_time = current_time;
      back = s;
      switchState = s;
    }
  }
  
  if(delta != 0) {
      if(this->func_rotated != nullptr) {
        this->func_rotated(delta);
      }
  }

  if(back) {
      if(this->func_pressed != nullptr) {
        this->func_pressed();
      }
  }
}