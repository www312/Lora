/**
 * 应用配置：改距离阈值、LoRa 目标地址/信道、串口波特率等只需编辑本文件后重新编译。
 */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ========== 距离触发（单位：厘米）========== */
#define DIST_THRESHOLD_CM 20u

#define HCSR04_RANGE_MIN_CM 2u
#define HCSR04_RANGE_MAX_CM 400u
#define HCSR04_SAMPLE_PERIOD_MS 80u
#define LORA_ALERT_COOLDOWN_MS 5000u

/* ========== EWM22A LoRa（须与模组 AT/寄存器配置一致）========== */
/** USART3 波特率，默认与模组出厂一致；改模组 AT+UART 后请同步修改 */
#define LORA_EWM22A_BAUD 115200u

/** 目标模组 16 位地址（0x0003 → 帧头 ADDH=0x00, ADDL=0x03） */
#define LORA_DST_ADDR 0x0003u
/**
 * 定点帧第 3 字节 = LoRa 信道号 CH（与 AT+CHANNEL 的十进制值相同）。
 * 例：接收端 AT+CHANNEL=30 → 此处写 30u（串口发 0x1E），不是 0x30u。
 * 旧 A39/AS_DS 勾选 HEX 填 30 时才是 0x30u，EWM22A 不要混用。
 */
#define LORA_DST_CHAN 30u

/** 定点模式下载荷最大长度（LoRa 单包 240 字节 − 3 字节头） */
#define LORA_EWM22A_PAYLOAD_MAX 237u

/* ========== 遥测正文 ========== */
#define LORA_TEAM_NUMBER 63
#define CLOCK_BOOT_HOUR 12u
#define CLOCK_BOOT_MIN 0u
#define CLOCK_BOOT_SEC 0u

#endif
