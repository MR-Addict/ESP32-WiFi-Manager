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

// Header files for NTP time
#include <NTPClient.h>
#include <WiFiUdp.h>

// For weather APIKEY
#include <arduino_secrets.h>

// For WiFi manager
bool isAPMode;
bool isReconfigWiFi;
const uint8_t INT_PIN = 12;

// weather data stucture
struct weatherData {
    int code;
    float temp;
    float humidity;
    String weather;
};

// WiFi configuration structure
struct WiFiData {
    String ssid;
    String pwd;
    String hostname;
    String city;
};

weatherData weather;
WiFiData wifi;

// Define object for Server
AsyncWebServer server(80);
WebSocketsServer websocket(81);
// Define object for WiFi atuo reconnect
WiFiEventHandler WiFiStationDisconnected;
Scheduler tasks;
// Define for OLED
OLED oled(128, 64);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800);

// OELD elements file
#include "bitmap.h"
#include "font/Dialog_bold_10.h"
#include "font/Orbitron_Bold_16.h"

#include "SPIFFS_event.h"
#include "server_event.h"

#include "WiFi_event.h"
#include "websocket_event.h"

#include "tasks.h"

Task task1(0, TASK_FOREVER, &task1callback);
Task task2(30 * TASK_MINUTE, TASK_FOREVER, &task2callback);
Task task3(0, TASK_FOREVER, &task3callback);
Task task4(TASK_SECOND, TASK_FOREVER, &task4callback);

void setup() {
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), intReconfigWiFi, FALLING);
    Serial.begin(115200);
    initSPIFFS();
    initWiFi();
    initWebsocket();

    // Init for ntp client
    timeClient.begin();

    // Init for tasks
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
