#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


//-------------------------------------------[ matrixEffect ]-----------------------------------------------------------------------------

// Initialize global variables for sequences.
uint8_t    palIndex =  95;
int        thisdir =   0;
bool        huerot =   1;                                     // Does the hue rotate? 1 = yes
uint8_t      bri = 16;

void matrixEffect() {

    if (huerot) palIndex++;

    //CRGB led = (thisdir == 0) ? FastLED.leds()[0] : FastLED.leds()[NUM_LEDS-1];
    CRGB led = (thisdir == 0) ? g_led_buffer[0] : g_led_buffer[NUM_LEDS-1];

    if (random8(90) > 80) {
      bri = 256 / 3;
      led = ColorFromPalette(g_currentPalette, palIndex, 255, g_currentBlending);
    } else {
      bri = bri - random(16);
      if (bri < 0 || bri > 256 / 3) bri = 0;
      //led = CHSV(96, SATURATION, bri);
      led = ColorFromPalette(g_currentPalette, palIndex -16, bri, g_currentBlending);
    }

    if (thisdir == 0) {
      g_led_buffer[0] = led;
      for (int i = NUM_LEDS-1; i >0 ; i-- ) g_led_buffer[i] = g_led_buffer[i-1];
    } else {
      g_led_buffer[NUM_LEDS - 1] = led;
      for (int i = 0; i < NUM_LEDS-1 ; i++ ) g_led_buffer[i] = g_led_buffer[i+1];
    }

    for (int i = 0; i < NUM_LEDS; i++ ) FastLED.leds()[i] = g_led_buffer[i];

} // matrixEffect()


//-------------------------------------------[ matrix ray ]------------------------------------------------------------------------------

void matrix_ray(uint8_t colorIndex) {                                                 // Send a PWM'd sinewave instead of a random happening of LED's down the strand.

  static uint8_t thisdir = 0;                                                         // We could change the direction if we want to. Static means to assign that value only once.
  static int thisphase = 0;                                                           // Phase change value gets calculated. Static means to assign that value only once.
  uint8_t thiscutoff;                                                                 // You can change the cutoff value to display this wave. Lower value = longer wave.

  thisphase += beatsin8(1,20, 50);                                                    // You can change direction and speed individually.
  thiscutoff = beatsin8(50,164,248);                                                  // This variable is used for the PWM of the lighting with the qsubd command below.

  int thisbright = qsubd(cubicwave8(thisphase), thiscutoff);                          // It's PWM time. qsubd sets a minimum value called thiscutoff. If < thiscutoff, then thisbright = 0. Otherwise, thisbright = thiscutoff.

  if (thisdir == 0) {                                                                 // Depending on the direction, we'll put that brightness in one end of the array. Andrew Tuline wrote this.
    FastLED.leds()[0] = ColorFromPalette(g_currentPalette, colorIndex, thisbright, LINEARBLEND);
    memmove(FastLED.leds()+1, FastLED.leds(), (NUM_LEDS-1)*3);                                            // Oh look, the FastLED method of copying LED values up/down the strand.
  } else {
    FastLED.leds()[NUM_LEDS-1] = ColorFromPalette( g_currentPalette, colorIndex, thisbright, LINEARBLEND);
    memmove(FastLED.leds(), FastLED.leds()+1, (NUM_LEDS-1)*3);
  }

} // matrix_ray()

void matrixRayEffect () {
                                                              // This section changes the delay, which adjusts how fast the 'rays' are travelling down the length of the strand.
  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,20,50);                     // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.

    matrix_ray(millis()>>4);                                  // This is the main function that's called. We could have not passed the millis()>>4, but it's a quick example of passing an argument.
  }

  EVERY_N_MILLISECONDS(100) {                                 // Fixed rate of a palette blending capability.
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    //static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    g_targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

} // matrixRayEffect()