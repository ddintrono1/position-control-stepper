/*
 * command.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Dell
 */


#include "command.h"
#include "stepper.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;
extern Stepper nema_17;

uint8_t error_message[] = "ERROR: unvalid command\r\n";
uint8_t g0_message[] = "G0 command launched\r\n";
uint8_t g1_message[] = "G1 command launched\r\n";
uint8_t m203_message[] = "M203 command launched\r\n";
uint8_t m204_message[] = "M204 command launched\r\n";


void Command_Init(Command *command, uint8_t string[]){
	strcpy((char *) command->string, (char *) string);
	sscanf((char *) command->string, "%c %d %c %d", &(command->command_id), &(command->command_num), &(command->flag_id), &(command->flag_num));

	// Setting default speed
	if (command->travelSpeed == 0){
		command->travelSpeed = 10;
	}
	if (command->workSpeed == 0){
		command->workSpeed = 5;
	}
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
		Command_G0(command);
	}
	else if (command->command_id == 'G' && command->command_num == 1){
		Command_G1(command);
	}
	else if (command->command_id == 'M' && command->command_num == 203){
		Command_M203(command);
	}
	else if (command->command_id == 'M' && command->command_num == 204){
		Command_M204(command);
		HAL_UART_Transmit_IT(&huart2, m204_message, sizeof(m204_message));
	}
	else {
		HAL_UART_Transmit_IT(&huart2, error_message, sizeof(error_message));
	}
}

void Command_G0(Command *command){
	// This command moves the extruder of a certain coordinate at travelling speed
	__HAL_TIM_SET_AUTORELOAD(&htim3, command->flag_num);
	HAL_UART_Transmit_IT(&huart2, g0_message, sizeof(g0_message));
	Stepper_SetSpeed(&nema_17, command->travelSpeed);
	Stepper_Enable(&nema_17);
	Stepper_Start(&nema_17);
}

void Command_G1(Command *command){
	// This command moves the extruder of a certain coordinate at working speed
	__HAL_TIM_SET_AUTORELOAD(&htim3, command->flag_num);
	HAL_UART_Transmit_IT(&huart2, g1_message, sizeof(g1_message));
	Stepper_SetSpeed(&nema_17, command->workSpeed);
	Stepper_Enable(&nema_17);
	Stepper_Start(&nema_17);
}

void Command_M203(Command *command){
	// This command sets the travelling or working speed
	if (command->flag_id == 'T'){
		// Modify travelling speed
		command->travelSpeed = command->flag_num;
		HAL_UART_Transmit_IT(&huart2, m203_message, sizeof(m203_message));
	}
	else if (command->flag_id == 'S'){
		// Modify extruding speed
		command->workSpeed = command->flag_num;
		HAL_UART_Transmit_IT(&huart2, m203_message, sizeof(m203_message));
	}
	else {
		HAL_UART_Transmit_IT(&huart2, error_message, sizeof(error_message));
	}
}

void Command_M204(Command *command){

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if (htim->Instance == TIM3){
		Stepper_Stop(&nema_17);
		Stepper_Disable(&nema_17);
	}
}


