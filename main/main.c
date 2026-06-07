#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "nvs_flash.h"
#include "portmacro.h"

#include "esp_now_app.h"
#include "ui_manager.h"
#include "wifi_app.h"

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    EventGroupHandle_t system_events = xEventGroupCreate();
    ui_manager_start(system_events);
    wifi_app_start(system_events);
    esp_now_app_start(system_events);

    // while (1);
}