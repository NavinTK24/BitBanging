#include<Arduino.h>
#include<string.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<math.h>

const float sensitivity = 0.066;
int offset = 512;
int samples = 10;

RF24 radio(9,8);

const byte writingAddress[6]  = "10000";
const byte readingAddress1[6] = "00001";
const byte readingAddress2[6] = "00002";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(writingAddress);
  radio.openReadingPipe(1,readingAddress1);
  radio.openReadingPipe(2,readingAddress2);
  radio.startListening();
}

void loop() {
  float load1[3] = {0,0,0};

  for(int i=0; i<samples; i++) {
    load1[0] += pow( (((analogRead(A1)*5.0)/1023)-2.5/sensitivity), 2);
    load1[1] += pow( (((analogRead(A2)*5.0)/1023)-2.5/sensitivity), 2);
    load1[2] += pow( (((analogRead(A3)*5.0)/1023)-2.5/sensitivity), 2);
  }

  load1[0] = sqrt(load1[0]/samples);
  load1[1] = sqrt(load1[1]/samples);
  load1[2] = sqrt(load1[2]/samples);

  uint8_t pipeNum;
  if(radio.available(&pipeNum)) {
    float R = 0;
    float Y = 0;
    float B = 0;
    radio.read(&R, sizeof(R)); 
    radio.read(&Y, sizeof(Y));
    radio.read(&B, sizeof(B));

    float Rphase[3] = {0,0,0};
    float Yphase[3] = {0,0,0};
    float Bphase[3] = {0,0,0};
    
    Rphase[0]=load1[0];
    Yphase[1]=load1[1];
    Bphase[2]=load1[2];

  }
}