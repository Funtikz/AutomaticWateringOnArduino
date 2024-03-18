#include <DS3231.h> //Библиотека для датчика реального времени 
#include <LCD_1602_RUS.h>   // Библиотека для монитора!
#include <Arduino.h> //Библиотека для кнопки
#include <EncButton.h> // Библиотека для кнопки
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(48, 49); // Номер пинов для WI FI

byte address[] = "1Node"; // Адрес приёмника
bool valueToSend = false;



#define RELAY_PIN8 33
#define RELAY_PIN1 35
#define RELAY_PIN2 36
#define RELAY_PIN3 37
#define RELAY_PIN4 38
#define RELAY_PIN5 39
#define RELAY_PIN6 40
#define RELAY_PIN7 41






LCD_1602_RUS lcd(0x27, 16, 4);  // Инициализация экрана
DS3231 myRTC;  // Инициализация дачика реального времени 
EncButton enc(3,4,2); // Инициализация кнопки
int value = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;




bool century = false;  // |
bool h12Flag;         //  | -> Все для настройки часов 
bool pmFlag;          //  |
bool ifDisplay1 = true; // Проверка на 1 экран
bool ifDisplay2 = false; // Проверка на 2 экран
bool ifDisplay3 = false;
bool ifDisplay4 = false;
bool ifDisplay5 = false;


// Проверка на реле
bool isReleOn1 = false;
bool isReleOn2 = false;
bool isReleOn3 = false;
bool isReleOn4 = false;
bool isReleOn5 = false;
bool isReleOn6 = false;
bool isReleOn7 = false;
bool isReleOn8 = false;
//



// Название РЕЛЕ 
char line1[] = "Papin kust";
char line2[] = "Mamina klubnika";
char line3[] = "Yblony";
char line4[] = "Grusha";
char line5[] = "Cveti";
char line6[] = "Landish";
char line7[] = "Abrikos";
char line8[] = "Malina";
//




void printDisplay1() {   // Главное меню
  lcd.clear();
  lcd.print("-Menu");
  lcd.setCursor(7, 0);
  lcd.print("Время");
  printCurrentTime();
  lcd.setCursor(9, 1);
  double temp = 23.3; // Взять температуру с датчика TODO!
  lcd.print("t. ул ");
  lcd.print(temp);
  lcd.setCursor(0, 2);
  lcd.print("Включено");
  lcd.setCursor(9, 2);
  double temp2 = 26.6; // TODO Взять темературу с датчика № 2 TODO!
  lcd.print("t. те ");
  lcd.print(temp2);
  lcd.setCursor(0, 3);
  if (isReleOn1){
    lcd.print(line1);
  }
  else if(isReleOn2){
    lcd.print(line2);
  }
  else if(isReleOn3){
    lcd.print(line3);
  }
  else if(isReleOn4){
    lcd.print(line4);
  }
  else if(isReleOn5){
    lcd.print(line5);
  }
  else if(isReleOn6){
    lcd.print(line6);
  }
  else if(isReleOn7){
    lcd.print(line7);
  }
  else if(isReleOn8){
    lcd.print(line8);
  }
}

void printCurrentTime(){     // Для вывода корректного времени с 0! Например 01:01
  lcd.setCursor(13, 0);
  if( myRTC.getHour(h12Flag, pmFlag) < 10){
    lcd.print("0");
    lcd.setCursor(14, 0);
    lcd.print(myRTC.getHour(h12Flag, pmFlag), DEC);
  }
  else{
    lcd.print(myRTC.getHour(h12Flag, pmFlag), DEC);
  }
  lcd.setCursor(15, 0);
  lcd.print(":");
  lcd.setCursor(16, 0);
  if (myRTC.getMinute() < 10){
    lcd.print("0");
    lcd.setCursor(17, 0);
    lcd.print(myRTC.getMinute(), DEC);
  }
  else{
    lcd.print(myRTC.getMinute(), DEC);
  }
}


void printDisplay2(){ //2 мини меню
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Назад");
  lcd.setCursor(0,1);
  lcd.print("  Ручное управление");
  lcd.setCursor(0,2);
  lcd.print("  Vibor rejima");
  lcd.setCursor(0,3);
  lcd.print("  Nastroika rejimov");
  ifDisplay2 = true;

}

void printOnOrOff(bool rele){
  if (rele){
    lcd.print("On");
  }
  else {
    lcd.print("Off");

  }
}


void printDisplay3(){ //Ручное управление
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Nazad");
  lcd.setCursor(0,1);
  lcd.print("  Nasos");
  lcd.setCursor(17,1);
  printOnOrOff(valueToSend);
  lcd.setCursor(2,2);
  lcd.print(line1);
  lcd.setCursor ( 17, 2);
  printOnOrOff(isReleOn1);
  lcd.setCursor(2,3);
  lcd.print(line2);
  lcd.setCursor ( 17, 3);
  printOnOrOff(isReleOn2);
}

void printDisplay3_1(){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(line3);
  lcd.setCursor ( 17, 0);
  printOnOrOff(isReleOn3);
  lcd.setCursor(2,1);
  lcd.print(line4);
  lcd.setCursor ( 17, 1);
  printOnOrOff(isReleOn4);
  lcd.setCursor(2,2);
  lcd.print(line5);
  lcd.setCursor ( 17, 2);
  printOnOrOff(isReleOn5);
  lcd.setCursor(2,3);
  lcd.print(line6);
  lcd.setCursor ( 17, 3);
  printOnOrOff(isReleOn6);
}

void printDisplay3_2(){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(line7);
  lcd.setCursor ( 17, 0);
  printOnOrOff(isReleOn7);
  lcd.setCursor(2,1);
  lcd.print(line8);
  lcd.setCursor ( 17, 1);
  printOnOrOff(isReleOn8);
}








void printDisplay4(){ // настройка режимов 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Nazad");
  lcd.setCursor(0,1);
  lcd.print("  Vibrat rejim 1");
  lcd.setCursor(0,2);
  lcd.print("  Vibrat rejim 2");
  lcd.setCursor(0,3);
  lcd.print("  Vibrat rejim 3");
}

void printDisplay4_1(){ // настройка режимов 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Vibrat rejim 4");
}

void printDisplay5(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Nazad");
  lcd.setCursor(0,1);
  lcd.print("  Rele 1");
  lcd.setCursor(17,1);
  printOnOrOff(line1);
  lcd.setCursor(0,2);
  lcd.print("  Rele 2");
  lcd.setCursor(17,2);
  printOnOrOff(line2);
  lcd.setCursor(0,3);
  lcd.print("  Rele 3");
  lcd.setCursor(17,3);
  printOnOrOff(line3);
}
void printDisplay5_1(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Rele 4");
  lcd.setCursor(17,0);
  printOnOrOff(line4);
  lcd.setCursor(0,1);
  lcd.print("  Rele 5");
  lcd.setCursor(17,1);
  printOnOrOff(line5);
  lcd.setCursor(0,2);
  lcd.print("  Rele 6");
  lcd.setCursor(17,1);
  printOnOrOff(line6);
  lcd.setCursor(0,3);
  lcd.print("  Rele 7");
  lcd.setCursor(17,3);
  printOnOrOff(line7);
}

void printDisplay5_2(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Rele 8");
  lcd.setCursor(17,0);
  printOnOrOff(line8);
}




void printDisplayExample(){
  lcd.clear();
  lcd.setCursor(0,0);
  drawArrow(0,1);
}


// Стрелка
void drawArrow(byte col, byte row) {
  lcd.setCursor(col, row); lcd.write(126);
}




void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  printDisplay1();
  enc.setBtnLevel(LOW);
  enc.setClickTimeout(500);
  enc.setDebTimeout(50);
  enc.setHoldTimeout(600);
  enc.setStepTimeout(200);
  enc.setEncReverse(0);
  enc.setEncType(EB_STEP4_LOW);
  enc.setFastTimeout(30);
  //Настройка пинов 
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  pinMode(RELAY_PIN5, OUTPUT);
  pinMode(RELAY_PIN6, OUTPUT);
  pinMode(RELAY_PIN7, OUTPUT);
  pinMode(RELAY_PIN8, OUTPUT);
  digitalWrite(RELAY_PIN1, 1); //Выключить все пины
  digitalWrite(RELAY_PIN2, 1);
  digitalWrite(RELAY_PIN3, 1);
  digitalWrite(RELAY_PIN4, 1);
  digitalWrite(RELAY_PIN5, 1);
  digitalWrite(RELAY_PIN6, 1);
  digitalWrite(RELAY_PIN7,1);
  digitalWrite(RELAY_PIN8,1);
  //Настройка пинов
  //Настройка передачи
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(0, 15);
  radio.enableAckPayload();
  radio.setPayloadSize(32);
  radio.openWritingPipe(address);
  radio.setChannel(0x60);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.powerUp();
}

void loop() {
  enc.tick();
  loopDisplay1();
  loopDisplay2();
  loopDisplay3();
  loopDisplay4();
  loopDisplay5();
  radio.write(&valueToSend, sizeof(valueToSend));

}


void loopDisplay1(){
 if (ifDisplay1){
   printCurrentTime(); 
  }  
  if (enc.click() && ifDisplay1 == true){
    ifDisplay1 = false;
    printDisplay2();
  }
}

void loopDisplay2(){
  
  if (ifDisplay2){

      if (value < 0){
        value = 1;
      }
      if (value > 4){
        value = 4;
      }
      if (enc.right()){
        value++;
      }
      if (enc.left()){
        value--;
      }
      if (value == 0){
        lcd.setCursor(0,0);
        lcd.print("  ");

      }
      if (value == 1){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value == 2){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      else if (value == 3){
        drawArrow(0,2);
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,3);
        lcd.print("  ");
      }
      else if (value == 4){
        drawArrow(0,3);
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      if (enc.click() && value == 1){
          ifDisplay2 = false;
          ifDisplay1 = true;
          value = 0;
          printDisplay1();   
      }
      if (enc.click() && value == 2){
          ifDisplay2 = false;
          value = 0;
          ifDisplay3 = true;
          printDisplay3();
      }
      
      if (enc.click() && value == 3){
          ifDisplay2 = false;
          value = 0;
          ifDisplay4 = true;
          printDisplay4();
      }
      if (enc.click() && value == 4){
          ifDisplay2 = false;
          value = 0;
          ifDisplay5 = true;
          printDisplay5();
      }
  }
}



void loopDisplay3(){
  if (ifDisplay3){
    if (value2 < 0){
        value2 = 1;
      }
      if (value2 > 12){
        value2 = 12;
      }
      if (enc.right()){
        value2++;
      }
      if (enc.left()){
        value2--;
      }
      if (value2 == 0){
        lcd.setCursor(0,0);
        lcd.print("  ");

      }
      if (value2 == 1){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value2 == 2){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      else if (value2 == 3){
        drawArrow(0,2);
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,3);
        lcd.print("  ");
      }
      else if (value2 == 4){
        drawArrow(0,3);
        lcd.setCursor(0,2);
        lcd.print("  ");
        lcd.setCursor(0,0);
        lcd.print("  ");
      }
      if (value2 == 5 && enc.right()){
        printDisplay3_1();
        value2++;
      }
      if (value2 == 5 && enc.left()){
        printDisplay3();
        value2--;
      }
      if (value2 == 6){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value2 == 7){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      else if (value2 == 8){
        drawArrow(0,2);
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,3);
        lcd.print("  ");
      }
      else if (value2 == 9){
        drawArrow(0,3);
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      if (value2 == 10 && enc.right()){
        printDisplay3_2();
        value2++;
      }
      if (value2 == 10 && enc.left()){
        printDisplay3_1();
        value2--;
      }
      if (value2 == 11){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value2 == 12){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      if (enc.click() && value2 == 1){
          ifDisplay3 = false;
          ifDisplay2 = true;
          value2 = 0;
          printDisplay2();   
      }
      
      if (enc.click() && value2 == 2 && valueToSend == false){
          valueToSend = true;
          printDisplay3();
      }
      else if(enc.click() && value2 == 2){
          valueToSend = false;
          printDisplay3();
      }


      if (enc.click() && value2 == 3 && isReleOn1 == false){
          isReleOn1 = true;
          digitalWrite(RELAY_PIN1, 0);
          printDisplay3();
      }
      else if(enc.click() && value2 == 3){
          isReleOn1 = false;
          digitalWrite(RELAY_PIN1, 1);
          printDisplay3();
      }

      if (enc.click() && value2 == 4 && isReleOn2 == false){
          isReleOn2 = true;
          digitalWrite(RELAY_PIN2, 0);
          printDisplay3();
      }
      else if(enc.click() && value2 == 4){
          isReleOn2 = false;
          digitalWrite(RELAY_PIN2, 1);
          printDisplay3();
      }
      if (enc.click() && value2 == 6 && isReleOn3 == false){
          isReleOn3 = true;
          digitalWrite(RELAY_PIN3, 0);
          printDisplay3_1();
      }
      else if(enc.click() && value2 == 6){
          isReleOn3 = false;
          digitalWrite(RELAY_PIN3, 1);
          printDisplay3_1();
      }
      if (enc.click() && value2 == 7 && isReleOn4 == false){
          isReleOn4 = true;
          digitalWrite(RELAY_PIN4, 0);
          printDisplay3_1();
      }
      else if(enc.click() && value2 == 7){
          isReleOn4 = false;
          digitalWrite(RELAY_PIN4, 1);
          printDisplay3_1();
      }
      if (enc.click() && value2 == 8 && isReleOn5 == false){
          isReleOn5 = true;
          digitalWrite(RELAY_PIN5, 0);
          printDisplay3_1();
      }
      else if(enc.click() && value2 == 8){
          isReleOn5 = false;
          digitalWrite(RELAY_PIN5, 1);
          printDisplay3_1();
      }
      if (enc.click() && value2 == 9 && isReleOn6 == false){
          isReleOn6 = true;
          digitalWrite(RELAY_PIN6, 0);
          printDisplay3_1();
      }
      else if(enc.click() && value2 == 9){
          isReleOn6 = false;
          digitalWrite(RELAY_PIN6, 1);
          printDisplay3_1();
      }
      if (enc.click() && value2 == 11 && isReleOn7 == false){
          isReleOn7 = true;
          digitalWrite(RELAY_PIN7, 0);
          printDisplay3_2();
      }
      else if(enc.click() && value2 == 11){
          isReleOn7 = false;
          digitalWrite(RELAY_PIN7, 1);
          printDisplay3_2();
      }
      if (enc.click() && value2 == 12 && isReleOn8 == false){
          isReleOn8 = true;
          digitalWrite(RELAY_PIN8, 0);
          printDisplay3_2();
      }
      else if(enc.click() && value2 == 12){
          isReleOn8 = false;
          digitalWrite(RELAY_PIN8, 1);
          printDisplay3_2();
      }





  }
}

void loopDisplay4(){
  if (ifDisplay4){
    if (value3 < 0){
        value3 = 1;
      }
      if (value3 > 6){
        value3 = 6;
      }
      if (enc.right()){
        value3++;
      }
      if (enc.left()){
        value3--;
      }
      if (value3 == 0){
        lcd.setCursor(0,0);
        lcd.print("  ");

      }
      if (value3 == 1){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value3 == 2){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      else if (value3 == 3){
        drawArrow(0,2);
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,3);
        lcd.print("  ");
      }
      else if (value3 == 4){
        drawArrow(0,3);
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      if (value3 == 5 && enc.right()){
        printDisplay4_1();
        value3++;
      }
      if (value3 == 5 && enc.left()){
        printDisplay4();
        value3--;
      }
      if (value3 == 6){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      if (enc.click() && value3 == 1){
          ifDisplay4 = false;
          ifDisplay2 = true;
          value3 = 0;
          printDisplay2();   
      }
  }
}

void loopDisplay5(){
  if (ifDisplay5){
    if (value4 < 0){
        value4 = 1;
      }
      if (value4 > 11){
        value4 = 11;
      }
      if (enc.right()){
        value4++;
      }
      if (enc.left()){
        value4--;
      }
      if (value4 == 0){
        lcd.setCursor(0,0);
        lcd.print("  ");

      }
      if (value4 == 1){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value4 == 2){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      else if (value4 == 3){
        drawArrow(0,2);
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,3);
        lcd.print("  ");
               
      }
      else if (value4 == 4){
        drawArrow(0,3);
        lcd.setCursor(0,2);
        lcd.print("  ");
        lcd.setCursor(0,0);
        lcd.print("  "); 
      }
      if (value4 == 5 && enc.right()){
        printDisplay5_1();
        value4++;
      }
      if (value4 == 5 && enc.left()){
        printDisplay5();
        value4--;
      }
      if (value4 == 6){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      else if (value4 == 7){
        drawArrow(0,1);
        lcd.setCursor(0,0);
        lcd.print("  ");
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      else if (value4 == 8){
        drawArrow(0,2);
        lcd.setCursor(0,1);
        lcd.print("  ");
        lcd.setCursor(0,3);
        lcd.print("  ");
      }
      else if (value4 == 9){
        drawArrow(0,3);
        lcd.setCursor(0,2);
        lcd.print("  ");
      }
      if (value4 == 10 && enc.right()){
        printDisplay5_2();
        value4++;
      }
      if (value4 == 10 && enc.left()){
        printDisplay5_1();
        value4--;
      }
      if (value4 == 11){
        drawArrow(0,0);
        lcd.setCursor(0,1);
        lcd.print("  ");
      }
      

      if (enc.click() && value4 == 1){
          ifDisplay5 = false;
          ifDisplay2 = true;
          value4 = 0;
          printDisplay2();   
      }
  }
}





