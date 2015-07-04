/*
 * Timer.cpp
 * 
 * JavaScript style timer library.
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

#include "Timer.h"

#include <stdlib.h>

Timer::Timer() {
  for(int i=0; i<MAX_CALLBACKS; i++) {
    timerCallbacks[i] = NULL;
  }
}

Timer::~Timer() { }

void Timer::setInterval(timerCallback timerCallback, unsigned long milliSeconds) {
  bool success = false;
  unsigned long currentMilliSeconds = millis();
  for(int i=0; i<MAX_CALLBACKS; i++) {
    if(timerCallbacks[i] == NULL) {
      timerCallbacks[i] = timerCallback;
      previousMilliSeconds[i] = currentMilliSeconds;
      milliSecondDelays[i] = milliSeconds;
      success = true;
      break;
    }
  }
  if(success == false) {
    abort();
  }
}

void Timer::runTimer() {
  unsigned long currentMilliSeconds = millis();
  for(int i=0; i<MAX_CALLBACKS; i++) {
    if(timerCallbacks[i] != NULL) {
      unsigned long milliSeconds = currentMilliSeconds - previousMilliSeconds[i];
      if(milliSeconds >= milliSecondDelays[i]) {
        previousMilliSeconds[i] = currentMilliSeconds;
        timerCallbacks[i]();
      }
    }
  }
}
