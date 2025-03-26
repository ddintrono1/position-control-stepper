/*
 * tof_sensor.h
 *
 *  Created on: Mar 19, 2025
 *      Author: Dell
 */

#ifndef INC_TOF_SENSOR_H_
#define INC_TOF_SENSOR_H_

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "i2c.h"

// Dichiarazione della struttura del sensore
extern VL53L0X_Dev_t vl53l0x_dev;
extern VL53L0X_Error status;
extern VL53L0X_RangingMeasurementData_t RangingData;


void TOF_Init(int32_t);
uint16_t TOF_GetDistance();
int16_t TOF_GetFilteredDistance(uint16_t, uint16_t);

#endif /* INC_TOF_SENSOR_H_ */


