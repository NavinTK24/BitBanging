//Bluetooth library code for enabling UART communicatoin between HC-05 and Serial Monitor bidirectionally
#include<Arduino.h>
#include <string.h>

const int TXpin = 11;
const int RXpin = 9;
const int baudRate = 9600;
const float bitTime = 1000000/baudRate;

void setup() {
    pinMode(TXpin, OUTPUT);
    digitalWrite(TXpin, HIGH);

    pinMode(RXpin, INPUT);
    digitalWrite(RXpin, HIGH);
    Serial.begin(9600);
}

void loop() {
    if(Serial.available()) {
        Serial.println("SM -> BT : ");
        String msgData = Serial.readStringUntil('\n');
        sendToBluetooth(msgData.c_str());
        Serial.print(msgData);
    }

    Serial.print("BT -> SM :");
    if(digitalRead(RXpin) == LOW) {
        char receivedData = readByByte();
        Serial.print(receivedData);
    }
    Serial.println("");
}

void sendToBluetooth (const char* msg) {
    while(*msg) {
        sendByByte(*msg);
        msg++;
    } 
    
    sendByByte('\r'); //cariage return
    sendByByte('\n');
}

void sendByByte(char c) {
    //start bit
    digitalWrite(TXpin, LOW);
    delayMicroseconds(bitTime);
    //data bit
    for(int i=0; i<8; i++) {
        if(c & (1<<i)) {
            digitalWrite(TXpin, HIGH);
        } else {
            digitalWrite(TXpin, LOW);
        }
        delayMicroseconds(bitTime);
    }
    //stop bit
    digitalWrite(TXpin, HIGH);
    delayMicroseconds(bitTime);
    
}

char readByByte() {
    char c = 0;
    delayMicroseconds(bitTime/2);

    for(int i=0; i<8; i++) {
        if(digitalRead(RXpin)) {
            c |= (1<<i);
        }
        delayMicroseconds(bitTime);
    }

    return c;
}
