#define MQTT_HEADER "GXY:"

#define GT_PRINT_MODE
#define GT_MICRO_TX

#include <GyverTransfer.h>
GyverTransfer<13, GT_TX, 5000> tx;

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqtt(espClient);

const uint8_t hLen = 4;

struct gestureData {
  char ssid[2][32] = {
    { "53" },
    { "Dima" },
  };
  char pass[2][32] = {
    { "Zz11116666" },
    { "MagmaXY2020" },
  };
  char local[32] = "GXY_Дом";
  char remote[32] = "GXY_Контроллер";
  char host[32] = "test.mosquitto.org";
  uint16_t port = 1883;
};

gestureData gData;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  WiFi.begin(gData.ssid[0], gData.pass[0]);
  bool flag = false;
  uint32_t timer = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (!flag) {
      timer += 500;
    }
    if (timer > 15000 and !flag) {
      flag = true;
      WiFi.disconnect();
      WiFi.begin(gData.ssid[1], gData.pass[1]);
    }
  }
  Serial.println("Connected");
  mqtt.setServer(gData.host, gData.port);
  mqtt.setCallback(callback);
}

void loop() {
  mqttTick();
  if (Serial.available()) {
    String data = Serial.readString();
    Serial.println(data);
    tx.print(data);
  }
}

void mqttTick() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}

void connectMQTT() {
  String id("GestureXY-");
  id += String(random(0xffffff), HEX);
  if (mqtt.connect(id.c_str())) mqtt.subscribe(gData.local);
  delay(1000);
}

void callback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  String str = (char*)payload;
  if (!str.startsWith(MQTT_HEADER)) return;
  Serial.println(str);
  String readData = str.substring(hLen);
  Serial.println(readData);
  tx.print(readData);
}

void sendPacket(String msg) {
  String s = MQTT_HEADER;
  s += msg;
  mqtt.publish(gData.remote, s.c_str());
}
