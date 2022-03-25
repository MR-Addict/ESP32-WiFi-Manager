void WiFiManageServer() {
    server.on("/", HTTP_POST, [](AsyncWebServerRequest* request) {
        // Get POST data
        String wmssid, wmpwd, wmhostname;
        uint8_t params = request->params();
        for (uint8_t i = 0; i < params; i++) {
            AsyncWebParameter* p = request->getParam(i);
            if (strcmp(p->name().c_str(), "ssid") == 0) {
                wmssid = p->value().c_str();
            } else if (strcmp(p->name().c_str(), "password") == 0) {
                wmpwd = p->value().c_str();
            } else if (strcmp(p->name().c_str(), "hostname") == 0) {
                wmhostname = p->value().c_str();
            }
        }
        writeSPIFFS(wmssid, wmpwd, wmhostname);
        request->send(200, "text/plain", "Configure Done. ESP restarting...");
        delay(1000);
        ESP.restart();
    });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/wifimanager.html", "text/html");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.begin();
}

void getWeatherData() {
    HTTPClient http;
    String server =
        "http://api.openweathermap.org/data/2.5/weather?q=" + cityCode +
        "&APPID=" + APIKEY;
    http.begin(server);
    int8_t httpCode = http.GET();
    if (httpCode) {
        String JSONBuffer = http.getString();
        Serial.println(JSONBuffer);
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, JSONBuffer);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
    } else {
        Serial.println("Error on http request");
    }
    http.end();
    delay(5000);
}
