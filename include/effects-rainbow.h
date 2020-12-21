#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>



void RainbowCycle(int SpeedDelay = 10, byte Amount = 1) {
  byte *c;
  for(int i=0; i< NUM_LEDS; i++) {
    c = Wheel(((i * 256 * Amount / NUM_LEDS) + g_hue) & 255);
    FastLED.leds()[i] = CRGB(*c, *(c+1), *(c+2));
  }
}

void RainbowBeat(){

  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(FastLED.leds(), NUM_LEDS, (beatA+beatB)/2, 8);            // Use FastLED's fill_rainbow routine.

}

void RainbowLong(byte DENSITY = 2, byte TRESHOLD = 64)
{

  for (int i = 0; i < NUM_LEDS; i++)
    FastLED.leds()[i] = CHSV(i - (g_hue * DENSITY), SATURATION, 255);

}

void RainbowPride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  //uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint8_t brightdepth = beatsin88( 341, 32, BRIGHTNESS);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;

  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;

    nblend( FastLED.leds()[pixelnumber], newcolor, 64);
  }
}

void PrideRider(int EyeSize = 8, int SpeedDelay = 15){
  for(int i = 0; i < NUM_LEDS-EyeSize; i++) {
    FastLED.clear();
    for(int j = 0; j <= EyeSize; j++) {
      FastLED.leds()[i+j] = CHSV(i * (256 / (NUM_LEDS-EyeSize))-j, SATURATION, 255);
    }
    FastLED.show(BRIGHTNESS);
    delay(SpeedDelay);
  }
  delay(EFFECT_SPEED);
  for(int i = NUM_LEDS-EyeSize; i > 0; i--) {
    FastLED.clear();
    for(int j = 0; j <= EyeSize; j++) {
      FastLED.leds()[i+j] = CHSV(i * (256 / (NUM_LEDS-EyeSize))+j, SATURATION, 255);
    }
    FastLED.show(BRIGHTNESS);
    delay(SpeedDelay);
  }
  delay(EFFECT_SPEED);
}

void RainbowBPM() {

  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);

  for(int i = 0; i < NUM_LEDS; i++) { //9948
    FastLED.leds()[i] = ColorFromPalette(palette, g_hue+(i*2), beat-g_hue+(i*10));
  }
}

void RainbowSinelon(){

  //fadeToBlackBy(FastLED.leds(), NUM_LEDS, 20);
  //int pos = beatsin16(13,0,NUM_LEDS-1);
  //FastLED.leds()[pos] += CHSV(g_hue, SATURATION, 255);

  int pos1 = beatsin16(COLOR_SPEED,0,NUM_LEDS - 1);
  int pos2 = beatsin16(COLOR_SPEED,0,NUM_LEDS - 1);
  FastLED.leds()[(pos1+pos2)/2] += ColorFromPalette(g_currentPalette, g_hue, 255, g_currentBlending);

}

void RainbowMarch(uint8_t thisdelay, uint8_t deltahue) {       // The fill_rainbow call doesn't support brightness levels.

  uint8_t thishue = millis()*(255-thisdelay)/255;

  //thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
  //thishue = beatsin8(EFFECT_SPEED,0,255);                    // This can change speeds on the fly. You can also add these to each other.

  fill_rainbow(FastLED.leds(), NUM_LEDS, thishue, deltahue);             // Use FastLED's fill_rainbow routine.

} // RainbowMarch()
