// Header files for WIFI
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <WiFi.h>

// For server build
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// For WiFi configuration
String ssid;
String password;
String hostname;
// For weather API
String cityCode = "Nanjing,CN";
String APIKEY = "b89072aa8cd18aab5f76df1a0debe2eb";
// For WiFi manager
bool isAPMode;
bool isReconfigWiFi;
const uint8_t INT_PIN = 0;

AsyncWebServer server(80);
WebSocketsServer websocket(81);

#include "SPIFFS_event.h"
#include "server_event.h"

#include "WiFi_event.h"
#include "websocket_event.h"

// TODO: Rewrite auto reconnect
// TODO: Rewrite auto fill
// TODO: Rewrite web server

void setup() {
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
    initWebsocket();
}

void loop() {
    if (isReconfigWiFi) {
        Serial.println("Reconfig WiFi started!");
        setAP();
        WiFiManageServer();
        isReconfigWiFi = false;
    }
    if (isAPMode) {
        websocket.loop();
    } else {
        getWeatherData();
    }
}
