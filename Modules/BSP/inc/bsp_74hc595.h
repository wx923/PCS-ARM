#ifndef __BSP_74HC595_H
#define __BSP_74HC595_H

#include "gd32f30x.h"

#define HC595_SER_PORT GPIOB
#define HC595_SER_PIN GPIO_PIN_3

#define HC595_OE_PORT GPIOB
#define HC595_OE_PIN GPIO_PIN_4

#define HC595_RCLK_PORT GPIOB
#define HC595_RCLK_PIN GPIO_PIN_5

#define HC595_SRCLK_PORT GPIOB
#define HC595_SRCLK_PIN GPIO_PIN_6

/*硬件操作宏*/
#define HC595_SER_LOW()    gpio_bit_reset(HC595_SER_PORT, HC595_SER_PIN)
#define HC595_SER_HIGH()   gpio_bit_set(HC595_SER_PORT, HC595_SER_PIN)

#define HC595_SRCLK_LOW()  gpio_bit_reset(HC595_SRCLK_PORT, HC595_SRCLK_PIN)
#define HC595_SRCLK_HIGH() gpio_bit_set(HC595_SRCLK_PORT, HC595_SRCLK_PIN)

#define HC595_RCLK_LOW()   gpio_bit_reset(HC595_RCLK_PORT, HC595_RCLK_PIN)
#define HC595_RCLK_HIGH()  gpio_bit_set(HC595_RCLK_PORT, HC595_RCLK_PIN)

#define HC595_OE_ENABLE()  gpio_bit_reset(HC595_OE_PORT, HC595_OE_PIN)   // 低电平使能
#define HC595_OE_DISABLE() gpio_bit_set(HC595_OE_PORT, HC595_OE_PIN)     // 高电平禁用


/*函数声明*/
void bsp_74hc595_init(void);
void bsp_74hc595_send_byte(uint8_t data);
void bsp_74hc595_send_data(uint16_t data);
#endif