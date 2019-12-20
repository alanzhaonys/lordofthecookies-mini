//
// Lord Of The Cookies Mini Arduino Sketch
//
// Author: Alan Zhao
// Date: 11/10/2019
//

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
  #define PSTR // Make Arduino Due happy
#endif

#define DATA_PIN 8
#define TRIGGER_PIN A1
#define POT_PIN A0
#define BRIGHTNESS 5
#define WIDTH 8
#define HEIGHT 8

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, DATA_PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

void setup() {
  // Matrix setup
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(8);
  matrix.setTextColor(colors[0]);

  // Trigger pin setup
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // POT pin setup
  pinMode(POT_PIN, INPUT);

  // Starting
  Serial.begin(9600);
  Serial.println("Starting now...");
}

int x = matrix.width();
int pass = 0;

const int heart[] = {       // Heart bitmap
  0, 1, 1, 0, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

void scrollText(int brightness) {
  matrix.setBrightness(brightness);
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("MERRY XMAS!"));
  if (--x < -60) {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
}

void loop() {

  float potReading = analogRead(POT_PIN);
  //float brightness = (potReading / 1024.0) * 255;
  float brightness = map(potReading, 0, 1023, 0, 255);
  if (brightness == 0) {
    brightness = 1;
  }

  //Serial.println(brightness);

  //Serial.println(digitalRead(triggerPin));
  if (digitalRead(TRIGGER_PIN) == LOW) {
    scrollText(brightness);
  } else {
    Serial.println("Flashing LED...");
  }
}
