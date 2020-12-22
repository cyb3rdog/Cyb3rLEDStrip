// FractionalColor
//
// Returns a fraction of a color; abstracts the fadeToBlack out to this function in case we
// want to improve the color math or do color correction all in one location at a later date.

inline CRGB ColorFraction(CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}

inline float RandomFloat()
{
    float r = random(1000000L) / 1000000.0f;
    return r;
}

void DrawAllPixels(CRGB* led_buffer){
  for (int i = 0; i < NUM_LEDS; i++ ) FastLED.leds()[i] = led_buffer[i];
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    FastLED.leds()[Pixel] = CRGB(255 / 256.0*BRIGHTNESS, 255 / 256.0*BRIGHTNESS, heatramp / 256.0*BRIGHTNESS);
  } else if( t192 > 0x40 ) {             // middle
    FastLED.leds()[Pixel] = CRGB(255 / 256.0*BRIGHTNESS, heatramp / 256.0*BRIGHTNESS, 0);
  } else {                               // coolest
    FastLED.leds()[Pixel] = CRGB(heatramp / 256.0*BRIGHTNESS, 0, 0);
  }
}

void blink(CRGB color, int duration)
{

  fill_solid(FastLED.leds(), NUM_LEDS, color);
  fadeToBlackBy(FastLED.leds(), NUM_LEDS, 255 - BRIGHTNESS);
  FastLED.show(BRIGHTNESS);
  delay(duration);

  fill_solid(FastLED.leds(), NUM_LEDS, CRGB::Black);
  FastLED.show(BRIGHTNESS);
  delay(duration / 2);

}

void addSnow(int flakes = 1){
  for (int i = 0; i < flakes; i++){
    FastLED.leds()[random(NUM_LEDS)] = CRGB(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);
  }
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    FastLED.leds()[ random16(NUM_LEDS) ] += CRGB(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);}
}
