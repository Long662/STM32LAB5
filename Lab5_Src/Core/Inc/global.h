/*
 * global.h
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include "error.h"
#include "timer.h"
#include <string.h>
#include "stdio.h"

#define CMD_CONTENT_MAX_LENGTH		50
#define MIN_CMD_LENGTH				3
#define RESPONSE_LENGTH				50
#define TIMER_CYCLE					10

enum UART_State {UART_IDLE, UART_RST, UART_WAIT_OK, UART_RESPONSE, UART_END, UART_ERROR};
enum CMD_State {CMD_IDLE, CMD_R, CMD_S, CMD_T, CMD_O, CMD_K, CMD_EXCLAMATION, CMD_HASTAG_K, CMD_HASTAG_T};
enum ERROR_State {ERROR_IDLE, CMD_NOT_EXISTED, BUFFER_IS_FULL, INVALID_INPUT};

extern enum UART_State UARTState;
extern enum CMD_State CMDState;
extern enum ERROR_State ERRState;

extern uint8_t cmdBuffer[CMD_CONTENT_MAX_LENGTH];
extern uint8_t cmd_content_index;
extern uint8_t buffer_flag;

extern int TimerCounter;
extern int TimerFlag;



#endif /* INC_GLOBAL_H_ */
