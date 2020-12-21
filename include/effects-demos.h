#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


//-------------------------------------------[ RainbowMarchDemo ]------------------------------------------------------------------------------

// Initialize global variables for sequences
uint8_t rainbowMatchHue = 0;                // Starting hue value.
uint8_t rainbowMarchRotation = 1;            // Hue rotation speed. Includes direction.
uint8_t rainbowMatchDeltahue = 1;           // Hue change between pixels.
bool rainbowMarchDirection = 0;             // I use a direction variable instead of signed math so I can use it in multiple routines.

void rainbowMarchEffect(int thishue = 0) {  // The fill_rainbow call doesn't support brightness levels. You would need to change the max_bright value.

  if (rainbowMarchDirection == 0)
    rainbowMatchHue += rainbowMarchRotation;
  else
    rainbowMatchHue -= rainbowMarchRotation;  // I could use signed math, but 'thisdir' works with other routines.

  fill_rainbow(FastLED.leds(), NUM_LEDS, rainbowMatchHue, rainbowMatchDeltahue);            // I don't change deltahue on the fly as it's too fast near the end of the strip.

} // rainbow_march()

void ChangeRainbowMarch() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.

  uint8_t secondHand = (millis() / 1000) % 60;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: rainbowMarchRotation=1; rainbowMatchDeltahue=5; break;
      case  5: rainbowMarchDirection=-1; rainbowMatchDeltahue=10; break;
      case 10: rainbowMarchRotation=5; break;
      case 15: rainbowMarchRotation=5; rainbowMarchDirection=-1; rainbowMatchDeltahue=20; break;
      case 20: rainbowMatchDeltahue=30; break;
      case 25: rainbowMatchDeltahue=2; rainbowMarchRotation=5; break;
      case 30: break;
    }
  }

} // ChangeRainbowMarch()

void RainbowMarchDemo () {

  ChangeRainbowMarch();
  EVERY_N_MILLISECONDS(EFFECT_SPEED) {                           // FastLED based non-blocking delay to update/display the sequence.
    rainbowMarchEffect();
  }

} // RainbowMarchDemo()

//-------------------------------------------[ beatwaveEffect ]------------------------------------------------------------------------------

void beatwaveEffect()
{

  uint8_t wave1 = beatsin8(9, 0, 255);  // That's the same as beatsin8(9);
  uint8_t wave2 = beatsin8(8, 0, 255);
  uint8_t wave3 = beatsin8(7, 0, 255);
  uint8_t wave4 = beatsin8(6, 0, 255);

  for (int i=0; i<NUM_LEDS; i++) {
    FastLED.leds()[i] = ColorFromPalette( g_currentPalette, i+wave1+wave2+wave3+wave4, 255, g_currentBlending);
  }

  EVERY_N_MILLISECONDS(EFFECT_SPEED) {
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {  // Change the target palette to a random one every 5 seconds.
    g_targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

} // beatwaveEffect()

//-------------------------------------------[ blendwaveEffect ]------------------------------------------------------------------------------

CRGB clr1;
CRGB clr2;
uint8_t speed;
uint8_t loc1;
uint8_t loc2;
uint8_t ran1;
uint8_t ran2;

void blendwaveEffect() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);

  fill_gradient_RGB(FastLED.leds(), 0, clr2, loc1, clr1);
  fill_gradient_RGB(FastLED.leds(), loc1, clr2, NUM_LEDS-1, clr1);

} // blendwaveEffect()

//-------------------------------------------[ stripeMixEffect ]------------------------------------------------------------------------------

CRGB ledsB[NUM_LEDS];

void animationA() {                                             // running red stripe.

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 10) + (i * 12);                    // speed, length
    if (red > 128) red = 0;
    g_led_buffer[i] = CRGB(red, 0, 0);
  }
} // animationA()

void animationB() {                                               // running green stripe in opposite direction.
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t green = (millis() / 5) - (i * 12);                    // speed, length
    if (green > 128) green = 0;
    ledsB[i] = CRGB(0, green, 0);
  }
} // animationB()

void stripeMixEffect()
{

    animationA();                                               // render the first animation into leds2
    animationB();                                               // render the second animation into leds3

    uint8_t ratio = beatsin8(2, 64, 255);                       // Alternate every minute

    for (int i = 0; i < NUM_LEDS; i++) {                        // mix the 2 arrays together
      FastLED.leds()[i] = blend( g_led_buffer[i], ledsB[i], ratio );
    }

} // stripeMixEffect()

//-------------------------------------------[ blur ]------------------------------------------------------------------------------

void blurEffect()
{

  uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
  blur1d( FastLED.leds(), NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.

  uint8_t  i = beatsin8(  9, 0, NUM_LEDS);
  uint8_t  j = beatsin8( 7, 0, NUM_LEDS);
  uint8_t  k = beatsin8(  5, 0, NUM_LEDS);

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();
  FastLED.leds()[(i+j)/2] = CHSV( ms / 29, 200, 255);
  FastLED.leds()[(j+k)/2] = CHSV( ms / 41, 200, 255);
  FastLED.leds()[(k+i)/2] = CHSV( ms / 73, 200, 255);
  FastLED.leds()[(k+i+j)/3] = CHSV( ms / 53, 200, 255);

} // blurEffect()

//-------------------------------------------[ easeEffect ]------------------------------------------------------------------------------

void easeEffect() {

  static uint8_t easeOutVal = 0;
  static uint8_t easeInVal  = 0;
  static uint8_t lerpVal    = 0;

  easeOutVal = ease8InOutQuad(easeInVal);                     // Start with easeInVal at 0 and then go to 255 for the full easing.
  easeInVal++;

  lerpVal = lerp8by8(0, NUM_LEDS, easeOutVal);                // Map it to the number of LED's you have.

  FastLED.leds()[lerpVal] = CRGB::Red;
  fadeToBlackBy(FastLED.leds(), NUM_LEDS, 16);                          // 8 bit, 1 = slow fade, 255 = fast fade

} // easeEffect()

//-------------------------------------------[ fadeEffect ]------------------------------------------------------------------------------

void fadein() {

  random16_set_seed(535);                                                           // The randomizer needs to be re-set each time through the loop in order for the 'random' numbers to be the same each time through.

  for (int i = 0; i<NUM_LEDS; i++) {
    uint8_t fader = sin8(millis()/random8(10,20));                                  // The random number for each 'i' will be the same every time.
    FastLED.leds()[i] = ColorFromPalette(g_currentPalette, i*20, fader, g_currentBlending);       // Now, let's run it through the palette lookup.
  }

  random16_set_seed(millis());                                                      // Re-randomizing the random number seed for other routines.

} // fadein()

void fadeEffect(){

  EVERY_N_MILLISECONDS(100) {                                                 // FastLED based non-blocking FIXED delay.
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);    // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {                                                        // Change the target palette to a random one every 5 seconds.
    uint8_t baseC = random8(255);                                             // Use the built-in random number generator as we are re-initializing the FastLED one.
    g_targetPalette = CRGBPalette16(CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 192, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)));
  }

  fadein();

} // fadeEffect()

//-------------------------------------------[ fillGradientEffect ]------------------------------------------------------------------------------

void fillGradientEffect() {

  uint8_t starthue = beatsin8(5, 0, 255);
  uint8_t endhue = beatsin8(7, 0, 255);

  if (starthue < endhue) {
    fill_gradient(FastLED.leds(), NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around.
  } else {
    fill_gradient(FastLED.leds(), NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
  }

} // fillGradientEffect()

//-------------------------------------------[ moverEffect ]------------------------------------------------------------------------------

static int16_t dist;                                          // A moving location for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.

void inoise8_mover() {

  uint8_t locn = inoise8(xscale, dist+yscale) % 255;          // Get a new pixel location from moving noise.
  uint8_t pixlen = map(locn,0,255,0,NUM_LEDS);                // Map that to the length of the strand.
  FastLED.leds()[pixlen] = ColorFromPalette(g_currentPalette, pixlen, 255, LINEARBLEND);   // Use that value for both the location as well as the palette index colour for the pixel.

  dist += beatsin8(10,1,4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.

} // inoise8_mover()

void moverEffect(int maxChanges = 24) {

  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);   // AWESOME palette blending capability.

    inoise8_mover();                                          // Update the LED array with noise at the new location
    fadeToBlackBy(FastLED.leds(), NUM_LEDS, 4);
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    g_targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

} // moverEffect()


//-------------------------------------------[ plasmaEffect ]------------------------------------------------------------------------------

void plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.

  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

    FastLED.leds()[k] = ColorFromPalette(g_currentPalette, colorIndex, thisBright, g_currentBlending);  // Let's now add the foreground colour.
  }

} // plasma()

void plasmaEffect () {

  EVERY_N_MILLISECONDS(50) {                                  // FastLED based non-blocking delay to update/display the sequence.
    plasma();
  }

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {                                 // Change the target palette to a random one every 5 seconds.
    uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    g_targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
  }

} // plasmaEffect()

//-------------------------------------------[ sawtooth ]------------------------------------------------------------------------------

void sawtoothEffect() {

  int bpm = 60;
  int ms_per_beat = 60000/bpm;                                // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
  int ms_per_led = 60000/bpm/NUM_LEDS;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(NUM_LEDS);     // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
   fill_solid(FastLED.leds(), NUM_LEDS, CRGB::Black);
  else
    FastLED.leds()[cur_led] = ColorFromPalette(g_currentPalette, 0, 255, g_currentBlending);    // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtoothEffect()


//-------------------------------------------[  ]------------------------------------------------------------------------------
