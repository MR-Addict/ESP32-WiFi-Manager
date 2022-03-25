void setAP() {
    // Show AP Mode message
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    oled.drawBitmap(44, 0, 40, 40, AP_40x40);
    oled.print(0, 38, "SSID:WiFi Manager");
    oled.print(0, 51, "PWD:123456789");
    oled.show();
    Serial.println("Reconfig WiFi started!");
    // Reset server first before set AP mode
    server.reset();
    // Disconnect WIFI first before set AP Mode
    WiFi.disconnect();
    // Configure AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP("WiFi Manager", "123456789");
    Serial.print("Access Point:");
    Serial.println(WiFi.softAPIP());
}

bool setSTA() {
    // Reset server first before set STA mode
    server.reset();
    // Read data first
    if (!updateSPIFFS()) {
        return false;
    }
    // Show connection message
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    char message[50];
    sprintf(message, "Tring to connect to %s...", ssid);
    oled.drawBitmap(44, 0, 40, 32, WiFi_Lost_40x32);
    oled.print(0, 35, message);
    oled.show();
    // Configure STA
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connect to ");
    Serial.print(ssid);

    // Connect to WIFI
    unsigned long connectTime = millis();
    while (true) {
        delay(500);
        Serial.print('.');
        // Connect succeeded
        if (WiFi.status() == WL_CONNECTED) {
            break;
        }
        // Connect failed and timeout
        else if (millis() - connectTime > 50000) {
            Serial.println();
            Serial.println("Timeout! Connect failed!");
            // Show connection failed message
            oled.clear();
            oled.setFont(&Dialog_bold_10);
            oled.drawBitmap(44, 0, 40, 32, WiFi_Lost_40x32);
            oled.print(0, 35, "Timeout. Switch to AP mode");
            oled.show();
            delay(3000);
            return false;
        }
    }
    Serial.println();
    Serial.println(WiFi.localIP());
    // Show connection success message
    oled.clear();
    oled.setFont(&Dialog_bold_10);
    oled.drawBitmap(44, 0, 40, 32, WiFi_40x32);
    oled.print(0, 35, "Connect successful.");
    sprintf(message, "IP:%s", WiFi.localIP().toString().c_str());
    oled.print(0, 48, message);
    oled.show();
    delay(3000);
    return true;
}

void ICACHE_RAM_ATTR intReconfigWiFi() {
    isReconfigWiFi = true;
    isAPMode = true;
}

void initWiFi() {
    // Set STA mode
    if (!setSTA()) {
        isAPMode = true;
    } else {
        isAPMode = false;
    }
    if (isAPMode) {
        setAP();
        WiFiManageServer();
    }
    // WiFi STA auto reconnect
    WiFiStationDisconnected = WiFi.onStationModeDisconnected(
        [](const WiFiEventStationModeDisconnected& event) {
            if (isAPMode)
                return;
            // Show connection message
            oled.clear();
            oled.setFont(&Dialog_bold_10);
            char message[50];
            sprintf(message, "Tring to connect to %s...", ssid);
            oled.drawBitmap(44, 0, 40, 32, WiFi_Lost_40x32);
            oled.print(0, 35, message);
            oled.show();
            Serial.println("WiFi lost connection. Trying to reconnect...");
            WiFi.begin(ssid.c_str(), password.c_str());
        });
}
