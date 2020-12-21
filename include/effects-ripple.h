#include "FastLED.h"

// Min is 2 and value has to be divisible by two because each ripple has a left and right component. This cuts down on bouncing code.
#define maxRipples 6

//----------------- Ripple structure definition ----------------------------------------------------------------

// Reko Meriö's structures
struct ripple {

// Temporary local variables
  uint8_t brightness;                                   // 0 to 255
  int8_t color;                                         // 0 to 255
  int16_t pos;                                           // -1 to NUM_LEDS  (up to 127 LED's)
  int8_t velocity;                                      // 1 or -1
  uint8_t life;                                         // 0 to 20
  uint8_t maxLife;                                      // 10. If it's too long, it just goes on and on. . .
  uint8_t fade;                                         // 192
  bool exist;                                           // 0 to 1

  void Move() {

    pos += velocity;
    life++;

    if (pos > NUM_LEDS - 1) {                           // Bounce back from far end.
      velocity *= -1;
      pos = NUM_LEDS - 1;
    }

    if (pos < 0) {                                      // Bounce back from 0.
      velocity *= -1;
      pos = 0;
    }

    brightness = scale8(brightness, fade);              // Adjust brightness accordingly to strip length

    if (life > maxLife) exist = false;                  // Kill it once it's expired.

  } // Move()

  void Init(uint8_t Fade, uint8_t MaxLife) {                 // Typically 216, 20

    pos = random8(NUM_LEDS/8, NUM_LEDS-NUM_LEDS/8);     // Avoid spawning too close to edge.
    velocity = 1;                                       // -1 or 1
    life = 0;                                           // 0 to Maxlife
    maxLife = MaxLife;                                  // 10 called
    exist = true;                                       // 0, 1
    brightness = 255;                                   // 0 to 255
    color = millis();                                   // hue;
    fade = Fade;                                        // 192 called

  } // Init()

}; // struct ripple

typedef struct ripple Ripple;

Ripple ripples[maxRipples];


void rippless() {

  for (int i = 0; i < maxRipples; i += 2) {                                       // Check to see if ripple has expired, and if so, create a new one.
    if (random8() > 224 && !ripples[i].exist) {                                   // Randomly create a new ripple if this one has expired.
      ripples[i].Init(200, 10);                                                   // Initialize the ripple array with Fade and MaxLife values.
      ripples[i + 1] = ripples[i];                                                // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
      ripples[i + 1].velocity *= -1;                                              // We want the other half of the ripple to go opposite direction.
    }
  }

  for (int i = 0; i < maxRipples; i++) {                                          // Move the ripple if it exists
    if (ripples[i].exist) {
      FastLED.leds()[ripples[i].pos] = ColorFromPalette(g_currentPalette, ripples[i].color, ripples[i].brightness, LINEARBLEND);
      ripples[i].Move();
    }
  }

  fadeToBlackBy(FastLED.leds(), NUM_LEDS, 160);

} // rippless()


void SetupMySimilar4Palette() {                                                   // Create a palette with colours close to each other.

  uint8_t thishue = random8();

  g_targetPalette = CRGBPalette16(CHSV(thishue+random8(32), 255, random8(128,255)),
                                CHSV(thishue+random8(32), 255, random8(192,255)),
                                CHSV(thishue+random8(32), 192, random8(192,255)),
                                CHSV(thishue+random8(32), 255, random8(128,255)));

} // SetupMySimilar4Palette()


void RipplessEffect() {

  EVERY_N_MILLISECONDS(50) {                                                      // Smooth palette transitioning runs continuously.
    uint8_t maxChanges = 24;
      nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);
  }

  EVERY_N_SECONDS(5) {
    SetupMySimilar4Palette();                                                     // Change palette colours every 5 seconds.
  }

  EVERY_N_MILLIS(50) {                                                            // Sets the original delay time.
    rippless();                                                                   // Run the ripple routine.
  }

} // RipplessEffect()


//-------------------------------------------[ ripple ]------------------------------------------------------------------------------


uint8_t rippleColour;                                               // Ripple colour is randomized.
int rippleCenter = 0;                                               // Center of the current ripple.
int rippleStep = -1;                                                // -1 is the initializing step.
uint8_t rippleFade = 255;                                           // Starting brightness.
#define rippleMaxSteps 12                                           // Case statement wouldn't allow a variable.
uint8_t rippleFader = 50;


void ripple() {

  fadeToBlackBy(g_led_buffer, NUM_LEDS, rippleFader);                 // 8 bit, 1 = slow, 255 = fast

  switch (rippleStep) {

    case -1:                                                 // Initialize ripple variables.
      rippleCenter = random(NUM_LEDS);
      rippleColour = random8();
      rippleStep = 0;
      break;

    case 0:
      g_led_buffer[rippleCenter] = ColorFromPalette(g_currentPalette, rippleColour, rippleFade, g_currentBlending);

      rippleStep ++;
      break;

    case rippleMaxSteps:                                      // At the end of the ripples.
      rippleStep = -1;
      break;

    default:                                                  // Middle of the ripples.
      g_led_buffer[(rippleCenter + rippleStep + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(g_currentPalette, rippleColour, rippleFade/rippleStep*2, g_currentBlending);       // Simple wrap from Marc Miller
      g_led_buffer[(rippleCenter - rippleStep + NUM_LEDS) % NUM_LEDS] += ColorFromPalette(g_currentPalette, rippleColour, rippleFade/rippleStep*2, g_currentBlending);
      rippleStep ++;                                          // Next step.
      break;
  } // switch step

  DrawAllPixels(g_led_buffer);

} // ripple()


void RippleEffect () {

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(3) {
    g_targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255));
  }

  EVERY_N_MILLISECONDS(EFFECT_SPEED) {                                   // FastLED based non-blocking delay to update/display the sequence.
    ripple();
  }

} // rippleEffect()


