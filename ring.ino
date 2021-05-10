
String getRandomRingtone()
{
  HTTPClient http;

  http.begin(URL_RINGTONE_RAND);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[HTTP] GET failed, error %d\n", httpCode);
    http.end();
    return "";
  }

  String payload = http.getString();
  Serial.println(payload);

  http.end();
  return payload;
}


// It's time! Let's ring
void ring() {
  if (ringing) {
    return;
  }

  ringing = 1;
  Serial.println("RINGING");
  String ringtone = getRandomRingtone();
  playMP3FileURL(ringtone.c_str());
}

// Get back to non-ringing mode
void stopRinging() {
  if (!ringing) {
    return;
  }

  Serial.println("stop ringing");
  ringing = 0;
  stopPlaying();
}

// Will check if it's time to ring
void isItTimeToRing() {
  time_t now;

  if (ringing) {
    return;
  }

  time(&now);
  time_t sec_to_ring = nextRing - now;

  // use a window, smoothing any schedule irregularity
  if (sec_to_ring >=0 && sec_to_ring < 3) {
    nextRing = 0;
    ring();
  }
}
