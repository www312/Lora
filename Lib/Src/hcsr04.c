#include "hcsr04.h"
#include "main.h"
#include "stm32f1xx_hal.h"

#define TRIG_PORT GPIOA
#define TRIG_PIN GPIO_PIN_0
#define ECHO_PORT GPIOA
#define ECHO_PIN GPIO_PIN_1

#define TIM2_US_PSC 71u
#define ECHO_WAIT_HIGH_TIMEOUT_US 30000u
#define ECHO_PULSE_MAX_US 25000u

static void tim2_us_start(void)
{
  __HAL_RCC_TIM2_CLK_ENABLE();
  TIM2->PSC = TIM2_US_PSC;
  TIM2->ARR = 0xFFFFu;
  TIM2->EGR = TIM_EGR_UG;
  TIM2->CNT = 0u;
  TIM2->CR1 = TIM_CR1_CEN;
}

static inline uint16_t tim2_us_now(void)
{
  return (uint16_t)TIM2->CNT;
}

static inline uint16_t tim2_us_elapsed(uint16_t start, uint16_t end)
{
  return (uint16_t)(end - start);
}

static void delay_us_tim2(uint16_t us)
{
  const uint16_t t0 = tim2_us_now();
  while (tim2_us_elapsed(t0, tim2_us_now()) < us) {
  }
}

static GPIO_PinState echo_read(void)
{
  return HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN);
}

void hcsr04_init(void)
{
  tim2_us_start();
  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

static void hcsr04_trig_pulse(void)
{
  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
  delay_us_tim2(2u);
  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
  delay_us_tim2(12u);
  HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

uint16_t hcsr04_read_cm(void)
{
  uint16_t t0;
  uint16_t t1;
  uint32_t pulse_us;

  hcsr04_trig_pulse();

  t0 = tim2_us_now();
  while (echo_read() == GPIO_PIN_RESET) {
    if (tim2_us_elapsed(t0, tim2_us_now()) > ECHO_WAIT_HIGH_TIMEOUT_US) {
      return 0u;
    }
  }

  t0 = tim2_us_now();
  while (echo_read() == GPIO_PIN_SET) {
    if (tim2_us_elapsed(t0, tim2_us_now()) > ECHO_PULSE_MAX_US) {
      return 0u;
    }
  }
  t1 = tim2_us_now();
  pulse_us = tim2_us_elapsed(t0, t1);

  if (pulse_us < 100u) {
    return 0u;
  }

  return (uint16_t)(pulse_us / 58u);
}
