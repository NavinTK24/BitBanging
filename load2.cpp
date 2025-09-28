#include<Arduino.h>
#include<string.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<math.h>

RF24 radio(9,8);

const byte writingAddress[6] = "00001";
const byte readingAddress[6] = "00002";
int samples = 10;

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.openWritingPipe(writingAddress);
    radio.openReadingPipe(1,readingAddress);
    radio.startListening();
}

void loop() {
    float load2[3] = {0,0,0};

    for(int i=0; i<samples; i++) {
    load1[0] += pow( (((analogRead(A1)*5.0)/1023)-2.5/sensitivity), 2);
    load1[1] += pow( (((analogRead(A2)*5.0)/1023)-2.5/sensitivity), 2);
    load1[2] += pow( (((analogRead(A3)*5.0)/1023)-2.5/sensitivity), 2);
  }

  load1[0] = sqrt(load1[0]/samples);
  load1[1] = sqrt(load1[1]/samples);
  load1[2] = sqrt(load1[2]/samples);
}