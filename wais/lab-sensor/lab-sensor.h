#ifndef LAB_SENSOR_H
#define LAB_SENSOR_H

#define HISTORY 16

float floor(float x);
int get_battery(void);
int get_temp(void);
float get_mybatt(void);
float get_mytemp(void);
void generate_chart(const char *title, const char *unit, int min, int max, int *values);


#endif