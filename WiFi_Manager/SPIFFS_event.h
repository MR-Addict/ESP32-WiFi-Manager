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
        wifi.ssid = doc["ssid"].as<String>();
    }
    if (doc.containsKey("password")) {
        wifi.pwd = doc["password"].as<String>();
    }
    if (doc.containsKey("hostname")) {
        wifi.hostname = doc["hostname"].as<String>();
    }
    if (doc.containsKey("city")) {
        wifi.city = doc["city"].as<String>();
    }
    if (wifi.ssid == "" || wifi.city == "") {
        return false;
    }
    return true;
}

void writeSPIFFS(String wmssid,
                 String wmpwd,
                 String wmhostname,
                 String wmcity) {
    SPIFFS.remove("/JSON.json");
    File file = SPIFFS.open("/JSON.json", "w");
    if (!file || file.isDirectory()) {
        Serial.println("Failed to creat file for writing!");
        return;
    }

    // Set the values in the document
    StaticJsonDocument<200> doc;
    doc["ssid"] = wmssid;
    doc["password"] = wmpwd;
    doc["hostname"] = wmhostname;
    doc["city"] = wmcity;

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }

    // Close the file
    file.close();
}
