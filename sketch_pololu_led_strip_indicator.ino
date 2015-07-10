#include "Timer.h"
#include "LedHelix.h"

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_LSM9DS0.h"

#include <stdlib.h>
#include <math.h>

Timer timer;
LedHelix ledHelix;
Adafruit_LSM9DS0 inertialModule = Adafruit_LSM9DS0();

/*int angle = 90;
char c = '!';*/

static const unsigned char LED_ACTIVITY = 13;

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

unsigned int handleInertialModuleCompass() {
  inertialModule.read();
  double heading = 180.0*atan2(inertialModule.magData.x, inertialModule.magData.y)/3.14;
  heading = (heading < 0) ? heading + 360.0 : heading;
  return (unsigned int) heading;
}

void updatePololuLedStrip() {
  /*
  ledHelix.clearColors();
  ledHelix.pointCompassToDirection(angle);
  ledHelix.updateLeds();
  angle += 9;
  angle %= 360;
  */
  /*
  ledHelix.clearColors();
  //ledHelix.drawCharacterAtDirectionWithColor(c, angle, (rgb_color){255,0,0});
  ledHelix.drawCharacterAtDirectionWithColor('W', angle, (rgb_color){255,0,0});
  ledHelix.updateLeds();
  angle += 9;
  if(angle >= 360) {
    angle = 30;
    c++;
    if(c > '~') {
      c = '!';
    }
  }
  angle %= 360;
  */
  ledHelix.clearColors();
  ledHelix.pointCompassToDirection(handleInertialModuleCompass());
  ledHelix.updateLeds();
}

void setupInertialModule() {
  if(inertialModule.begin()) {
    inertialModule.setupMag(inertialModule.LSM9DS0_MAGGAIN_12GAUSS);
    inertialModule.setupAccel(inertialModule.LSM9DS0_ACCELRANGE_16G);
  } else {
    abort();
  }
}

void setup() {
  // put your setup code here, to run once:

  //Serial.begin(115200);
  
  pinMode(LED_ACTIVITY, OUTPUT);
  
  timer.setInterval(toggleLedActivity, 1000);
  timer.setInterval(updatePololuLedStrip, 250); // 25fps.

  setupInertialModule();
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.runTimer();
}

