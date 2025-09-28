#include<Arduino.h>
#include<string.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

RF24 radio(9,8);

const byte address1[6] = "10001";
const byte address2[6] = "10002";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(address1); //TX
  radio.openReadingPipe(0,address2); //RX
  radio.startListening();
  Serial.print("Me: ");

}

String msg = "";

void loop() {
  if(Serial.available()>0) {
    char c = Serial.read();
    Serial.print(c);
    msg += c;

    if(c == '\n') {
      Serial.print("Me: ");
      radio.stopListening();
      radio.write(msg.c_str(),msg.length()+1);
      radio.startListening();
      msg = "";
    }

  }

  char lastMessage[32] = "";

  if(radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    if(strcmp(text, lastMessage) != 0) {
      Serial.print("They: ");
      Serial.println(text);
    }
  }


}