// Header files for WIFI
#include <SPIFFS.h>
#include <WiFi.h>

// Heade files for WIFI
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

String ssid;
String password;
String hostname;
bool isReconfigWiFi;
bool isDisplay;
const uint8_t LED = 2;
const uint8_t INT_PIN = 0;

AsyncWebServer server(80);
WebSocketsServer websocket(81);

#include "SPIFFS_Function.h"
#include "server_Function.h"
#include "wifi_Function.h"

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
}

void loop() {
    if (isReconfigWiFi) {
        Serial.println("Reconfig WiFi started!");
        setAP();
        manageServer();
        isReconfigWiFi = false;
    }
}
