#include "Timer.h"
#include "LedHelix.h"

Timer timer;
LedHelix ledHelix;
int angle = 0;

static const unsigned char LED_ACTIVITY = 13;

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

void refreshPololuLedStrip() {
  ledHelix.updateLeds();
}

void rotatePololuLedStrip() {
  ledHelix.clearColors();
  ledHelix.pointToDirectionWithColor(angle, (rgb_color) {255,0,0});
  angle += 5;
  angle %= 360;
}

void setup() {
  // put your setup code here, to run once:
  
  pinMode(LED_ACTIVITY, OUTPUT);
  
  timer.setInterval(toggleLedActivity, 1000);
  timer.setInterval(refreshPololuLedStrip, 50); // 20fps.
  timer.setInterval(rotatePololuLedStrip, 50);
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.runTimer();
}

