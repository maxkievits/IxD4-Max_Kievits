void setupESP8266() {
  setupESP8266(false);
}

void setupESP8266(bool forceReconnect) {
  // connect the shield to the Arduino
  int found = 0;
  while (found == 0) {
    Serial.println(F("Connecting to ESP8266 shield..."));
    found = esp8266.begin();
    if (found) {
      Serial.println(F("ESP8266 shield found!"));
    } else {
      Serial.println(F("Could not connect to ESP8266 - retrying"));
      delay(3000);
    }
  }

  // make sure esp8266 is in STATION (client) mode (instead of AP)
  int mode = esp8266.getMode();
  while (mode != ESP8266_MODE_STA) {
    Serial.println(F("ESP8266 not in station mode - switching..."));
    int result = esp8266.setMode(ESP8266_MODE_STA);
    if (result < 0) {
      Serial.println(F("Error setting station mode - retry"));
      delay(3000);
    } else {
      Serial.println(F("ESP8266 switched to station mode"));
    }
    mode = esp8266.getMode();
  }

  // if no network connection, or reconnect is forced
  if (esp8266.status() < 0 || forceReconnect) {
    int result = -1;
    while (result < 0) {
      Serial.print(F("Connecting to network "));
      Serial.println(F(NETWORK_NAME));
      result = esp8266.connect(NETWORK_NAME, NETWORK_PASSWORD);
      if (result < 0) {
        Serial.print(F("Error connecting: "));
        if (result == ESP8266_RSP_TIMEOUT) {
          Serial.println(F("TIMEOUT"));
        } else if (result == ESP8266_RSP_UNKNOWN) {
          Serial.println(F("UNKNOWN"));
        } else if (result == ESP8266_RSP_FAIL) {
          Serial.println(F("FAIL"));
        } else if (result == ESP8266_RSP_MEMORY_ERR) {
          Serial.println(F("MEMORY_ERR"));
        } else {
          Serial.println(result);
        }
        delay(3000);
      }
    }
  }

  char strbuffer[24];
  memset(strbuffer, 0, 24);

  Serial.print(F("Connected to network "));
  int result = esp8266.getAP(strbuffer);
  if (result > 0) {
    Serial.println(strbuffer);
  } else {
    Serial.println("<UNKNOWN>");
  }

  IPAddress ip = esp8266.localIP();
  Serial.print(F("Local IP: "));
  Serial.println(ip);

  Serial.print(F("Local MAC: "));
  result = esp8266.localMAC(strbuffer);
  if (result > 0) {
    Serial.println(strbuffer);
  } else {
    Serial.println("<UNKNOWN>");
  }
}
