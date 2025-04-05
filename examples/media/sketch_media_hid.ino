#define GT_PRINT_MODE
#define GT_MICRO_TX

#include <GyverTransfer.h>
GyverTransfer<13, GT_TX, 5000> tx;

#include <WiFi.h>
#include <PubSubClient.h>
#include "SettingsGyver.h"
#include "GyverDBFile.h"
#include "LittleFS.h"
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
  pauseGest,
  muteGest,
  backspaceGest,
  nextGest,
  previousGest,
  upGest,
  downGest,
  powerGest,
  sleepGest,
  wakeGest,
  pauseScenes,
  sleepScenes,
  powerScenes,
  wakeScenes,
  sep,
  str);
;

GyverDBFile db(&LittleFS, "🎶 MediaXY.db");
SettingsGyver sett((String) "🎵 MediaXY", &db);

void setup() {
  db_init();
  Serial.begin(db[kk::serial]);
  Serial.setTimeout(50);
  WiFi.begin(db[kk::ssid], db[kk::pass]);

  bool flag = false;
  byte timer = 0;

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
  sett.begin();
  sett.config.theme = sets::Colors::Green;
  sett.onBuild(build);
  mqtt.setServer(db[kk::host].c_str(), db[kk::port]);
  mqtt.setCallback(callback);
  connectMQTT();
  Serial.println(WiFi.localIP());
  tx.print(WiFi.localIP());
}

void loop() {
  mqttTick();
  sett.tick();
}
