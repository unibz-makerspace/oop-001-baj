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

/*
 * The LED strip helix has been wound on a cylinder with diameter of 79.58mm with nearly
 * four complete turns in clockwise downwards direction. The last not 360° complete turn 
 * had a circular arc length of 212mm measured relative from the first LED. The LED strip
 * consists of 144 individually addressable light sources with RGB colors. It is assumed
 * that the first addressable LED on the top of the cylinder marks the relative 0° of the 
 * entire LED strip.
 * 
 * To calculate the LED mapping following formulas were used:
 * 
 * Total circular arc degrees:
 * alphaComplete = 3*360°
 *               = 1080°
 * alphaRemainder = (lengthCircularArc * 360°) / (diameter * Pi)
 *                = (212mm * 360°) / (79.58mm * Pi)
 *                = 305.27°
 * alphaCircularArc = alphaComplete + alphaRemainder
 *                  = 1080° + 305.27°
 *                  = 1385.27°
 * The circular arc degrees were linearly distributed for the 144 LEDs which resulted in
 * following to an integer value rounded LED_MAP array. The array is stored in the program
 * memory (PROGMEM) instead of the RAM to save memory space.
 */
static const int LED_MAP[LedHelix::LED_COUNT] PROGMEM = {
  0,10,19,29,39,48,58,68,77,87,97,107,116,126,136,145,155,165,174,184,194,203,213,223,232,
  242,252,262,271,281,291,300,310,320,329,339,349,358,368,378,387,397,407,417,426,436,446,
  455,465,475,484,494,504,513,523,533,542,552,562,572,581,591,601,610,620,630,639,649,659,
  668,678,688,697,707,717,727,736,746,756,765,775,785,794,804,814,823,833,843,852,862,872,
  882,891,901,911,920,930,940,949,959,969,978,988,998,1007,1017,1027,1037,1046,1056,1066,
  1075,1085,1095,1104,1114,1124,1133,1143,1153,1162,1172,1182,1192,1201,1211,1221,1230,
  1240,1250,1259,1269,1279,1288,1298,1308,1317,1327,1337,1347,1356,1366,1376,1385
};

/* Common used colors. */
static const rgb_color COLOR_RED = (rgb_color){255,0,0};
static const rgb_color COLOR_WHITE = (rgb_color){255,255,255};

LedHelix::LedHelix() {
  clearColors();
}

LedHelix::~LedHelix() { }

void LedHelix::clearColors() {
  for(int i=0; i<LED_COUNT; i++) {
    rgbColors[i] = (rgb_color){ 0, 0, 0 };
  }
}

void LedHelix::pointToDirectionWithColor(int angleInDegrees, rgb_color rgbColor) {
  for(int i=0,segment=0; i<LED_COUNT; i++) {
    if(pgm_read_word(&LED_MAP[i]) >= (360*segment + angleInDegrees)) {
      rgbColors[i] = rgbColor;
      segment++;
    }
  }
}

void LedHelix::pointCompassToDirection(int angleInDegrees) {
  pointToDirectionWithColor(angleInDegrees, COLOR_RED);
  pointToDirectionWithColor(((angleInDegrees + 180) % 360), COLOR_WHITE);
}

void LedHelix::updateLeds() {
  ledStrip.write(rgbColors, LED_COUNT);
}
