/*
 * command.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Dell
 */


#include "command.h"
#include "pid_controller.h"


uint8_t error_message[] = "ERROR: unvalid command\r\n";
uint8_t g0_message[] = "G0 command launched\r\n";
uint8_t g1_message[] = "G1 command launched\r\n";
uint8_t m203_message[] = "M203 command launched\r\n";
uint8_t m204_message[] = "M204 command launched\r\n";
uint8_t m205_message[] = "M205 command launched\r\n";


void Command_Init(Command *command, PID_Controller *pid, UART_HandleTypeDef *huart){
	/*
	 * Assign a pid and a uart to the command, sets default gains
	 */

	command->pid = pid;
	command->huart = huart;

	command->Kp0 = command->pid->Kp;
	command->Kp1 = command->pid->Kp;
	command->Ki0 = command->pid->Ki;
	command->Ki1 = command->pid->Ki;
	command->Kd0 = command->pid->Kd;
	command->Kd1 = command->pid->Kd;

}

void Command_Parse(Command *command, uint8_t string[]){
	/*
	 * This function parses the provided command
	 */
	// String setting
	strcpy((char *) command->string, (char *) string);

	// String parsing
	sscanf((char *) command->string, "%c %d %c %d", &(command->command_id), &(command->command_num), &(command->flag_id), &(command->flag_num));
}

void Command_Clear(Command *command){
	/*
	 * This function clears command fields
	 */

	memset(command->string, 0, sizeof(command->string));
	command->command_id = 0;
	command->command_num = 0;
	command->flag_id = 0;
	command->flag_num = 0;
}

void Command_Execute(Command *command){
	/*
	 * This function runs the command
	 */

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
	}
	else if (command->command_id == 'M' && command->command_num == 205){
		Command_M205(command);
	}
	else {
		HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
	}
}

void Command_G0(Command *command){
	/*
	 * This function sets a new setpoint to be reached with the first set of gains
	 */

	if (command->flag_num >= 0 && command->flag_num <= 800){
		PID_UpdateProportional(command->pid, command->Kp0);
		PID_UpdateIntegral(command->pid, command->Ki0);
		PID_UpdateDerivative(command->pid, command->Kd0);

		PID_UpdateSetpoint(command->pid, command->flag_num);

		HAL_UART_Transmit_IT(command->huart, g0_message, sizeof(g0_message));
	}
	else {
		HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
	}

}

void Command_G1(Command *command){
	/*
	 * This function sets a new setpoint to be reached with the second set of gains
	 */

	if (command->flag_num >= 0 && command->flag_num <= 800){
		PID_UpdateProportional(command->pid, command->Kp1);
		PID_UpdateIntegral(command->pid, command->Ki1);
		PID_UpdateDerivative(command->pid, command->Kd1);

		PID_UpdateSetpoint(command->pid, command->flag_num);

		HAL_UART_Transmit_IT(command->huart, g1_message, sizeof(g1_message));
	}
	else {
		HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
	}

}

void Command_M203(Command *command){
	/*
	 * This command modifies proportional gain for G0 and G1 in a 0-100% range
	 */

	// Check if the provided value for Kp is in the range 0-100%
	if (command->flag_num >= 0 && command->flag_num <= 100){

		if (command->flag_id == 'T'){
			command->Kp0 = command->flag_num * 0.01;
			HAL_UART_Transmit_IT(command->huart, m203_message, sizeof(m203_message));
		}
		else if (command->flag_id == 'S'){
			command->Kp1 = command->flag_num * 0.01;
			HAL_UART_Transmit_IT(command->huart, m203_message, sizeof(m203_message));
		}
		else {
			HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
		}
	}
	else {
		HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
	}

}

void Command_M204(Command *command) {
	/*
	 * This command modifies integral gain for G0 and G1 in a 0-100% range
	 */
	if (command->flag_num >= 0 && command->flag_num <= 100){

		if(command->flag_id == 'T'){
			command->Ki0 = command->flag_num * 0.001;
			HAL_UART_Transmit_IT(command->huart, m204_message, sizeof(m204_message));
		}
		else if (command->flag_id == 'S'){
			command->Ki1 = command->flag_num * 0.001;
			HAL_UART_Transmit_IT(command->huart, m204_message, sizeof(m204_message));
		}
		else {
			HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
		}
	}

	else {
		HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
	}

}

void Command_M205(Command *command) {
	/*
	 * This command modifies derivative gain for G0 and G1 in a 0-100% range
	 */
	if (command->flag_num >= 0 && command->flag_num <= 100) {
		if (command->flag_id == 'T'){
				command->Kd0 = command->flag_num;
				HAL_UART_Transmit_IT(command->huart, m205_message, sizeof(m205_message));
			}
			else if (command->flag_id == 'S'){
				command->Kd1 = command->flag_num;
				HAL_UART_Transmit_IT(command->huart, m205_message, sizeof(m205_message));
			}
			else{
				HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
			}
	}

	else {
		HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
	}

}











