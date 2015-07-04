#include "Timer.h"
#include "LedHelix.h"

Timer timer;
LedHelix ledHelix;
rgb_color * rgbColors = ledHelix.rgbColors;

static const unsigned char LED_ACTIVITY = 13;
static const unsigned char LED_COUNT = LedHelix::LED_COUNT;
static const unsigned char LED_SEGMENT_COUNT = LED_COUNT/4;

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

void refreshPololuLedStrip() {
  ledHelix.updateLeds();
}

void rotatePololuLedStrip() {
  rgb_color temp = rgbColors[LED_COUNT-1];
  for(int i=LED_COUNT-2; i>=0; i--) {
    rgbColors[i+1] = rgbColors[i];
  }
  rgbColors[0] = temp;
}

void setup() {
  // put your setup code here, to run once:
  
  for(int i=0; i<LED_COUNT; i+=LED_SEGMENT_COUNT) {
    rgbColors[i] = (rgb_color){ 32, 0, 0 };
  }
  
  pinMode(LED_ACTIVITY, OUTPUT);
  
  timer.setInterval(toggleLedActivity, 1000);
  timer.setInterval(refreshPololuLedStrip, 50); // 20fps.
  timer.setInterval(rotatePololuLedStrip, 50);
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.runTimer();
}

