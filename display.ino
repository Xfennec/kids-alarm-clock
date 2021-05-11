
// Configure TFP display
void setupDisplay() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.drawCentreString(WELCOME_TEXT, 160, 110, 4);
}

// Draw the current time using big numbers, with seconds
void drawTime(int ypos) {
  static byte omm = 99, oss = 99;
  static byte xcolon = 0, xsecs = 0;

  uint8_t hh, mm, ss;
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  hh = timeinfo.tm_hour;
  mm = timeinfo.tm_min;
  ss = timeinfo.tm_sec;
  
  // Update digital time
  int xpos = 0;
  int ysecs = ypos + 24;

  if (omm != mm) { // Redraw hours and minutes time every minute
    omm = mm;
    // Draw hours and minutes
    if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
    xpos += tft.drawNumber(hh, xpos, ypos, 8);             // Draw hours
    xcolon = xpos; // Save colon coord for later to flash on/off later
    xpos += tft.drawChar(':', xpos, ypos /*- 8*/, 8);
    if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
    xpos += tft.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
    xsecs = xpos; // See seconds 'x' position for later display updates
  }
  if (oss != ss) { // Redraw seconds time every second
    oss = ss;
    xpos = xsecs;

    if (ss % 2) { // Flash the colons on/off
      tft.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
      tft.drawChar(':', xcolon, ypos /*- 8*/, 8);     // Hour:minute colon
      xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
    }
    else {
      tft.drawChar(':', xcolon, ypos /*- 8*/, 8);     // Hour:minute colon
      xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
    }

    //Draw seconds
    if (ss < 10) xpos += tft.drawChar('0', xpos, ysecs, 6); // Add leading zero
    tft.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
  }
}
