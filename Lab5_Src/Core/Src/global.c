/*
 * global.c
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */
#include "global.h"

char starting[50];

void global_Init(void){
	HAL_UART_Receive_IT(&huart1, &tempValue, 1);
  	HAL_TIM_Base_Start_IT(&htim2);
  	HAL_ADC_Start(&hadc1);
	// Write welcome string
	HAL_UART_Transmit(&huart1, (void *)starting, sprintf(starting, "STARTING STM32_LAB5\r\n"), 50);
	// Turn off all led
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	// Set timer delay for blinking led Yellow
	setTimerLed(500);
}
