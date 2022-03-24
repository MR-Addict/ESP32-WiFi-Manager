void manageServer() {
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
        writeData(wmssid, wmpwd, wmhostname);
        request->send(200, "text/plain", "Configure Done. ESP restarting...");
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

String processor(const String& var) {
    if (var == "STATE") {
        if (isDisplay) {
            return "ON";
        } else {
            return "OFF";
        }
    }
    return String();
}

void appServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    // Route to set GPIO state to HIGH
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest* request) {
        isDisplay = true;
#if defined ESP8266
        digitalWrite(LED, !isDisplay);
#elif defined ESP32
        digitalWrite(LED,isDisplay);
#endif
        request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

    // Route to set GPIO state to LOW
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest* request) {
        isDisplay = false;
#if defined ESP8266
        digitalWrite(LED, !isDisplay);
#elif defined ESP32
        digitalWrite(LED,isDisplay);
#endif
        request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

    server.begin();
}
