#include "Timer.h"
#include "LedHelix.h"

Timer timer;
LedHelix ledHelix;
int angle = 0;

static const unsigned char LED_ACTIVITY = 13;

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

void updatePololuLedStrip() {
  ledHelix.clearColors();
  ledHelix.pointCompassToDirection(angle);
  ledHelix.updateLeds();
  angle += 9;
  angle %= 360;
}

void setup() {
  // put your setup code here, to run once:
  
  pinMode(LED_ACTIVITY, OUTPUT);
  
  timer.setInterval(toggleLedActivity, 1000);
  timer.setInterval(updatePololuLedStrip, 40); // 25fps.
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.runTimer();
}

