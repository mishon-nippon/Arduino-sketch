#include "EEPROM.h"
 
#define D1 2          // направление вращения лев.мотора
#define M1 3          // управление лев.мотором
#define D2 4          // направление вращения прав.мотора
#define M2 5          // управление прав.мотором
#define left 'L'      // команда для лев. мотора
#define right 'R'      // команда для прав. мотора
 
char incomingByte;    // входящие данные 
char LeftArray[16];    // массив получаемых команд левого мотора
byte LeftIndex = 0;     // индекс массива
char RightArray[16];   // массив получаемых команд правого мтора
byte RightIndex = 0;     // индекс массива
char command;         //текущая команда
  
void setup() {
  Serial.begin(9600);       // инициализация порта
  pinMode(D1, OUTPUT);      // пин направления вращения лев. мотора 
  pinMode(D2, OUTPUT);      // пин направления вращения прав. мотора
}  
 
  
void loop() {
  if (Serial.available() > 0) {          // если пришла команда
    incomingByte = Serial.read();        
    if(incomingByte == left) {           // если пришла команда левого мотора
      command = left;                    // текущая команда
      memset(LeftArray,0,sizeof(LeftArray));   // очистка массива
      LeftIndex = 0;                       // сброс индекса 
    }
    else if(incomingByte == right) {      // если пришла команда правого мотора
      command = right;
      memset(RightArray,0,sizeof(RightArray));
      RightIndex = 0;
    }
    else if(incomingByte == '\r') command = 'e';   // конец строки
   
    if(command == left && incomingByte != left){
      LeftArray[LeftIndex] = incomingByte;              // сохранение данных в массив
      LeftIndex++;                                   // увеличение индекса массива
    }
    else if(command == right && incomingByte != right){
      RightArray[RightIndex] = incomingByte;
      RightIndex++;
    }
    else if(command == 'e'){                       // если конец строки
      Drive(atoi(LeftArray),atoi(RightArray));
      delay(10);
    }
  }
}    

void Drive(int mLeft, int mRight){
 
  bool directionL, directionR;      // направление вращения 
  byte valueL, valueR;              // значение ШИМ левый, правый (0-255)
   
  if(mLeft > 0){
    valueL = mLeft;
    directionL = 0;
  }
  else if(mLeft < 0){
    valueL = 255 - abs(mLeft);
    directionL = 1;
  }
  else {
    directionL = 0;
    valueL = 0;
  }
  
  if(mRight > 0){
    valueR = mRight;
    directionR = 0;
  }
  else if(mRight < 0){
    valueR = 255 - abs(mRight);
    directionR = 1;
  }
  else {
    directionR = 0;
    valueR = 0;
  }
    
  analogWrite(M1, valueL);            // задаем скорость вращения для L
  analogWrite(M2, valueR);            // задаем скорость вращения для R
  digitalWrite(D1, directionL);       // задаем направление вращения для L
  digitalWrite(D2, directionR);       // задаем направление вращения для R   
}
 

