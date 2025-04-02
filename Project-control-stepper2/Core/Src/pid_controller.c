/*
 * pid_controller.c
 *
 *  Created on: Mar 19, 2025
 *      Author: Dell
 */

#include "pid_controller.h"


void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float out_min, float out_max, float threshold) {
	/*
	 * This function initializes PID
	 */
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->setpoint = 0.0f;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->threshold = threshold;

}

float PID_Compute(PID_Controller *pid, float measurement, float dt) {
	/*
	 * This function initializes pid controller
	 */
    float error = pid->setpoint - measurement;

    // Proportional contribution computation
    float P_out = pid->Kp * error;

    // Integral contribution computation
    pid->integral += error * dt;
    float I_out = pid->Ki * pid->integral;

    // Derivative contribution computation
    float derivative = (error - pid->prev_error) / dt;
    float D_out = pid->Kd * derivative;

    // Summing every contribution
    float output = P_out + I_out + D_out;

    // Output saturation
    if (output > pid->output_max) output = pid->output_max;
    if (output < pid->output_min) output = pid->output_min;

    // Neglect small control actions
    if (fabs(output)< pid->threshold) output = 0.0f;

    // Updating previous error
    pid->prev_error = error;

    return output;
}

void PID_UpdateSetpoint(PID_Controller* pid, float setpoint){
	/*
	 * This function updates the setpoint to the given value
	 */
	pid->setpoint = setpoint;
}

void PID_UpdateProportional(PID_Controller* pid, float Kp){
	/*
	 * This function updates the proportional gain to a given value
	 */
	pid->Kp = Kp;
}

void PID_UpdateIntegral(PID_Controller* pid, float Ki){
	/*
	 * This function updates the integral gain to a given value
	 */
	pid->Ki = Ki;
}

void PID_UpdateDerivative(PID_Controller* pid, float Kd){
	/*
	 * This function updates the derivative gain to a given value
	 */
	pid->Kd = Kd;
}

void PID_UpdateThreshold(PID_Controller* pid, float threshold){
	/*
	 * This function updates the no-action threshold
	 */
	pid->threshold = threshold;
}

