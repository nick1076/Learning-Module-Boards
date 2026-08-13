#include <Adafruit_NeoPixel.h>

#define LED_PIN      D3
#define LED_COUNT    2

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);  // Set brightness level (0 to 255)
}

void loop() {

  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 150, 0)); 
    strip.show();
    delay(100);
  }

  delay(1000);

  strip.clear();
  strip.show();
  delay(1000);
}
