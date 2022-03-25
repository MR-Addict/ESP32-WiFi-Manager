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

void task4callback() {
    oled.clear();
    if (weather.weather == "Rain" || weather.weather == "Drizzle") {
        oled.drawBitmap(80, 0, 40, 40, rain_40x40);
    } else if (weather.weather == "Thunderstorm") {
        oled.drawBitmap(80, 0, 40, 40, thunderstorm_40x40);
    } else if (weather.weather == "Snow") {
        oled.drawBitmap(80, 0, 40, 40, snow_40x40);
    } else if (weather.weather == "Clouds") {
        oled.drawBitmap(80, 0, 40, 32, cloud_40x32);
    } else if (weather.weather == "Clear") {
        oled.drawBitmap(80, 0, 40, 32, sun_40x32);
    } else {
        oled.drawBitmap(80, 0, 40, 32, mist_40x32);
    }
    char message[50] = {0};
    sprintf(message, "%3.1f-C", weather.temp);
    oled.print(0, 0, message);
    sprintf(message, "%3.1f%c", weather.humidity, '%');
    oled.print(0, 20, message);
    oled.show();
}