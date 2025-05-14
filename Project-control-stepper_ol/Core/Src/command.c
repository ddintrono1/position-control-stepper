/*
 * command.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Dell
 */


#include "command.h"
#include "stepper.h"
#include <stdlib.h>
#include <math.h>

extern float *speed_table;
extern uint32_t speed_table_size;
extern int last_position;

uint8_t error_message[] = "ERROR: unvalid command\r\n";
uint8_t g0_message[] = "G0 command launched\r\n";
uint8_t g1_message[] = "G1 command launched\r\n";
uint8_t m203_message[] = "M203 command launched\r\n";
uint8_t m204_message[] = "M204 command launched\r\n";
uint8_t m205_message_clockwise[] = "M205 command launched: clockwise rotation\r\n";
uint8_t m205_message_anticlockwise[] = "M205 command launched: anti-clockwise rotation\r\n";


void Command_Init(Command *command, Stepper *stepper, TIM_HandleTypeDef *htim, UART_HandleTypeDef *huart){
	command->stepper = stepper;
	command->htim = htim;
	command->huart = huart;

	// Setting default speeds and accelerations
	command->travelSpeed = 40;
	command->workSpeed = 20;
	command->travelAcceleration = 30;
	command->workAcceleration = 15;

}

void Command_Parse(Command *command, uint8_t string[]){
	/*
	 * This command sets and parse the command string
	 */
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
    /*
     * This function runs the corresponding command
     */

    switch (command->command_id) {
        case 'G':
            Command_G0(command); return;

        case 'M':
            switch (command->command_num) {
                case 203: Command_M203(command); return;
                case 204: Command_M204(command); return;
                case 205: Command_M205(command); return;
            }
            break;

    }

    HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));
}

void Command_G0(Command *command){

	float step_displacement = command->stepper->stepDist;

    int total_steps = (command->flag_num - last_position) / step_displacement;

    if (total_steps < 0){
    	total_steps = fabs(total_steps);
    	Stepper_SetDirection(command->stepper, CLOCKWISE);
    }
    else{
    	Stepper_SetDirection(command->stepper, COUNTER_CLOCKWISE);
    }

    __HAL_TIM_SET_COUNTER(command->htim, 0);                    // Reset contatore
    __HAL_TIM_SET_AUTORELOAD(command->htim, total_steps);       // Set ARR = numero passi
    __HAL_TIM_CLEAR_IT(command->htim, TIM_IT_UPDATE);           // Pulisci eventuali interrupt vecchi
    __HAL_TIM_ENABLE_IT(command->htim, TIM_IT_UPDATE);

    // Verifica input
    if (total_steps <= 0 || total_steps > 15000) {
        HAL_UART_Transmit(command->huart, (uint8_t *)"Errore: passi non validi\r\n", 27, HAL_MAX_DELAY);
        return;
    }

    // Libera vecchia tabella
    if (speed_table != NULL) {
        free(speed_table);
        speed_table = NULL;
    }

    // Alloca nuova tabella
    speed_table = malloc(total_steps * sizeof(float));
    if (speed_table == NULL) {
        HAL_UART_Transmit(command->huart, (uint8_t *)"Malloc fallito\r\n", 17, HAL_MAX_DELAY);
        return;
    }

    speed_table_size = total_steps;

    // Calcolo del profilo di velocità
    ComputeSpeedProfile(speed_table, total_steps, command->travelSpeed, command->travelAcceleration, step_displacement);

    // Update last position
    last_position = command->flag_num;

    // Avvio motore
    HAL_UART_Transmit_IT(command->huart, g0_message, sizeof(g0_message));
    Stepper_Enable(command->stepper);
    Stepper_Start(command->stepper);
}


void Command_M203(Command *command){
	/*
	 * This command sets the travelling or working speed
	 */
	if (command->flag_id == 'T'){
		// Modify travelling speed
		command->travelSpeed = command->flag_num;
		HAL_UART_Transmit_IT(command->huart, m203_message, sizeof(m203_message));
		return;
	}

	if (command->flag_id == 'S'){
		// Modify extruding speed
		command->workSpeed = command->flag_num;
		HAL_UART_Transmit_IT(command->huart, m203_message, sizeof(m203_message));
		return;
	}

	HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));

}

void Command_M204(Command *command){
	/*
	 * This command modifies the acceleration
	 */
	if (command->flag_id == 'T'){
		// Modify traveling acceleration
		command->travelAcceleration = command->flag_num;
		HAL_UART_Transmit_IT(command->huart, m204_message, sizeof(m204_message));
		return;

	}
	if (command->flag_id == 'S'){
		// Modify extruding acceleration
		command->workAcceleration = command->flag_num;
		HAL_UART_Transmit_IT(command->huart, m204_message, sizeof(m204_message));
		return;
	}

	HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));

}

void Command_M205(Command *command){
	/*
	 * This command changes the rotation direction
	 */
	if (command->flag_num == 1){
		Stepper_SetDirection(command->stepper, CLOCKWISE);
		HAL_UART_Transmit_IT(command->huart, m205_message_clockwise, sizeof(m205_message_clockwise));
		return;
	}

	if (command->flag_num == 2){
		Stepper_SetDirection(command->stepper, COUNTER_CLOCKWISE);
		HAL_UART_Transmit_IT(command->huart, m205_message_anticlockwise, sizeof(m205_message_anticlockwise));
		return;
	}

	HAL_UART_Transmit_IT(command->huart, error_message, sizeof(error_message));

}

void ComputeSpeedProfile(float* table, uint32_t total_steps, int max_speed, int acceleration, float step_displacement) {

    // Calcolo dei passi necessari per raggiungere max_speed
    float acc_steps_f = (max_speed * max_speed) / (2.0f * acceleration * step_displacement);

    if (2 * acc_steps_f > total_steps) {
        // Non c'è spazio sufficiente per raggiungere max_speed
        // Calcolo la massima velocità raggiungibile in metà dei passi disponibili
        max_speed = sqrtf(2.0f * acceleration * step_displacement * (total_steps / 2));
        acc_steps_f = (max_speed * max_speed) / (2.0f * acceleration * step_displacement);
    }

    uint32_t acc_steps = (uint32_t)acc_steps_f;
    uint32_t dec_steps = total_steps - acc_steps;

    for (uint32_t i = 0; i < total_steps; i++) {
        if (i < acc_steps) {
            table[i] = sqrtf(2.0f * acceleration * step_displacement * i);
        } else if (i < dec_steps) {
            table[i] = (float)max_speed;
        } else {
            table[i] = sqrtf(2.0f * acceleration * step_displacement * (total_steps - i));
        }
    }

    // Evita stallo iniziale
    if (table[0] < 0.1f) table[0] = 0.1f;
}


