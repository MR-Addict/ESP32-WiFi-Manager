void setAP() {
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
        else if (millis() - connectTime > 20000) {
            Serial.println();
            Serial.println("Timeout! Connect failed!");
            return false;
        }
    }
    Serial.println();
    Serial.println(WiFi.localIP());
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
            Serial.println("WiFi lost connection. Trying to reconnect...");
            WiFi.begin(ssid.c_str(), password.c_str());
        });
}
