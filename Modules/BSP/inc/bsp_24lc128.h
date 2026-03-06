#ifndef __BSP_24LC128_H
#define __BSP_24LC128_H
#include "gd32f30x.h"

#define EEPROM_I2C            I2C0
#define EEPROM_I2C_RCU        RCU_I2C0

#define EEPROM_I2C_SCL_PORT   GPIOB
#define EEPROM_I2C_SCL_PIN    GPIO_PIN_8

#define EEPROM_I2C_SDA_PORT   GPIOB
#define EEPROM_I2C_SDA_PIN    GPIO_PIN_9

#define EEPROM_I2C_WP_PORT    GPIOB
#define EEPROM_I2C_WP_PIN     GPIO_PIN_10

/* WP 宏 - 低电平写使能，高电平禁止写 */
#define EEPROM_WP_ENABLE()    gpio_bit_set(EEPROM_I2C_WP_PORT, EEPROM_I2C_WP_PIN)
#define EEPROM_WP_DISABLE()   gpio_bit_reset(EEPROM_I2C_WP_PORT, EEPROM_I2C_WP_PIN)

/* 24LC128 地址 (A0,A1,A2 都接地) */
#define EEPROM_ADDR            0x50

void bsp_24lc128_init(void);
uint8_t bsp_24lc128_read(uint16_t addr, uint8_t *data);
uint8_t bsp_24lc128_write(uint16_t addr, uint8_t data);
uint8_t bsp_24lc128_write_page(uint16_t addr, uint8_t *buf, uint8_t len);
uint8_t bsp_24lc128_read_seq(uint16_t addr, uint8_t *buf, uint16_t len);

#endif