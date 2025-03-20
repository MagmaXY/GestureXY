#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "SettingsGyver.h"
#include "GyverDBFile.h"
#include "LittleFS.h"
#include "GyverOLED.h"
#include "EncButton.h"

#include "VibroXY.h"
#include "GestureXY.h"

DB_KEYS(
  kk,
  ssid,
  pass,
  ssid1,
  pass1,
  host,
  port,
  connect,
  names,
  scenes,
  sep,
  header,
  ons,
  searchs,
  serial,
  brightness,
  turn,
  vibration,
  power,
  state,
  reaction,
  quit,
  stateGest,
  vibrationGest,
  plusBrightnessGest,
  plusBrightness,
  minusBrightnessGest,
  nextNameGest,
  previousNameGest,
  nextSceneGest,
  previousSceneGest,
  sceneGest);
;
WiFiClient espClient;
PubSubClient mqtt(espClient);
GyverDBFile db(&LittleFS, "🤘 GestureXY.db");
SettingsGyver sett("🤟 GestureXY", &db);
GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
Button btn(D5);
VibroXY vibro(D6);
GestureXY g;

struct gestureXY {
  byte name = 0;
  byte scene = 0;
  uint32_t timer = 0;
} gData;

void setup() {

  db_init();

  g.setQuit(db[kk::quit]);
  g.setReaction(db[kk::reaction]);
  g.setTurn(db[kk::turn]);
  vibro.flag(db[kk::vibration]);
  db[kk::state] = false;
  db[kk::connect] = false;

  Serial.begin(db[kk::serial]);
  Serial.setTimeout(50);

  oled.init(D2, D1);
  oled.setContrast(db[kk::brightness]);
  setupGraph();
  uint8_t error = g.init();

  if (error) {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
    oled.print("Error paj7620");
    delay(1000);
    ESP.restart();
  }

  Serial.println("WiFi");
  oled.print("WiFi");
  WiFi.begin(db[kk::ssid], db[kk::pass]);

  bool flag = false;
  byte timer = 0;

  while (WiFi.status() != WL_CONNECTED) {
    if (btn.read()) timer = 15;

    if (timer >= 15) {
      timer = 0;
      if (!flag) {
        flag = true;
        Serial.println("reWiFi");
        WiFi.disconnect();
        WiFi.begin(db[kk::ssid1], db[kk::pass1]);
        setupGraph();
        oled.print("reWiFi");
      } else {
        ESP.restart();
      }
    }
    Serial.print(".");
    oled.print(".");
    timer++;
    delay(1000);
  }
  Serial.println("Connected");
  sett.begin();
  sett.config.theme = sets::Colors::Green;
  sett.onBuild(build);
  mqtt.setServer(db[kk::host].c_str(), db[kk::port]);
  mqtt.setCallback(callback);
  mqtt.setSocketTimeout(1000);
  mqttGraph();
  connectMQTT();
  Serial.println(WiFi.localIP());
  setupGraph();
  oled.print(WiFi.localIP());
  vibro.setPower(db[kk::power]);
  vibro.on(1000);
  gData.timer = millis();
}

void loop() {
  btnTick();
  mqttTick();
  sett.tick();
  g.tick();
  gestTick();
  oledTick();
  vibro.tick();
}


void btnTick() {
  btn.tick();
  if (btn.hold()) ESP.restart();
  if (db[kk::state] and btn.click()) {
    setupGraph();
    oled.print(WiFi.localIP());
    gData.timer = millis();
  }
}

void oledTick() {
  if (!db[kk::ons]) return;
  if (millis() - gData.timer >= (uint32_t)db[kk::ons]) {
    if (db[kk::state]) Serial.println("Inactive");
    oled.clear();
    db[kk::state] = false;
  }
}
