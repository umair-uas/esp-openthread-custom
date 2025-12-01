#include "rgb_status.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#ifndef CONFIG_RGB_STATUS_LED_GPIO
#define CONFIG_RGB_STATUS_LED_GPIO 8
#endif

static const char *TAG = "rgb_status";
static led_strip_handle_t s_strip = NULL;
static TaskHandle_t s_task = NULL;
static volatile rgb_status_pattern_t s_pattern = RGB_STATUS_OFF;

static void strip_init(void)
{
    if (s_strip) return;
    led_strip_config_t strip_config = {
        .strip_gpio_num = CONFIG_RGB_STATUS_LED_GPIO,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };
    if (led_strip_new_rmt_device(&strip_config, &rmt_config, &s_strip) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init LED strip");
        s_strip = NULL;
        return;
    }
    led_strip_clear(s_strip);
}

static void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    if (!s_strip) return;
    led_strip_set_pixel(s_strip, 0, r, g, b);
    led_strip_refresh(s_strip);
}

static void task_fn(void *arg)
{
    (void)arg;
    strip_init();
    uint32_t t = 0;
    for (;;) {
        rgb_status_pattern_t p = s_pattern;
        switch (p) {
        case RGB_STATUS_OFF:
            set_rgb(0,0,0);
            vTaskDelay(pdMS_TO_TICKS(200));
            break;
        case RGB_STATUS_BOOTING: {
            // Quick cyan blink 2Hz
            bool on = ((t / 250) % 2) == 0;
            set_rgb(on ? 0 : 0, on ? 60 : 0, on ? 60 : 0);
            vTaskDelay(pdMS_TO_TICKS(50));
            t += 50;
            break; }
        case RGB_STATUS_READY:
            set_rgb(0, 80, 0);
            vTaskDelay(pdMS_TO_TICKS(500));
            break;
        case RGB_STATUS_ACTIVE: {
            // Slow blue heartbeat (fade in/out)
            uint8_t phase = (t / 50) % 40; // 2s cycle
            uint8_t val = phase < 20 ? (phase * 5) : ((39 - phase) * 5);
            set_rgb(0, 0, val);
            vTaskDelay(pdMS_TO_TICKS(50));
            t += 50;
            break; }
        case RGB_STATUS_ERROR: {
            bool on = ((t / 150) % 2) == 0;
            set_rgb(on ? 80 : 0, 0, 0);
            vTaskDelay(pdMS_TO_TICKS(50));
            t += 50;
            break; }
        }
    }
}

void rgb_status_init(void)
{
    if (s_task) return;
    xTaskCreate(task_fn, "rgb_status", 2048, NULL, 4, &s_task);
}

void rgb_status_set(rgb_status_pattern_t pattern)
{
    s_pattern = pattern;
}
