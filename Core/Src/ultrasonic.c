/**
 ******************************************************************************
 * @file    ultrasonic.c
 * @brief   DYP-A02 超声波传感器驱动实现
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ultrasonic.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/

/** 全局超声波传感器实例 */
Ultrasonic_t g_ultra;

/* -------------------------------------------------------------------------- */

/**
 * @brief  初始化超声波模块, 清零状态
 */
void Ultrasonic_Init(void)
{
    memset(&g_ultra, 0, sizeof(g_ultra));
}

/**
 * @brief  喂入一个字节, 解析DYP-A02 4字节数据帧
 * @note   帧格式: 0xFF + Data_H + Data_L + SUM
 *         在 HAL_UART_RxCpltCallback 中每收到一个字节调用一次
 * @param  byte: 接收到的字节
 */
void Ultrasonic_ParseByte(uint8_t byte)
{
    /* ---- 帧头检测 ---- */
    if (g_ultra.index == 0) {
        if (byte == 0xFF) {
            g_ultra.frame[0] = byte;
            g_ultra.index = 1;
        }
        /* 不是帧头则丢弃, 继续等待 */
    }
    /* ---- 数据字节 ---- */
    else {
        g_ultra.frame[g_ultra.index] = byte;
        g_ultra.index++;

        /* 收满4字节, 进行校验 */
        if (g_ultra.index >= 4) {
            g_ultra.index = 0;

            uint8_t sum = (uint8_t)(0xFF + g_ultra.frame[1] + g_ultra.frame[2]);
            if (sum == g_ultra.frame[3]) {
                /* 校验通过 */
                g_ultra.distance_mm = ((uint16_t)g_ultra.frame[1] << 8) | g_ultra.frame[2];
                g_ultra.data_ready = 1;
            }
            /* 校验失败则丢弃, data_ready保持0 */
        }
    }
}

/**
 * @brief  获取最新有效距离
 * @return 距离(毫米), 读取后清除就绪标志
 */
uint16_t Ultrasonic_GetDistance(void)
{
    g_ultra.data_ready = 0;
    return g_ultra.distance_mm;
}
