void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// seconds, minute, hour, day, month, year, microseconds
void mySetTime(int sc, int mn, int hr, int dy, int mt, int yr, int us) {
  struct tm t = {0};
  t.tm_year = yr - 1900;
  t.tm_mon = mt - 1;
  t.tm_mday = dy;
  t.tm_hour = hr;
  t.tm_min = mn;
  t.tm_sec = sc;
  time_t timeSinceEpoch = mktime(&t);

  struct timeval tv;
  tv.tv_sec = timeSinceEpoch;  // epoch time (seconds)
  tv.tv_usec = us;    // microseconds
  settimeofday(&tv, NULL);
}


void setupTimeFromNetwork() {
  HTTPClient http;
  int yr, mnth, d, h, m, s, us;

  http.begin(URL_TIME_GET);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP request failed with code %d\n", httpCode);
    http.end();
    return;
  }

  String payload = http.getString();
  Serial.println("time: " + payload);
  sscanf(payload.c_str(), "%4d-%2d-%2d %2d:%2d:%2d.%6d", &yr, &mnth, &d, &h, &m, &s, &us);
  http.end();

  mySetTime(s, m, h, d, mnth, yr, us);
  printLocalTime();
}

time_t getNextRingTime() {
  HTTPClient http;
  int yr, mt, d, h, m, s;

  http.begin(URL_RING_TIME_GET);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP request failed with code %d\n", httpCode);
    http.end();
    return 0;
  }

  String payload = http.getString();
  if (payload == "") {
    Serial.println("no new ring information");
    return 0;
  }

  Serial.println("ring: " + payload);
  sscanf(payload.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &yr, &mt, &d, &h, &m, &s);
  http.end();

  struct tm t = {0};
  t.tm_year = yr - 1900;
  t.tm_mon = mt - 1;
  t.tm_mday = d;
  t.tm_hour = h;
  t.tm_min = m;
  t.tm_sec = s;

  time_t ts = mktime(&t);
  return ts;
}
