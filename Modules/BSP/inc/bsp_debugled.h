#ifndef __BSP_DEBUGLED_H
#define __BSP_DEBUGLED_H

#include "gd32f30x.h"
#include "app.h"
/*定义调试LED端口和引脚*/
#define DEBUGLED_PORT GPIOA
#define DEBUGLED_PIN GPIO_PIN_7

/*led 状态*/
#define DEBUGLED_ON 1
#define DEBUGLED_OFF 0

/*函数声明*/
void bsp_debugled_init(void);
void bsp_debugled_on(void);
void bsp_debugled_off(void);
void bsp_debugled_toggle(void);

#endif
