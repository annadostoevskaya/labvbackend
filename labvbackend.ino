#include <stdint.h>

// управление мнохроматором
const int monochr_pin_1 = 10;
const int monochr_pin_2 = 11;
const int monochr_pin_3 = 12;
const int monochr_pin_4 = 13;
// int led_pin = 13; // пин, к которому подключен светодиод

void setup() {
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
 
  Serial.begin(9600);
}

uint8_t wait_byte()
{
  while (Serial.available() == 0);

  return (uint8_t)Serial.read();
}

////////////////////////////////////////////////////////
// monochr.c
void rotate_monochr(int first, int second, int third, int fours)
{

}

////////////////////////////////////////////////////////
// loop.c

void loop() 
{
  char command_pc = wait_byte();
  
  // digitalWrite(led_pin, HIGH); // включаем светодиод 
  // delay(250); // ждем

  uint16_t rotate = 0;
  ((uint8_t*)&rotate)[1] = wait_byte();
  ((uint8_t*)&rotate)[0] = wait_byte();

  bool processing = true;
  while (processing)
  {
    switch (command_pc)
    {
      case 'L':
      {
        for (int i=0; i <= rotate; i++) 
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

