/*
   Пример работы релейного регулятора в автоматическом режиме по встроенному таймеру
   Давайте представим, что на 3 пине у нас спираль нагрева, подключенная через реле
   И есть какой то абстрактный датчик температуры, на который влияет спираль
*/
#include "GyverRelay.h"
// установка, гистерезис, направление регулирования
GyverRelay regulator(REVERSE);
// либо GyverRelay regulator(); без указания направления (будет REVERSE)
void setup() {
  pinMode(3, OUTPUT);         // пин реле
  regulator.setpoint = 40;    // установка (ставим на 40 градусов)
  regulator.hysteresis = 5;   // ширина гистерезиса
  regulator.k = 0.5;          // коэффициент обратной связи (подбирается по факту)
  //regulator.dT = 500;       // установить время итерации для getResultTimer
}
// вариант с delay
void loop() {
  int temp;                 // например читаем с датчика температуру
  regulator.input = temp;   // сообщаем регулятору текущую температуру
  // getResult возвращает значение для управляющего устройства
  digitalWrite(3, regulator.getResult());  // отправляем на реле (ОС работает по своему таймеру)
  delay(100);
}
/*
  // вариант со встроенным таймером
  void loop() {
  int temp;                 // например читаем с датчика температуру
  regulator.input = temp;   // сообщаем регулятору текущую температуру
  // getResult возвращает значение для управляющего устройства
  digitalWrite(3, regulator.getResultTimer());  // отправляем на реле
  
  // также можно получить значение с выхода регулятора
  // regulator
  }
*/
/*
// вариант со своим таймером
void loop() {
  static uint32_t myTimer = 0;
  if (millis() - myTimer > 2000) {    // свой таймер на миллис, 2 секунды
    myTimer = millis();
    int temp;                 // например читаем с датчика температуру
    regulator.input = temp;   // сообщаем регулятору текущую температуру
    // getResult возвращает значение для управляющего устройства
    digitalWrite(3, regulator.compute(2));  // отправляем на реле. Время передаём вручную, у нас 2 секунды
  }
}
*/