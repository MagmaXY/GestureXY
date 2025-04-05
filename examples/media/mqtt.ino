void mqttTick() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}

void connectMQTT() {
  static uint32_t timer;
  if (millis() - timer >= 1000) {
    timer = millis();
    String id("GestureXY-");
    id += String(random(0xffffff), HEX);
    if (mqtt.connect(id.c_str())) mqtt.subscribe(db[kk::local].c_str());
  }
}

void callback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  String str = (char*)payload;
  if (!str.startsWith(db[kk::header])) return;
  Serial.println(str);
  String data = str.substring(db[kk::header].length());
  Serial.println(data);
  if (data == db[kk::pauseGest]) {
    tx.print("MEDIA_PAUSE");
  } else if (data == db[kk::muteGest]) {
    tx.print("MEDIA_VOLUME_MUTE");
  } else if (data == db[kk::backspaceGest]) {
    tx.print("KEY_BACKSPACE");
  } else if (data == db[kk::githubGest]) {
    tx.print("https://github.com/MagmaXY/GestureXY/");
  } else if (data == db[kk::nextGest]) {
    tx.print("MEDIA_NEXT");
  } else if (data == db[kk::previousGest]) {
    tx.print("MEDIA_PREVIOUS");
  } else if (data == db[kk::upGest]) {
    tx.print("MEDIA_VOLUME_UP");
  } else if (data == db[kk::downGest]) {
    tx.print("MEDIA_VOLUME_DOWN");
  } else if (data == db[kk::powerGest]) {
    tx.print("SYSTEM_POWER_DOWN");
  } else if (data == db[kk::sleepGest]) {
    tx.print("SYSTEM_SLEEP");
  } else if (data == db[kk::wakeGest]) {
    tx.print("SYSTEM_WAKE_UP");
  } else tx.print(data);
}

void sendPacket(String msg) {
  String s = db[kk::header];
  s += msg;
  mqtt.publish(db[kk::remote].c_str(), s.c_str());
}
