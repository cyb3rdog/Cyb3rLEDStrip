#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


void RainbowTheaterChase()
{
  byte *c;


        for (int i=0; i < NUM_LEDS; i++) {
          c = Wheel(((i * 256 / NUM_LEDS) + g_hue) & 255);
          FastLED.leds()[i] = CRGB(*c, *(c+1), *(c+2));
        }

        for (int i=NUM_LEDS-beatsin8(180,0,1); i >= 0; i=i-2) {
          FastLED.leds()[i] = CRGB(0,0,0);  //turn every third pixel off
        }

}

void TheaterChase(byte red, byte green, byte blue, int SpeedDelay)
{
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        FastLED.leds()[i + q] = CRGB(red, green, blue);
      }
      FastLED.show(BRIGHTNESS);
      delay(SpeedDelay);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        FastLED.leds()[i+q] = CRGB(0,0,0);  //turn every third pixel off
      }
    }
  }
}


void Sparkle(byte red, byte green, byte blue, int SparkleDelay = 5)
{
  int Pixel = random(NUM_LEDS);
  FastLED.leds()[Pixel] = CRGB(red, green, blue);
  FastLED.show(BRIGHTNESS);
  delay(SparkleDelay);
  FastLED.leds()[Pixel] = CRGB::Black;
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay = 5)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    FastLED.leds()[i] = CRGB(red, green, blue);
  }

  int Pixel = random(NUM_LEDS);
  FastLED.leds()[Pixel] = CRGB(255, 255, 255);
  FastLED.show(BRIGHTNESS);
  delay(SparkleDelay);

  FastLED.leds()[Pixel] = CRGB(red, green, blue);
  FastLED.show(BRIGHTNESS);
}