void checkWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
    return;
  }

  Serial.println("Not connected to WiFi");

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Try forever (TODO: fix this!)
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf("...Connecting to %s\n", WIFI_SSID);
    delay(500);
  }
  Serial.println("Connected");
}
