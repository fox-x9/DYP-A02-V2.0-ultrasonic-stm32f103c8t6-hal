# DYP-A02-V2.0 超声波测距 STM32F103C8T6

基于 STM32CubeMX + HAL 库，UART 受控模式驱动 DYP-A02 超声波传感器。

## 硬件

| 项目 | 型号 |
|------|------|
| MCU | STM32F103C8T6 (64KB) |
| 传感器 | DYP-A02-V2.0 |
| 调试器 | ST-LINK V2 |

## 引脚

| MCU | 连接 | 功能 |
|-----|------|------|
| PA9 | 传感器 RX | USART1_TX 触发 |
| PA10 | 传感器 TX | USART1_RX 接收 |
| PA2 | 上位机 RX | USART2_TX 输出 |
| PA3 | 上位机 TX | USART2_RX |
| PC13 | LED | 运行指示 |

## 编译 & 烧录

```powershell
$env:Path = "C:/Users/1/AppData/Local/stm32cube/bundles/cmake/4.0.1+st.3/bin;C:/Users/1/AppData/Local/stm32cube/bundles/ninja/1.13.2+st.1/bin;C:/Users/1/.eide/tools/gcc_arm/bin;" + $env:Path
cmake --preset Release -S . -B build/Release
cmake --build build/Release
arm-none-eabi-objcopy -O ihex build/Release/chaoshenbo.elf build/Release/chaoshenbo.hex
& "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -P "build\Release\chaoshenbo.hex" -V -Rst
```

## 协议

DYP-A02 UART 受控模式，9600 8N1，触发后 45~60ms 返回 4 字节：

| 字节 | 含义 |
|------|------|
| 0 | 帧头 0xFF |
| 1 | Data_H |
| 2 | Data_L |
| 3 | SUM = (0xFF + DH + DL) & 0xFF |

距离(mm) = Data_H × 256 + Data_L
