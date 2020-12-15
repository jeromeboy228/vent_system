// 0UvRgBT1VJArmWx53273nbCVoJXaZsF7

/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino Ethernet Shield 2 (W5500)
  to connect your project to Blynk.

  Note: This requires Ethernet2 library
    from http://librarymanager/all#Ethernet2

        Pins 10, 11, 12 and 13 are reserved for Ethernet module.
        DON'T use them in your sketch directly!

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet2.h>
#include <BlynkSimpleEthernet2.h>
#include <Arduino.h>
// ПОДКЛЮЧЕНИЕ ЖЕЛТЫЙ 2
//КРАСНЫЙ 4
//ЧЕРНЫЙ ЗЕМЛЯ
#include <OneWire.h>           // Подключаем библиотеку для взаимодействия с устройствами, работающими на шине и по протоколу 1-Wire
#include <DallasTemperature.h> // Подключаем библиотеку с функциями для работы с DS18B20 (запросы, считывание и преобразование возвращаемых данных)
#include <OneWire.h>
#define ONE_WIRE_BUS 2 // Указываем пин подключения data-вывода датчика температуры
#define term_power 4   // Указываем пин подключения питания датчика температуры
#define VENT 8
char auth[] = "0UvRgBT1VJArmWx53273nbCVoJXaZsF7";
OneWire oneWire(ONE_WIRE_BUS);       // Сообщаем библиотеке об устройстве, работающем по протоколу 1-Wire
DallasTemperature sensors(&oneWire); // Связываем функции библиотеки DallasTemperature с нашим 1-Wire устройством (DS18B20)

void setup()
{

  Serial.begin(9600);
  sensors.begin();             // Запускаем библиотеку измерения температуры
  pinMode(term_power, OUTPUT); // Определяем пин подключения питания датчика температуры
  pinMode(VENT, OUTPUT);
  Blynk.begin(auth);
}

double temp()
{

  digitalWrite(term_power, HIGH); // подаем напряжение на термометр
  delay(100);                     // задержка перед измерением
  sensors.requestTemperatures();  // измерение
  delay(100);
  sensors.requestTemperatures();                         // измерение
  double real_temp = double(sensors.getTempCByIndex(0)); // запись измерения

  digitalWrite(term_power, LOW); // отключение питания с термометра
  delay(100);                    // просто задержка в 5 сек чтоб не перегривался

  return (real_temp);
}
void T_control()
{
  double temperarura = temp();
  Blynk.virtualWrite(V0, temperarura);
}

BLYNK_WRITE(V1)
{
  double temper = temp();
  int value = param.asInt();
  Serial.print("value = ");
  Serial.print(value);
  Serial.print(" temperature = ");
  Serial.println(temper);
  T_control();
  Blynk.syncVirtual(V1);
  if (temper < value - 2)
  {
    digitalWrite(VENT, HIGH);
  }
  if (temper > value + 2)
  {
    digitalWrite(VENT, LOW);
  }
  delay(2700);
}

void loop()
{
  Blynk.run();
//  T_control();
}
