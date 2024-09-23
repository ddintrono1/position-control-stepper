/*
 * command.h
 *
 *  Created on: Sep 6, 2024
 *      Author: Dell
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include "main.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stepper.h"

typedef struct{

	Stepper* stepper;

	uint8_t string[20];
	uint8_t command_id;
	int command_num;
	uint8_t flag_id;
	int flag_num;
	int speed;
	int travelSpeed;
	int workSpeed;
	int travelAcceleration;
	int workAcceleration;

} Command;

void Command_Init(Command *command, Stepper *stepper);
void Command_Parse(Command *command, uint8_t string[]);
void Command_Clear(Command *command);
void Command_Execute(Command *command);
void Command_G0(Command *command);
void Command_G1(Command *command);
void Command_M203(Command *command);
void Command_M204(Command *command);
void Command_M205(Command *command);

#endif /* INC_COMMAND_H_ */
