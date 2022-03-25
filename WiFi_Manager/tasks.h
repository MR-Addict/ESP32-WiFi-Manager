void task1callback() {
    if (isAPMode) {
        websocket.loop();
    }
}

void task2callback() {
    getWeatherData();
}

void task3callback() {
    if (isReconfigWiFi) {
        Serial.println("Reconfig WiFi started!");
        setAP();
        WiFiManageServer();
        isReconfigWiFi = false;
    }
}