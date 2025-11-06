#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHTesp.h>
#include <MPU6050_light.h>
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"

// ===== WiFi + Telegram =====
const char* ssid = "****";
const char* password = "***";
#define BOT_TOKEN "****"
#define CHAT_ID_USER "***"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ===== Sensors =====
#define DHTPIN 19
#define PULSE_PIN 32
#define FLEX_PIN 34
#define BUZZER_PIN 25
DHTesp dht;
MPU6050 mpu(Wire);

// ===== Pulse detection =====
int pulseValue = 0;
int lastValue = 0;
unsigned long lastBeatTime = 0;
float bpm = 0;

// ===== Thresholds =====
const int PULSE_THRESHOLD = 2000;
const float TEMP_HIGH = 39.0;
const float TEMP_VERY_HIGH = 40.0;
const float TEMP_LOW = 20.0;
const float PULSE_HIGH = 120.0;
const float PULSE_LOW = 60.0;
const int FLEX_THRESHOLD = 2000;

// ===== Bluetooth (HC-05) =====
const char* targetAddress = "****";
int currentRSSI = -999;
String proximityStatus = "Unknown";

void btCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
  if (event == ESP_BT_GAP_DISC_RES_EVT) {
    char bda_str[18];
    sprintf(bda_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            param->disc_res.bda[0], param->disc_res.bda[1], param->disc_res.bda[2],
            param->disc_res.bda[3], param->disc_res.bda[4], param->disc_res.bda[5]);

    int rssi = 0;
    for (int i = 0; i < param->disc_res.num_prop; i++) {
      if (param->disc_res.prop[i].type == ESP_BT_GAP_DEV_PROP_RSSI)
        rssi = *(int8_t*)param->disc_res.prop[i].val;
    }

    if (strcmp(bda_str, targetAddress) == 0) {
      currentRSSI = rssi;
      if (rssi > -50) proximityStatus = "Very close";
      else if (rssi > -70) proximityStatus = "Nearby";
      else proximityStatus = "Far";
    }
  }
}

void checkBluetoothProximity() {
  currentRSSI = -999;
  proximityStatus = "Not found";
  esp_bt_gap_register_callback(btCallback);
  esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 5, 0);
  delay(500);
  esp_bt_gap_cancel_discovery();
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  dht.setup(DHTPIN, DHTesp::DHT11);
  Wire.begin(21, 22);
  mpu.begin();
  delay(1000);
  mpu.calcOffsets();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setInsecure();

  btStart();
  esp_bluedroid_init();
  esp_bluedroid_enable();

  Serial.println("✅ System started");
}

// ===== Main loop =====
void loop() {
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  mpu.update();
  float angleX = mpu.getAngleX();
  float angleY = mpu.getAngleY();
  String orientation = (abs(angleX) < 30 && abs(angleY) < 30) ? "Standing" : "Lying down";

  int flexValue = analogRead(FLEX_PIN);

  // ----- Real Pulse Detection -----
  pulseValue = analogRead(PULSE_PIN);
  unsigned long now = millis();
  if (pulseValue > PULSE_THRESHOLD && lastValue <= PULSE_THRESHOLD) {
    unsigned long interval = now - lastBeatTime;
    lastBeatTime = now;
    if (interval > 300 && interval < 2000) bpm = 60000.0 / interval;
  }
  lastValue = pulseValue;
  if (millis() - lastBeatTime > 2000) bpm = 0;

  // ----- Telegram Requests -----
  int newMsg = bot.getUpdates(bot.last_message_received + 1);
  for (int i = 0; i < newMsg; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (text == "/status") {
      checkBluetoothProximity();

      String message = "🌡 Temp: " + String(temperature) + " °C\n";
      message += "💧 Humidity: " + String(humidity) + " %\n";
      message += "📐 Orientation: " + orientation + "\n";
      message += "💓 Pulse: " + String((int)bpm) + " BPM\n";
      message += "🦴 Flex: " + String(flexValue) + "\n";
      message += "📶 Proximity: " + proximityStatus + " (RSSI " + String(currentRSSI) + ")\n";
      bot.sendMessage(chat_id, message, "Markdown");
    }
  }



// ----- Automatic Alerts -----
static unsigned long lastAlert = 0;
static bool fallDetected = false;  // remembers if a fall was detected previously

if (millis() - lastAlert > 500) {   // check every 5ms seconds
  String alertMsg = "";

  // ---- Temperature Alerts ----
  if (temperature > TEMP_VERY_HIGH)
    alertMsg += "🔥 *Severe Heat!* " + String(temperature) + " °C\n";
  else if (temperature > TEMP_HIGH)
    alertMsg += "⚠️ *Fever!* " + String(temperature) + " °C\n";
  else if (temperature < TEMP_LOW)
    alertMsg += "❄️ *Too Cold!* " + String(temperature) + " °C\n";

  // ---- Pulse Alerts ----
  if (bpm == 0)
    alertMsg += "⚠️ *Pulse not detected!* Check sensor.\n";
  else if (bpm > PULSE_HIGH)
    alertMsg += "❤️ *High Pulse!* " + String((int)bpm) + "\n";
  else if (bpm < PULSE_LOW && bpm > 0)
    alertMsg += "💓 *Low Pulse!* " + String((int)bpm) + "\n";

  // ---- Fall Detection ----
  const int FLEX_GOOD_BEND = 700; // threshold for good bend
  bool isLying = (orientation == "Lying down");
  bool isBent = (flexValue > FLEX_GOOD_BEND);
  bool isClose = false;

  if (isLying && isBent) {
    checkBluetoothProximity();
    isClose = (proximityStatus == "Very close");
  }

  // === FALL DETECTED ===
  if (isLying && isBent && isClose && !fallDetected) {
    fallDetected = true; // prevent repeated alert
    alertMsg += "🚨 *Fall Detected!* It has fallen on something, high bending detected!\n";

    // Buzzer ON for 5 seconds
    digitalWrite(BUZZER_PIN, HIGH);
    unsigned long buzzerStart = millis();
    while (millis() - buzzerStart < 5000) {
      delay(10);
    }
    digitalWrite(BUZZER_PIN, LOW);
  }

  // === RECOVERY DETECTED ===
  else if (fallDetected && (!isLying || !isBent || !isClose)) {
    fallDetected = false;
    alertMsg += "✅ *Position Normal!* Bending normal or the animal has stood up. Safe now.\n";
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ---- Send Alert ----
  if (alertMsg != "") bot.sendMessage(CHAT_ID_USER, alertMsg, "Markdown");

  lastAlert = millis();
}

delay(100);
}
