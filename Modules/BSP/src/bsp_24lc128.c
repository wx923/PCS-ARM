#include "bsp_24lc128.h"

/*
@brief 初始化I2C和WP引脚
*/
void bsp_24lc128_init(void)
{
    /* 使能时钟 */
    rcu_periph_clock_enable(EEPROM_I2C_RCU);
    rcu_periph_clock_enable(RCU_GPIOB);

    /* 先配置WP引脚 - 初始禁止写入 */
    gpio_init(EEPROM_I2C_WP_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, EEPROM_I2C_WP_PIN);
    EEPROM_WP_ENABLE();

    /* 配置I2C引脚 PB8=SCL, PB9=SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, 
              EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN);


    /* 初始化I2C */
    /* 1. 软件复位：先开启复位，再关闭复位（相当于重启I2C模块） */
    i2c_software_reset_config(EEPROM_I2C, I2C_SRESET_SET);
    i2c_software_reset_config(EEPROM_I2C, I2C_SRESET_RESET);

    /* 2. 配置时钟：100kHz, 占空比 2 (标准模式) */
    i2c_clock_config(EEPROM_I2C, 100000, I2C_DTCY_2);

    /* 3. 配置模式与地址：开启I2C模式, 使用7位地址格式, 主机自身地址设为0 */
    i2c_mode_addr_config(EEPROM_I2C, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0);

    /* 4. 使能I2C外设 */
    i2c_enable(EEPROM_I2C);

    /* 允许写入 */
    EEPROM_WP_DISABLE();
}

/*
@brief 写一个字节
@param addr 内存地址(0-16383)
@param data 要写入的数据
@return 0=成功, 1=失败
*/
uint8_t bsp_24lc128_write(uint16_t addr, uint8_t data)
{
    /* 使能写 */
    EEPROM_WP_DISABLE();

    /* 发送起始信号 */
    i2c_start_on_bus(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_SBSEND));

    /* 发送设备地址+写 */
    i2c_master_addressing(EEPROM_I2C, EEPROM_ADDR, I2C_TRANSMITTER);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_ADDSEND));

    /* 发送高字节地址 */
    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr >> 8));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    /* 发送低字节地址 */
    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr & 0xFF));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    /* 发送数据 */
    i2c_data_transmit(EEPROM_I2C, data);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    /* 发送停止信号 */
    i2c_stop_on_bus(EEPROM_I2C);
    while (I2C_CTL0(EEPROM_I2C) & 0x0100);

    /* 禁止写 */
    EEPROM_WP_ENABLE();

    /* 等待内部写完成 */
    for (volatile int i = 0; i < 50000; i++);
    return 0;
}

/*
@brief 读一个字节
@param addr 内存地址
@param data 读取的数据存放指针
@return 0=成功, 1=失败
*/
uint8_t bsp_24lc128_read(uint16_t addr, uint8_t *data)
{
    /* 先写地址 */
    i2c_start_on_bus(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_SBSEND));

    i2c_master_addressing(EEPROM_I2C, EEPROM_ADDR, I2C_TRANSMITTER);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_ADDSEND));

    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr >> 8));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr & 0xFF));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    /* 重新启动 */
    i2c_start_on_bus(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_SBSEND));

    i2c_master_addressing(EEPROM_I2C, EEPROM_ADDR, I2C_RECEIVER);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_ADDSEND));

    /* 禁用ACK */
    i2c_ack_config(EEPROM_I2C, I2C_ACK_DISABLE);

    /* 接收数据 */
    *data = i2c_data_receive(EEPROM_I2C);

    /* 发送停止信号 */
    i2c_stop_on_bus(EEPROM_I2C);
    while (I2C_CTL0(EEPROM_I2C) & 0x0100);

    /* 恢复ACK以便下次使用 */
    i2c_ack_config(EEPROM_I2C, I2C_ACK_ENABLE);

    return 0;
}

/*
@brief 页写(最多64字节)
@param addr 页起始地址(必须64字节对齐)
@param buf 数据缓冲区
@param len 长度(<=64)
@return 0=成功, 1=失败
*/
uint8_t bsp_24lc128_write_page(uint16_t addr, uint8_t *buf, uint8_t len)
{
    if (len > 64) return 1;

    EEPROM_WP_DISABLE();

    i2c_start_on_bus(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_SBSEND));

    i2c_master_addressing(EEPROM_I2C, EEPROM_ADDR, I2C_TRANSMITTER);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_ADDSEND));

    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr >> 8));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr & 0xFF));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    for (uint8_t i = 0; i < len; i++) {
        i2c_data_transmit(EEPROM_I2C, buf[i]);
        while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));
    }

    i2c_stop_on_bus(EEPROM_I2C);
    while (I2C_CTL0(EEPROM_I2C) & 0x0100);

    EEPROM_WP_ENABLE();

    for (volatile int i = 0; i < 50000; i++);
    return 0;
}

/*
@brief 连续读
@param addr 起始地址
@param buf 数据缓冲区
@param len 长度
@return 0=成功
*/
uint8_t bsp_24lc128_read_seq(uint16_t addr, uint8_t *buf, uint16_t len)
{
    /* 发送地址 */
    i2c_start_on_bus(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_SBSEND));

    i2c_master_addressing(EEPROM_I2C, EEPROM_ADDR, I2C_TRANSMITTER);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_ADDSEND));

    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr >> 8));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    i2c_data_transmit(EEPROM_I2C, (uint8_t)(addr & 0xFF));
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_TBE));

    /* 重启并切换为接收 */
    i2c_start_on_bus(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_SBSEND));

    i2c_master_addressing(EEPROM_I2C, EEPROM_ADDR, I2C_RECEIVER);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_ADDSEND));

    /* 读数据 - 前N-1个字节发送ACK */
    for (uint16_t i = 0; i < len - 1; i++) {
        i2c_ack_config(EEPROM_I2C, I2C_ACK_ENABLE);
        buf[i] = i2c_data_receive(EEPROM_I2C);
        while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_RBNE));
    }

    /* 最后一个字节发送NACK */
    i2c_ack_config(EEPROM_I2C, I2C_ACK_DISABLE);
    buf[len - 1] = i2c_data_receive(EEPROM_I2C);
    while (!i2c_flag_get(EEPROM_I2C, I2C_FLAG_RBNE));

    i2c_stop_on_bus(EEPROM_I2C);
    while (I2C_CTL0(EEPROM_I2C) & 0x0100);

    /* 恢复ACK */
    i2c_ack_config(EEPROM_I2C, I2C_ACK_ENABLE);

    return 0;
}
