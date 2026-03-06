#include "srv_dsp_status.h"

/* 段码表: 0-9 */
static const uint8_t seg_code[10] = {
    0x3F, // 0: 0111111
    0x06, // 1: 0000110
    0x5B, // 2: 1011011
    0x4F, // 3: 1001111
    0x66, // 4: 1100110
    0x6D, // 5: 1101101
    0x7D, // 6: 1111101
    0x07, // 7: 0000111
    0x7F, // 8: 1111111
    0x6F  // 9: 1101111
};

/* 动态扫描计数器: 0=个位, 1=十位, 2=LED */
static uint8_t dsp_scan_cnt = 0;

/* LED闪烁计数器 */
static uint8_t run_led_flash_cnt = 0;

/*
 * @brief 构建显示数据
 * @note bit分布:
 *       bit15: dspfault
 *       bit14: dsprun
 *       bit13: dspcharge
 *       bit12: dspdischarge
 *       bit11: DP
 *       bit10: G
 *       bit9:  F
 *       bit8:  E
 *       bit7:  D
 *       bit6:  C
 *       bit5:  B
 *       bit4:  A
 *       bit3:  个位位选 (1:选中)
 *       bit2:  十位位选 (1:选中)
 *       bit1:  不用
 *       bit0:  不用
 */
static uint16_t dsp_build_display(uint8_t digit, uint8_t value, dsp_led_t led) {
    uint16_t data = 0;
    
    /* 段码 */
    if (value < 10) {
        data |= (seg_code[value]<<4);
    }
    
    /* 位选: digit=0选十位, digit=1选个位 */
    if (digit == 0) {
        data |= (1 << 2);  // 十位
    } else {
        data |= (1 << 3);  // 个位
    }
    
    /* LED状态 */
    data |= (led.dspfault << 15);
    data |= (led.dsprun << 14);
    data |= (led.dspcharge << 13);
    data |= (led.dspdischarge << 12);
    
    return data;
}

/* 构建黑屏数据 */
static uint16_t dsp_build_blank(dsp_led_t led) {
    uint16_t data = 0;
    data |= (led.dspfault << 15);
    data |= (led.dsprun << 14);
    data |= (led.dspcharge << 13);
    data |= (led.dspdischarge << 12);
    return data;
}

/* 只显示LED */
static uint16_t dsp_build_led_only(dsp_led_t led) {
    return dsp_build_blank(led);
}

/* 获取LED状态 */
static dsp_led_t get_led_status(sys_state_t state) {
    dsp_led_t led = {0};
    
    switch (state) {
        case SYS_STATE_FAULT:
            led.dspfault = 1;
            break;
        case SYS_STATE_STOP:
            break;
        case SYS_STATE_STANDBY:
            led.dsprun = 1;
            break;
        case SYS_STATE_CHARGE:
            led.dspcharge = 1;
            break;
        case SYS_STATE_DISCHARGE:
            led.dspdischarge = 1;
            break;
        case SYS_STATE_RUN:
        default:
            led.dsprun = 1;
            break;
    }
    return led;
}

void src_dsp_status_task(void *pvParameters) {
    (void)pvParameters;
    sys_state_t state;
    dsp_led_t led;
    uint8_t value, tens, ones;
    uint16_t data;
    
    bsp_74hc595_init();
    
    while (1) {
        /* 1. 获取状态和显示值 */
        state = 1;
        value =  srv_eeprom_get_counter();
        if (value > 99) value = 99;
        
        tens = value / 10;
        ones = value % 10;
        
        /* 2. 获取LED状态 */
        led = get_led_status(state);
        
        /* 充电/放电状态: 运行灯闪烁 */
        if (state == SYS_STATE_CHARGE || state == SYS_STATE_DISCHARGE) {
            led.dsprun = (run_led_flash_cnt < 100) ? 1 : 0;
            run_led_flash_cnt++;
            if (run_led_flash_cnt >= 200) run_led_flash_cnt = 0;
        }
        
        /* 3. 动态扫描: 个位→十位→LED */
        switch (dsp_scan_cnt) {
            case 0:  /* 个位 */
                data = (state == SYS_STATE_STOP) ? dsp_build_blank(led) 
                                                  : dsp_build_display(1, ones, led);
                break;
            case 1:  /* 十位 */
                data = (state == SYS_STATE_STOP) ? dsp_build_blank(led) 
                                                  : dsp_build_display(0, tens, led);
                break;
            case 2:  /* LED */
                data = (state == SYS_STATE_STOP) ? dsp_build_blank(led) 
                                                  : dsp_build_led_only(led);
                break;
            default:
                data = 0;
                break;
        }
        
        /* 计数器循环 */
        dsp_scan_cnt++;
        if (dsp_scan_cnt >= 3) dsp_scan_cnt = 0;
        
        /* 4. 发送数据 */
        bsp_74hc595_send_data(data);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}