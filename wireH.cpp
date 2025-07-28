#include <Arduino.h>
#include <string.h>
#define SDApin 9
#define SCLpin 11
#define ADDRESS1 0x40

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

uint8_t i2cWriteData(uint8_t data) {
    pinMode(SCLpin, OUTPUT);
    pinMode(SDApin, OUTPUT);

    for(int i=0; i<8; i++) {
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(SDApin, (data & (1<<(7-i)))? HIGH : LOW);
        delayMicroseconds(5);
        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
    }
    pinMode(SDApin, INPUT);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    digitalRead(SDApin);

    delayMicroseconds(5);
}

char i2cReadData() {
    pinMode(SDApin, INPUT_PULLUP);
    pinMode(SCLpin, OUTPUT);
    char c = 0;
    
    for(int i=0; i<8; i++) {
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);
        c<<=1;
        if(digitalRead(SDApin)) {
            c |= 1;
        }

        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
    }

    pinMode(SDApin, OUTPUT);
    digitalWrite(SCLpin, HIGH);
    digitalWrite(SDApin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);

    return c;
}

void charConversion(const char* msg) {
    while(*msg) {
        i2cWriteData(*msg);
        msg++;
    }
}


void loop() {
    delay(10);

    if(Serial.available()) {
        String sendData = Serial.readStringUntil('\n');
        i2cStart();
        i2cWriteData((ADDRESS1 << 1) | 0); 
        charConversion(sendData.c_str());
        i2cEnd();
        delay(10);

    } else {
        i2cStart();
        i2cWriteData( (ADDRESS1 <<1 ) | 1); 
        char receivedData = i2cReadData();
        Serial.print(receivedData);
        i2cEnd();
    }

    delay(2000);
}
