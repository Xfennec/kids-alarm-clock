/* Kids Alarm Clock
 *
 * TODO:
 *  - check daylight time saving switch
 *  - adjust volume? ramp-up?
 *  - connect WiFi when network is needed and then disconnect?
 *  - display picture during ringing (+ hour) or small animation?
 *  - more colors in drawTime()
 *  - dial with needles (not compatible with ringing animations?) - see TFT_Clock sample
 *  - background animations? (ex: Spiro)
 *
 *  - hardware: case, better speaker, button(s)?
 *  - server: holidays
 */

#include <Arduino.h>

#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioFileSourceID3.h"
#include "AudioFileSourceHTTPStream.h"
#include "AudioOutputI2S.h"

#include "time.h"
#include <sys/time.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <HTTPClient.h>
#include <TJpg_Decoder.h>

#define TFT_GREY 0x5AEB

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

time_t nextRing;
bool ringing;

AudioOutputI2S *out;
AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPStream *file;
AudioFileSourceBuffer *buff;
AudioFileSourceID3 *id3;
bool playing = 0;

// Setup screen, network, audio, time, …
void setup()
{
  Serial.begin(115200);

  setupDisplay();
  setupAudio(); // PINs, volume
  setupImage();

  checkWifi();

  setupTimeFromNetwork();
  nextRing = getNextRingTime();

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);

  showRandomImage();

  //disconnect WiFi?
  /*WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("DISCONNECTED");*/

  Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
  //tft.writecommand(ILI9341_SLPIN);

  /*tft.writecommand(ILI9341_WRCTRLD);
  tft.writedata(0x28);

  tft.writecommand(ILI9341_WRDISBV);
  tft.writedata(0x00);*/

}


// "Fast" loop
void loop()
{
  if (mp3 && mp3->isRunning()) {
    // stop MP3 if no more looping
    if (!mp3->loop()) {
      stopRinging();
    }
  }

  every_second();
  every_minute();
  every_hour();
}


// Run every second when called from the loop()
void every_second() {
  static int lastms = 0;
  if (millis()-lastms < 1000) {
    return;
  }
  lastms = millis();

  // start the work
  drawTime(165); // 165 = bottom, 85 = middle
  isItTimeToRing();
  //Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
}

// Run every minute when called from the loop()
void every_minute() {
  static int lastms = 0;
  if (millis()-lastms < (1000 * 60)) {
    return;
  }
  lastms = millis();

  // start the work
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
}

// Run every hour when called from the loop()
void every_hour() {
  static int lastms = 0;
  if (millis()-lastms < (1000 * 60 * 60)) {
    return;
  }
  lastms = millis();

  // start the work
  checkWifi();
  showRandomImage();
  setupTimeFromNetwork();
  nextRing = getNextRingTime();
}
