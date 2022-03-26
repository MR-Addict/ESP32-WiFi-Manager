void show_receive_error() {
    // Update NTP time
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime();

    oled.clear();
    oled.drawBitmap(80, 30, 40, 32, error_40x32);
    oled.drawBitmap(112, 4, 16, 16, signal_16x16);
    oled.setFont(&Dialog_bold_10);
    oled.print(30, 4, formattedTime.c_str());
    oled.drawFastHLine(0, 20, 128);
    oled.print(0, 24, "Receive");
    oled.print(0, 37, "Error:");
    oled.print(0, 50, String(weather.code).c_str());
    oled.show();
}

void show_weather() {
    // Update NTP time
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime();

    oled.clear();
    // Show weather status
    if (weather.weather == "Rain" || weather.weather == "Drizzle") {
        oled.drawBitmap(80, 25, 40, 40, rain_40x40);
    } else if (weather.weather == "Thunderstorm") {
        oled.drawBitmap(80, 25, 40, 40, thunderstorm_40x40);
    } else if (weather.weather == "Snow") {
        oled.drawBitmap(80, 25, 40, 40, snow_40x40);
    } else if (weather.weather == "Clouds") {
        oled.drawBitmap(80, 30, 40, 32, cloud_40x32);
    } else if (weather.weather == "Clear") {
        oled.drawBitmap(80, 30, 40, 32, sun_40x32);
    } else {
        oled.drawBitmap(80, 30, 40, 32, mist_40x32);
    }
    oled.drawBitmap(112, 4, 16, 16, signal_16x16);
    // Show temp and humidity
    oled.setFont(&Orbitron_Bold_16);
    char message[50] = {0};
    sprintf(message, "%3.1f-C", weather.temp);
    oled.print(0, 24, message);
    sprintf(message, "%3.1f%c", weather.humidity, '%');
    oled.print(0, 44, message);
    // Show time
    oled.setFont(&Dialog_bold_10);
    oled.drawFastHLine(0, 20, 128);
    oled.print(30, 4, formattedTime.c_str());
    oled.show();
}

void show_AP_mode() {
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    oled.drawBitmap(44, 0, 40, 40, AP_40x40);
    oled.print(0, 38, "SSID:WiFi Manager");
    oled.print(0, 51, "PWD:123456789");
    oled.show();
}

void show_trying_connect() {
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    char message[50];
    sprintf(message, "Tring to connect to %s...", wifi.ssid);
    oled.drawBitmap(44, 0, 40, 32, WiFi_Lost_40x32);
    oled.print(0, 35, message);
    oled.show();
}

void show_connect_failed() {
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    oled.drawBitmap(44, 0, 40, 32, WiFi_Lost_40x32);
    oled.print(0, 35, "Timeout. Switch to AP mode");
    oled.show();
}

void show_connect_success() {
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    oled.drawBitmap(44, 0, 40, 32, WiFi_40x32);
    oled.print(0, 35, "Connect successful.");
    char message[50];
    sprintf(message, "IP:%s", WiFi.localIP().toString().c_str());
    oled.print(0, 48, message);
    oled.show();
}

void show_restart() {
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    oled.drawBitmap(44, 0, 40, 40, restart_40x40);
    oled.print(20, 40, "Restarting...");
    oled.show();
}