#include "main.h"

int main(void) { 
    xTaskCreate(
        srv_breatheled_task,
        "Breather_LED",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY+3,
        NULL
    );
    vTaskStartScheduler();
    while(1);
 }