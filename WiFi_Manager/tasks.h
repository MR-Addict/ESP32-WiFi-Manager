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
        setAP();
        WiFiManageServer();
        isReconfigWiFi = false;
    }
}

void task4callback() {
    if (isAPMode || WiFi.status() != WL_CONNECTED)
        return;
    if (weather.code != 200) {
        // OLED show receive error
        show_receive_error();
    } else {
        // OLED show time and weather information
        show_weather();
    }
}
