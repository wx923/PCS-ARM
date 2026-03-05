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
        srv_74hc595_task,
        "74HC595_Test",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        NULL
    );


    vTaskStartScheduler();
    while(1);
 }