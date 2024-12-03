/*
 * uart_fsm.c
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */
#include "uart_fsm.h"

#define COUNT_TO_ERROR	3

uint8_t count_to_err = 0;
uint8_t cmdBuffer[CMD_CONTENT_MAX_LENGTH];
uint8_t buffer_flag = 0;
uint8_t cmd_content_index = 0;
uint8_t tempValue = 0;

uint32_t ADC_value = 0;
char response[RESPONSE_LENGTH];
//uint8_t data[] = "HELLO WORD\r\n";

void uart_communication_fsm()
{
	switch(UARTState){
	case UART_IDLE:
		break;
	case UART_RST:
		ADC_value = HAL_ADC_GetValue(&hadc1);
		sprintf(response, "!ADC=%d#\r\n", ADC_value);
		HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 0x500);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
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
		if(is_OK == 0)
		{
			count_to_err++;
			if (count_to_err >= COUNT_TO_ERROR)
			{
				HAL_UART_Transmit(&huart1, (void *)response, sprintf(response, "ERROR\r\n"), 0x500);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				UARTState = UART_IDLE;
			}
			else
			{
				UARTState = UART_RST;
			}
		}
		else if(is_OK == 1)
		{
			is_OK = 0;
			count_to_err = 0;
			HAL_UART_Transmit(&huart1, (void *)response, sprintf(response, "COMPLETED\r\n"), 0x500);
			UARTState = UART_IDLE;
		}
		//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
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

// Add the received character into a buffer
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1)
	{
		//HAL_UART_Transmit(&huart1, &tempValue, 1, 50);
		cmd_content_index++;
		cmdBuffer[cmd_content_index] = tempValue;
		buffer_flag = 1;
		if (cmd_content_index >= CMD_CONTENT_MAX_LENGTH)
		{
			cmd_content_index = 0;
		}
		HAL_UART_Receive_IT(&huart1, &tempValue, 1);
	}
}
