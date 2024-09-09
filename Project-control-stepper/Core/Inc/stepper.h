/*
 * stepper.h
 *
 *  Created on: Sep 5, 2024
 *      Author: Dell
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "stdint.h"
#include "main.h"


typedef struct {
	TIM_HandleTypeDef* htim;	// PWM timer
	float stepDist;				// distance per step in mm
	GPIO_TypeDef* enablePort;	// enable port
	uint16_t enablePin;			// enable pin
	GPIO_TypeDef* stepPort;		// step port
	uint16_t stepPin;			// step pin
	GPIO_TypeDef* dirPort;		// dir port
	uint16_t dirPin;			// dir pin
	GPIO_TypeDef* ms1Port;		// microstepping port 1
	uint16_t ms1Pin;			// microstepping pin 1
	GPIO_TypeDef* ms2Port;		// microstepping port 2
	uint16_t ms2Pin;			// microstepping pin 2
	GPIO_TypeDef* ms3Port;		// microstepping port 3
	uint16_t ms3Pin;			// microstepping pin 3

	int speed;

} Stepper;

typedef enum{
	ANTI_CLOCKWISE,
	CLOCKWISE
} Direction;

typedef enum {
    FULL_STEP = 1,    	 	// 1/1 step
    HALF_STEP = 2,     		// 1/2 step
    QUARTER_STEP = 4,  		// 1/4 step
    EIGHTH_STEP = 8,   		// 1/8 step
    SIXTEENTH_STEP = 16, 	// 1/16 step
} MicrosteppingMode;

void Stepper_Init(Stepper* stepper, float stepDist, GPIO_TypeDef* enablePort, uint16_t enablePin, TIM_HandleTypeDef* htim, GPIO_TypeDef* stepPort, uint16_t stepPin, GPIO_TypeDef* dirPort, uint16_t dirPin,
		GPIO_TypeDef* ms1Port, uint16_t ms1Pin, GPIO_TypeDef* ms2Port, uint16_t ms2Pin, GPIO_TypeDef* ms3Port, uint16_t ms3Pin);
void Stepper_Enable(Stepper* stepper);
void Stepper_Start(Stepper* stepper);
void Stepper_Stop(Stepper* stepper);
void Stepper_ToggleDirection(Stepper* stepper);
void Stepper_SetDirection(Stepper* stepper, Direction direction);
void Stepper_SetMicroStep(Stepper* stepper, MicrosteppingMode divider);
void Stepper_SetSpeed(Stepper* stepper, int speed);


#endif /* INC_STEPPER_H_ */
