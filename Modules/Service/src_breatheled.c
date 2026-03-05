#include "srv_breatheled.h"
#include "app.h"

void srv_breatheled_task(void *pvParameters){
    (void)pvParameters;
    bsp_debugled_init();

    uint16_t duty=0;
    int8_t direction=1;

    while(1){
        bsp_debugled_on();
        vTaskDelay(pdMS_TO_TICKS(duty));

        bsp_debugled_off();
        vTaskDelay(pdMS_TO_TICKS(100-duty));

        duty+=direction;
        if(duty>=100){
            direction=-1;
        }else if(duty<=0){
            direction=1;
        }
    }
}