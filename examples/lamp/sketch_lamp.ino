//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "SettingsGyver.h"
#include "GyverDBFile.h"
#include "LittleFS.h"
#include <FastLED.h>

#include "GestureXY.h"
#include "ParsingXY.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);
GestureXY g;

DB_KEYS(
  kk,
  ssid,
  pass,
  ssid1,
  pass1,
  host,
  port,
  local,
  remote,
  header,
  serial,
  brightness,
  state,
  stateGest,
  tempGest,
  plusBrightnessGest,
  plusBrightness,
  minusBrightnessGest,
  nextGest,
  previousGest,
  offScenes,
  onScenes,
  sep);
;

GyverDBFile db(&LittleFS, "🔦 LampXY.db");
SettingsGyver sett((String) "🏮 LampXY", &db);

struct gestureData {
  byte scene = 0;
  String scenes[4] = { "Цветной", "Холодный", "Нормальный", "Тёплый" };
  byte colors[8][3]{
    { 255, 0, 0 },
    { 255, 110, 0 },
    { 255, 255, 0 },
    { 0, 255, 0 },
    { 0, 255, 110 },
    { 0, 0, 255 },
    { 255, 0, 110 },
    { 255, 0, 255 }
  };
  byte color = 6;
} gData;

#define PIN 13
#define NUM 64
#define TYPE WS2812
#define ORDER GRB

CRGB leds[NUM];

void setup() {
  db_init();
  Serial.begin(db[kk::serial]);
  Serial.setTimeout(50);
  FastLED.addLeds<TYPE, PIN, ORDER>(leds, NUM);
  FastLED.setBrightness(db[kk::brightness]);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 800);

  for (byte i = 0; i < NUM; i++) {
    leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
  }
  FastLED.show();
  bool flag = false;
  byte timer = 0;
  WiFi.begin(db[kk::ssid], db[kk::pass]);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (!flag) timer++;
    if (timer > 15 and !flag) {
      flag = true;
      WiFi.disconnect();
      WiFi.begin(db[kk::ssid1], db[kk::pass1]);
    }
  }
  Serial.println("Подключился");
  for (int i = 0; i < NUM; i++) leds[i].setRGB(0, 0, 0);
  FastLED.show();
  sett.begin();
  sett.config.theme = sets::Colors::Green;
  sett.onBuild(build);
  mqtt.setServer(db[kk::host].c_str(), db[kk::port]);
  mqtt.setCallback(callback);
  mqtt.setSocketTimeout(1000);
  connectMQTT();
  Serial.println(WiFi.localIP());
}

void loop() {
  mqttTick();
  sett.tick();
}

void light() {
  for (int i = 0; i < NUM; i++) {
    switch (gData.scene) {
      case 0:
        leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
        break;
      case 1:
        leds[i].setRGB(0, 213, 255);
        break;
      case 2:
        leds[i].setRGB(255, 255, 255);
        break;
      case 3:
        leds[i].setRGB(255, 200, 0);
        break;
    }
  }
  FastLED.show();
}
