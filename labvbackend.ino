/*
Author: github.com/annadostoevskaya
File: labvbackend.ino
Date: 12/09/23 14:22:03

Description: Orange - TX, Blue - RX
*/

#include <stdint.h>
#include <Adafruit_ADS1X15.h> // "Adafruit ADS1X15"@2.4.1
#include "monochr.h"
#include "unique_ptr.h"

#define RELEASE_MODE
#ifndef RELEASE_MODE
# define DEBUG_MODE
#endif

#ifdef DEBUG_MODE
# define D_PRINT(var) do {      \
    Serial.print(#var ": ");    \
    Serial.print(var);          \
    Serial.print('\n');         \
  } while(0);
#else
# define D_PRINT(var)
#endif

Monochr monochr(10, 11, 12, 13);

void panic(const char *msg)
{
  Serial.println(msg);
#ifdef RELEASE_MODE
  while (1) { delay(10); }
#endif
}

uint8_t serial_wait_byte()
{
  while (Serial.available() == 0);
  return (uint8_t)Serial.read();
}

template <typename T>
void swap_endians(T& data)
{
  uint8_t tmp[sizeof(T)];
  for (int16_t i = sizeof(T) - 1; i >= 0; i -= 1)
  {
    int16_t shift = (sizeof(T) - 1 - i) * 8;
    tmp[i] = (data >> shift) & 0xff;
  }

  data = *reinterpret_cast<T*>(tmp);
}

template <typename T>
T serial_get_data()
{
  T data = 0;
  size_t byte_count = 0;
  while (byte_count == 0 && byte_count != sizeof(T))
    byte_count = Serial.readBytes(reinterpret_cast<uint8_t*>(&data), sizeof(T));

  return data;
}

template <typename T>
size_t serial_put_data(T data)
{
  return Serial.write(reinterpret_cast<uint8_t*>(&data), sizeof(T));
}

auto g_ads1115 = unique_ptr<Adafruit_ADS1115>(new Adafruit_ADS1115());

void setup()
{
  Serial.begin(9600);
  
  if (g_ads1115->begin())
  {
    g_ads1115->setGain(GAIN_TWOTHIRDS);
  }
  else 
    panic("Failed to find ADS1115 chip!");
}

////////////////////////////////////////////////////////
// loop.c

union f32_i32
{
  float f32;
  int32_t i32;
};

void loop() 
{  
  char command_pc = serial_get_data<char>();
   
  bool processing = true;
  while (processing)
  {
    switch (command_pc)
    {
      case 'L':
      {
        uint16_t rotate_nm = serial_get_data<uint16_t>();
        swap_endians(rotate_nm);
        uint32_t turns = Monochr::nm2turns * static_cast<uint32_t>(rotate_nm);
        for (uint32_t i = 0; i < turns; i++)
          monochr.turn_left();
        
        command_pc = 'K';
      } break;
 
      case 'R':
      {
        uint16_t rotate_nm = serial_get_data<uint16_t>();
        swap_endians(rotate_nm);
        uint32_t turns = Monochr::nm2turns * static_cast<uint32_t>(rotate_nm);
        for (uint32_t i = 0; i < turns; i++)
          monochr.turn_right();
        
        command_pc = 'K';
      } break;

      case 'A':
      {
        f32_i32 mV = {};
        int num = 10;
        for (int i = 0; i < num; i += 1)
        {
          int16_t adc0 = g_ads1115->readADC_SingleEnded(0);
          mV.f32 += (0.1875f * (float)adc0);
        }
        mV.f32 /= ((float)num);
        
        swap_endians(mV.i32);
        serial_put_data(mV);

        command_pc = '\0';
      } break;
      
      case 'K':
      {
        Serial.write('K');
        Serial.flush();
        command_pc = '\0';
      } break;

      case '\0':
      {
        processing = false;
      } break;

      default:
      {
        processing = false;
      } break;
    }
  }

  return;
}

// vim:tabstop=2 shiftwidth=2 expandtab
