#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <BH1750.h>

Adafruit_INA219 ina219;
BH1750 bh1750;

const int relayPin = 7;

void setup(void) {
  pinMode(9, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  Wire.begin();

  while (!Serial) {
      delay(1);
  }
    
  Serial.println("Hello World");
  
  if(bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialized");
  } else {
    Serial.println("Error initializing BH1750");
    delay(500);
  }

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { 
        Serial.println("Processing");
        delay(10); }
  }

}

float relayNO () {
  float current_mA = 0;
  current_mA = ina219.getCurrent_mA();
  return current_mA;
}

float relayNC () {
  float voltage_V = 0;
  voltage_V = ((analogRead(A1) *5.0) /1023) *(1240.0/560.0); //as measuring across 560 ohm 
  return voltage_V;

}

void loop(void) {
  float lux = bh1750.readLightLevel();
  float irradiance = lux * 0.0079;
  Serial.print("Irradiance: "); Serial.print(lux, 4); Serial.print(" lx       ");
  Serial.print("(Irradiance: "); Serial.print(irradiance); Serial.println(" W/m2)");
  delay(500);

  Serial.print("Voltage:       "); Serial.print(relayNC(), 4);          Serial.println("V");
  digitalWrite(relayPin, HIGH);
  delay(500);
  Serial.print("Current:       "); Serial.print(relayNO(), 4); Serial.println(" mA");
  digitalWrite(relayPin, LOW);
  Serial.println("");

  delay(1000);
}