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
    // Read data first
    if (!readData()) {
        return false;
    }

    // Configure STA
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to ");
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
}

void initWiFi() {
    if (!setSTA()) {
        setAP();
        manageServer();
    } else {
        appServer();
    }
}