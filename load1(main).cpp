#include <Arduino.h>
#include <ArduinoSTL.h>
#include <map>
#include <vector>

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define IN5 6
#define IN6 7
#define IN7 8
#define IN8 9
#define IN9 10

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  pinMode(IN9, OUTPUT);
  Serial.begin(9600);
}

float A1() {
  float sum = 0;
  for (int i = 0; i < 1000; i++) {
    float I = ((analogRead(A0) * (5.0 / 1023.0)) - 2.5) * 1000 /
              100.0;  // 100 mV/A for ACS712-20A
    sum += I * I;
  }

  return (sqrt(sum / 1000), 3);
}

float A2() {
  float sum = 0;
  for (int i = 0; i < 1000; i++) {
    float I = ((analogRead(A0) * (5.0 / 1023.0)) - 2.5) * 1000 /
              66.0;  // 66 mV/A for ACS712-30A
    sum += I * I;
  }

  return (sqrt(sum / 1000), 3);
}

float A3() {
  float sum = 0;
  for (int i = 0; i < 1000; i++) {
    float I = ((analogRead(A0) * (5.0 / 1023.0)) - 2.5) * 1000 /
              100.0;  // 100 mV/A for ACS712-20A
    sum += I * I;
  }

  return (sqrt(sum / 1000), 3);
}

void loop() {
  std :: map<int, std:: vector<float>> phaseLoads;

  phaseLoads[1] = {3,A1(),0};  //R
  phaseLoads[2] = {3, 0  ,0};  //Y
  phaseLoads[3] = {0, 0  ,0};  //B

  float phaseTotals[3] = {0, 0, 0};
  for(int i=0; i<phaseLoads[1].size(); i++) {
    phaseTotals[0] += phaseLoads[1][i]; 
    phaseTotals[1] += phaseLoads[2][i]; 
    phaseTotals[2] += phaseLoads[3][i]; 
  }

  Serial.print("R: ");
  Serial.print(phaseTotals[0]);
  Serial.print("A, ");

  Serial.print("Y: ");
  Serial.print(phaseTotals[1]);
  Serial.print("A, ");

  Serial.print("B: ");
  Serial.print(phaseTotals[2]);
  Serial.print("A ");

  int totalLoad = phaseTotals[0] + phaseTotals[1] + phaseTotals[2];
  int balanced[3];
  balanced[0] = totalLoad / 3;
  balanced[1] = (totalLoad - (balanced[0])) / 2;
  balanced[2] = (totalLoad - (balanced[0] + balanced[1]));

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (phaseTotals[i] == balanced[j]) {
        int n = balanced[i];
        balanced[i] = balanced[j];
        balanced[j] = n;
      }
    }
  }

  int phaseNeeds[3];
  for (int i = 0; i < 3; i++) {
    phaseNeeds[i] = balanced[i] - phaseTotals[i];
  }

  std ::vector<int> positives;
  std ::vector<int> negatives;

  for (int i = 0; i < 3; i++) {
    if (phaseNeeds[i] < 0) {
      negatives.push_back(i);
    }
    if (phaseNeeds[i] > 0) {
      positives.push_back(i);
    }
  }
  for (int p : positives) {
    for (int n : negatives) {
      int target = phaseNeeds[p];
      int toGive = -phaseNeeds[n];
      int search = toGive;
      int state = true;
      int m = 0;

      if (phaseNeeds[p] < (-phaseNeeds[n])) {
        toGive = target;
        search = toGive;
      }

      while (state) {
        bool status = true;
        for (int i = 0; i < phaseLoads[n+1].size(); i++) {
          if (phaseLoads[n + 1][i] == search) {
            if((n+1)==1) {
              
            }

            state = false;
            phaseLoads[n + 1].erase(phaseLoads[n + 1].begin() + i);
            if (m < toGive) {
              m += search;
              search = toGive - m;
            }
            phaseNeeds[n] = phaseNeeds[n] + phaseLoads[n + 1][i];
            phaseNeeds[p] = phaseNeeds[p] - toGive;
          }
        }
        if (state) {
          search = search - 1;
        }
        if (search <= 0) {
          state = false;
        }
      }
    }
  }

}