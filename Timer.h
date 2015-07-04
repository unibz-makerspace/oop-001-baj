/*
 * Timer.h
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

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

typedef void(*timerCallback)();

class Timer {
  
public:
  /*
   * Sets the maximum number of possible callback functions that can be handled by the
   * Timer class. Modify this value to your needs.
   */
  const static int MAX_CALLBACKS = 4;

  Timer();
  ~Timer();
  
  /*
   * The setInterval() method calls back a function at specified intervals (in
   * milliseconds). The setInterval() method will continously call the function.
   * 
   * Example:
   * 
   * void myFunction() {
   *   // Do something ...
   * }
   * 
   * Timer timer;
   * 
   * void setup() {
   *   timer.setInterval(myFunction, 1000) // Execute every 1000 milli seconds.
   * }
   */
  void setInterval(timerCallback timerCallback, unsigned long milliSeconds);
  
  /*
   * The runTimer() method is required to place inside the loop() function of the main
   * sketch such that the Timer class can work properly.
   * 
   * Example:
   * 
   * Timer timer;
   * 
   * void setup() {
   *   // Setup some timer.setInterval() ...
   * }
   * 
   * void loop() {
   *   timer.runTimer(); // Run the timer.
   * }
   */
  void runTimer();
  
private:
  timerCallback timerCallbacks[MAX_CALLBACKS];
  unsigned long previousMilliSeconds[MAX_CALLBACKS];
  unsigned long milliSecondDelays[MAX_CALLBACKS];
};

#endif // TIMER_H
