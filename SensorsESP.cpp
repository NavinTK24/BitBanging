#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHTesp.h>
#include <MPU6050_light.h>

// ----- WiFi and Telegram -----
const char* ssid = "LAVA LXX506";
const char* password = "aabbccdd";
#define BOT_TOKEN "8088677440:AAHHKj5eTF0TnZMLNYwA-xKOOa927xIHyj0" // Telegram Bot Token
#define CHAT_ID "6395140537"                                       // Your chat ID

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// DHT11 setup
#define DHTPIN 19
DHTesp dht;

// MPU6050 setup
MPU6050 mpu(Wire);

// Pulse sensor setup
#define PULSE_PIN 18  // Connect Pulse Sensor Signal pin here
int pulseValue = 0;
int lastValue = 0;
unsigned long lastBeatTime = 0;
float bpm = 0;

// For Telegram timing
unsigned long lastTelegramTime = 0;
const long telegramInterval = 5000; // send every 5 sec

void setup() {
  Serial.begin(115200);

  // DHT11 setup
  dht.setup(DHTPIN, DHTesp::DHT11);

  // MPU6050 setup
  Wire.begin(21, 22);
  mpu.begin();
  delay(1000);
  mpu.calcOffsets();

  // WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");

  // Allow insecure connection (for Telegram HTTPS)
  client.setInsecure();
}

void loop() {
  // ---- DHT11 ----
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  // ---- MPU6050 ----
  mpu.update();
  float angleX = mpu.getAngleX();
  float angleY = mpu.getAngleY();
  String orientation;
  if (abs(angleX) < 30 && abs(angleY) < 30) orientation = "Standing";
  else orientation = "Lying down";

  // ---- Pulse Sensor ----
  pulseValue = analogRead(PULSE_PIN);  // raw ADC value (0–4095)
  Serial.print("Pulse Sensor Value: ");
  Serial.print(pulseValue);

  // Detect beat when crossing threshold
  if (pulseValue > 600 && lastValue <= 600) {
    unsigned long now = millis();
    unsigned long interval = now - lastBeatTime;
    lastBeatTime = now;

    if (interval > 300 && interval < 2000) { // valid range for 30–200 BPM
      bpm = 60000.0 / interval;
      Serial.print("  💓 Beat detected! BPM: ");
      Serial.print(bpm);
    }
  }
  Serial.println();
  lastValue = pulseValue;

  // ---- Send Data to Telegram ----
  if (millis() - lastTelegramTime > telegramInterval) {
    String message = "🌡 *Temperature:* " + String(temperature) + " °C\n";
    message += "💧 *Humidity:* " + String(humidity) + " %\n";
    message += "📐 *Orientation:* " + orientation + "\n";
    message += "📊 *Pulse Reading:* " + String(pulseValue) + "\n";
    message += "❤️ *Heart Rate:* " + String((int)bpm) + " BPM\n";

    bot.sendMessage(CHAT_ID, message, "Markdown");
    lastTelegramTime = millis();
  }

  delay(100); // small delay for stability
}
