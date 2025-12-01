idf_component_register(SRCS "esp_ot_rcp.c"
                       PRIV_REQUIRES esp_coex esp_event nvs_flash openthread rgb_status
                       INCLUDE_DIRS ".")
