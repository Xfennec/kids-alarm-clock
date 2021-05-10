
void setupImage() {
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
}

// called during JPEG decoding
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // off screen
  if ( y >= tft.height() ) return 0;

  // will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // decode next block
  return 1;
}

uint8_t *getImage(String url, int *allocSize)
{
  HTTPClient http;
  uint8_t *res = NULL;

  Serial.printf("Loading image '%s'\n", url.c_str());
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[HTTP] GET failed, error %d\n", httpCode);
    http.end();
    return NULL;
  }

  // get lenght of document (is -1 when Server sends no Content-Length header)
  int total = http.getSize();
  *allocSize = total;

  Serial.println("HTTP SIZE IS " + String(total));

  // get tcp stream
  WiFiClient * stream = http.getStreamPtr();
  int readed;

  // create buffer for read
  res = (uint8_t *)malloc(total);
  if (res == NULL) {
    http.end();
    return NULL;
  }

  readed = stream->readBytes(res, total);
  if (readed != total) {
    Serial.printf("can't read full content, got %d\n", readed);
    http.end();
    return NULL;
  }

  Serial.println("[HTTP] end.");
  http.end();
  return res;
}


void showRandomImage() {
  uint8_t *img;
  int imgSize;

 img = getImage(URL_IMAGE_RAND, &imgSize);
  if (!img) {
    Serial.println("getImage failed");
    return;
  }

  TJpgDec.drawJpg(0, 0, img, imgSize);
  free(img);
}
