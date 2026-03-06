#ifndef __SRV_EEPROM_H
#define __SRV_EEPROM_H

#include "app.h"
#include "bsp_24lc128.h"

void srv_eeprom_init(void);

void srv_eeprom_init_with_data(uint16_t count);

/* 读取一个字节 */
uint8_t srv_eeprom_read(uint16_t addr);

/* 写入一个字节 */
void srv_eeprom_write(uint16_t addr, uint8_t data);

/* 读取计数器的当前值 (存放在EEPROM地址0) */
uint8_t srv_eeprom_get_counter(void);

/* 计数器+1 并保存到EEPROM */
void srv_eeprom_increment_counter(void);

/* 重置计数器为0 */
void srv_eeprom_reset_counter(void);

#endif