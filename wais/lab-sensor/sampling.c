#include "sampling.h"


void 
setup_sensors(void)
{
	SENSORS_ACTIVATE(battery_sensor);
    SENSORS_ACTIVATE(temperature_sensor);
}


float 
floor(float x)
{ 
    if(x>=0.0f){
        return (float) ((int)x);  
    } else{
        return (float) ((int)x-1);        
    }   
}

/*---------------------------------------------------------------------------*/
int
get_battery(void)
{
    return battery_sensor.value(0);
}
/*---------------------------------------------------------------------------*/
int 
get_internal_temp(void)
{
    return temperature_sensor.value(0);
}

float 
get_mybatt(void){ 
    return (float) ((get_battery()*2.500*2)/4096);
}

float 
get_internal_temp_converted(void){ 
    return (float) (((get_internal_temp()*2.500)/4096)-0.986)*282;
}


int16_t get_sensor_acc_x(void)
{
  return accm_read_axis(X_AXIS);
}

int16_t get_sensor_acc_y(void)
{
  return accm_read_axis(Y_AXIS);
}

int16_t get_sensor_acc_z(void)
{
  return accm_read_axis(Z_AXIS);
}