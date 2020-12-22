#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


static uint8_t        g_hue = 0;
static CRGB           g_led_buffer[NUM_LEDS];
static TBlendType     g_currentBlending = LINEARBLEND;
static CRGBPalette16  g_currentPalette = RainbowColors_p;
static CRGBPalette16  g_targetPalette = g_currentPalette;


#include "EEPROM.h"
#include "ClickButton.h"

#include "utils.h"
#include "macros.h"
#include "effects-bounce.h"
#include "effects-comet.h"
#include "effects-demos.h"
#include "effects-fire.h"
#include "effects-marquee.h"
#include "effects-matrix.h"
#include "effects-ripple.h"
#include "effects-rainbow.h"
#include "effects-twinkle.h"


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

void DrawRainbowChase(){

  ChaseEffect();

} // DrawRainbowChase()

//--------------------[ Fire ]------------------------------------------------------------------------------

void DrawFireFromCenter(){                        // Fire From Center

  FastLED.clear(false);
  Fire(128,120, NUM_LEDS / 2,  1);
  Fire(128,120, NUM_LEDS / 2, -1);

} // DrawFire()

void DrawFireToCenter(){                          // Fire towards Center

  FastLED.clear(false);
  Fire(128,120, 0, 1);
  Fire(128,120, NUM_LEDS - 1, -1);

} // DrawFire()

void DrawChangingFire()                           // Palette changing fire from Center
{

  FastLED.clear(false);
  ChangingFire(30, 128,120, NUM_LEDS / 2, 1);
  ChangingFire(30, 128,120, NUM_LEDS / 2,-1);

} // DrawChangingFire()

void DrawRipple(){                                // Fireworks

  RippleEffect();

} // DrawRipple()

//--------------------[ Stripes ]------------------------------------------------------------------------------

void DrawComet() {

  CometRainbow(NUM_LEDS / 8, 64, 16);

} // DrawComet()

void DrawKITT() {

  CometKITT(NUM_LEDS / 8, 0, 16, 160);

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


BouncingBall balls[4];

void DrawBouncingBalls(){

  balls[0].HueColor = g_hue + 64 % 256;
  balls[1].HueColor = g_hue + 128 % 256;
  balls[2].HueColor = g_hue + 192 % 256;
  balls[2].HueColor = g_hue + 256 % 256;
  BouncingColoredBalls(balls, 4);

}

//--------------------[ Other ]------------------------------------------------------------------------------

void DrawRainbowSinelon() {                       // A colored dot sweeping back and forth, with fading trails.

  RainbowSinelon();

} // DrawSinelon()

void DrawConfetti() {                             // Random colored speckles that blink in and fade smoothly.

  confettiEffect();

} // DrawConfetti()

void DrawJuggle() {                               // Eight colored dots, weaving in and out of sync with each other.

  juggleEffect(6);

} // DrawJuggle()

void DrawTwinkle() {

  //twinkleEffect();
  if (random8() < 100)
    FastLED.leds()[random16(NUM_LEDS)] += ColorFromPalette(g_currentPalette, (1 ? random8() : g_hue), 255, g_currentBlending);

} // DrawTwinkle()

void DrawSnowSparke() {

  snowEffect(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);

} // DrawSnowSparke()

//--------------------[ BLACK OFF ]--------------------------------------------------------------------------

void DrawBlack(){                                 // Black OFF

  fill_solid(FastLED.leds(), NUM_LEDS, CRGB::Black);

} // DrawBlack()

//-------------------------------------------[ EFFECT LIST ]-------------------------------------------------------


typedef void (*SimplePatternList[])();            // List of effects to cycle through. Each is defined as a separate function.

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
  //fill_grad,

  /* STRIPES */
  DrawMarqueeMirrored,
  DrawRainbowChase,
  //DrawRainbowSinelon,
  DrawComet,
  DrawKITT,
  DrawMeteor,
  DrawRipple,
  DrawMatrix,
  DrawMatrixRay,
  DrawStripeMix,

  /* ---- FIRE ---- */
  DrawFireToCenter,
  DrawFireFromCenter,
  DrawChangingFire,

  /* ---- OTHER ---- */
  DrawConfetti,
  DrawTwinkle,
  DrawJuggle,

  //beatwaveEffect,
  //blendwaveEffect,
  blurEffect,

  //DrawBouncingBalls,
  DrawSnowSparke,
  //ease,

  /* ---- OFF ---- */
  DrawBlack // should to be last one
};

//-------------------------------------------[ Effects Library ]-------------------------------------------------------

class EffectsLibrary
{

protected:

  uint8_t     SwichButtonPin;
  uint8_t     EEPROM_Address;
  uint8_t     CurrentEffectNumber;
  uint8_t     CurrentBrightness;
  ClickButton Button;

private:

  void readbutton(){

    Button.Update();

    if (Button.clicks == 1) {                     // Just a click event to advance to next pattern
      CurrentEffectNumber = (CurrentEffectNumber + 1) % ARRAY_SIZE(EffectsList);
      EEPROM.write(EEPROM_Address, CurrentEffectNumber);
    }

    if (Button.clicks == 2) {                     // A double-click event to reset to 0 pattern
      CurrentEffectNumber = 0;
      EEPROM.write(EEPROM_Address, CurrentEffectNumber);
    }

    if (Button.clicks == 3) {                     // Triple-click to toggle auto-change mode
      SetAutoChange(!AutoChangeEnabled);
    }

    if (Button.clicks == -1) {                    // A hold event to change brightness
      SetBrightness(CurrentBrightness + 32 % 256);
      blink(CRGB::White, 100);
    }
    //if (Button.clicks == -2){
    //  SetBrightness(BRIGHTNESS);
    //  { blink(CRGB::White, 100); blink(CRGB::White, 100); blink(CRGB::White, 100); delay(50); }
    //}

  }

public:

  bool        AutoChangeEnabled = false;
  uint8_t     AutoChangeInterval = 5;

  EffectsLibrary(uint8_t switch_button_pin = 0, uint8_t eeprom_address = 0)
  : SwichButtonPin(switch_button_pin),
    EEPROM_Address(eeprom_address)
  {

    Button = ClickButton(switch_button_pin, LOW, CLICKBTN_PULLUP);

    CurrentEffectNumber = EEPROM.read(EEPROM_Address) % ARRAY_SIZE(EffectsList);
    if (CurrentEffectNumber < 0) CurrentEffectNumber = 0;
    CurrentBrightness = EEPROM.read(EEPROM_Address + 0x04) % 256;
    if (CurrentBrightness < 32) CurrentBrightness = BRIGHTNESS;
    AutoChangeEnabled = (EEPROM.read(EEPROM_Address + 0x08) % 1) == 1;

  } // EffectsLibrary() constructor


  void ChangeEffectByTime(uint8_t minutes = 5){

    EVERY_N_MINUTES(minutes){
      CurrentEffectNumber = (CurrentEffectNumber + 1) % ARRAY_SIZE(EffectsList);
      if (CurrentEffectNumber == ARRAY_SIZE(EffectsList) -1) CurrentEffectNumber = 0; // Dont auto change to OFF
      EEPROM.write(EEPROM_Address, CurrentEffectNumber);
    }

  } // ChangeEffectByTime()


  void SetAutoChange(bool autoChangeEnabled){

      AutoChangeEnabled = autoChangeEnabled;
      EEPROM.write(EEPROM_Address + 0x08, AutoChangeEnabled ? 1 : 0);

      if (AutoChangeEnabled) { blink(CRGB::Green, 150); blink(CRGB::Green, 150); blink(CRGB::Green, 200); delay(50); }
      else { blink(CRGB::Red, 150); blink(CRGB::Red, 150); blink(CRGB::Red, 150); delay(50); }

  } // SetAutoChange()

  void SetBrightness(uint8_t brightness){

      CurrentBrightness = brightness;
      FastLED.setBrightness(CurrentBrightness);
      EEPROM.write(EEPROM_Address + 0x04, CurrentBrightness);

  } // SetBrightness()


  void MainLoop(){

    readbutton();

    if (AutoChangeEnabled)
      ChangeEffectByTime(AutoChangeInterval);

    EVERY_N_MILLISECONDS(EFFECT_SPEED) {
      // Call the current pattern function once, updating the 'leds' array
      EffectsList[CurrentEffectNumber]();
      // Fade all effects with CurrentBrightness ratio
      fadeToBlackBy(FastLED.leds(), NUM_LEDS, 255 - CurrentBrightness);
    }

    EVERY_N_MILLISECONDS(COLOR_SPEED) {
      // slowly cycle the "base color" through the rainbow
      g_hue++;
    }

  } // MainLoop()

};