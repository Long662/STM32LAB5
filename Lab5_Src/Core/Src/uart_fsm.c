/*
 * uart_fsm.c
 *
 *  Created on: Nov 25, 2024
 *      Author: NGUYEN THANH LONG_2011554
 */
#include "uart_fsm.h"

#define COUNT_TO_ERROR	3

uint8_t count_to_err = 0;
uint8_t cmdBuffer[CMD_CONTENT_MAX_LENGTH];
uint8_t buffer_flag = 0;
uint8_t cmd_content_index = 0;
uint8_t tempValue = 0;
uint8_t request_flag = 0;
volatile uint32_t ADC_value;
char response[RESPONSE_LENGTH];

//--------------------------------------
//*****UART_COM_STATE_MACHINE*****
//--------------------------------------
void uart_communication_fsm()
{
	switch(UARTState){
	case UART_IDLE:
		ERRState = ERROR_IDLE;
		break;
	case UART_RST:
		request_flag = 1;
		if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
		{
			ADC_value = HAL_ADC_GetValue(&hadc1);
			sprintf(response, "!ADC=%lu#\r\n", ADC_value);
			HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 0x500);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			setTimer(3000);
			ERRState = ERROR_IDLE;
			UARTState = UART_WAIT_OK;
		}
		else
		{
//	        sprintf(response, "ADC_ERROR\r\n");
//	        HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 0x500);
	        ERRState = ADC_ERR;
	        UARTState = UART_IDLE;
		}
		break;
	case UART_WAIT_OK:
		if(TimerFlag == 1)
		{
			ERRState = ERROR_IDLE;
			UARTState = UART_END;
		}
		break;
	case UART_END:
		if(is_OK == 0)
		{
			count_to_err++;
			if (count_to_err >= COUNT_TO_ERROR)
			{
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				ERRState = CMD_NOT_EXISTED;
				UARTState = UART_IDLE;
			}
			else
			{
				ERRState = ERROR_IDLE;
				UARTState = UART_RST;
			}
		}
		else if(is_OK == 1)
		{
			is_OK = 0;
			count_to_err = 0;
			HAL_UART_Transmit(&huart1, (void *)response, sprintf(response, "COMPLETED\r\n"), 0x500);
			ERRState = ERROR_IDLE;
			UARTState = UART_IDLE;
		}
		request_flag = 0;
		break;
	case UART_ERROR:
		ERRState = ERROR_IDLE;
		UARTState = UART_IDLE;
		break;
	default:
		break;
	}
}

//--------------------------------------
//*****COMMAND_PRASER_STATE_MACHINE*****
//--------------------------------------
void command_praser_fsm()
{
	switch(CMDState){
	case CMD_IDLE:
//		ERRState = ERROR_IDLE;
		if(cmdBuffer[cmd_content_index] == '!')
		{
			CMDState = CMD_EXCLAMATION;
		}
		else if (cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			cmd_content_index = 0;
			CMDState = CMD_IDLE;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
			UARTState = UART_ERROR;
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
			ERRState = INVALID_INPUT;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
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
			ERRState = INVALID_INPUT;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
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
			ERRState = INVALID_INPUT;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
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
			ERRState = INVALID_INPUT;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
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
			ERRState = INVALID_INPUT;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
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
			ERRState = INVALID_INPUT;
			cmd_content_index = 0;
			UARTState = UART_ERROR;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_HASTAG_K:
		if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			if(request_flag == 1)
			{
				is_OK = 1;
				CMDState = CMD_IDLE;
				ERRState = ERROR_IDLE;
				cmd_content_index = 0;
				UARTState = UART_END;
			}
			else
			{
				CMDState = CMD_IDLE;
				ERRState = NOT_REQUEST_YET;
				cmd_content_index = 0;
				UARTState = UART_ERROR;
			}
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
			UARTState = UART_ERROR;
		}
		break;
	case CMD_HASTAG_T:
		if(cmdBuffer[cmd_content_index] == '\r' || cmdBuffer[cmd_content_index] == '\n')
		{
			CMDState = CMD_IDLE;
			cmd_content_index = 0;
			UARTState = UART_RST;
		}
		else
		{
			ERRState = INVALID_INPUT;
			CMDState = CMD_IDLE;
			UARTState = UART_ERROR;
		}
		break;
	default:
		break;
	}
}

//--------------------------------------
// Add the received character into a buffer
//--------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1)
	{
		//HAL_UART_Transmit(&huart1, &tempValue, 1, 50);
		cmd_content_index++;
		cmdBuffer[cmd_content_index] = tempValue;
		buffer_flag = 1;
		if (cmd_content_index >= CMD_CONTENT_MAX_LENGTH)
		{
			ERRState = BUFFER_IS_FULL;
			cmd_content_index = 0;
		}
		HAL_UART_Receive_IT(&huart1, &tempValue, 1);
	}
}
