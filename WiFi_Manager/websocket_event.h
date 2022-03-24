void webSocketEvent(uint8_t num,
                    WStype_t type,
                    uint8_t* payload,
                    size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = websocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num,
                          ip[0], ip[1], ip[2], ip[3], payload);

            // Set the values in the document
            String message;
            StaticJsonDocument<200> doc;
            doc["ssid"] = ssid;
            doc["password"] = password;
            doc["hostname"] = hostname;

            // Serialize JSON to file
            serializeJson(doc, message);

            // Send message to connected client
            websocket.sendTXT(num, message);
            Serial.println(message);
        } break;
        case WStype_TEXT: {
            Serial.printf("[%u] get Text: %s\n", num, payload);
            String message = String((char*)(payload));
            websocket.broadcastTXT(message);
        } break;
    }
}

void initWebsocket() {
    websocket.begin();
    websocket.onEvent(webSocketEvent);
}