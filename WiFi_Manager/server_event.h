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
        oled.clear();
        oled.setFont(&Dialog_bold_10);
        oled.drawBitmap(44, 0, 40, 40, restart_40x40);
        oled.print(20, 40, "Restarting...");
        oled.show();
        delay(3000);
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
    if (isAPMode)
        return;
    WiFiClient client;
    HTTPClient http;
    String server =
        "http://api.openweathermap.org/data/2.5/weather?q=" + cityCode +
        "&APPID=" + APIKEY + "&mode=json&units=metric";
    http.begin(client, server);
    int8_t httpCode = http.GET();
    if (httpCode) {
        String JSONBuffer = http.getString();
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, JSONBuffer);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
        weather.code = doc["cod"];
        if (weather.code == 200) {
            weather.temp = doc["main"]["temp"];
            weather.humidity = doc["main"]["humidity"];
            weather.weather = doc["weather"][0]["main"].as<String>();
        }
    } else {
        Serial.println("Error on http request");
    }
    http.end();
}
