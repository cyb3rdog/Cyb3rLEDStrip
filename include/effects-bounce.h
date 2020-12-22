#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


/*
void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
 
  for (int i = 0 ; i < BallCount ; i++) {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
  }

  while (true) {
    FastLED.clear(false);

    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
 
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
 
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) {
      FastLED.leds()[Position[i]] = CRGB(colors[i][0],colors[i][1],colors[i][2]);
    }
   
    FastLED.show(BRIGHTNESS);    
  }
}
*/

const float Gravity = -9.81;

struct BouncingBall {
  int   Position = 0;
  float Height = 1;
  float Dampening = 0.90 - float(random(10)/20.0);
  float ImpactVelocity = sqrt( -2 * Gravity * Height );
  float TimeSinceLastBounce = 0;
  long  ClockTimeSinceLastBounce = millis();
  byte  HueColor = 255;
};

void BouncingColoredBalls(struct BouncingBall * balls, int ballCount) {

  int StartHeight = 1;
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );  
  
  FastLED.clear(false);  
  for (int i = 0 ; i < ballCount; i++) {    
    balls[i].TimeSinceLastBounce =  millis() - balls[i].ClockTimeSinceLastBounce;
    balls[i].Height = 0.5 * Gravity * pow( balls[i].TimeSinceLastBounce/1000 , 2.0 ) + balls[i].ImpactVelocity * balls[i].TimeSinceLastBounce/1000;

    if ( balls[i].Height < 0 ) {                      
      balls[i].Height = 0;
      balls[i].ImpactVelocity = balls[i].Dampening * balls[i].ImpactVelocity;
      balls[i].ClockTimeSinceLastBounce = millis();

      if ( balls[i].ImpactVelocity < 0.01 ) {
        balls[i].ImpactVelocity = ImpactVelocityStart;
      }
    }
    balls[i].Position = round( balls[i].Height * (NUM_LEDS - 1) / StartHeight);
  }

  for (int i = 0 ; i < ballCount; i++) {    
    FastLED.leds()[balls[i].Position].setHue(balls[i].HueColor);
  }
}
