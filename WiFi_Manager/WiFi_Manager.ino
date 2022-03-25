// Header files for WiFi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <FS.h>

// Headers files for server build
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// Header files for weather sation
#include <SSD1306.h>
#include <TaskScheduler.h>

// For WiFi configuration
String ssid;
String password;
String hostname;
// For openweathermap API
String cityCode = "Nanjing, CN";
#include <arduino_secrets.h>
// For WiFi manager
bool isAPMode;
bool isReconfigWiFi;
const uint8_t INT_PIN = 0;

// weather data stucture
struct weatherData {
    float temp;
    float humidity;
    String weather;
};

weatherData weather;

AsyncWebServer server(80);
WebSocketsServer websocket(81);
WiFiEventHandler WiFiStationDisconnected;
Scheduler tasks;
OLED oled(128, 64);

#include "SPIFFS_event.h"
#include "server_event.h"

#include "WiFi_event.h"
#include "websocket_event.h"

#include "OLED.h"
#include "bitmap.h"
#include "font/Orbitron_Bold_16.h"
#include "tasks.h"

Task task1(0, TASK_FOREVER, &task1callback);
Task task2(TASK_HOUR, TASK_FOREVER, &task2callback);
Task task3(0, TASK_FOREVER, &task3callback);
Task task4(TASK_SECOND, TASK_FOREVER, &task4callback);

void setup() {
    oled.setFont(&Orbitron_Bold_16);
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
    initWebsocket();

    tasks.addTask(task1);
    tasks.addTask(task3);
    tasks.addTask(task2);
    tasks.addTask(task4);
    task1.enable();
    task2.enable();
    task3.enable();
    task4.enable();
}

void loop() {
    tasks.execute();
}
