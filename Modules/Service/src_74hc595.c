#include "src_74hc595.h"

void srv_74hc595_task(void *pvParameters){
    (void)pvParameters;
    bsp_74hc595_init();

    while(1){
        bsp_74hc595_send_data(0xFFFF);
    }
}

