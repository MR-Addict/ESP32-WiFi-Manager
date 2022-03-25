void initSPIFFS() {
    if (!SPIFFS.begin()) {
        Serial.println("An error has occurred while mounting SPIFFS");
        ESP.restart();
        return;
    }
}

bool updateSPIFFS() {
    // Read file
    File file = SPIFFS.open("/JSON.json", "r");
    if (!file || file.isDirectory()) {
        Serial.println("Failed to open file for reading!");
        return false;
    }
    String message;
    while (file.available()) {
        message += (char)file.read();
    }
    file.close();

    // Read JSON
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }
    if (doc.containsKey("ssid")) {
        ssid = doc["ssid"].as<String>();
    }
    if (doc.containsKey("password")) {
        password = doc["password"].as<String>();
    }
    if (doc.containsKey("hostname")) {
        hostname = doc["hostname"].as<String>();
    }
    if (ssid == "") {
        return false;
    }
    return true;
}

void writeSPIFFS(String wmssid, String wmpassword, String wmhostname) {
    SPIFFS.remove("/JSON.json");
    File file = SPIFFS.open("/JSON.json", "w");
    if (!file || file.isDirectory()) {
        Serial.println("Failed to creat file for writing!");
        return;
    }

    // Set the values in the document
    StaticJsonDocument<200> doc;
    doc["ssid"] = wmssid;
    doc["password"] = wmpassword;
    doc["hostname"] = wmhostname;

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }

    // Close the file
    file.close();
}
