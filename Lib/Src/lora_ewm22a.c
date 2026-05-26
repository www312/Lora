#include "lora_ewm22a.h"
#include "app_config.h"
#include <string.h>

#define LORA_FIXED_HDR_LEN 3u
#define LORA_TX_PAYLOAD_MAX LORA_EWM22A_PAYLOAD_MAX
#define LORA_TX_FRAME_MAX (LORA_FIXED_HDR_LEN + LORA_TX_PAYLOAD_MAX)

static UART_HandleTypeDef *s_huart;
static uint8_t s_tx_frame[LORA_TX_FRAME_MAX];

void lora_ewm22a_init(UART_HandleTypeDef *huart)
{
  s_huart = huart;
}

HAL_StatusTypeDef lora_ewm22a_send_it(const uint8_t *data, uint16_t len)
{
  if (s_huart == NULL || data == NULL || len == 0u) {
    return HAL_ERROR;
  }
  return HAL_UART_Transmit_IT(s_huart, (uint8_t *)data, len);
}

HAL_StatusTypeDef lora_ewm22a_send_fixed_it(uint16_t dst_addr, uint8_t dst_chan, const uint8_t *payload,
                                            uint16_t payload_len)
{
  uint16_t total;

  if (s_huart == NULL || payload == NULL || payload_len == 0u) {
    return HAL_ERROR;
  }
  total = (uint16_t)(LORA_FIXED_HDR_LEN + payload_len);
  if (total > LORA_TX_FRAME_MAX) {
    return HAL_ERROR;
  }

  s_tx_frame[0] = (uint8_t)((dst_addr >> 8) & 0xFFu);
  s_tx_frame[1] = (uint8_t)(dst_addr & 0xFFu);
  s_tx_frame[2] = dst_chan;
  (void)memcpy(&s_tx_frame[LORA_FIXED_HDR_LEN], payload, payload_len);

  return HAL_UART_Transmit_IT(s_huart, s_tx_frame, total);
}
