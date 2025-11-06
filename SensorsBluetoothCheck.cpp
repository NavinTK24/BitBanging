#include <Arduino.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"


#define LED_PIN 2  // onboard LED

const char* targetAddress = "00:25:05:31:13:3d";  // HC-05 MAC

void btCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
  if (event == ESP_BT_GAP_DISC_RES_EVT) {
    char bda_str[18];
    sprintf(bda_str, "%02x:%02x:%02x:%02x:%02x:%02x",
            param->disc_res.bda[0], param->disc_res.bda[1], param->disc_res.bda[2],
            param->disc_res.bda[3], param->disc_res.bda[4], param->disc_res.bda[5]);

    int rssi = 0;
    for (int i = 0; i < param->disc_res.num_prop; i++) {
      if (param->disc_res.prop[i].type == ESP_BT_GAP_DEV_PROP_RSSI) {
        rssi = *(int8_t*)param->disc_res.prop[i].val;
      }
    }

    if (strcmp(bda_str, targetAddress) == 0) {
      Serial.print("📡 Target device found: ");
      Serial.print(bda_str);
      Serial.print(" | RSSI: ");
      Serial.println(rssi);

      if (rssi > -50) {        // very close
        Serial.println("➡️ Very close!");
        digitalWrite(LED_PIN, HIGH);
      } else if (rssi > -70) { // nearby
        Serial.println("➡️ Nearby");
        digitalWrite(LED_PIN, LOW);
      } else {                  // far
        Serial.println("➡️ Far away");
        digitalWrite(LED_PIN, LOW);
      }
    }
  } else if (event == ESP_BT_GAP_DISC_STATE_CHANGED_EVT) {
    if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
      Serial.println("🔍 Scanning finished!");
      esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0); // restart scan
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("🚀 Starting Classic Bluetooth proximity detection...");

  if (!btStart()) {
    Serial.println("❌ Bluetooth start failed!");
    return;
  }

  esp_bluedroid_init();
  esp_bluedroid_enable();
  esp_bt_gap_register_callback(btCallback);
  esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void loop() {
  // Nothing needed here; callback handles everything
}
