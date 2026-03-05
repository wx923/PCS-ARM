#include "srv_breatheled.h"
#include "app.h"

void srv_breatheled_task(void *pvParameters){
    (void)pvParameters;
    bsp_debugled_init();


    while(1){
        bsp_debugled_on();
        vTaskDelay(1000);

        bsp_debugled_off();
        vTaskDelay(1000);
    }
}