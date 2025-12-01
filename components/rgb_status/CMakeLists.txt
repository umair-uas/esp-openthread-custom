if(CONFIG_RGB_STATUS_LED_ENABLED)
    idf_component_register(SRCS "rgb_status.c"
                           INCLUDE_DIRS "include"
                           PRIV_REQUIRES espressif__led_strip esp_timer freertos)
else()
    idf_component_register(SRCS "rgb_status_stub.c"
                           INCLUDE_DIRS "include")
endif()
