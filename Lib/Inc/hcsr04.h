/**
 * HC-SR04 @ PA0(Trig) PA1(Echo)，TIM2 提供 1us 计时。
 * Echo 为 5V 时请用 1k+2k 分压至 3.3V 再进 PA1。
 */
#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void hcsr04_init(void);

/**
 * @return 距离 cm；0 表示超时/无回波/无效
 */
uint16_t hcsr04_read_cm(void);

#ifdef __cplusplus
}
#endif

#endif
