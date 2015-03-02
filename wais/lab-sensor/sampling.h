#ifndef WAIS_SAMPLING_H
#define WAIS_SAMPLING_H

#include "contiki.h"
#include "dev/temperature-sensor.h"
#include "dev/battery-sensor.h"
#include "dev/cc2420.h"

void setup_sensors(void);

float floor(float x);
int get_battery(void);
int get_temp(void);
float get_mybatt(void);
float get_mytemp(void);

#endif