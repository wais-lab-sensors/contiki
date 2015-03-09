#include "contiki.h"
#include "dev/sht25.h"
#include <stdio.h>

PROCESS(test_sht25_process, "SHT25 test");
AUTOSTART_PROCESSES(&test_sht25_process);

float
floor(float x)
{
    if(x>=0.0f){
        return (float) ((int)x);
    } else{
        return (float) ((int)x-1);
    }
}

PROCESS_THREAD(test_sht25_process, ev, data)
{
    static struct etimer et;
    static uint16_t raw_temp;
    static uint16_t raw_hum;
    static float temp;
    static float hum;
    PROCESS_BEGIN();
    sht25_init();
    printf("Starting loop\n");
    while(1){
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_YIELD();
        raw_temp = sht25_temp();
        printf("Raw temp =  %d \n",  (raw_temp));
        temp = SHT2x_CalcTemperatureC(raw_temp);
        printf("Temp = %ld.%03d\n", (long)temp, (unsigned)((temp-floor(temp)) * 1000));
        //rh = sht25_humidity();
        //printf("Rel. humidity: %u%%\n",
        //    (unsigned) (SHT2x_CalcRH(rh)));
        etimer_reset(&et);
    }
    PROCESS_END();
}
