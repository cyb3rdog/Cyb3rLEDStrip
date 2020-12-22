#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


#define BUTTON_PIN    6
#define DATA_PIN      2
#define CLOCK_PIN     3
#define COLOR_ORDER   RGB
#define CHIPSET       WS2801

#define NUM_LEDS      93
#define BRIGHTNESS    96
#define SATURATION    255

#define EFFECT_SPEED  50
#define COLOR_SPEED   20
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
  effectsLibrary.AutoChangeInterval = 5;
  effectsLibrary.AutoChangeEnabled = true;

} // setup()

void loop()
{

  effectsLibrary.MainLoop();

  FastLED.show(BRIGHTNESS);                                 //  Show

  //Serial.println(LEDS.getFPS());                          // Display frames per second on the serial monitor.

} // loop()