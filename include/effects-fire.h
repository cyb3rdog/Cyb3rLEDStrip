#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


static byte fireHeat[NUM_LEDS];

void Fire(CRGBPalette16 firePalette, int Cooling, int Sparking, int start = 0, int direction = 1, int ratio = 2){

  int offset = start + (NUM_LEDS / ratio * direction);

  // Step 1.  Cool down every cell a little
  for( int i = start; i != offset - 1; i = i + direction ) {
    int cooldown = random(0, ((Cooling * 10) / NUM_LEDS / ratio) + 2);

    if(cooldown>fireHeat[i]) {
      fireHeat[i]=0;
    } else {
      fireHeat[i]=fireHeat[i]-cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k = offset - 1; k != start; k = k - direction ) {
    fireHeat[k] = (fireHeat[k - (1 * direction)] + fireHeat[k - (2 * direction)] + fireHeat[k - (2 * direction)]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    fireHeat[start + y * direction] = fireHeat[start + y * direction] + random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = start; j != offset - 1; j = j + direction ) {

    byte colorindex = scale8( fireHeat[j], 240 );
    FastLED.leds()[j] = ColorFromPalette( firePalette, colorindex );
    //setPixelHeatColor(j, heat[j] );
  }
}


int currentFireNumber = 0;

void ChangingFire(int changeSpeed, int Cooling, int Sparking, int start = 0, int direction = 1, int ratio = 2){

  EVERY_N_SECONDS(changeSpeed){
    currentFireNumber = (currentFireNumber + 1) % 11;
    switch(currentFireNumber) {
      case  0: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White); break;
      case  1: g_targetPalette=CloudColors_p; break;
      case  2: g_targetPalette=OceanColors_p; break;
      case  3: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::DarkGreen, CRGB::Green, CRGB::White); break;
      case  4: g_targetPalette=ForestColors_p; break;
      case  5: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::Orange, CRGB::Yellow, CRGB::White); break;
      case  6: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::DarkGray, CRGB::Gray, CRGB::Silver); break;
      case  7: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::DarkRed, CRGB::Orange, CRGB::White); break;
      case  8: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White); break;
      case  9: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::Indigo, CRGB::DarkViolet,CRGB::Violet); break;
      case 10: g_targetPalette=CRGBPalette16( CRGB::Black, CRGB::Indigo, CRGB::Crimson, CRGB::Pink); break;
    }
  }

  EVERY_N_MILLISECONDS(EFFECT_SPEED) {
    uint8_t maxChanges = 32;
    nblendPaletteTowardPalette(g_currentPalette, g_targetPalette, maxChanges);
  }

  Fire(g_currentPalette, Cooling, Sparking, start, direction, ratio);

}

void Fire(int Cooling, int Sparking, int start = 0, int direction = 1, int ratio = 2){

  g_currentPalette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  g_targetPalette = g_currentPalette;
  Fire(g_currentPalette, Cooling, Sparking, start, direction, ratio);

}