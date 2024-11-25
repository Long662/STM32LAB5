/*
 * uart_fsm.c
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */
#include "uart_fsm.h"

uint32_t ADC_value = 0;\
char response[RESPONSE_LENGTH];
uint8_t data[] = "HELLO WORD\r\n";

void uart_communication_fsm()
{
	switch(UARTState){
	case UART_IDLE:
		break;
	case UART_RST:
		UARTState = UART_RESPONSE;
		break;
	case UART_RESPONSE:
		ADC_value = HAL_ADC_GetValue(&hadc1);
		HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "!ADC%ld#\r\n", ADC_value), 1000);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		setTimer(3000);
		UARTState = UART_WAIT_OK;
		break;
	case UART_WAIT_OK:
		if(TimerFlag == 1)
		{
			UARTState = UART_END;
		}
		break;
	case UART_END:
		if(is_OK == 1)	is_OK = 0;
		HAL_UART_Transmit(&huart2, (void *)response, sprintf(response, "ERROR\r\n"), 1000);
		UARTState = UART_IDLE;
		break;
	default:
		break;
	}
}

void command_praser_fsm()
{
	switch(CMDState){
	case CMD_IDLE:
		if(cmdBuffer[cmd_content_index] == '!')
		{
			CMDState = CMD_EXCLAMATION;
		}
		else if (cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
		}
		break;
	case CMD_EXCLAMATION:
		if (cmdBuffer[cmd_content_index] == 'R'){
			CMDState = CMD_R;
		}
		else if (cmdBuffer[cmd_content_index] == 'O')
		{
			CMDState = CMD_O;
		}
		else if (cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_R:
		if(cmdBuffer[cmd_content_index] == 'S')
		{
			CMDState = CMD_S;
		}
		else if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_S:
		if(cmdBuffer[cmd_content_index] == 'T')
		{
			CMDState = CMD_T;
		}
		else if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_T:
		if(cmdBuffer[cmd_content_index] == '#')
		{
			CMDState = CMD_HASTAG_T;
		}
		else if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_O:
		if(cmdBuffer[cmd_content_index] == 'K')
		{
			CMDState = CMD_K;
		}
		else if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_K:
		if(cmdBuffer[cmd_content_index] == '#')
		{
			CMDState = CMD_HASTAG_K;
		}
		else if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_HASTAG_K:
		if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			is_OK = 1;
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_END;
		}
		break;
	case CMD_HASTAG_T:
		if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_RST;
		}
		break;
	default:
		break;
	}
}
