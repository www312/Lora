#include "lora_telemetry.h"
#include "app_config.h"
#include <stdio.h>

int lora_telemetry_format(char *buf, size_t bufsz, uint32_t wall_anchor_ms, uint32_t race_start_ms,
                          uint32_t now_ms)
{
  uint32_t elapsed_sec;
  uint32_t sec_of_day;
  unsigned wh;
  unsigned wm;
  unsigned ws;
  uint32_t race_ms;
  unsigned rm;
  unsigned rs;
  int n;

  if (buf == NULL || bufsz == 0u) {
    return 0;
  }

  elapsed_sec = (now_ms - wall_anchor_ms) / 1000u;
  sec_of_day = (CLOCK_BOOT_HOUR * 3600u + CLOCK_BOOT_MIN * 60u + CLOCK_BOOT_SEC + elapsed_sec) % 86400u;
  wh = (unsigned)(sec_of_day / 3600u);
  wm = (unsigned)((sec_of_day % 3600u) / 60u);
  ws = (unsigned)(sec_of_day % 60u);
  race_ms = now_ms - race_start_ms;
  rm = (unsigned)(race_ms / 60000u);
  rs = (unsigned)((race_ms / 1000u) % 60u);

  n = snprintf(buf, bufsz,
               "Current time %02u:%02u:%02u, Team number %d, Name Baba booey, Race duration %u:%02u\r\n",
               wh, wm, ws, LORA_TEAM_NUMBER, rm, rs);
  if (n < 0) {
    return 0;
  }
  if ((size_t)n >= bufsz) {
    n = (int)bufsz - 1;
  }
  return n;
}
