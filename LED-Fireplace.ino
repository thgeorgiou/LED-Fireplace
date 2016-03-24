#include <SmartMatrix3.h>
#include <FastLED.h> // For some macros
#include <TimeLib.h>

// Matrix
#define COLOR_DEPTH 24
const uint8_t kMatrixWidth = 32;    // 32x16
const uint8_t kMatrixHeight = 16;
const uint8_t kRefreshDepth = 36;   // Known to work with this display, 32x32 would require another value
const uint8_t kDmaBufferRows = 4;       // We don't have any memory constraints
const uint8_t kPanelType = SMARTMATRIX_HUB75_16ROW_MOD8SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

/* Allocate buffer and one layer */
SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

/* How quickly does the flame cool? */
byte cooling = 100;

/* Change of making */
byte sparking = 30;

/* Buffer for flames */
byte _heat[32 * 16];

/* 0 = off, 1 = on, 2 = on w/ clock */
byte state = 1;

/* Rate limit clock refresh */
unsigned long lastClockRefresh = 0;

/* Return a color from the palette */
rgb24 getColor(int index);

/* Get the time from the internal RTC */
time_t getTeensy3Time();

/* Draw the time on the display */
void drawClock();

/* First time setup */
void setup() {
  // Use internal RTC
  setSyncProvider(getTeensy3Time);

  // Prepare matrix
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&indexedLayer);
  matrix.begin();
  matrix.setBrightness(100 * (255 / 100)); // Max brightness

  // Enable color correction
  backgroundLayer.enableColorCorrection(true);

  // Empty background
  backgroundLayer.fillScreen({0, 0, 0});
  backgroundLayer.swapBuffers();

  // Empty clock layer
  indexedLayer.fillScreen(0);
  indexedLayer.setFont(gohufont11b);
  indexedLayer.setIndexedColor(1, {150, 150, 150});
  indexedLayer.swapBuffers(false);
}

void loop() {
  uint16_t touch = touchRead(15);

  if (touch > 2000) {
    state++;
    if (state == 3) state = 0;

    // If state == 0 (off), clear screen)
    if (state == 0) {
      backgroundLayer.fillScreen({0, 0, 0});
      indexedLayer.fillScreen(0);
      backgroundLayer.swapBuffers();
      indexedLayer.swapBuffers();
    }
  }

  while (touchRead(15) > 2000) {}

  if (state == 0) return;

  // Step 1. Cool down every cell a little
  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 16; y++) {
      _heat[(y * 32) + x] = qsub8(_heat[(y * 32) + x], random(0, ((cooling * 10) / 16) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int y = 0; y < 16; y++) {
      _heat[(y * 32) + x] = (byte)(
                              (_heat[(((y + 1 > 13) ? 13 : y + 1) * 32) + x] +
                               _heat[(((y + 2 > 13) ? 13 : y + 2) * 32) + x] +
                               _heat[(((y + 3 > 13) ? 13 : y + 3) * 32) + x]) / 3
                            );
    }

    // Step 3.  Randomly ignite new 'sparks' of heat
    if (random(0, 233) < sparking) {
      // int x = (p[0] + p[1] + p[2]) / 3;

      _heat[(13 * 32) + x] = qadd8(_heat[(13 * 32) + x], random(160, 233));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int y = 0; y < 16; y++) {
      byte colorIndex = _heat[(y * 32) + x];

      // Recommend that you use values 0-240 rather than
      // the usual 0-233, as the last 13 colors will be
      // 'wrapping around' from the hot end to the cold end,
      // which looks wrong.
      colorIndex = scale8(colorIndex, 240);

      // Draw pixel
      rgb24 color = getColor(colorIndex);
      backgroundLayer.drawPixel(x, y, color);
    }
  }

  // Refresh screen
  backgroundLayer.swapBuffers();

  if (millis() - lastClockRefresh > 1000 && state == 2) {
    lastClockRefresh = millis();
    drawClock();
  }  
  delay(13);
}

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

rgb24 getColor(int index) {
  byte b = (byte) (index * (399 / 233));

  return {
    (uint8_t) b,
    (uint8_t) scale8(qsub8(b, 83), 233),
    (uint8_t) scale8(qsub8(8, 83 * 2), 233)
  };
}

void drawClock() {
  int x = kMatrixWidth / 2 - 15;
  char timeBuffer[9];
  // clear screen before writing new text
  indexedLayer.fillScreen(0);

  /* Draw Clock to SmartMatrix */
  uint8_t hours = hour();
  if (hours > 12)
    hours -= 12;
  sprintf(timeBuffer, "%d:%02d", hours, minute());
  if (hours < 10)
    x += 3;
  indexedLayer.setFont(gohufont11b);
  indexedLayer.drawString(x, kMatrixHeight / 2 - 6, 1, timeBuffer);
  indexedLayer.swapBuffers();
}

