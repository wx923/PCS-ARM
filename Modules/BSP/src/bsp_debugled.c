#include "bsp_debugled.h"

void bsp_debugled_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(DEBUGLED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DEBUGLED_PIN);
    gpio_bit_write(DEBUGLED_PORT, DEBUGLED_PIN, DEBUGLED_OFF);
}

void bsp_debugled_on(void){
    gpio_bit_reset(DEBUGLED_PORT, DEBUGLED_PIN);
}
void bsp_debugled_off(void){
    gpio_bit_set(DEBUGLED_PORT, DEBUGLED_PIN);
}
void bsp_debugled_toggle(void){
    bit_status status = gpio_output_bit_get(DEBUGLED_PORT, DEBUGLED_PIN);
    if(status == SET){
        gpio_bit_reset(DEBUGLED_PORT, DEBUGLED_PIN);
    }else{
        gpio_bit_set(DEBUGLED_PORT, DEBUGLED_PIN);
    }
}