#include "contiki.h"
#include "dev/sht25.h"
#include <stdio.h>

PROCESS(test_sht25_process, "SHT25 test");
AUTOSTART_PROCESSES(&test_sht25_process);

PROCESS_THREAD(test_sht25_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();
    sht25_init();
    printf("Starting loop\n");
    while(1){
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_YIELD();
        printf("Raw temp =  %u \n", (unsigned) (sht25_temp()));
        //rh = sht25_humidity();
        //printf("Rel. humidity: %u%%\n",
        //    (unsigned) (SHT2x_CalcRH(rh)));
        etimer_reset(&et);
    }
    PROCESS_END();
}
