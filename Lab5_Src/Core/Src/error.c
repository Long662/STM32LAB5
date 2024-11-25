/*
 * error.c
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */

#include "error.h"

uint8_t error_1[] = "\r\nERROR: CMD_NOT_EXISTED\r\n";
uint8_t error_2[] = "\r\nERROR: BUFFER_IS_FULL\r\n";
uint8_t error_3[] = "\r\nERROR: INVALID_INPUT\r\n";

void errorHandle(void)
{
	switch(ERRState)
	{
	case ERROR_IDLE:
		break;
	case CMD_NOT_EXISTED:
		HAL_UART_Transmit(&huart2, error_1, sizeof(error_1), 100);
		break;
	case BUFFER_IS_FULL:
		HAL_UART_Transmit(&huart2, error_2, sizeof(error_2), 100);
		break;
	case INVALID_INPUT:
		HAL_UART_Transmit(&huart2, error_3, sizeof(error_3), 100);
		break;
	default:
		break;
	}
}
