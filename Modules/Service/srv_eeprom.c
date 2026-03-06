#include "srv_eeprom.h"
#include "bsp_24lc128.h"

#define EEPROM_COUNTER_ADDR    0

static uint8_t eeprom_initialized = 0;

/*
@brief 初始化EEPROM (只初始化一次)
*/
void srv_eeprom_init(void)
{
    if (eeprom_initialized) return;
    
    bsp_24lc128_init();
    eeprom_initialized = 1;
}

/*
@brief 初始化并写入初始值
*/
void srv_eeprom_init_with_data(uint16_t count)
{
    bsp_24lc128_init();
    
    /* 写入初始值: 0, 1, 2, 3, ... */
    for (uint16_t i = 0; i < count; i++) {
        bsp_24lc128_write(i, (uint8_t)i);
    }
    
    eeprom_initialized = 1;
}

/*
@brief 读取一个字节
*/
uint8_t srv_eeprom_read(uint16_t addr)
{
    uint8_t data = 0;
    bsp_24lc128_read(addr, &data);
    return data;
}

/*
@brief 写入一个字节
*/
void srv_eeprom_write(uint16_t addr, uint8_t data)
{
    bsp_24lc128_write(addr, data);
}

/*
@brief 获取计数器当前值
*/
uint8_t srv_eeprom_get_counter(void)
{
    return srv_eeprom_read(EEPROM_COUNTER_ADDR);
}

/*
@brief 计数器+1 并保存
*/
void srv_eeprom_increment_counter(void)
{
    uint8_t cnt = srv_eeprom_get_counter();
    cnt++;
    srv_eeprom_write(EEPROM_COUNTER_ADDR, cnt);
}

/*
@brief 重置计数器
*/
void srv_eeprom_reset_counter(void)
{
    srv_eeprom_write(EEPROM_COUNTER_ADDR, 0);
}