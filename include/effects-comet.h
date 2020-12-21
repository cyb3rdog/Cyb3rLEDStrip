#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


void CometKITT(int cometSize = 5, byte baseHueColor = 0, byte commetSpeed = 32, byte fadeSpeed = 128)
{  

    TIMES_PER_SECOND(EFFECT_SPEED)
    {
      fadeToBlackBy(FastLED.leds(), NUM_LEDS, fadeSpeed);
      int iPos = beatsin16(commetSpeed, 0, NUM_LEDS-cometSize);
      for (int i = iPos; i < iPos + cometSize; i++)
        FastLED.leds()[i] = CHSV(baseHueColor, SATURATION, 255);
    }

} // CometKITT()

void CometRainbow(int cometSize = 5, byte colorSpeed = 64, byte commetSpeed = 32, byte fadeSpeed = 128)
{  

    TIMES_PER_SECOND(EFFECT_SPEED)
    {
      fadeToBlackBy(FastLED.leds(), NUM_LEDS, fadeSpeed);
      int iPos = beatsin16(commetSpeed, 0, NUM_LEDS-cometSize);
      byte hue = beatsin8(colorSpeed);   
      for (int i = iPos; i < iPos + cometSize; i++)
        FastLED.leds()[i] = CHSV(hue, SATURATION, 255);
    }

} // CometRainbow()


void CometMeteorRain(byte red, byte green, byte blue, byte cometSize = 5, byte commetSpeed = 32, byte fadeSpeed = 92, boolean meteorRandomDecay = true)
{  

  TIMES_PER_SECOND(EFFECT_SPEED)
  {
    int iPos = beatsin16(commetSpeed, 0, NUM_LEDS-cometSize); 
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        FastLED.leds()[j].fadeToBlackBy(fadeSpeed);
      }
    }   
    // draw meteor
    for (int i = iPos; i < iPos + cometSize; i++)
      FastLED.leds()[i] = CRGB(red, green, blue);
  }
  
} // CometMeteorRain()
