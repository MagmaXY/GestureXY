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
  name,
  scene, 
  sep, 
  header, 
  searchs,
  serial,
  brightness,
  turn,
  vibration,
  state,
  reaction,
  quit);
;

WiFiClient espClient;
PubSubClient mqtt(espClient);
GyverDBFile db(&LittleFS, "🤘 GestureXY.db");
SettingsGyver sett("🤟 GestureXY", &db);
GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
Button btn(D5);
VibroXY vibro(D6, 50);
GestureXY g(100);

struct gestureXY {
  byte name = 0;
  byte scene = 0;
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
  vibro.on(1000);
}

void loop() {
  btnTick();
  mqttTick();
  sett.tick();
  g.tick();
  gestTick();
  vibro.tick();
}


void btnTick() {
  btn.tick();
  if (btn.releaseHold()) ESP.restart();
  if (btn.click(1)) {
    setupGraph();
    oled.print(WiFi.localIP());
  }
}
