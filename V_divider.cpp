#include<Arduino.h>

void setup() {
    pinMode(A0, INPUT);
    Serial.begin(9600);
}

void loop() {
    float Vread = (analogRead(A0))/204.8;
    float Vinput = Vread*(120/590);
    Serial.print("Voltage: ");
    Serial.print(Vread);
    Serial.print(" V      ");

    float Iread = Vread/120;
    Serial.print("Current: ");
    Serial.print(Iread);
    Serial.println(" mA");
    
    delay(1000); 
}