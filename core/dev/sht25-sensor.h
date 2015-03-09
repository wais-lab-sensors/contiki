/*
 * -----------------------------------------------------------------
 *
 * Author  : Philip Basford <pjb@ecs.soton.ac.uk>
 * Created : 2015-03-09
 */

#ifndef __SHT25_SENSOR_H__
#define __SHT25_SENSOR_H__

#include "lib/sensors.h"

extern const struct sensors_sensor sh25_sensor;

#define SH25_SENSOR_TEMP              0
#define SH25_SENSOR_HUMIDITY          1

#endif