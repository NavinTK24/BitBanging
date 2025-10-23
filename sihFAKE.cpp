#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(30, OUTPUT);
  digitalWrite(34, HIGH);

  // First line static
  lcd.setCursor(0, 0);
  lcd.print("R: 3A Y:3A B:0A");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("R: 7A Y:3A B:0A");
  delay(1000);
  // Step 1: 5-second countdown on second line
  for (int i = 5; i > 0; i--) {
    lcd.setCursor(0, 1);       // second line
    lcd.print("Time: ");
    lcd.print(i);
    lcd.print("s  ");          // extra spaces to clear previous number
    delay(1000);
  }

  // Step 2: Balancing message for 1 second
  lcd.setCursor(0, 1);
  lcd.print("balancing     ");  // pad with spaces
  delay(1000);

  // Step 3: Final balanced values
  lcd.setCursor(0, 0);
  lcd.print("R: 3A Y:3A B:3A");
  lcd.setCursor(0, 1);
  lcd.print("Done!        ");   // pad with spaces

  delay(5000);  // hold final display

  // Optional: repeat sequence
}

void loop() {
  
}
