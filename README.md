# Kids Alarm Clock

A DIY alarm clock for kids: display cute pictures and play their favorite tunes when it's time to wake up!

photo1

Uses WiFi to get time, ring settings, pictures and tunes: no fiddly local setup, no power loss resets.

## Hardware
 - ESP32 (ESP8266 is probably memory short)
 - ILI9341 TFT screen
 - I2S MAX98357A 3W amplifier, Adafruit board
 - 3W audio speaker

Less than $40, add $10 if you need a prototype board, some wires and headers for a clean setup.

photo2

## Software

### ESP program
Add the following libraries to your Arduino/PlatformIO/whatever setup:
 - TFT_eSPI (screen)
 - TJpg_Decoder
 - ESP8266Audio

Create a `config.ino` file (see `config.ino.sample`)

You must also configure TFT_eSPI library, in its `User_Setup.h` (most values have correct default values):
```
#define ILI9341_DRIVER
…
#define TFT_HEIGHT 240 // GC9A01 240 x 240
…
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)
…
#define TOUCH_CS 21     // Chip select pin (T_CS) of touch screen
```

### Server
In `config.ino`, 4 URLs a defined, see the following list.
You can drop the provided PHP script somewhere or write your own server following the correct data formats.

- URL_TIME_GET (time.php): return the current date/time in `Y-m-d H:i:s.u` format (ex: `2021-05-10 11:24:36.254200`)
- URL_RING_TIME_GET (ring.php): return the next ring time in `Y-m-d H:i:s` format (ex: `2021-05-11 07:55:00`)
- URL_IMAGE_RAND (rand_image.php): return **raw** JPEG image data (`image/jpeg` content type)
- URL_RINGTONE_RAND (rand_ringtone.php): return the **absolute URL** of the MP3 file to play

### Configure ring time

To configure the time (and days of the week) of the alaram with the provided scripts, see `ring.php` → `ring_this_day()` function.

### Assets
Provided PHP scripts will look for the following sub-directories:

 - `images`: JPEG files, 320x160, **non-progressive**
 - `ringtones`: MP3 files, no duration limit (streamed)
