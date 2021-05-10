
// Setup audio output (I2S MAX98357A 3W amplifier, Adafruit board)
void setupAudio() {
  out = new AudioOutputI2S();
  out->SetPinout(27, 26, 25);
  out->SetGain(0.05);
}

// Play the MP3 from an HTTP URL
void playMP3FileURL(const char *URL) {
  if (playing) {
    stopPlaying();
  }

  playing = 1;
  
  file = new AudioFileSourceHTTPStream(URL);
  buff = new AudioFileSourceBuffer(file, 4096);
  mp3 = new AudioGeneratorMP3();
  id3 = new AudioFileSourceID3(file);

  mp3->begin(id3, out);
}

// Stop playing the sound (if needed) and free all objects
void stopPlaying() {
  playing = 0;

  if (mp3) {
    mp3->stop();
    delete mp3;
    mp3 = NULL;
  }

  if (id3) {
    id3->close();
    delete id3;
    id3 = NULL;
  }

  if (buff) {
    buff->close();
    delete buff;
    buff = NULL;
  }

  if (file) {
    file->close();
    delete file;
    file = NULL;
  }
}
