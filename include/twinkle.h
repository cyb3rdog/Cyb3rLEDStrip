#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

static const CRGB TwinkleColors [] = 
{
    CRGB::Red,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Green,
    CRGB::Yellow
};

void DrawTwinkle()
{
    static int passCount = 0;
    if (passCount++ == NUM_LEDS/4)
    {
        passCount = 0;
        FastLED.clear(false);
    }
    FastLED.leds()[random(NUM_LEDS)] = TwinkleColors[random(0, ARRAY_SIZE(TwinkleColors))];    
}
