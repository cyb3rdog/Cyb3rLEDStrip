#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


static const CRGB TwinkleColors [] =
{
    CRGB::Red,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Green,
    CRGB::Yellow,
    CRGB::Cyan
};

void twinkleEffect()
{
    static int passCount = 0;
    if (passCount++ == NUM_LEDS/4)
    {
        passCount = 0;
        FastLED.clear(false);
    }
    FastLED.leds()[random(NUM_LEDS)] = TwinkleColors[random(0, ARRAY_SIZE(TwinkleColors))];
}

void juggleEffect(int count = 8, int fadeSpeed = 50){

  fadeToBlackBy(g_led_buffer, NUM_LEDS, fadeSpeed);
  byte dothue = 0;

  for(int i = 0; i < count; i++) {
    g_led_buffer[beatsin16(i+count - 1,0,NUM_LEDS-1)] |= CHSV(dothue, SATURATION, 255);
    dothue += 256 / count;
  }

  DrawAllPixels(g_led_buffer);

}

void confettiEffect(){

  fadeToBlackBy(g_led_buffer, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  g_led_buffer[pos] += CHSV(g_hue + random8(64), SATURATION, 255);

  DrawAllPixels(g_led_buffer);

}
