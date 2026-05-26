# Lora 工程改动说明 — A39 → EWM22A

## 改动概要

将 LoRa 模组从 **A39-T900（M0/M1 + 9600）** 迁移为 **EWM22A-400/900BWL22S（UART 115200，无 AUX）**。超声波触发逻辑与遥测正文格式保持不变。

## 新增文件

| 文件 | 说明 |
|------|------|
| `Lib/Inc/lora_ewm22a.h` | EWM22A 驱动头文件 |
| `Lib/Src/lora_ewm22a.c` | 定点组帧 + `HAL_UART_Transmit_IT` 发送 |
| `README_接线说明.md` | 当前引脚接线（本文件配套） |
| `README_改动说明.md` | 本改动说明 |

## 删除文件

| 文件 | 说明 |
|------|------|
| `Lib/Inc/lora_a39.h` | 原 A39 驱动 |
| `Lib/Src/lora_a39.c` | 原 A39 M0/M1 休眠唤醒逻辑 |

## 修改文件

| 文件 | 主要变更 |
|------|----------|
| `Lib/Inc/app_config.h` | `LORA_EWM22A_BAUD`、`LORA_EWM22A_PAYLOAD_MAX`；移除 `LORA_A39_WAKE_SETTLE_MS` |
| `Core/Src/main.c` | 使用 `lora_ewm22a_*`；USART3 波特率来自 `LORA_EWM22A_BAUD`；移除 M0/M1 GPIO 与 sleep/wake 回调 |
| `Core/Inc/main.h` | 删除 `M0_Pin` / `M1_Pin` 定义 |
| `Lib/Inc/lora_telemetry.h` | 注释更新（业务未改） |

未改：`hcsr04.*`、`lora_telemetry.c`、`CMakeLists.txt`（仍通过 `Lib/Src/*.c` 自动收录新驱动）。

## 行为对比

| 项目 | 改前（A39） | 改后（EWM22A） |
|------|-------------|----------------|
| 串口波特率 | 9600 | **115200**（`app_config.h` 可改） |
| 模式引脚 | M0/M1 休眠与唤醒 | **无** |
| AUX | 无 | **不使用** |
| 定点帧 | MCU 发 3 字节头 + 载荷 | **相同** |
| 触发条件 | 距离 < 阈值 | **相同** |
| 冷却时间 | 5 s | **相同** |
| 运行时 AT 配参 | 无 | **无**（假定模组已配置） |

## 使用注意

1. **改波特率**：同时改模组 `AT+UART`（或寄存器）与 `LORA_EWM22A_BAUD`，重启/保存后一致。
2. **改目标地址/信道**：改 `LORA_DST_ADDR`、`LORA_DST_CHAN`，并与接收端 `AT+ADDR` / `AT+CHANNEL` 一致（信道写 **十进制信道号**，不是 A39 AS_DS 的 HEX 字节习惯）。
3. **工作模式**：运行时模组应为 UART→LoRa 透传类模式（如 `AT+HMODE=1`），且 `AT+TRANS=1`；勿保持在 `HMODE=0` 配置模式下发业务数据。
4. 旧文档 `接线说明.txt`、`LoRa配置与排错.txt` 针对 A39/AS_DS，**以 `README_接线说明.md` 为准**。

## 编译

在工程目录执行原有 CMake/CLion 构建即可；确保 `lora_a39.c` 已从树中移除（已删除）。
