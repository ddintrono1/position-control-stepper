/*
 * stepper.c
 *
 *  Created on: Sep 5, 2024
 *      Author: Dell
 */

#define clk_freq 84000000

#include "stepper.h"

extern TIM_HandleTypeDef htim6;


void Stepper_Init(Stepper* stepper, float stepDist, GPIO_TypeDef* enablePort, uint16_t enablePin, TIM_HandleTypeDef* htim, GPIO_TypeDef* stepPort, uint16_t stepPin, GPIO_TypeDef* dirPort, uint16_t dirPin,
		GPIO_TypeDef* ms1Port, uint16_t ms1Pin, GPIO_TypeDef* ms2Port, uint16_t ms2Pin, GPIO_TypeDef* ms3Port, uint16_t ms3Pin){
	stepper->htim = htim;
	stepper->stepDist = stepDist;
	stepper->enablePort = enablePort;
	stepper->enablePin = enablePin;
	stepper->stepPort = stepPort;
	stepper->stepPin = stepPin;
	stepper->dirPort = dirPort;
	stepper->dirPin = dirPin;
	stepper->ms1Port = ms1Port;
	stepper->ms1Pin = ms1Pin;
	stepper->ms2Port = ms2Port;
	stepper->ms2Pin = ms2Pin;
	stepper->ms3Port = ms3Port;
	stepper->ms3Pin = ms3Pin;

	// Set default not enabled (NOT ENABLED = HIGH)
	HAL_GPIO_WritePin(enablePort, enablePin, GPIO_PIN_SET);

}

void Stepper_Enable(Stepper* stepper){
	HAL_GPIO_WritePin(stepper->enablePort, stepper->enablePin, GPIO_PIN_RESET);
}

void Stepper_Disable(Stepper *stepper){
	HAL_GPIO_WritePin(stepper->enablePort, stepper->enablePin, GPIO_PIN_SET);
}

void Stepper_Start(Stepper* stepper){
	/*
	 * This function starts the stepper resetting the initial speed to zero
	 * and starting the master timer (the PWM timer that generates the pulses)
	 * and the timer which accelerates the stepper
	*/

	// Reset speed at launching
	Stepper_SetSpeed(stepper, 0);

	// Start PWM timer which pulses the stepper
	HAL_TIM_PWM_Start(stepper->htim, TIM_CHANNEL_1);


}

void Stepper_Stop(Stepper* stepper){
	/*
	 * This function stops the stepper by stopping the master timer (the PWM
	 * timer that generates the pulses) and the timer which accelerates the stepper
	 */

	// Stop PWM timer which pulses the stepper
	HAL_TIM_PWM_Stop(stepper->htim, TIM_CHANNEL_1);

}

void Stepper_ToggleDirection(Stepper* stepper){
	/*
	 * This function toggles rotation direction by toggling the direction pin of
	 * the stepper driver
	 */

	HAL_GPIO_TogglePin(stepper->dirPort, stepper->dirPin);
}

void Stepper_SetDirection(Stepper* stepper, Direction direction){
	/*
	 * This function sets rotation direction by setting or resetting direction pin
	 * of the stepper driver
	 */

	HAL_GPIO_WritePin(stepper->dirPort, stepper->dirPin, direction);
}

void Stepper_SetMicroStep(Stepper* stepper, MicrosteppingMode divider){
	/*
	 * This function sets microstepping mode by acting on microstepping pin of the
	 * stepper driver
	 */

	switch (divider){

	case FULL_STEP:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
		break;
	case HALF_STEP:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
		break;
	case QUARTER_STEP:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
		break;
	case EIGHTH_STEP:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
		break;
	case SIXTEENTH_STEP:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void Stepper_SetSpeed(Stepper* stepper, float speed){
	/*
	 * This function sets speed to a certain level by acting on the slave timer ARR register
	 */

	// This control avoids division by zero in new ARR value, stops the master timer by setting ARR=0
	if (speed <= 0.0f) {
		stepper->speed = 0;
		__HAL_TIM_SET_AUTORELOAD(stepper->htim, 0);
		return;
	}

	stepper->speed = speed;
	// Resetting the CNT to avoid CNT>ARR when speeding up (ARR decreases)
	__HAL_TIM_SET_COUNTER(stepper->htim,0);
	__HAL_TIM_SET_AUTORELOAD(stepper->htim, (uint32_t) clk_freq*stepper->stepDist/(stepper->speed*(stepper->htim->Instance->PSC+1))-1);
	stepper->htim->Instance->EGR = TIM_EGR_UG;

}

void Stepper_SpeedUp(Stepper *stepper, float deltaSpeed){
	/*
	 * This function increses stepper speed of the given value by recalling Stepper_SetSpeed
	 * function
	 */

	Stepper_SetSpeed(stepper, stepper->speed + deltaSpeed);
}











