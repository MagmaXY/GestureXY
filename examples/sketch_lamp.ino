#define MQTT_HEADER "GXY:"

//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>

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
  String local = "GXY_Лампа";
  String remote = "GXY_Контроллер";
  String host = "test.mosquitto.org";
  uint16_t port = 1883;

  String gest = "";
  bool state = false;
  byte brightness = 102;
  byte scene = 0;
  String scenes[4] = { "Нет", "Холодный", "Нормальный", "Тёплый" };
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

#define LED_PIN 13
#define LED_NUM 64
#define LED_TYPE WS2812
#define LED_ORDER GRB

CRGB leds[LED_NUM];

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, LED_NUM);
  FastLED.setBrightness(gData.brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 800);
  for (byte i = 0; i < LED_NUM; i++) {
    leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
  }
  FastLED.show();
  bool flag = false;
  byte timer = 0;
  WiFi.begin(gData.ssid[0], gData.pass[0]);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (!flag) timer++;
    if (timer > 15 and !flag) {
      flag = true;
      WiFi.disconnect();
      WiFi.begin(gData.ssid[1], gData.pass[1]);
    }
  }
  Serial.println("WiFi");
  for (int i = 0; i < LED_NUM; i++) leds[i].setRGB(0, 0, 0);
  FastLED.show();
  mqtt.setServer(gData.host, gData.port);
  mqtt.setCallback(callback);
  mqtt.setSocketTimeout(1000);
  connectMQTT();
  Serial.println("MQTT");
}

void loop() {
  mqttTick();
}

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
    if (mqtt.connect(id.c_str())) mqtt.subscribe(gData.local);
  }
}

void callback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  String str = (char*)payload;
  if (!str.startsWith(MQTT_HEADER)) return;
  Serial.println(str);
  String readData = str.substring(hLen);
  Serial.println(readData);
  if (readData == "По часовой") {
    gData.brightness += 51;
    FastLED.setBrightness(gData.brightness);
    FastLED.show();
    sendPacket("Яркость лампы: " + String(gData.brightness));
  } else if (readData == "Против часовой") {
    gData.brightness -= 51;
    FastLED.setBrightness(gData.brightness);
    FastLED.show();
    sendPacket("Яркость лампы: " + String(gData.brightness));
  } else if (readData == "Вперёд") {
    gData.state = !gData.state;
    if (gData.state) {
      if (gData.scenes[gData.scene] == "Нет") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
        }
      } else if (gData.scenes[gData.scene] == "Холодный") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(0, 213, 255);
        }
      } else if (gData.scenes[gData.scene] == "Нормальный") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(255, 255, 255);
        }
      } else if (gData.scenes[gData.scene] == "Тёплый") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(255, 200, 0);
        }
      }
    } else {
      for (int i = 0; i < LED_NUM; i++) {
        leds[i].setRGB(0, 0, 0);
      }
    }
    FastLED.show();
    sendPacket(gData.state ? "Лампа включилась" : "Лампа выключилась");
  } else if (readData == "Назад") {
    gData.scene = ++gData.scene % 4;
    if (gData.state) {
      if (gData.scenes[gData.scene] == "Нет") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
        }
      } else if (gData.scenes[gData.scene] == "Холодный") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(0, 213, 255);
        }
      } else if (gData.scenes[gData.scene] == "Нормальный") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(255, 255, 255);
        }
      } else if (gData.scenes[gData.scene] == "Тёплый") {
        for (int i = 0; i < LED_NUM; i++) {
          leds[i].setRGB(255, 200, 0);
        }
      }
    }
    FastLED.show();
    sendPacket(gData.scenes[gData.scene] + " свет лампы");
  } else if (readData == "Вперёд-Назад") {
    gData.color = ++gData.color % 7;
    String msg;
    for (byte i = 0; i < 2; i++) {
      msg += String(gData.colors[gData.color][i]);
      msg += ", ";
    }
    msg += String(gData.colors[gData.color][2]);
    for (int i = 0; i < LED_NUM; i++) {
      leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
    }
    FastLED.show();
    sendPacket(msg + " цвет лампы");
  } else if (readData == "Назад-Вперёд") {
    gData.color--;
    if (gData.color < 0) gData.color = 0;
    String msg;
    for (byte i = 0; i < 2; i++) {
      msg += String(gData.colors[gData.color][i]);
      msg += ", ";
    }
    msg += String(gData.colors[gData.color][2]);
    for (int i = 0; i < LED_NUM; i++) {
      leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
    }
    FastLED.show();
    sendPacket(msg + " цвет лампы");
  } else if (readData == "Уход" or readData == "Сон") {
    gData.state = false;
    for (int i = 0; i < LED_NUM; i++) {
      leds[i].setRGB(0, 0, 0);
    }
    FastLED.show();
  } else if (readData == "Приход") {
    gData.state = true;
    if (gData.scenes[gData.scene] == "Нет") {
      for (int i = 0; i < LED_NUM; i++) {
        leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
      }
    } else if (gData.scenes[gData.scene] == "Холодный") {
      for (int i = 0; i < LED_NUM; i++) {
        leds[i].setRGB(0, 213, 255);
      }
    } else if (gData.scenes[gData.scene] == "Нормальный") {
      for (int i = 0; i < LED_NUM; i++) {
        leds[i].setRGB(255, 255, 255);
      }
    } else if (gData.scenes[gData.scene] == "Тёплый") {
      for (int i = 0; i < LED_NUM; i++) {
        leds[i].setRGB(255, 200, 0);
      }
    }
    FastLED.show();
  }
}

void sendPacket(String msg) {
  String s = MQTT_HEADER;
  s += msg;
  mqtt.publish(gData.remote, s.c_str());
}
