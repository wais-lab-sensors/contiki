#ifndef WAIS_SAMPLING_H
#define WAIS_SAMPLING_H

#include "contiki.h"
#include "dev/temperature-sensor.h"
#include "dev/battery-sensor.h"
#include "dev/cc2420.h"
#include "adxl345.h"
#include <stdint.h>

void setup_sensors(void);

float floor(float x);
int get_battery(void);
int get_internal_temp(void);
float get_mybatt(void);
float get_internal_temp_converted(void);
int16_t get_sensor_acc_x(void);
int16_t get_sensor_acc_y(void);
int16_t get_sensor_acc_z(void);

#endif