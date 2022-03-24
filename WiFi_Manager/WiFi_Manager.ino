// Header files for WIFI
#include <SPIFFS.h>
#include <WiFi.h>

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

#include "SPIFFS_event.h"
#include "server_event.h"

#include "WiFi_event.h"
#include "websocket_event.h"

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
    initWebsocket();
}

void loop() {
    websocket.loop();
    if (isReconfigWiFi) {
        Serial.println("Reconfig WiFi started!");
        setAP();
        manageServer();
        isReconfigWiFi = false;
    }
}
