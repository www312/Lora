/* USER CODE BEGIN Header */

/**

  * @brief  HC-SR04 测距；距离 < DIST_THRESHOLD_CM 时经 EWM22A 定点发往 LORA_DST_ADDR

  */

/* USER CODE END Header */

#include "main.h"



/* USER CODE BEGIN Includes */

#include "app_config.h"

#include "hcsr04.h"

#include "lora_ewm22a.h"

#include "lora_telemetry.h"

/* USER CODE END Includes */



UART_HandleTypeDef huart3;



/* USER CODE BEGIN PV */

static uint32_t s_last_alert_ms;

static uint32_t s_last_sample_ms;

static uint32_t s_wall_anchor_ms;

static uint32_t s_race_start_ms;

static char s_lora_payload[LORA_EWM22A_PAYLOAD_MAX + 1];

/* USER CODE END PV */



void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_USART3_UART_Init(void);



/* USER CODE BEGIN PFP */

static void app_try_lora_alert(void);

/* USER CODE END PFP */



/* USER CODE BEGIN 0 */



static void app_try_lora_alert(void)

{

  const uint32_t now = HAL_GetTick();

  int n;



  if (s_last_alert_ms != 0u && (now - s_last_alert_ms) < LORA_ALERT_COOLDOWN_MS) {

    return;

  }

  if (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {

    return;

  }



  n = lora_telemetry_format(s_lora_payload, sizeof(s_lora_payload), s_wall_anchor_ms, s_race_start_ms, now);

  if (n <= 0) {

    return;

  }



  if (lora_ewm22a_send_fixed_it(LORA_DST_ADDR, LORA_DST_CHAN, (const uint8_t *)s_lora_payload, (uint16_t)n) ==

      HAL_OK) {

    s_last_alert_ms = now;

  }

}



/* USER CODE END 0 */



int main(void)

{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  MX_USART3_UART_Init();



  /* USER CODE BEGIN 2 */

  hcsr04_init();

  lora_ewm22a_init(&huart3);

  s_wall_anchor_ms = HAL_GetTick();

  s_race_start_ms = s_wall_anchor_ms;

  s_last_alert_ms = 0u;

  s_last_sample_ms = s_wall_anchor_ms;

  /* USER CODE END 2 */



  while (1) {

    /* USER CODE BEGIN 3 */

    {

      const uint32_t now = HAL_GetTick();

      if ((now - s_last_sample_ms) >= HCSR04_SAMPLE_PERIOD_MS) {

        const uint16_t dist = hcsr04_read_cm();



        s_last_sample_ms = now;



        if (dist >= HCSR04_RANGE_MIN_CM && dist < DIST_THRESHOLD_CM) {

          app_try_lora_alert();

        }

      }

    }

    /* USER CODE END 3 */

  }

}



void SystemClock_Config(void)

{

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};



  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;

  RCC_OscInitStruct.HSEState = RCC_HSE_ON;

  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;

  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {

    Error_Handler();

  }



  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |

                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;



  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {

    Error_Handler();

  }

}



static void MX_USART3_UART_Init(void)

{

  huart3.Instance = USART3;

  huart3.Init.BaudRate = LORA_EWM22A_BAUD;

  huart3.Init.WordLength = UART_WORDLENGTH_8B;

  huart3.Init.StopBits = UART_STOPBITS_1;

  huart3.Init.Parity = UART_PARITY_NONE;

  huart3.Init.Mode = UART_MODE_TX_RX;

  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;

  huart3.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart3) != HAL_OK) {

    Error_Handler();

  }

}



static void MX_GPIO_Init(void)

{

  GPIO_InitTypeDef GPIO_InitStruct = {0};



  __HAL_RCC_GPIOD_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();



  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);



  GPIO_InitStruct.Pin = GPIO_PIN_0;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



  GPIO_InitStruct.Pin = GPIO_PIN_1;

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}



/* USER CODE BEGIN 4 */



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)

{

  if (huart->Instance == USART3) {

    (void)HAL_UART_Abort_IT(huart);

  }

}



/* USER CODE END 4 */



void Error_Handler(void)

{

  __disable_irq();

  while (1) {

  }

}



#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)

{

  (void)file;

  (void)line;

}

#endif

