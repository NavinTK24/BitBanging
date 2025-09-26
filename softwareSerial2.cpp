//Bluetooth library code for enabling UART communicatoin between HC-05 and Serial Monitor bidirectionally

#include<Arduino.h>
#include <string.h>

const int TXpin = 11;
const int RXpin = 10;
const int baudRate = 9600;
const float bitTime = 104.1667;

void setup() {
  pinMode(TXpin, OUTPUT);
  pinMode(RXpin, INPUT);
  digitalWrite(TXpin, HIGH);
  digitalWrite(RXpin, HIGH);
}

void sendToBluetooth(const char* msg) {

  while(*msg) {

    digitalWrite(TXpin, LOW);
    delayMicroseconds(bitTime);
    char c = *msg;

    for(int i=0; i<=7; i++) {
      if(c & (1<<i)) {
        digitalWrite(TXpin, HIGH);
      } else {
        digitalWrite(TXpin, LOW);
      }
      delayMicroseconds(bitTime);

    }
    msg++;

    digitalWrite(TXpin, HIGH);
    delayMicroseconds(bitTime);
  }
}

int j=0;
void loop() {
  if(!digitalRead(RXpin)) {
    j++;
  }

  if(Serial.available() > 0) {
    Serial.print("SM->BT: ");
    String msgData = Serial.readStringUntil('\n');
    sendToBluetooth(msgData.c_str());

  }
  if (j==1){
    int dec = 0;
    Serial.println("BT->SM: ");

    delayMicroseconds(bitTime/2);

    for(int i=0; i<=7; i++) {
      if(digitalRead(RXpin)) {
        dec |= (1 << i);
      } 
      delayMicroseconds(bitTime);
    }  

    char c = dec;
    Serial.print(c);

  }
}