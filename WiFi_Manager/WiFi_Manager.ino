// Header files for WiFi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <FS.h>

// Headers files for server build
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// For WiFi configuration
String ssid;
String password;
String hostname;
// For openweathermap API
String cityCode = "Nanjing, CN";
String APIKEY = "b89072aa8cd18aab5f76df1a0debe2eb";
// For WiFi manager
bool isAPMode;
bool isReconfigWiFi;
const uint8_t INT_PIN = 0;

AsyncWebServer server(80);
WebSocketsServer websocket(81);
WiFiEventHandler WiFiStationDisconnected;

#include "SPIFFS_event.h"
#include "server_event.h"

#include "WiFi_event.h"
#include "websocket_event.h"

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
