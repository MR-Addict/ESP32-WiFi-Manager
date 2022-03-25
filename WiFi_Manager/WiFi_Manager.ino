// Header files for WiFi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <FS.h>

// Headers files for server build
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// Task scheduler
#include <TaskScheduler.h>

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
Scheduler tasks;

#include "SPIFFS_event.h"
#include "server_event.h"

#include "WiFi_event.h"
#include "websocket_event.h"

#include "tasks.h"

Task task1(0, TASK_FOREVER, &task1callback);
Task task2(5000, TASK_FOREVER, &task2callback);
Task task3(0, TASK_FOREVER, &task3callback);

void setup() {
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
    initWebsocket();

    tasks.addTask(task1);
    tasks.addTask(task3);
    tasks.addTask(task2);
    task1.enable();
    task2.enable();
    task3.enable();
}

void loop() {
    tasks.execute();
}
