/*
 * command.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Dell
 */


#include "command.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef tim3;

uint8_t error_message[] = "ERROR: unvalid command\r\n";
uint8_t g0_message[] = "G0 command launched\r\n";
uint8_t g1_message[] = "G1 command launched\r\n";
uint8_t m203_message[] = "M203 command launched\r\n";
uint8_t m204_message[] = "M204 command launched\r\n";


void Command_Init(Command *command, uint8_t string[]){
	strcpy((char *) command->string, (char *) string);
	sscanf((char *) command->string, "%c %d %c %d", &(command->command_id), &(command->command_num), &(command->flag_id), &(command->flag_num));
}

void Command_Clear(Command *command){
	memset(command->string, 0, sizeof(command->string));
	command->command_id = 0;
	command->command_num = 0;
	command->flag_id = 0;
	command->flag_num = 0;
}

void Command_Execute(Command *command){
	if (command->command_id == 'G' && command-> command_num == 0){
		HAL_UART_Transmit_IT(&huart2, g0_message, sizeof(g0_message));
	}
	else if (command->command_id == 'G' && command-> command_num == 1){
			HAL_UART_Transmit_IT(&huart2, g1_message, sizeof(g1_message));
	}
	else if (command->command_id == 'M' && command->command_num == 203){
		HAL_UART_Transmit_IT(&huart2, m203_message, sizeof(m203_message));
	}
	else if (command->command_id == 'M' && command->command_num == 204){
			HAL_UART_Transmit_IT(&huart2, m204_message, sizeof(m204_message));
	}
	else {
		HAL_UART_Transmit_IT(&huart2, error_message, sizeof(error_message));
	}
}

void Command_G0(Command *command){
	__HAL_TIM_SET_AUTORELOAD(&htim3, command->flag_num);

	//to be continued


}


