#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


void DrawMarqueeMirrored()
{
  static byte j = 0;
  j+=4;
  byte k = j;

  CRGB c;
  for (int i = 0; i < (NUM_LEDS + 1) / 2; i ++)
  {
      g_led_buffer[i] = c.setHue(k);
      g_led_buffer[NUM_LEDS - 1 - i] = c.setHue(k);
      k+= 8;
  }

  static int scroll = 0;
  scroll++;

  for (int i = scroll % 5; i < NUM_LEDS / 2; i += 5)
  {
      g_led_buffer[i] = CRGB::Black;
      g_led_buffer[NUM_LEDS - 1 - i] = CRGB::Black;
  }

  DrawAllPixels(g_led_buffer);
}


int16_t pos;                    // Pixel position.
uint8_t width = 3;              // Marque width.
uint8_t advance = -1 * width;   // Stores the advance amount.

void ChaseEffect(uint8_t hueShift = 10, uint8_t spacing = 5)
{
  uint8_t delta = 1;            // Sets forward or backwards direction amount. (Can be negative.)

  /*
  EVERY_N_SECONDS(10){  // Demo: Change direction every N seconds.
    delta = -1*delta;
  }
  */

  //EVERY_N_SECONDS(10){  // Demo: Changing the pixel spacing every N seconds.
  //  spacing = spacing + 1;
  //  if (spacing == 9) { spacing = 2; }  // Reset spacing to 2
  //  if (spacing >4) { spacing = 8; }  // Jump spacing up to 8
  //}

  EVERY_N_MILLISECONDS(EFFECT_SPEED * 2){  // Advance pixels to next position.

    // Advance pixel postion down strip, and rollover if needed.
    advance = (advance + delta + NUM_LEDS) % NUM_LEDS;

    // Fade out tail or set back to black for next loop around.
    for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint8_t w = 0; w<width; w++){
        //pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        pos = (spacing * (i-1) + spacing + advance + w - 1) % NUM_LEDS;
        g_led_buffer[pos] = CRGB::Black;
      }
    }

    // Update pixels down the strip.
    for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint8_t w = 0; w<width; w++){
        pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        uint8_t color = g_hue;
        if ( w % 2 == 1 ) color = g_hue + hueShift;
        g_led_buffer[pos] = CHSV(color + i,SATURATION,255);
      }
    }

  }//end_every_n

  DrawAllPixels(g_led_buffer);
}