/*
 * wifi_app.c
 *
 *  Created on: 04-05-2026
 *      Author: Francisco
 */

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "lwip/netdb.h"

#include "tasks_common.h"
#include "wifi_app.h"

// TAG used for ESP logs
static const char TAG [] = "wifi_app";

static EventGroupHandle_t local_system_events = NULL;

// Queue handle for WiFi application messages
static QueueHandle_t wifi_app_queue_handle;

// Network interface for station mode
esp_netif_t* esp_netif_sta = NULL;

/**
 * Wifi application handler
 * @param arg data that is passed to the handler when is called
 * @param event_base the base of the event that triggered the handler
 * @param event_id the ID of the event that triggered the handler
 * @param event_data data associated with the event that triggered the handler
 */
static void wifi_app_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "WiFi started, connecting to AP...");
                int ret = esp_wifi_connect();
                ESP_LOGI(TAG, "Termino el connect: %d", ret);
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGW(TAG, "WiFi disconnected, retrying...");
                esp_wifi_connect();
                xEventGroupClearBits(local_system_events, WIFI_CONNECTED_BIT);
                break;

            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "WiFi connected to AP");
                break;

            case WIFI_EVENT_HOME_CHANNEL_CHANGE:
                ESP_LOGI(TAG, "Home channel change event received");
                break;

            default:
                ESP_LOGW(TAG, "Unhandled WiFi event: %d", event_id);
                break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(TAG, "Got IP address");
                wifi_app_send_message(WIFI_APP_MSG_STA_CONNECTED_GOT_IP);
                xEventGroupSetBits(local_system_events, WIFI_CONNECTED_BIT);
                break;

            default:
                ESP_LOGW(TAG, "Unhandled IP event: %d", event_id);
                break;
        }
    }
     else
    {
        ESP_LOGW(TAG, "Unhandled event base: %s", event_base);
    }
}

/**
 * Initialize TCP/IP stack and WiFi configuration
 */
static void wifi_app_default_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    wifi_config_t wifi_config = {
		.sta =
			{
				.ssid = EXAMPLE_ESP_WIFI_SSID,
				.password = EXAMPLE_ESP_WIFI_PASS,
				.threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
				.sae_pwe_h2e = ESP_WIFI_SAE_MODE,
				.sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
#ifdef CONFIG_ESP_WIFI_WPA3_COMPATIBLE_SUPPORT
				.disable_wpa3_compatible_mode = 0,
#endif
			},
	};

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    esp_netif_sta = esp_netif_create_default_wifi_sta();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
}

/**
 * Initialize the WiFi application event handler
 */
static void wifi_app_event_handler_init(void)
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_app_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_app_event_handler, NULL, NULL));
}

/**
 * Main task for the WiFi application
 * @param pvParameters Parameters passed to the task (not used)
 */
static void wifi_app_task(void* pvParameters)
{
    wifi_app_queue_message_t msg;

    wifi_app_event_handler_init();

    wifi_app_default_init();

    ESP_ERROR_CHECK(esp_wifi_start());

    for (;;)
    {
        if (xQueueReceive(wifi_app_queue_handle, &msg, portMAX_DELAY) == pdTRUE)
        {
            switch (msg.msg_id)
            {
                case WIFI_APP_MSG_STA_CONNECTED_GOT_IP:
                    ESP_LOGI(TAG, "Station connected and got IP address");
                    break;

                default:
                    ESP_LOGW(TAG, "Received unknown message ID: %d", msg.msg_id);
                    break;
            }
        }
    }
}

BaseType_t wifi_app_send_message(wifi_app_message_e msg_id)
{
    wifi_app_queue_message_t msg = {
        .msg_id = msg_id
    };
    return xQueueSend(wifi_app_queue_handle, &msg, portMAX_DELAY);
}

void wifi_app_start(EventGroupHandle_t system_events)
{
    ESP_LOGI(TAG, "Starting WiFi application...");

    local_system_events = system_events;
    wifi_app_queue_handle = xQueueCreate(3, sizeof(wifi_app_message_e));

    xTaskCreatePinnedToCore(&wifi_app_task, "wifi_app_task", WIFI_APP_TASK_STACK_SIZE, NULL, WIFI_APP_TASK_PRIORITY, NULL, WIFI_APP_TASK_CORE_ID);
}