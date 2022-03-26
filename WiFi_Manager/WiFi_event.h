void setAP() {
    // OLE show AP Mode message
    show_AP_mode();
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
    // OLED show connection message
    show_trying_connect();
    // Configure STA
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(wifi.hostname.c_str());
    WiFi.begin(wifi.ssid.c_str(), wifi.pwd.c_str());
    Serial.print("Connect to ");
    Serial.print(wifi.ssid);

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
            // OELD show connection failed message
            show_connect_failed();
            delay(3000);
            return false;
        }
    }
    Serial.println();
    Serial.println(WiFi.localIP());
    // Show connection success message
    show_connect_success();
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
            show_trying_connect();
            Serial.println("WiFi lost connection. Trying to reconnect...");
            WiFi.begin(wifi.ssid.c_str(), wifi.pwd.c_str());
        });
}
