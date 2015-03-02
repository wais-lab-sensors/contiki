#ifndef LAB_SENSOR_H
#define LAB_SENSOR_H

#include "contiki.h"
#include "httpd-simple.h"
#include "webserver-nogui.h"
#include "dev/cc2420.h"
#include <stdio.h>
#include <string.h>
#include "sampling.h"


#define HISTORY 16



void generate_chart(const char *title, const char *unit, int min, int max, int *values);
httpd_simple_script_t httpd_simple_get_script(const char *name);

const char *TOP = "<html><head><title>Contiki Web Sense</title></head><body>\n";
const char *BOTTOM = "</body></html>\n";

#endif