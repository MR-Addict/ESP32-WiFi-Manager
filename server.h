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
        request->send(200, "text/plain", "Configure Done. ESP will restart!");
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

void appServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.begin();
}
