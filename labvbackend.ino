/*
Author: github.com/annadostoevskaya
File: labvbackend.ino
Date: 12/09/23 14:22:03

Description: Orange - TX, Blue - RX
*/

#include <stdint.h>
#include <Adafruit_INA219.h>
#include "monochr.h"
#include "unique_ptr.h"

#define DEBUG_INA219_DISABLED
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

// int16_t led_pin = 13; // пин, к которому подключен светодиод

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
T serial_get_data()
{
  T data = 0;
#if 1
  uint8_t *p_data = reinterpret_cast<uint8_t*>(&data);
  for (int16_t i = sizeof(T) - 1; i >= 0; i -= 1)
    p_data[i] = serial_wait_byte();
  D_PRINT(sizeof(T));
#else
  size_t byte_count = 0;
  while (byte_count == 0 && byte_count != sizeof(T))
    byte_count = Serial.readBytes(reinterpret_cast<uint8_t*>(&data), sizeof(T));
  D_PRINT(byte_count);
#endif
  D_PRINT(data);
  return data;
}

auto g_ina219 = unique_ptr<Adafruit_INA219>(new Adafruit_INA219);

void setup()
{
  Serial.begin(9600);
  
  if (!g_ina219->begin())
  {
#ifndef DEBUG_INA219_DISABLED 
    panic("Failed to find INA219 chip!");
#endif
  }

  g_ina219->setCalibration_32V_2A();
  
  D_PRINT((size_t)g_ina219.get());
  D_PRINT((size_t)&g_ina219);
  
  // pinMode(led_pin, OUTPUT); //светодиод
}

////////////////////////////////////////////////////////
// loop.c

//SoftwareSerial DebugSerial();

void loop() 
{
/*
  float busV = g_ina219->getBusVoltage_V();
  float mV = g_ina219->getShuntVoltage_mV();
  float mA = g_ina219->getCurrent_mA();
  D_PRINT(busV);
  D_PRINT(mA);
  D_PRINT(mV);
  return;
*/
  char command_pc = serial_get_data<char>();
 
  // digitalWrite(led_pin, HIGH); // включаем светодиод 
  // delay(250); // ждем
    
  bool processing = true;
  while (processing)
  {
    switch (command_pc)
    {
      case 'L':
      {
        uint32_t rotate = Monochr::to_nm * static_cast<uint32_t>(serial_get_data<uint16_t>());
        // D_PRINT(rotate / Monochr::to_nm);
        // D_PRINT(rotate);
        for (uint32_t i = 0; i < rotate; i++)
          monochr.turn_left();
        
        command_pc = 'K';
      } break;
          
      case 'R':
      {
        uint32_t rotate = Monochr::to_nm * static_cast<uint32_t>(serial_get_data<uint16_t>());
        // D_PRINT(rotate / Monochr::to_nm);
        // D_PRINT(rotate);
        for (uint32_t i = 0; i < rotate; i++)
          monochr.turn_right();
        
        command_pc = 'K';
      } break;
      
      case 'K':
      {
        Serial.write('K');
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
