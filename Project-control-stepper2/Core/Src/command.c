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

uint8_t error_message[] = "ERROR: unvalid command\r\n";
uint8_t g0_message[] = "G0 command launched\r\n";
uint8_t g1_message[] = "G1 command launched\r\n";
uint8_t m203_message[] = "M203 command launched\r\n";
uint8_t m204_message[] = "M204 command launched\r\n";
uint8_t m205_message_clockwise[] = "M205 command launched: clockwise rotation\r\n";
uint8_t m205_message_anticlockwise[] = "M205 command launched: anti-clockwise rotation\r\n";


void Command_Init(Command *command, Stepper *stepper){
	command->stepper = stepper;

	// Setting default speeds and accelerations
	command->travelSpeed = 40;
	command->workSpeed = 20;
	command->travelAcceleration = 30;
	command->workAcceleration = 15;

}

void Command_Parse(Command *command, uint8_t string[]){
	// String setting
	strcpy((char *) command->string, (char *) string);

	// String parsing
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
		Command_G0(command);
	}
	else if (command->command_id == 'G' && command->command_num == 1){
		Command_G1(command);
	}
	else if (command->command_id == 'M' && command->command_num == 203){
		Command_M203(command);
	}
	else {
		HAL_UART_Transmit_IT(&huart2, error_message, sizeof(error_message));
	}
}

void Command_G0(Command *command){
	/*
	 * This function sets a new setpoint
	 */
	HAL_UART_Transmit_IT(&huart2, g0_message, sizeof(g0_message));

}

void Command_G1(Command *command){
	/*
	 * This function sets a new setpoint to be reached with new PID parameters
	 */
	HAL_UART_Transmit_IT(&huart2, g1_message, sizeof(g1_message));

}

void Command_M203(Command *command){
	/*
	 * This command modifies PID parameters for G0 and G1 in a 0-100 value
	 */
	if (command->flag_id == 'T'){
		HAL_UART_Transmit_IT(&huart2, m203_message, sizeof(m203_message));

	}
	else if (command->flag_id == 'S'){
		// Modify extruding speed
		HAL_UART_Transmit_IT(&huart2, m203_message, sizeof(m203_message));
	}
	else {
		HAL_UART_Transmit_IT(&huart2, error_message, sizeof(error_message));
	}
}






