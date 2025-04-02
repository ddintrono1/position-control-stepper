/*
 * command_instructions.h
 *
 *  Created on: Apr 2, 2025
 *      Author: Dell
 */

#ifndef INC_COMMAND_INSTRUCTIONS_H_
#define INC_COMMAND_INSTRUCTIONS_H_


#include <stdint.h>

const uint8_t command_help_message[] =
		"______________________________________________________________\r\n\r\n"
		"Available Commands:\r\n\r\n"
		"G0 X<value>: Set the G0 setpoint using the first set of PID gains.\r\n"
		"         - Example: 'G0 X50' adjusts the setpoint G0 to 50 mm.\r\n"
		"         - Value range: 0-800\r\n"
		"\r\n"
		"G1 X<value>: Set the G1 setpoint using the second set of PID gains.\r\n"
		"         - Example: 'G1 X75' adjusts the setpoint G1 to 75 mm.\r\n"
		"         - Value range: 0-800\r\n"
		"\r\n"
		"M203 <flag> <value>: Modify the proportional gain (Kp) for commands G0 and G1.\r\n"
		"         - Flags: 'T' for G0, 'S' for G1.\r\n"
		"         - Example: 'M203 T50' sets Kp for G0 to 50% of its max value.\r\n"
		"         - Value range: 0-100%\r\n"
		"\r\n"
		"M204 <flag> <value>: Modify the integral gain (Ki) for commands G0 and G1.\r\n"
		"         - Flags: 'T' for G0, 'S' for G1.\r\n"
		"         - Example: 'M204 S20' sets Ki for G1 to 20% of its max value.\r\n"
		"         - Value range: 0-100%\r\n"
		"\r\n"
		"M205 <flag> <value>: Modify the derivative gain (Kd) for commands G0 and G1.\r\n"
		"         - Flags: 'T' for G0, 'S' for G1.\r\n"
		"         - Example: 'M205 T30' sets Kd for G0 to 30% of its max value.\r\n"
		"         - Value range: 0-100%\r\n"
		"\r\n"
		"T0 <flag> <value>: Change the threshold for motor control.\r\n"
		"         - Flags: 'T' for G0, 'S' for G1.\r\n"
		"         - Example: 'T0 S150' sets the threshold for G1 to 150.\r\n"
		"______________________________________________________________\r\n\r\n";



#endif /* INC_COMMAND_INSTRUCTIONS_H_ */
