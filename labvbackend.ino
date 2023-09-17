#include <stdint.h>
#include <Adafruit_INA219.h>

#define D_PRINT(var) do {     \
    Serial.print(#var ": ");  \
    Serial.print(var);        \
    Serial.print('\n');       \
  } while(0);

// управление мнохроматором
const int monochr_pin_1 = 10;
const int monochr_pin_2 = 11;
const int monochr_pin_3 = 12;
const int monochr_pin_4 = 13;
// int led_pin = 13; // пин, к которому подключен светодиод

void panic(const char *msg)
{
  Serial.println(msg);
  while (1) { delay(10); }
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
  while (byte_count == 0) 
  {
    byte_count = Serial.readBytes(reinterpret_cast<uint8_t*>(&data), sizeof(T));
  }

  // D_PRINT(byte_count);
  // D_PRINT(data);
  
  return data;
}

Adafruit_INA219 *g_ina219;

void setup() 
{
  Serial.begin(115200);
  
  g_ina219 = new Adafruit_INA219();
  
  if (!g_ina219->begin()) 
    panic("Failed to find INA219 chip!");

  g_ina219->setCalibration_16V_400mA();
  
  // Устанавливаем пин как выход для управления монохроматором
  pinMode(monochr_pin_1, OUTPUT);
  pinMode(monochr_pin_2, OUTPUT);
  pinMode(monochr_pin_3, OUTPUT);
  pinMode(monochr_pin_4, OUTPUT);

  // Устанавливаем все пины на высокое значение
  digitalWrite(monochr_pin_1, HIGH);
  digitalWrite(monochr_pin_2, HIGH);
  digitalWrite(monochr_pin_3, HIGH);
  digitalWrite(monochr_pin_4, HIGH);
  
  // pinMode(led_pin, OUTPUT); //светодиод
}

////////////////////////////////////////////////////////
// monochr.c
#define MONOCHR_TO_NM 75 // NOTE(annad): monochr.h, rotate = 75 ~ 1nm

void rotate_monochr(int first, int second, int third, int fours)
{

}

////////////////////////////////////////////////////////
// loop.c

void loop() 
{
  float busV = g_ina219->getBusVoltage_V();
  float mV = g_ina219->getShuntVoltage_mV();
  float mA = g_ina219->getCurrent_mA();
  D_PRINT(busV);
  D_PRINT(mA);
  D_PRINT(mV);
  return;
  
  char command_pc = serial_get_data<char>();
 
  // digitalWrite(led_pin, HIGH); // включаем светодиод 
  // delay(250); // ждем
  uint16_t rotate = MONOCHR_TO_NM * serial_get_data<uint16_t>();
  return;
    
  bool processing = true;
  while (processing)
  {
    switch (command_pc)
    {
      case 'L':
      {
        uint16_t rotate = MONOCHR_TO_NM * serial_get_data<uint16_t>();
        for (int i = 0; i <= rotate; i++) 
        {
          // Включаем 4 обмотку, отключаем 2
          digitalWrite(monochr_pin_4, LOW);
          digitalWrite(monochr_pin_2, HIGH);
          delay(4);

          // Включаем 3 обмотку, выключаем 1
          digitalWrite(monochr_pin_3, LOW);
          digitalWrite(monochr_pin_1, HIGH);
          delay(4);

          // Включаем 2, выключаем 4 обмотку, 
          digitalWrite(monochr_pin_2, LOW);
          digitalWrite(monochr_pin_4, HIGH);
          delay(4);

          // Выключаем 3 обмотку, включаем 1
          digitalWrite(monochr_pin_1, LOW);
          digitalWrite(monochr_pin_3, HIGH);
          delay(4);
        }
        
        command_pc = 'K';
      } break;
          
      case 'R':
      {
        uint16_t rotate = MONOCHR_TO_NM * serial_get_data<uint16_t>();
        for (int i = 0; i <= rotate; i++) {    
          // Включаем 1 обмотку, отключаем 3
          digitalWrite(monochr_pin_1, LOW);
          digitalWrite(monochr_pin_3, HIGH);
          delay(4);

          // Включаем 2 обмотку, выключаем 4
          digitalWrite(monochr_pin_2, LOW);
          digitalWrite(monochr_pin_4, HIGH);
          delay(4);

          // Включаем 3, выключаем 1 обмотку
          digitalWrite(monochr_pin_3, LOW);
          digitalWrite(monochr_pin_1, HIGH);
          delay(4);

          // Включаем 4, выключаем 2 обмотку,
          digitalWrite(monochr_pin_4, LOW);
          digitalWrite(monochr_pin_2, HIGH);
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
