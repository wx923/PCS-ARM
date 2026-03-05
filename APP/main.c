#include "main.h"

int main(void) { 
    xTaskCreate(
        srv_breatheled_task,
        "Breather_LED",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        src_dsp_status_task,
        "Dsp_Status",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        NULL
    );


    vTaskStartScheduler();
    while(1);
 }