#include <Arduino.h>
#include <string.h>
#define SDApin 9
#define SCLpin 11
String ADDRESS1 = "10000001"; //0x40 for reading

void setup() {
    Serial.begin(9600);
    pinMode(SCLpin, INPUT_PULLUP);
    pinMode(SDApin, INPUT_PULLUP);
}

void i2cStart() {
    pinMode(SCLpin, OUTPUT);
    pinMode(SDApin, OUTPUT);
    digitalWrite(SCLpin, HIGH);
    digitalWrite(SDApin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SDApin, LOW);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);
    delayMicroseconds(5);
}

void i2cEnd() {
    pinMode(SCLpin, OUTPUT);
    pinMode(SDApin, OUTPUT);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SDApin, HIGH);
    delayMicroseconds(5);
}

int twoPower(int n) {
    int result = 1;
    for(int i=0; i<n; i++) {
        result*=2;
    }

    return result;
}

int i2cWriteData(char data) {
    pinMode(SCLpin, OUTPUT);
    pinMode(SDApin, OUTPUT);

    for(int i=0; i<8; i++) {
        digitalWrite(SDApin, ((data & (1<<(7-i)))? HIGH : LOW));
        delayMicroseconds(5);
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
        digitalWrite(SDApin, LOW);
        delayMicroseconds(5);
    }
    pinMode(SDApin, INPUT);
    digitalRead(SDApin);
    delayMicroseconds(5);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);
}

int i2cReadData() {
    char c = 0;
    int dec = 0;
    pinMode(SDApin, INPUT_PULLUP);
    delayMicroseconds(5);
    pinMode(SCLpin, OUTPUT);
    
    for(int i=0; i<8; i++) {
        digitalRead(SDApin);
        c<<=1;
        if(digitalRead(SDApin)) {
            c |= 1;
            dec += twoPower(7-i);
        } 
        delayMicroseconds(5);
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);

        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
    }
    // pinMode(SDApin, INPUT_PULLUP);
    // delayMicroseconds(2);
    pinMode(SDApin, OUTPUT);
    digitalWrite(SDApin, LOW);
    delayMicroseconds(5);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);

    return dec+48;
}

void charConversion(const char* msg) {
    while(*msg) {
        i2cWriteData(*msg);
        msg++;
    }
}

void SlaveAdressWrite (String ADDRESS1) { 
    pinMode(SCLpin, OUTPUT);
    digitalWrite(SCLpin, LOW);
    pinMode(SDApin, OUTPUT);
    for(int i=0; i<ADDRESS1.length(); i++) {
        char v = ADDRESS1[i];
        digitalWrite(SDApin, ((v=='1')? HIGH : LOW));
        delayMicroseconds(5);
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
        digitalWrite(SDApin, LOW);
        delayMicroseconds(5);
    }
    pinMode(SDApin, INPUT);
    digitalRead(SDApin);
    delayMicroseconds(5);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);
}


void loop() {
    delay(10);

    if(Serial.available()) {
        String sendData = Serial.readStringUntil('\n');
        i2cStart();
        SlaveAdressWrite("10000000");
        charConversion(sendData.c_str());
        i2cEnd();
        delay(10);

    } else {
        i2cStart();
        SlaveAdressWrite("10000001");
        char receivedData = i2cReadData();
        Serial.print(receivedData);
        i2cEnd();
    }

    delay(2000);
}
