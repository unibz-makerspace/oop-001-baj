/*
 * Font.h
 * 
 * Font library for Arduino.
 * Author: Julian Sanin <sanin89julian@gmail.com>
 * Contributors: Angelo Ventura <angelo.ventura@unibz.it>
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Julian Sanin, Angelo Ventura
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

#ifndef FONT_H
#define FONT_H

#include <Arduino.h>
#include <stdint.h>

struct Font {
  const uint8_t characterCount;
  const uint8_t characterStartOffset;
  const uint8_t characterHeight;
  const uint8_t * const characterWidths; // PROGMEM
  const uint16_t * const characterMappings;  // PROGMEM
};

extern Font Font_BAJ;

#endif // FONT_H
