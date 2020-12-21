// FramesPerSecond
//
// Tracks a weighted average to smooth out the values that it calcs as the simple reciprocal
// of the amount of time taken specified by the caller.  So 1/3 of a second is 3 fps, and it
// will take up to 10 frames or so to stabilize on that value.

double FramesPerSecond(double seconds)
{
  static double framesPerSecond;
  framesPerSecond = (framesPerSecond * .9) + (1.0 / seconds * .1);
  return framesPerSecond;
}


inline float RandomFloat()
{
    float r = random(1000000L) / 1000000.0f;
    return r;
}

// FractionalColor
//
// Returns a fraction of a color; abstracts the fadeToBlack out to this function in case we
// want to improve the color math or do color correction all in one location at a later date.

CRGB ColorFraction(CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}

void DrawPixels(float fPos, float count, CRGB color)
{
  // Calculate how much the first pixel will hold
  float availFirstPixel = 1.0f - (fPos - (long)(fPos));
  float amtFirstPixel = min(availFirstPixel, count);
  float remaining = min(count, FastLED.size()-fPos);
  int iPos = fPos;

  // Blend (add) in the color of the first partial pixel

  if (remaining > 0.0f)
  {
    FastLED.leds()[iPos++] += ColorFraction(color, amtFirstPixel);
    remaining -= amtFirstPixel;
  }

  // Now draw any full pixels in the middle

  while (remaining > 1.0f)
  {
    FastLED.leds()[iPos++] += color;
    remaining--;
  }

  // Draw tail pixel, up to a single full pixel

  if (remaining > 0.0f)
  {
    FastLED.leds()[iPos] += ColorFraction(color, remaining);
  }
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



void addSnow(int flakes = 1){
  for (int i = 0; i < flakes; i++){
    FastLED.leds()[random(NUM_LEDS)] = CRGB(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);
  }
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    FastLED.leds()[ random16(NUM_LEDS) ] += CRGB(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);}
}

