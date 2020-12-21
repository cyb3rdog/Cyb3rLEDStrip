#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


void DrawMarquee()
{
    static byte j = 0;
    j+=4;
    byte k = j;

    // Roughly equivalent to fill_rainbow(g_LEDs, NUM_LEDS, j, 8);

    CRGB c;
    for (int i = 0; i < FastLED.count(); i ++)
        FastLED.leds()[i] = c.setHue(k+=8);

    static int scroll = 0;
    scroll++;

    for (int i = scroll % 5; i < FastLED.count() - 1; i += 5)
    {
        FastLED.leds()[i] = CRGB::Black;
    }
    delay(EFFECT_SPEED);
}

void DrawMarqueeMirrored()
{
    static byte j = 0;
    j+=4;
    byte k = j;

    // Roughly equivalent to fill_rainbow(g_LEDs, NUM_LEDS, j, 8);

    CRGB c;
    for (int i = 0; i < (FastLED.count() + 1) / 2; i ++)
    {
        FastLED.leds()[i] = c.setHue(k);
        FastLED.leds()[FastLED.count() - 1 - i] = c.setHue(k);
        k+= 8;
    }

    static int scroll = 0;
    scroll++;

    for (int i = scroll % 5; i < FastLED.count() / 2; i += 5)
    {
        FastLED.leds()[i] = CRGB::Black;
        FastLED.leds()[FastLED.count() - 1 - i] = CRGB::Black;
    }

    delay(EFFECT_SPEED);
}

void DrawMarqueeScroll()
{
  static float scroll = 0.0f;
  scroll += 0.1f;
  if (scroll > 5.0)
    scroll -= 5.0;

  for (float i = scroll; i < NUM_LEDS/2 -1; i+= 5)
  {
    DrawPixels(i, 3.0, CRGB::Green);
    DrawPixels(NUM_LEDS-1-(int)i, 3, CRGB::Red);
  }
}