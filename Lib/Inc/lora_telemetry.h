#ifndef LORA_TELEMETRY_H
#define LORA_TELEMETRY_H

#include <stddef.h>
#include <stdint.h>

/**
 * 生成英文遥测串（不含定点 3 字节头，由 lora_ewm22a 组帧）。
 * @return 正文长度；<=0 表示失败
 */
int lora_telemetry_format(char *buf, size_t bufsz, uint32_t wall_anchor_ms, uint32_t race_start_ms,
                          uint32_t now_ms);

#endif
