void creatAP() {
    WiFi.disconnect();
    WiFi.softAP("WiFi Manager", "123456789");
    Serial.print("Access Point:");
    Serial.println(WiFi.softAPIP());
}

bool creatSTA() {
    // Read data failed
    if (!readData()) {
        return false;
    }

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
            Serial.println("Connect failed!");
            return false;
        }
    }
    Serial.println();
    Serial.println(WiFi.localIP());
    return true;
}

void intReconfigWiFi() {
    ssid = password = "";
    isReconfigWiFi = true;
}

void initWiFi() {
    if (!creatSTA()) {
        creatAP();
        manageServer();
    } else {
        appServer();
    }
}
