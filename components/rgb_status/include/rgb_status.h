#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RGB_STATUS_OFF = 0,
    RGB_STATUS_BOOTING,     // quick cyan blink
    RGB_STATUS_READY,       // steady green
    RGB_STATUS_ACTIVE,      // slow blue heartbeat
    RGB_STATUS_ERROR,       // fast red blink
} rgb_status_pattern_t;

void rgb_status_init(void);
void rgb_status_set(rgb_status_pattern_t pattern);

#ifdef __cplusplus
}
#endif
