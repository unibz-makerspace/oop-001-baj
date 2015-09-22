#include "Timer.h"

#include <PololuLedStrip.h>
#include "LedHelix.h"

#include "sketch_baj.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

#include <stdlib.h>
#include <math.h>

// Constants.
static const unsigned char LED_ACTIVITY = 13;
static const bool INVERT_MAGNETIC_X = false;
static const bool INVERT_MAGNETIC_Y = false; 
static const Mode MODE = MODE_ASCII;

Timer timer;

// RGB LED Strip.
LedHelix ledHelix;

// Inertial Module.
Adafruit_LSM9DS0 inertialModule = Adafruit_LSM9DS0();

int angle = 0; //90;
char c = 'A';

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

/*****************************************************************************************
 * Callbacks.
 *****************************************************************************************/

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

void updatePololuLedStrip() {
  switch(MODE) {
    case MODE_ROTATE_TEST:
      ledHelix.clearColors();
      ledHelix.pointCompassToDirection(angle);
      ledHelix.updateLeds();
      angle += ledHelix.ANGLE_RESOLUTION;
      angle %= 360;
      break;
    case MODE_COMPASS:
      ledHelix.clearColors();
      ledHelix.pointCompassToDirection(handleInertialModuleCompass());
      ledHelix.updateLeds();
      break;
    case MODE_ASCII:
      ledHelix.clearColors();
      ledHelix.drawCharacterAtDirectionWithColor(c, angle, (rgb_color){255,0,0});
      //ledHelix.drawCharacterAtDirectionWithColor('W', angle, (rgb_color){255,0,0});
      ledHelix.updateLeds();
      angle += ledHelix.ANGLE_RESOLUTION;
      if(angle >= 360) {
        angle = 0;
        c++;
        if(c > 'S') {
          c = 'A';
        }
      }
      break;
    default:
      abort();
  }
}

/*****************************************************************************************
 * Inertial module.
 *****************************************************************************************/

void setupInertialModule() {
  if(inertialModule.begin()) {
    inertialModule.setupMag(Adafruit_LSM9DS0::LSM9DS0_MAGGAIN_12GAUSS);
    inertialModule.setupAccel(Adafruit_LSM9DS0::LSM9DS0_ACCELRANGE_16G);
  } else {
    abort();
  }
}

unsigned int handleInertialModuleCompass() {
  inertialModule.read();
  Triple magneticTriple = (Triple) {
    (INVERT_MAGNETIC_X) ? -(int)inertialModule.magData.x : (int)inertialModule.magData.x,
    (INVERT_MAGNETIC_Y) ? -(int)inertialModule.magData.y : (int)inertialModule.magData.y,
    (int)inertialModule.magData.z
  };
  Triple accelerationTriple = (Triple) {
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
    (float)(accelerationTriple.x / sqrt(pow(accelerationTriple.x, 2)
                                        + pow(accelerationTriple.y, 2)
                                        + pow(accelerationTriple.z, 2))),
    (float)(accelerationTriple.y / sqrt(pow(accelerationTriple.x, 2)
                                        + pow(accelerationTriple.y, 2)
                                        + pow(accelerationTriple.z, 2)))
  };
  float pitch = asin(accelerationNormalized.x);
  float roll = -asin(accelerationNormalized.y / cos(pitch));
  Tuple magneticTupleCompensated = (Tuple) {
    (float)(magneticTriple.x * cos(pitch) + magneticTriple.z * sin(pitch)),
    (float)(magneticTriple.x * sin(roll) * sin(pitch) + magneticTriple.y * cos(roll)
            - magneticTriple.z * sin(roll) * cos(pitch))
  };
  float heading = 180 * atan2(magneticTupleCompensated.x, magneticTupleCompensated.y) 
                  / M_PI;
  heading = (heading < 0) ? heading + 360 : heading;
  return (unsigned int) heading;
}

