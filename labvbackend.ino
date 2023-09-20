/*
Author: github.com/annadostoevskaya
File: labvbackend.ino
Date: 12/09/23 14:22:03

Description: <empty> 
*/

#include <stdint.h>
#include <stdio.h>
#include <Adafruit_INA219.h>
#include "unique_ptr.h"

#ifndef RELEASE_MODE
#define D_PRINT(var) do {     \
    Serial.print(#var ": ");  \
    Serial.print(var);        \
    Serial.print('\n');       \
  } while(0);
#else
#define D_PRINT(var)
#endif

class Monochr
{
public:
  enum { to_nm = 75 };
  union
  {
    int pins[4];
    struct 
    {
      int pin_1;
      int pin_2;
      int pin_3;
      int pin_4;
    };
  };

  Monochr(int pin_1, int pin_2, int pin_3, int pin_4) 
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
  ~Monochr() = default;
};

Monochr monochr(10, 11, 12, 13);

// int led_pin = 13; // пин, к которому подключен светодиод

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
  size_t byte_count = 0;
  while (byte_count == 0 && byte_count != sizeof(T)) 
  {
    byte_count = Serial.readBytes(reinterpret_cast<uint8_t*>(&data), sizeof(T));
  }

  D_PRINT(byte_count);
  D_PRINT(data);

  return data;
}

auto g_ina219 = unique_ptr<Adafruit_INA219>(new Adafruit_INA219);

void setup()
{
  Serial.begin(115200);
  
  if (!g_ina219->begin()) 
    panic("Failed to find INA219 chip!");

  g_ina219->setCalibration_32V_2A();
  
  D_PRINT((size_t)g_ina219.get());
  D_PRINT((size_t)&g_ina219);
  
  // pinMode(led_pin, OUTPUT); //светодиод
}

////////////////////////////////////////////////////////
// monochr.c
// NOTE(annad): monochr.h, rotate = 75 ~ 1nm

void rotate_monochr(int first, int second, int third, int fours)
{

}

////////////////////////////////////////////////////////
// loop.c

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
        uint16_t rotate = Monochr::to_nm * serial_get_data<uint16_t>();
        for (int i = 0; i <= rotate; i++) 
        {
          /*
          for (size_t i = 3; i >= 0; i -= 1)
          {
            digitalWrite(pins[i], LOW); // 4, 3, 2, 1
            digitalWrite(pins[(i + 2) % 4], HIGH); // 2-1, 1-1, 4-1, 3-1  
            delay(4);
          }
          */
          // Включаем 4 обмотку, отключаем 2
          digitalWrite(monochr.pin_4, LOW);
          digitalWrite(monochr.pin_2, HIGH);
          delay(4);

          // Включаем 3 обмотку, выключаем 1
          digitalWrite(monochr.pin_3, LOW);
          digitalWrite(monochr.pin_1, HIGH);
          delay(4);

          // Включаем 2, выключаем 4 обмотку, 
          digitalWrite(monochr.pin_2, LOW);
          digitalWrite(monochr.pin_4, HIGH);
          delay(4);

          // Выключаем 3 обмотку, включаем 1
          digitalWrite(monochr.pin_1, LOW);
          digitalWrite(monochr.pin_3, HIGH);
          delay(4);
        }
        
        command_pc = 'K';
      } break;
          
      case 'R':
      {
        uint16_t rotate = Monochr::to_nm * serial_get_data<uint16_t>();
        for (int i = 0; i <= rotate; i++) {
          /* 
          for (size_t i = 0; i < 4; i += 1)
          {
            digitalWrite(pins[i], HIGH); // 1, 2, 3, 4
            digitalWrite(pins[(i + 2) % 4], LOW); // 3-1, 4-1, 1-1, 2-1,  
            delay(4);
          } 
          */
            
          // Включаем 1 обмотку, отключаем 3
          digitalWrite(monochr.pin_1, LOW);
          digitalWrite(monochr.pin_3, HIGH);
          delay(4);

          // Включаем 2 обмотку, выключаем 4
          digitalWrite(monochr.pin_2, LOW);
          digitalWrite(monochr.pin_4, HIGH);
          delay(4);

          // Включаем 3, выключаем 1 обмотку
          digitalWrite(monochr.pin_3, LOW);
          digitalWrite(monochr.pin_1, HIGH);
          delay(4);

          // Включаем 4, выключаем 2 обмотку,
          digitalWrite(monochr.pin_4, LOW);
          digitalWrite(monochr.pin_2, HIGH);
          delay(4);
        }
        
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
