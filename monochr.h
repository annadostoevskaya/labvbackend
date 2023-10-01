/*
Author: github.com/annadostoevskaya
File: monochr.h
Date: 21/09/23 23:56:19

Description: <empty>
*/

#pragma once
#include <stdint.h>

class Monochr
{
public:
  enum 
  { 
    step_delay = 4,
    nm2turn = 75 // NOTE(annad): monochr.h, rotate = 75 ~ 1nm
  };

  union
  {
    int16_t pins[4];
    struct 
    {
      int16_t pin_1;
      int16_t pin_2;
      int16_t pin_3;
      int16_t pin_4;
    };
  };

  Monochr(int16_t pin_1, int16_t pin_2, int16_t pin_3, int16_t pin_4);
  ~Monochr();
  void turn_right();
  void turn_left();
};
