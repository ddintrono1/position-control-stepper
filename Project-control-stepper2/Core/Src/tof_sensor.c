/*
 * tof_sensor.c
 *
 *  Created on: Mar 19, 2025
 *      Author: Dell
 */

#include "tof_sensor.h"

// Variabili di calibrazione
uint8_t VhvSettings = 0;
uint8_t PhaseCal = 0;
uint32_t refSpadCount;
uint8_t isApertureSpads;
int32_t pOffsetMicroMeter;

// Definizione della struttura del sensore
VL53L0X_Dev_t vl53l0x_dev;
VL53L0X_Error status;
VL53L0X_RangingMeasurementData_t RangingData;

void TOF_Init(int32_t CalDistanceMilliMeter) {
	/*
	 * This function initializes the sensor and sets the given calibration distance, using
	 * the VL530L0X sensor library from STM
	 */

    vl53l0x_dev.I2cHandle = &hi2c1;
    vl53l0x_dev.I2cDevAddr = 0x52;

    status = VL53L0X_WaitDeviceBooted(&vl53l0x_dev);
    status = VL53L0X_DataInit(&vl53l0x_dev);
    status = VL53L0X_StaticInit(&vl53l0x_dev);
    status = VL53L0X_PerformRefSpadManagement(&vl53l0x_dev, &refSpadCount, &isApertureSpads);
    status = VL53L0X_PerformRefCalibration(&vl53l0x_dev, &VhvSettings, &PhaseCal);
    VL53L0X_PerformOffsetCalibration(&vl53l0x_dev, CalDistanceMilliMeter, &pOffsetMicroMeter);

    VL53L0X_SetDeviceMode(&vl53l0x_dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    VL53L0X_SetMeasurementTimingBudgetMicroSeconds(&vl53l0x_dev, 10000);
    status = VL53L0X_StartMeasurement(&vl53l0x_dev);
}

uint16_t TOF_GetDistance() {
	/*
	 * This function returns the measured sensor distance, unfiltered
	 */
    status = VL53L0X_GetRangingMeasurementData(&vl53l0x_dev, &RangingData);
    return RangingData.RangeMilliMeter;
}

int16_t TOF_GetFilteredDistance(uint16_t min_value, uint16_t max_value) {
	/*
	 * This function returns an error value -1 if the measured distance is out of the given bound
	 * otherwise normally returns the distance. This is useful to filter sensor outliers
	 */

	uint16_t distance = TOF_GetDistance();

	if (distance > max_value || distance < min_value ){
		return -1;
	}
	else{
		return distance;
	}
}

