/*
 * uart_fsm.h
 *
 *  Created on: Nov 25, 2024
 *      Author: HP WORK
 */

#ifndef INC_UART_FSM_H_
#define INC_UART_FSM_H_

#include "global.h"

void uart_communication_fsm();
void command_praser_fsm();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_UART_FSM_H_ */
