/**
 * EWM22A-400/900BWL22S：USART3 TTL，定点帧由 MCU 组 ADDH+ADDL+CHAN+载荷。
 * 模组 LoRa/工作模式须已用 AT 或上位机配置并掉电保存；本驱动不做 AT 配参、不使用 AUX。
 */
#ifndef LORA_EWM22A_H
#define LORA_EWM22A_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 绑定 main.c 中的 USART3，须在 MX_USART3_UART_Init 之后调用 */
void lora_ewm22a_init(UART_HandleTypeDef *huart);

/** 透明发送（无 3 字节定点头） */
HAL_StatusTypeDef lora_ewm22a_send_it(const uint8_t *data, uint16_t len);

/** 定点发送：目标 16 位地址 + 信道字节 + 载荷（与手册/E22 定点格式一致） */
HAL_StatusTypeDef lora_ewm22a_send_fixed_it(uint16_t dst_addr, uint8_t dst_chan, const uint8_t *payload,
                                           uint16_t payload_len);

#ifdef __cplusplus
}
#endif

#endif
