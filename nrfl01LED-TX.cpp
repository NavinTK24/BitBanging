#include<Arduino.h>
#include<string.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

RF24 radio(9,8);

const byte address1[6] = "10001";
// const byte address2[6] = "10002";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address1); //TX
  radio.stopListening();
  Serial.print("Type 1 to ON, 0 to OFF: ");
}


void loop() {
  if(Serial.available()) {
    char input = Serial.read();

    if(input == '1' || input == '0') {
      radio.write(&input, sizeof(input));
      Serial.print("Sent: ");
      Serial.println(input);
    }
  }
}