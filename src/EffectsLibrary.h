#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


static uint8_t        g_hue = 0;
static CRGB           g_led_buffer[NUM_LEDS];
static TBlendType     g_currentBlending = LINEARBLEND;
static CRGBPalette16  g_currentPalette = RainbowColors_p;
static CRGBPalette16  g_targetPalette = g_currentPalette;


#include "EEPROM.h"
#include "button.h"

#include "utils.h"
#include "macros.h"
#include "effects.h"
#include "effects-fire.h"
#include "effects-comet.h"
#include "effects-matrix.h"
#include "effects-demos.h"
#include "effects-ripple.h"
#include "effects-rainbow.h"


#include "bounce.h"
#include "marquee.h"
#include "twinkle.h"



//--------------------[ Rainbows ]------------------------------------------------------------------------------


void DrawRainbow() {                              // FastLED's built-in rainbow generator.

  //fill_rainbow(leds, NUM_LEDS, g_hue, 7);
  RainbowBeat();

} // DrawRainbow()


void DrawRainbowWithGlitter() {                   // Built-in FastLED rainbow, plus some random sparkly glitter.

  fill_rainbow(FastLED.leds(), NUM_LEDS, g_hue, 7);
  addGlitter(80);

} // DrawRainbowWithGlitter()


void DrawRainbowMarch(){                          // Just Rainbow March

  RainbowMarch(EFFECT_SPEED*4, 10);

} // DrawRainbowMarch()


void DrawRainbowCycle() {                         // Cutstom Full Rainbow

  RainbowCycle(COLOR_SPEED, 1);

} // DrawRainbowCycle()


void DrawRainbowPride(){                          // Pride Rainbow

  RainbowPride();

} // DrawPride()


void DrawPrideWithGlitter(){                      // Pride Rainbow with Glitter

  RainbowPride();
  addGlitter(70);

} // DrawPrideWithGlitter()


void DrawRainbowFade() {                          // Fade Effect with Pallete

  g_currentBlending = LINEARBLEND;
  g_currentPalette = RainbowStripeColors_p;
  fadeEffect();

} // DrawFadeEffect()


void DrawRainbowBPM() {                           // Colored stripes pulsing at a defined Beats-Per-Minute.

  RainbowBPM();

} // DrawRainbowBPM()


void DrawRainbowLong() {                          // Custom long rainbow

  RainbowLong(1, 64);
  addSnow();

} // DrawRainbowLong


void DrawRainbowTheatre(){

  RainbowTheaterChase();

} // ERROR


//--------------------[ Fire ]------------------------------------------------------------------------------

void DrawFireFromCenter(){                        // Fire From Center

  FastLED.clear();
  Fire(128,120, NUM_LEDS / 2, 1);
  Fire(128,120, NUM_LEDS / 2, -1);

} // DrawFire();

void DrawFireToCenter(){                          // Fire towards Center

  FastLED.clear();
  Fire(128,120, 0, 1);
  Fire(128,120, NUM_LEDS - 1, -1);

} // DrawFire();


void DrawChangingFire()                           // Palette changing fire from Center
{

  FastLED.clear();
  ChangingFire(COLOR_SPEED, 128,120, NUM_LEDS / 2, 1);
  ChangingFire(COLOR_SPEED, 128,120, NUM_LEDS / 2, -1);

}

void DrawRipple(){                               // Fireworks

  RippleEffect();

}

//--------------------[ Stripes ]------------------------------------------------------------------------------

void DrawComet() {

  CometRainbow(NUM_LEDS / 8);

} // DrawComet()


void DrawKITT() {

  CometKITT(NUM_LEDS / 8);

} // DrawKITT()


void DrawMeteor() {

  CometMeteorRain(0xaa,0x44,0xff, NUM_LEDS / 8, 20);

} // DrawMeteor()


void DrawStripeMix(){

  stripeMixEffect();

} // DrawStripeMix()


void DrawMatrix(){

  //g_currentPalette = CRGBPalette16(CRGB::ForestGreen, CRGB::Green, CRGB::DarkGreen, CRGB::LimeGreen);
  g_currentPalette = ForestColors_p;
  g_targetPalette = g_currentPalette;
  g_currentBlending = LINEARBLEND;

  matrixEffect();

} // DrawMatrix()


void DrawMatrixRay(){

  // Palette definitions
  g_currentPalette = RainbowStripeColors_p;
  g_targetPalette = g_currentPalette;
  g_currentBlending = LINEARBLEND;

  matrixRayEffect();

} // DrawMatrixRay()


void DrawTest(){

  Animations();

}


//--------------------[ Other ]------------------------------------------------------------------------------


void DrawRainbowSinelon() {                                   // A colored dot sweeping back and forth, with fading trails.

  RainbowSinelon();

} // DrawSinelon()


void DrawConfetti() {                                         // Random colored speckles that blink in and fade smoothly.

  fadeToBlackBy(FastLED.leds(), NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  FastLED.leds()[pos] += CHSV(g_hue + random8(64), SATURATION, 255);

} // DrawConfetti()


void juggle() {                                               // Eight colored dots, weaving in and out of sync with each other.

  fadeToBlackBy(FastLED.leds(), NUM_LEDS, 20);
  byte dothue = 0;

  for(int i = 0; i < 8; i++) {
    FastLED.leds()[beatsin16(i+7,0,NUM_LEDS-1)] |= CHSV(dothue, SATURATION, 255);
    dothue += 32;
  }

} // juggle()


void twinkle() {

  if (random8() < 100) FastLED.leds()[random16(NUM_LEDS)] += ColorFromPalette(g_currentPalette, (1 ? random8() : g_hue), 255, g_currentBlending);

} // twinkle()


//--------------------[ BLACK OFF ]--------------------------------------------------------------------------

void DrawBlack(){                                            // Black OFF
  fill_solid(FastLED.leds(), NUM_LEDS, CRGB::Black);

} // DrawBlack()

//-------------------------------------------[ EFFECT LIST ]-------------------------------------------------------


typedef void (*SimplePatternList[])();                        // List of effects to cycle through. Each is defined as a separate function.

SimplePatternList EffectsList = {
  /* ---- RAINBOWS ---- */
  DrawRainbowCycle,
  DrawRainbow,
  DrawRainbowWithGlitter,
  DrawRainbowMarch,
  DrawRainbowPride,
  DrawPrideWithGlitter,
  DrawRainbowFade,
  DrawRainbowBPM,
  DrawRainbowLong,
  //DrawRainbowTheatre,
  //fill_grad,
  DrawTest,

  /* STRIPES */
  //drawMover,
  DrawRainbowSinelon,
  DrawMeteor,
  DrawComet,
  DrawKITT,
  DrawRipple,
  DrawStripeMix,

  DrawMatrix,
  DrawMatrixRay,

  /* ---- FIRE ---- */
  DrawFireToCenter,
  DrawFireFromCenter,
  DrawChangingFire,

  /* ---- OTHER ---- */
  //DrawConfetti,
  juggle,
  twinkle,
  //beatwaveEffect,
  //blendwaveEffect,
  blurEffect,

  //ease,

  /* ---- OFF ---- */
  DrawBlack
};

//-------------------------------------------[ Effects Library ]-------------------------------------------------------

class EffectsLibrary
{

protected:

  uint8_t     SwichButtonPin;
  uint8_t     EEPROM_Address;
  uint8_t     CurrentEffectNumber;
  uint8_t     CurrentBrightness;

private:

  void readbutton(){

    uint8_t b = checkButton(SwichButtonPin);

    if (b == 1) {                                               // Just a click event to advance to next pattern
      CurrentEffectNumber = (CurrentEffectNumber + 1) % ARRAY_SIZE(EffectsList);
      EEPROM.write(EEPROM_Address, CurrentEffectNumber);
    }

    if (b == 2) {                                               // A double-click event to reset to 0 pattern
      CurrentEffectNumber = 0;
      EEPROM.write(EEPROM_Address, CurrentEffectNumber);
    }

    if (b == 3) {                                               // A hold event to write current pattern to EEPROM
      CurrentBrightness += 32 % 256;
      EEPROM.write(EEPROM_Address + 0x04, CurrentBrightness);
    }
  }

public:

  EffectsLibrary(uint8_t switch_button_pin = 0, uint8_t eeprom_address = 0)
  : SwichButtonPin(switch_button_pin),
    EEPROM_Address(eeprom_address)
  {

    CurrentEffectNumber = EEPROM.read(EEPROM_Address) % ARRAY_SIZE(EffectsList);
    if (CurrentEffectNumber < 0) CurrentEffectNumber = 0;
    CurrentBrightness = EEPROM.read(EEPROM_Address + 0x04) % 256;
    if (CurrentBrightness < 32) CurrentBrightness = BRIGHTNESS;

  } // EffectsLibrary() constructor


  void ChangeEffectByTime(int minutes = 5){

    EVERY_N_MINUTES(minutes ){
      CurrentEffectNumber = (CurrentEffectNumber + 1) % ARRAY_SIZE(EffectsList);
      EEPROM.write(EEPROM_Address, CurrentEffectNumber);
    }

  } // ChangeEffectByTime()


  void ChangeEffect(){

      CurrentEffectNumber = (CurrentEffectNumber + 1) % ARRAY_SIZE(EffectsList);

  } // ChangeEffect()


  void MainLoop(){

    readbutton();

    EVERY_N_MILLISECONDS(EFFECT_SPEED) {
      EffectsList[CurrentEffectNumber]();         // Call the current pattern function once, updating the 'leds' array
      fadeToBlackBy(FastLED.leds(), NUM_LEDS, 255 - CurrentBrightness);
    }

    EVERY_N_MILLISECONDS(COLOR_SPEED) {               // slowly cycle the "base color" through the rainbow
      g_hue++;
    }

  } // MainLoop()

};