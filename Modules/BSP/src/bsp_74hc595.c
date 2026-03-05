#include "bsp_74hc595.h"

static void _74hc595_write_bit(uint8_t bit){
    if(bit){
        HC595_SER_HIGH();
    }else{
        HC595_SER_LOW();
    }
    HC595_SRCLK_HIGH();
    HC595_SRCLK_LOW();
}
/*
@brief 初始化74HC595
@param 无
@return 无
@auther wuxiao
*/
void bsp_74hc595_init(void){
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_init(HC595_SER_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, HC595_SER_PIN);
    gpio_init(HC595_SRCLK_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, HC595_SRCLK_PIN);
    gpio_init(HC595_RCLK_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, HC595_RCLK_PIN);
    gpio_init(HC595_OE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, HC595_OE_PIN);

    HC595_SER_LOW();
    HC595_SRCLK_LOW();
    HC595_RCLK_LOW();
    HC595_OE_DISABLE();

    for(int i=0;i<16;i++){
        _74hc595_write_bit(0);
    }

    HC595_RCLK_HIGH();
    for(volatile int i=0;i<5;i++);
    HC595_RCLK_LOW();

    HC595_OE_ENABLE();
}

void bsp_74hc595_send_byte(uint8_t data){
    for(int i=0;i<8;i++){
        _74hc595_write_bit((data>>(7-i)) & 0x01);
    }
}

/*
@brief 发送16位数据到74HC595
@param data 16位数据
@return 无
@auther wuxiao
*/
void bsp_74hc595_send_data(uint16_t data)
{
    HC595_OE_DISABLE();
    bsp_74hc595_send_byte((uint8_t)(data>>8));
    bsp_74hc595_send_byte((uint8_t)(data & 0xFF));

    HC595_RCLK_HIGH();
    for(volatile int i=0;i<5;i++);
    HC595_RCLK_LOW();
    HC595_OE_ENABLE();
}

