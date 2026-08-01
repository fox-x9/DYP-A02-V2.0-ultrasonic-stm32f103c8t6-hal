/**
 ******************************************************************************
 * @file    ultrasonic.h
 * @brief   DYP-A02 超声波传感器驱动 (UART 自动输出模式)
 * @note    帧格式: 0xFF + Data_H + Data_L + SUM (4字节)
 *          距离(mm) = Data_H * 256 + Data_L
 *          校验: SUM = (0xFF + Data_H + Data_L) & 0xFF
 ******************************************************************************
 */

#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/** 超声波数据帧 (DYP-A02 UART自动模式, 4字节) */
typedef struct {
    uint8_t  frame[4];       /* 原始帧缓冲 */
    uint8_t  index;          /* 当前接收字节索引 */
    uint16_t distance_mm;    /* 最新有效距离 (毫米) */
    uint8_t  data_ready;     /* 数据就绪标志, 1=有新数据 */
} Ultrasonic_t;

/* 全局传感器实例 */
extern Ultrasonic_t g_ultra;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  初始化超声波模块, 清零状态
 */
void Ultrasonic_Init(void);

/**
 * @brief  喂入一个字节, 解析DYP-A02数据帧
 * @note   在 HAL_UART_RxCpltCallback 中每收到一个字节调用一次
 * @param  byte: 接收到的字节
 */
void Ultrasonic_ParseByte(uint8_t byte);

/**
 * @brief  获取最新有效距离
 * @return 距离 (毫米), 若超时无新数据返回 0
 */
uint16_t Ultrasonic_GetDistance(void);

#ifdef __cplusplus
}
#endif

#endif /* __ULTRASONIC_H */
