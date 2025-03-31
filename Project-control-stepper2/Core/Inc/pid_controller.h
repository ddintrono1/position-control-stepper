/*
 * pid_controller.h
 *
 *  Created on: Mar 19, 2025
 *      Author: Dell
 */

#ifndef INC_PID_CONTROLLER_H_
#define INC_PID_CONTROLLER_H_

#include <stdint.h>


typedef struct {
    float Kp;  // Proportional gain
    float Ki;  // Integral gain
    float Kd;  // Derivative gain

    float setpoint;   // Desired value
    float prev_error; // Previous step error
    float integral;   // Error accumulation
    float output_min; // Upper saturation bound
    float output_max; // Lower saturation bound
} PID_Controller;

void PID_Init(PID_Controller *pid, float Kp, float Ki, float Kd, float out_min, float out_max);
float PID_Compute(PID_Controller *pid, float measurement, float dt);
void PID_UpdateSetpoint(PID_Controller* pid, float setpoint);
void PID_UpdateProportional(PID_Controller* pid, float Kp);
void PID_UpdateIntegral(PID_Controller* pid, float Ki);
void PID_UpdateDerivative(PID_Controller* pid, float Kd);

#endif /* INC_PID_CONTROLLER_H_ */
