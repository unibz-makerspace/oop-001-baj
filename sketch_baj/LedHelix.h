/*
 * LedHelix.h
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

#ifndef LEDHELIX_H
#define LEDHELIX_H

#include <Arduino.h>

#include <PololuLedStrip.h>

class LedHelix {

public:
  const static unsigned char PIN_NUMBER = 12;
  const static unsigned char LED_COUNT = 144;

  rgb_color rgbColors[LED_COUNT];

  const unsigned char ANGLE_RESOLUTION;
  
  LedHelix();
  ~LedHelix();

  /*
   * Clears all lights of the helix LED strip.
   */
  void clearColors();

  /*
   * Displays a vertical bar of given color on the helix LED strip.
   */
  void pointToDirectionWithColor(unsigned int angleInDegrees, rgb_color rgbColor);

  /*
   * Displays a compass like bar on the helix LED strip.
   * A red bar shows to the angle as in the parameter angleInDegrees.
   * A white bar shows to the 180° degree out of phase direction of the prameter
   * angleInDegrees.
   */
  void pointCompassToDirection(unsigned int angleInDegrees);

  void drawOnAngleWithColor(unsigned int angleInDegrees, rgb_color rgbColor);
  void drawCharacterAtDirectionWithColor(
      char character, unsigned int angleInDegrees, rgb_color rgbColor);

  /*
   * Writes the internal buffer to the helix LED strip.
   */
  void updateLeds();

private:
  PololuLedStrip<PIN_NUMBER> ledStrip;
};

#endif // LEDHELIX_H
