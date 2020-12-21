#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


uint8_t spacing = 5;      // Sets pixel spacing. [Use 2 or greater]
int8_t delta = 1;         // Sets forward or backwards direction amount. (Can be negative.)
uint8_t width = 3;        // Can increase the number of pixels (width) of the chase. [1 or greater]

boolean fadingTail = 1;   // Add fading tail? [1=true, 0=falue]
uint8_t fadeRate = 220;   // How fast to fade out tail. [0-255]

uint8_t hue = 180;        // Starting color.
uint8_t hue2_shift = 20;  // Hue shift for secondary color.  Use 0 for no shift. [0-255]

int16_t pos;                // Pixel position.
int8_t advance = -1*width;  // Stores the advance amount.
uint8_t color;              // Stores a hue color. 


void Animations()
{
  EVERY_N_SECONDS(10){  // Demo: Change direction every N seconds.
    delta = -1*delta;
  }

  //EVERY_N_SECONDS(10){  // Demo: Changing the pixel spacing every N seconds.
  //  spacing = spacing + 1;
  //  if (spacing == 9) { spacing = 2; }  // Reset spacing to 2
  //  if (spacing >4) { spacing = 8; }  // Jump spacing up to 8
  //}

  EVERY_N_SECONDS(10){  // Demo: Change the hue every N seconds.
    hue = hue + random8(30,61);  // Shift the hue to something new.
  }


  EVERY_N_MILLISECONDS(EFFECT_SPEED * 2){  // Advance pixels to next position.

    // Advance pixel postion down strip, and rollover if needed.
    advance = (advance + delta + NUM_LEDS) % NUM_LEDS;

    // Fade out tail or set back to black for next loop around.
    if (fadingTail == 1) {
      fadeToBlackBy(FastLED.leds(), NUM_LEDS, fadeRate);
    } else {
      for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
        for (uint8_t w = 0; w<width; w++){
          //pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
          pos = (spacing * (i-1) + spacing + advance + w - 1) % NUM_LEDS;
          FastLED.leds()[pos] = CRGB::Black;
        }
      }
    }

    // Update pixels down the strip.
    for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint8_t w = 0; w<width; w++){
        pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        if ( w % 2== 0 ){  // Is w even or odd?
          color = g_hue;
        } else {
          color = g_hue + hue2_shift;
        }
        
        FastLED.leds()[pos] = CHSV(color + i,SATURATION,255);
      }
    }

  }//end_every_n
}

void RainbowTheaterChase()
{
  byte *c;  

    
        for (int i=0; i < NUM_LEDS; i++) {
          c = Wheel(((i * 256 / NUM_LEDS) + g_hue) & 255);
          FastLED.leds()[i] = CRGB(*c, *(c+1), *(c+2));
        }
                
        for (int i=NUM_LEDS-beatsin8(180,0,1); i >= 0; i=i-2) {
          FastLED.leds()[i] = CRGB(0,0,0);  //turn every third pixel off
        }    
    
}

void TheaterChase(byte red, byte green, byte blue, int SpeedDelay)
{
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        FastLED.leds()[i + q] = CRGB(red, green, blue);
      }
      FastLED.show(BRIGHTNESS);   
      delay(SpeedDelay);
     
      for (int i=0; i < NUM_LEDS; i=i+3) {
        FastLED.leds()[i+q] = CRGB(0,0,0);  //turn every third pixel off
      }
    }
  }
}


void Sparkle(byte red, byte green, byte blue, int SparkleDelay = 5)
{
  int Pixel = random(NUM_LEDS);
  FastLED.leds()[Pixel] = CRGB(red, green, blue);  
  FastLED.show(BRIGHTNESS);
  delay(SparkleDelay);
  FastLED.leds()[Pixel] = CRGB::Black;
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay = 5)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    FastLED.leds()[i] = CRGB(red, green, blue);
  }
 
  int Pixel = random(NUM_LEDS);
  FastLED.leds()[Pixel] = CRGB(255, 255, 255);  
  FastLED.show(BRIGHTNESS);
  delay(SparkleDelay);

  FastLED.leds()[Pixel] = CRGB(red, green, blue);  
  FastLED.show(BRIGHTNESS);
}