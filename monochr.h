/*
Author: github.com/annadostoevskaya
File: monochr.h
Date: 21/09/23 23:56:19

Description: // 04000 - 06674 = 267.4nm per 20000turns (74,79431563201197nm)
*/

#pragma once
#include <stdint.h>

class Monochr
{
public:
  struct DirRight {};
  struct DirLeft  {};

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
  
  enum 
  { 
    step_delay = 4,
    nm2turns = 75, // NOTE(annad): monochr.h, rotate = 75 ~ 1nm
  };

  Monochr(int16_t pin_1, int16_t pin_2, int16_t pin_3, int16_t pin_4);
  ~Monochr() = default;

  template<typename T>
  void turn(); // NOTE(annad): it's a bit overengineering.  
};
