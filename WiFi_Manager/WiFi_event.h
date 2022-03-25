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
    // Update WiFi configuration failed
    if (!updateSPIFFS()) {
        return false;
    }
    // Configure STA
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connect to ");
    Serial.print(ssid);
    // Connect to WiFi
    unsigned long connectTime = millis();
    while (true) {
        delay(500);
        Serial.print('.');
        // Connect succeeded
        if (WiFi.status() == WL_CONNECTED) {
            break;
        }
        // Connect failed and timeout
        else if (millis() - connectTime > 15000) {
            Serial.println();
            Serial.println("Timeout! Connect failed!");
            return false;
        }
    }
    Serial.println();
    Serial.println(WiFi.localIP());
    return true;
}

void intReconfigWiFi() {
    isReconfigWiFi = true;
    isAPMode = true;
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    // Return when current mode is AP mpde for configuring WiFi
    if (isAPMode)
        return;
    // Reconnect WiFi
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.disconnected.reason);
    Serial.println("Trying to Reconnect");
    WiFi.begin(ssid.c_str(), password.c_str());
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
    // Set WIFI disconnect event
    WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
}
