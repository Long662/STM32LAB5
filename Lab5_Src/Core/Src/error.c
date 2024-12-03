/*
 * error.c
 *
 *  Created on: Nov 25, 2024
 *      Author: NGUYEN THANH LONG_2011554
 */

#include "error.h"

uint8_t error_1[] = "\r\nERROR: CMD_NOT_EXISTED\r\n";
uint8_t error_2[] = "\r\nERROR: BUFFER_IS_FULL\r\n";
uint8_t error_3[] = "\r\nERROR: INVALID_INPUT\r\n";
uint8_t error_4[] = "\r\nERROR: CMD_NOT_REQUEST_YET\r\n";
uint8_t error_5[] = "\r\nERROR: ADC_CONVERSION_ERR\r\n";

void errorHandle(void)
{
	switch(ERRState)
	{
	case ERROR_IDLE:
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		break;
	case CMD_NOT_EXISTED:
		HAL_UART_Transmit(&huart1, error_1, sizeof(error_1), 100);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	case BUFFER_IS_FULL:
		HAL_UART_Transmit(&huart1, error_2, sizeof(error_2), 100);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	case INVALID_INPUT:
		HAL_UART_Transmit(&huart1, error_3, sizeof(error_3), 100);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	case NOT_REQUEST_YET:
		HAL_UART_Transmit(&huart1, error_4, sizeof(error_4), 100);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	case ADC_ERR:
		HAL_UART_Transmit(&huart1, error_5, sizeof(error_5), 100);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}
