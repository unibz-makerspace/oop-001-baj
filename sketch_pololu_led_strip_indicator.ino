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
bool invertMagneticX = false;
bool invertMagneticY = false; 

/*int angle = 90;
char c = '!';*/

static const unsigned char LED_ACTIVITY = 13;

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

typedef struct {
  int x;
  int y;
  int z;
} InertialModule;

typedef struct {
  float x;
  float y;
} Tuple;

unsigned int handleInertialModuleCompass() {
  inertialModule.read();
  InertialModule magnetic = (InertialModule) {
    (invertMagneticX) ? -(int)inertialModule.magData.x : (int)inertialModule.magData.x,
    (invertMagneticX) ? -(int)inertialModule.magData.y : (int)inertialModule.magData.y,
    (int)inertialModule.magData.z
  };
  InertialModule acceleration = (InertialModule) {
    (int)inertialModule.accelData.x,
    (int)inertialModule.accelData.y,
    (int)inertialModule.accelData.z
  };
  /*
  float heading = 180 * atan2(magnetic.x, magnetic.y) / M_PI;
  heading = (heading < 0) ? heading + 360 : heading;
  return (unsigned int) heading;
  */
  Tuple accelerationNormalized = (Tuple) {
    (float)(acceleration.x / sqrt(
        pow(acceleration.x, 2) +
        pow(acceleration.y, 2) +
        pow(acceleration.z, 2))),
    (float)(acceleration.y / sqrt(
        pow(acceleration.x, 2) +
        pow(acceleration.y, 2) +
        pow(acceleration.z, 2)))
  };
  float pitch = asin(accelerationNormalized.x);
  float roll = -asin(accelerationNormalized.y / cos(pitch));
  Tuple magneticCompensated = (Tuple) {
    (float)(magnetic.x * cos(pitch) + magnetic.z * sin(pitch)),
    (float)(magnetic.x * sin(roll) * sin(pitch) + magnetic.y * cos(roll) - magnetic.z * sin(roll) * cos(pitch))
  };
  float heading = 180 * atan2(magneticCompensated.x, magneticCompensated.y) / M_PI;
  heading = (heading < 0) ? heading + 360 : heading;
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
    inertialModule.setupMag(Adafruit_LSM9DS0::LSM9DS0_MAGGAIN_12GAUSS);
    inertialModule.setupAccel(Adafruit_LSM9DS0::LSM9DS0_ACCELRANGE_16G);
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

