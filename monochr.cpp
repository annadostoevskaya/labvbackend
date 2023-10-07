/*
Author: github.com/annadostoevskaya
File: monochr.cpp
Date: 22/09/23 00:02:08

Description: <empty>
*/

#include <stdint.h>
#include <Arduino.h>
#include "monochr.h"

Monochr::~Monochr() = default;

Monochr::Monochr(int16_t pin_1, int16_t pin_2, int16_t pin_3, int16_t pin_4) 
  : pin_1(pin_1)
  , pin_2(pin_2)
  , pin_3(pin_3)
  , pin_4(pin_4)
{
  for (size_t i = 0; i < sizeof(pins) / sizeof(*pins); i += 1)
  {
    pinMode(this->pins[i], OUTPUT);
    digitalWrite(this->pins[i], HIGH);
  }
};

void Monochr::turn_right()
{
  for (int16_t i = 0; i < (int16_t)(sizeof(pins) / sizeof(*pins)); i += 1)
  {
    digitalWrite(pins[i], HIGH); // 1, 2, 3, 4
    digitalWrite(pins[(i + 2) % 4], LOW); // 3-1, 4-1, 1-1, 2-1,  
    delay(step_delay);
  }
}

void Monochr::turn_left()
{
  for (int16_t i = sizeof(pins) / sizeof(*pins) - 1; i >= 0; i -= 1)
  {
    digitalWrite(pins[i], LOW); // 4, 3, 2, 1
    digitalWrite(pins[(i + 2) % 4], HIGH); // 2-1, 1-1, 4-1, 3-1  
    delay(step_delay);
  }
}

// vim:tabstop=2 shiftwidth=2 expandtab
