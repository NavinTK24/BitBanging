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
    delayMicroseconds(5);
    digitalWrite(SDApin, LOW);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);
}

void i2cEnd() {
    digitalWrite(SDApin, LOW);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SDApin, HIGH);
    delayMicroseconds(5);

    pinMode(SCLpin, INPUT_PULLUP);
    pinMode(SDApin, INPUT_PULLUP);
}

uint8_t i2cWriteData(uint8_t data) {
    pinMode(SCLpin, OUTPUT);
    pinMode(SDApin, OUTPUT);
    digitalWrite(SDApin, HIGH);
    digitalWrite(SCLpin, HIGH);

    for(int i=0; i<8; i++) {
        digitalWrite(SCLpin, LOW);
        digitalWrite(SDApin, (data & (1<<(7-i)))? HIGH : LOW);
        delayMicroseconds(5);
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);
    }
    digitalWrite(SCLpin, LOW);
    pinMode(SDApin, INPUT);
    delayMicroseconds(5);
    digitalWrite(SCLpin, HIGH);
    delayMicroseconds(5);
    uint8_t ack = digitalRead(SDApin);
    pinMode(SDApin, OUTPUT);

    return ack;

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
        delay(5);
    }
}


void loop() {
    delay(10);

    i2cStart();
    if(i2cWriteData((ADDRESS1 << 1) | 0) == 0) { //checks if ready for writing
        if(Serial.available()) {
            String sendData = Serial.readStringUntil('\n');
            charConversion(sendData.c_str());
        }  
    } else {
        Serial.println("Device not responding");
    }
    i2cEnd();

    delay(10);

    i2cStart();
    if(i2cWriteData( (ADDRESS1 <<1 ) | 1) == 0) { //checks if ready for reading
        char receivedData = i2cReadData();
        Serial.print(receivedData);
    } else {
        Serial.println("Device not responding");
    } 
    
    i2cEnd();
}
