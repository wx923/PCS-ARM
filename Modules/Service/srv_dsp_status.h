#ifndef __SRV_DSP_STATUS_H
#define __SRV_DSP_STATUS_H
#include "app.h"
#include "bsp_74hc595.h"

typedef enum{
    SYS_STATE_RUN = 0,
    SYS_STATE_CHARGE,
    SYS_STATE_DISCHARGE,
    SYS_STATE_STANDBY,
    SYS_STATE_STOP,
    SYS_STATE_FAULT,
}sys_state_t;

typedef struct{
    uint8_t dspdischarge:1;
    uint8_t dspcharge:1;
    uint8_t dsprun:1;
    uint8_t dspfault:1;
} dsp_led_t;



void src_dsp_status_task(void *pvParameters);

#endif