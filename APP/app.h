#ifndef __APP_H
#define __APP_H
/* ==================== 芯片和内核相关 ==================== */
#include "gd32f30x.h"          // GD32F30x芯片定义
#include "FreeRTOS.h"          // FreeRTOS核心
#include "task.h"              // 任务管理

/* ==================== 项目公共头文件 ==================== */
#include "bsp_debugled.h"      // BSP层公共接口

/* ==================== 服务层头文件 ==================== */
#include "srv_breatheled.h"    // 呼吸灯服务

#endif