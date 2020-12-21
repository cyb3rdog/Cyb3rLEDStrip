#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


#define BUTTON_PIN    6
#define DATA_PIN      2
#define CLOCK_PIN     3
#define COLOR_ORDER   RGB
#define CHIPSET       WS2801

#define BRIGHTNESS    96
#define SATURATION    255

#define EFFECT_SPEED  50
#define COLOR_SPEED   20

#define NUM_LEDS      95

#define EEPROM_ADDR   0x00


#include "EffectsLibrary.h"


static CRGB             leds[NUM_LEDS];
static EffectsLibrary   effectsLibrary;


void setup() {

  //Serial.begin(115200);                                        // Initialize serial port for debugging.
  delay(2000); // sanity delay

  pinMode(BUTTON_PIN, INPUT);                                    // Set button input pin
  digitalWrite(BUTTON_PIN, HIGH );

  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds,NUM_LEDS);//.setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);                // FastLED Power management set at 5V, 500mA.
  set_max_power_indicator_LED(LED_BUILTIN);

  effectsLibrary = EffectsLibrary(BUTTON_PIN, EEPROM_ADDR);

}

void loop()
{

  effectsLibrary.ChangeEffectByTime(5);
  effectsLibrary.MainLoop();

  FastLED.show(BRIGHTNESS);                                 //  Show and delay

  //delay(SPEED);
  //Serial.println(LEDS.getFPS());                          // Display frames per second on the serial monitor.
}

void loop_bak() {

  while (true)
  {
    //beatwave();
    //blendwave();
    //stripeMix();
    //twinkleChange();
    //blur();
    //rainbowWithGlitter();

    //DrawMarqueeScroll();
    //DrawTwinkle();
    //DrawComet();
    //DrawKITT();
    //DrawRainbowComet(6, NUM_LEDS, 32);
    //DrawMarqueeScroll();

    //Fire(55,120,15);

    //Rainbow();
    //RainbowPulse();
    //RainbowLoop();
    //PrideRider();
    //RainbowCycle();
    //TheaterChase(0xff,0,0,50);
    //RainbowTheaterChase(50);

    //MeteorRain(0xaa,0x44,0xff,10, 64, true);
    //Sparkle(0x66, 0x11, 0x66);
    //SnowSparkle(0x80, 0x80, 0x80, 5, random(100,1000));

    //FastLED.clear(false);
    //balls.Draw();

    /*
    // RGB Spinners
    float b = beat16(60) / 65535.0f * FAN_SIZE;
    DrawFanPixels(b, 1, CRGB::Red, Sequential, 0);
    DrawFanPixels(b, 1, CRGB::Green, Sequential, 1);
    DrawFanPixels(b, 1, CRGB::Blue, Sequential, 2);
    */

    //FanWipe();
    //StripWipe();
    //ColorCycle();
    //SequentialRainbow();
    //VerticalRainbow();
    //HorizontalRainbow();
    //PaletteRainbow();

    /*
    // Sequential Fire Fans
    static FireEffect fire(NUM_LEDS, 20, 100, 3, NUM_LEDS, true, false);
    fire.DrawFire();
    */

    /*
    // Bottom Up Fire Effect with extra sparking on first fan only
    static FireEffect fire(NUM_LEDS, 20, 140, 3, NUM_LEDS, true, false);
    fire.DrawFire(BottomUp);
    */


    /*
    EVERY_N_MILLISECONDS(250)
    {
      uint32_t milliwatts = calculate_unscaled_power_mW(g_LEDs, NUM_LEDS);

      g_OLED.clearBuffer();
      g_OLED.setCursor(0, g_lineHeight);
      g_OLED.printf("FPS  : %u", FastLED.getFPS());
      g_OLED.setCursor(0, g_lineHeight * 2);
      g_OLED.printf("Power: %u mW", calculate_unscaled_power_mW(g_LEDs, 4));
      g_OLED.setCursor(0, g_lineHeight * 3);
      g_OLED.printf("Brite: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
      g_OLED.sendBuffer();
    }
    */

    FastLED.show(BRIGHTNESS);                          //  Show and delay
    //delay(SPEED);
    //Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.
  }
}