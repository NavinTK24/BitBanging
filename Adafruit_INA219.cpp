#include <Arduino.h>
#include <string.h>
#define SDApin 9
#define SCLpin 11
String ADDRESS1 = "10000001"; // INA219 address(0x40) for reading

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

bool ReadACK() {
    pinMode(SDApin, INPUT);
    digitalRead(SDApin);
    delayMicroseconds(5);
    digitalWrite(SCLpin, HIGH);
    bool ack = !digitalRead(SDApin);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);
    delayMicroseconds(5);

    return ack;
}

bool WriteACK() {
    pinMode(SDApin, OUTPUT);
    digitalWrite(SDApin, LOW);
    delayMicroseconds(5);
    digitalWrite(SCLpin, HIGH);
    bool ack = !digitalRead(SDApin);
    delayMicroseconds(5);
    digitalWrite(SCLpin, LOW);
    delayMicroseconds(5);

    return ack;
}

int twoPower(int n) {
    int result = 1;
    for(int i=0; i<n; i++) {
        result*=2;
    }

    return result;
}

void i2cWriteData(char data) {
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
    int dec = 0;
    pinMode(SDApin, INPUT_PULLUP);
    delayMicroseconds(5);
    pinMode(SCLpin, OUTPUT);
    
    for(int i=0; i<8; i++) {
        digitalRead(SDApin);
        if(digitalRead(SDApin)) {
            dec += twoPower(7-i);
        } 
        delayMicroseconds(5);
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);

        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
    }
    if(!ReadACK()) {
        Serial.println("\n\tNACK");
    }
    return dec;
}

void charConversion(const char* msg) {
    while(*msg) {
        i2cWriteData(*msg);
        msg++;
    }
}

void SlaveAdressWrite (String ADDRESS) { 
    pinMode(SCLpin, OUTPUT);
    pinMode(SDApin, OUTPUT);
    for(int i=0; i<8; i++) {
        char v = ADDRESS[i];
        digitalWrite(SDApin, ((v=='1')? HIGH : LOW));
        delayMicroseconds(5);
        digitalWrite(SCLpin, HIGH);
        delayMicroseconds(5);
        digitalWrite(SCLpin, LOW);
        delayMicroseconds(5);
        // digitalWrite(SDApin, LOW);
        // delayMicroseconds(5);
    }
    
    if(!WriteACK()) {
        Serial.println("\n\tNACK");
    }
}

void loop() {
    delay(10);
    float result = 0;
    Serial.print("\r");

    if(Serial.available()) {
        String sendData = Serial.readStringUntil('\n');
        i2cStart();
        SlaveAdressWrite("10000000");
        charConversion(sendData.c_str());
        i2cEnd();
        delay(10);

    } else {
        i2cStart();
        Serial.println("Start over");
        SlaveAdressWrite("10000000"); //address with write 0x80
        ReadACK();
        SlaveAdressWrite("00000100"); //for current reading 0x04
        ReadACK();
        Serial.println("Configuration over");

        i2cStart();
        SlaveAdressWrite("10000001"); //address with read 0x81
        ReadACK();
        result += i2cReadData(); //MSB with ACK
        WriteACK();
        Serial.println("Reading over");
        result += i2cReadData(); //LSB with NACK
        i2cEnd();

        // MASTER raw to real value conversion
        Serial.print(result/10);
        Serial.print(" mA");
    }
    delay(2000);
}
