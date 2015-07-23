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
#include "Font.h"

#include <avr/pgmspace.h>
#include <stdint.h>

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
static const uint16_t LED_MAP[LedHelix::LED_COUNT] PROGMEM = {
  0u,10u,19u,29u,39u,48u,58u,68u,77u,87u,97u,107u,116u,126u,136u,145u,155u,165u,174u,184u,
  194u,203u,213u,223u,232u,242u,252u,262u,271u,281u,291u,300u,310u,320u,329u,339u,349u,
  358u,368u,378u,387u,397u,407u,417u,426u,436u,446u,455u,465u,475u,484u,494u,504u,513u,
  523u,533u,542u,552u,562u,572u,581u,591u,601u,610u,620u,630u,639u,649u,659u,668u,678u,
  688u,697u,707u,717u,727u,736u,746u,756u,765u,775u,785u,794u,804u,814u,823u,833u,843u,
  852u,862u,872u,882u,891u,901u,911u,920u,930u,940u,949u,959u,969u,978u,988u,998u,1007u,
  1017u,1027u,1037u,1046u,1056u,1066u,1075u,1085u,1095u,1104u,1114u,1124u,1133u,1143u,
  1153u,1162u,1172u,1182u,1192u,1201u,1211u,1221u,1230u,1240u,1250u,1259u,1269u,1279u,
  1288u,1298u,1308u,1317u,1327u,1337u,1347u,1356u,1366u,1376u,1385u
};

/* Common used colors. */
static const rgb_color COLOR_RED = (rgb_color){255,0,0};
static const rgb_color COLOR_WHITE = (rgb_color){255,255,255};

LedHelix::LedHelix()
    : ANGLE_RESOLUTION((unsigned char)((LED_MAP[LED_COUNT-1])/LED_COUNT)) {
  clearColors();
}

LedHelix::~LedHelix() { }

void LedHelix::clearColors() {
  for(int i=0; i<LED_COUNT; i++) {
    rgbColors[i] = (rgb_color){ 0, 0, 0 };
  }
}

void LedHelix::pointToDirectionWithColor(unsigned int angleInDegrees, rgb_color rgbColor) {
  for(uint_fast8_t i=0,segment=0; i<LED_COUNT; i++) {
    if(pgm_read_word(&LED_MAP[i]) >= (angleInDegrees + 360*segment)) {
      rgbColors[i] = rgbColor;
      segment++;
    }
  }
}

void LedHelix::pointCompassToDirection(unsigned int angleInDegrees) {
  pointToDirectionWithColor(angleInDegrees, COLOR_RED);
  pointToDirectionWithColor(((angleInDegrees + 180) % 360), COLOR_WHITE);
}

void LedHelix::drawOnAngleWithColor(unsigned int angleInDegrees, rgb_color rgbColor) {
  for(uint_fast8_t i=0; i<LED_COUNT; i++) {
    if(pgm_read_word(&LED_MAP[i]) >= angleInDegrees) {
      rgbColors[i] = rgbColor;
      return;
    }
  }
}

void LedHelix::drawCharacterAtDirectionWithColor(
    char character, unsigned int angleInDegrees, rgb_color rgbColor) {
  const Font font = Font_BAJ;
  const char characterIndex = character - font.characterStartOffset;
  if((characterIndex < 0) || (characterIndex >= font.characterCount)) {
    return;
  }
  const uint8_t characterHeight = font.characterHeight;
  const uint8_t characterWidth = pgm_read_byte(&font.characterWidths[characterIndex]);
  for(uint_fast8_t i=0; i<characterHeight; i++) {
    const uint16_t mappingIndex = 4*characterIndex + i;
    uint16_t characterMapping = pgm_read_word(&font.characterMappings[mappingIndex]);
    for(uint_fast8_t j=0; j<characterWidth; j++) {
      unsigned int angle = angleInDegrees - j*ANGLE_RESOLUTION + 360*i;
      if((characterMapping & 0x8000) != 0) {
        drawOnAngleWithColor(angle, rgbColor);
      }
      characterMapping <<= 1;
    }
  }
}

void LedHelix::updateLeds() {
  ledStrip.write(rgbColors, LED_COUNT);
}
