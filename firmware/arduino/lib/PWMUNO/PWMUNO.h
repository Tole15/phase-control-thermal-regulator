/*
 *  Asynchronouos PWM utilities for 16 bit Timer1 on ATmega168/328
 *  Original code by Ramon Felix August 2008
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#ifndef PWMUNO_h_
#define PWMUNO_h_

#include <Arduino.h>

#define TIMER1_RESOLUTION 65536UL  // Timer1 is 16 bit

class PWM1class
{
  public:
    void dutyCycleA(unsigned cmp);
    void dutyCycleB(unsigned cmp);
    void restart();
    void start();
    void disable();
    void initialize();
    void setPeriod(unsigned long microseconds);
    unsigned getPeriod(void);
};

extern PWM1class PWM1;

#endif
