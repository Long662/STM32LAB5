/*
 * timer.c
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */


#include "timer.h"

void setTimer(int duration)
{
	TimerCounter = duration/TIMER_CYCLE;
	TimerFlag = 0;
}

void timer_run(){
	if(TimerCounter >= 0)
	{
		TimerCounter--;
		if (TimerCounter == 0)
			TimerFlag = 1;
	}
}
