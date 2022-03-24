// Header files for WiFi
#if defined ESP8266
#include <ESP8266WiFi.h>
#include <FS.h>
#elif defined ESP32
#include <SPIFFS.h>
#include <WiFi.h>
#endif

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
#if defined ESP8266
    digitalWrite(LED, HIGH);
#endif
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
