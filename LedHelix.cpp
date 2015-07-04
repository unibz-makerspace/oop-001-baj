/*
 * LedHelix.cpp
 * 
 * Abstraction library for a Pololu LED strip helix in 3D space.
 * Author: Julian Sanin <sanin89julian@gmail.com>
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Julian Sanin
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "LedHelix.h"

#include <avr/pgmspace.h>

// segment 1 = 0..36 (37 leds)
// segment 2 = 37..72 (36 leds)
// segment 3 = 73..110 (38 leds)
// segment 4 = 111..143 (33 leds)
// total 144 leds

// TODO: Calculate mapping for helix.
static const int LED_MAP[LedHelix::LED_COUNT] PROGMEM = {
  0,101,201,302,403,503,604,705,806,906,1007,1108,1208,1309,1410,1510,1611,1712,1813,1913,
  2014,2115,2215,2316,2417,2517,2618,2719,2820,2920,3021,3122,3222,3323,3424,3524,3625,
  3726,3827,3927,4028,4129,4229,4330,4431,4531,4632,4733,4834,4934,5035,5136,5236,5337,
  5438,5538,5639,5740,5841,5941,6042,6143,6243,6344,6445,6545,6646,6747,6848,6948,7049,
  7150,7250,7351,7452,7552,7653,7754,7855,7955,8056,8157,8257,8358,8459,8559,8660,8761,
  8862,8962,9063,9164,9264,9365,9466,9566,9667,9768,9869,9969,10070,10171,10271,10372,
  10473,10573,10674,10775,10876,10976,11077,11178,11278,11379,11480,11580,11681,11782,
  11882,11983,12084,12185,12285,12386,12487,12587,12688,12789,12889,12990,13091,13192,
  13292,13393,13494,13594,13695,13796,13896,13997,14098,14199,14299,14400
};

LedHelix::LedHelix() {
  for(int i=0; i<LED_COUNT; i++) {
    rgbColors[i] = (rgb_color){ 0, 0, 0 };
  }
}

LedHelix::~LedHelix() { }

void LedHelix::pointToDirection(int angleInDegrees) {
  const rgb_color red = (rgb_color){ 32, 0, 0 };
  int angle = angleInDegrees*10;
  for(int i=0,segment=1; i<LED_COUNT; i++) {
    if(LED_MAP[i] >= angle*segment) {
      rgbColors[i] = red;
      segment++;
    }
  }
}

void LedHelix::updateLeds() {
  ledStrip.write(rgbColors, LED_COUNT);
}
