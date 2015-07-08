#include "Timer.h"
#include "LedHelix.h"

Timer timer;
LedHelix ledHelix;
//int angle = 0;
int angle = 90;
char c = '!';

static const unsigned char LED_ACTIVITY = 13;

void toggleLedActivity() {
  digitalWrite(LED_ACTIVITY, !digitalRead(LED_ACTIVITY));
}

void updatePololuLedStrip() {
  /*ledHelix.clearColors();
  ledHelix.pointCompassToDirection(angle);
  ledHelix.updateLeds();
  angle += 9;
  angle %= 360;*/
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
}

void setup() {
  // put your setup code here, to run once:

  //Serial.begin(115200);
  
  pinMode(LED_ACTIVITY, OUTPUT);
  
  timer.setInterval(toggleLedActivity, 1000);
  timer.setInterval(updatePololuLedStrip, 40); // 25fps.
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.runTimer();
}

