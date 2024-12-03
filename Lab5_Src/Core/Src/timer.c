/*
 * timer.c
 *
 *  Created on: Nov 25, 2024
 *      Author: NGUYEN THANH LONG_2011554
 */


#include "timer.h"


uint16_t TimerCounter = 0;
uint8_t TimerFlag = 0;
uint16_t TimerCounter1 = 0;
uint8_t TimerFlag1 = 0;

void setTimer(int duration)
{
	TimerCounter = duration/TIMER_CYCLE;
	TimerFlag = 0;
}

void setTimerLed(int duration)
{
	TimerCounter1 = duration/TIMER_CYCLE;
	TimerFlag1 = 0;
}

void timer_run(){
	if(TimerCounter >= 0)
	{
		TimerCounter--;
		if (TimerCounter == 0)
			TimerFlag = 1;
	}
	if(TimerCounter1 >= 0)
	{
		TimerCounter1--;
		if (TimerCounter1 == 0)
			TimerFlag1 = 1;
	}
}
