#include "ParsingXY.h"

void mqttTick() {
  if (!gData.mqtt or WiFi.status() != WL_CONNECTED) return;
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}

void connectMQTT() {
  static uint32_t timer;
  if (millis() - timer >= 1000) {
    timer = millis();
    String id("GestureXY-");
    id += String(random(0xffffff), HEX);
    if (mqtt.connect(id.c_str())) mqtt.subscribe(getName(0).c_str());
  }
}

void callback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  String str = (char*)payload;
  if (!str.startsWith(db[kk::header])) return;
  Serial.println(str);
  String data = str.substring(db[kk::header].length());
  db[kk::state] = true;
  cursorGraph();
  oled.print(data);
  vibro.on(500);
  gData.timer = millis();
}

void sendGest() {
  String s;
  s += String(db[kk::header]);
  s += g.getRUGest();
  mqtt.publish(getName(gData.name).c_str(), s.c_str());
}

void sendScene() {
  String s;
  s += String(db[kk::header]);
  s += getScene();
  for (byte i = 1; i < getNamesCount(); i++) {
    mqtt.publish(getName(i).c_str(), s.c_str());
  }
}

String getScene() {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::scenes].c_str(), gData.scene);
}

byte getScenesCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::scenes].c_str());
}

String getName(byte name) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::names].c_str(), name);
}

byte getNamesCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::names].c_str());
}
