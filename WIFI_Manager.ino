// Header files for WIFI
#if defined ESP8266
#incldue < ESP8266WiFi.h>
#include <FS.h>
#elif defined ESP32
#include <SPIFFS.h>
#include <WiFi.h>
#endif

// Heade files for WIFI
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

String ssid;
String password;
String hostname;
bool isReconfigWiFi;
const uint8_t INT_PIN = 0;

AsyncWebServer server(80);
WebSocketsServer websocket(81);

#include "fileFunction.h"
#include "server.h"
#include "wifiFunction.h"

void setup() {
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
}

void loop() {
    if (isReconfigWiFi) {
        Serial.println("Reconfig WiFi started!");
        server.reset();
        creatAP();
        manageServer();
        isReconfigWiFi = false;
    }
}
