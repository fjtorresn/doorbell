/*
 * esp_now_app.c
 *
 *  Created on: 28-05-2026
 *      Author: Francisco
 */

#include <string.h>
#include "esp_now_app.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "tasks_common.h"

static const char *TAG = "esp_now_app";

static EventGroupHandle_t local_system_events = NULL;

static uint8_t s_bell_mac[ESP_NOW_ETH_ALEN] = {0x10, 0x52, 0x1c, 0xa4, 0xcc, 0xc0};
static uint8_t s_example_payload[4] = {'h', 'o', 'l', 'a'};


static void espnow_send_cb(const esp_now_send_info_t *tx_info, esp_now_send_status_t status)
{
	if (tx_info == NULL) {
		ESP_LOGE(TAG, "Send cb arg error");
		return;
	}
	ESP_LOGI(TAG, "Send data to " MACSTR ", status1: %d", MAC2STR(tx_info->des_addr), status);
}

static void espnow_task(void *arg)
{
	esp_err_t err;

	espnow_data_t *data = malloc(sizeof(espnow_data_t));
	if (data == NULL) {
		ESP_LOGE(TAG, "Malloc send parameter fail");
		esp_now_deinit();
		return;
	}

	memset(data, 0, sizeof(espnow_data_t));
	memcpy(data->dest_mac, s_bell_mac, ESP_NOW_ETH_ALEN);
	memcpy(data->payload, s_example_payload, ESPNOW_DATA_SIZE);
	data->len = ESPNOW_DATA_SIZE;

	while (1) {

		xEventGroupWaitBits(local_system_events, RING_PRESSED_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
		err = esp_now_send(data->dest_mac, data->payload, data->len);
		if (err != ESP_OK) {
			ESP_LOGE(TAG, "Send error: %s", esp_err_to_name(err));
			// example_espnow_deinit(send_param);
			//vTaskDelete(NULL);
		}
	}
}

static esp_err_t esp_now_app_init(void)
{
	ESP_ERROR_CHECK(esp_now_init());
	ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_cb));
	/* Set primary master key. */
	// ESP_ERROR_CHECK( esp_now_set_pmk((uint8_t *)CONFIG_ESPNOW_PMK) );

	/* Add broadcast peer information to peer list. */
	esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
	if (peer == NULL) {
		ESP_LOGE(TAG, "Malloc peer information fail");
		esp_now_deinit();
		return ESP_FAIL;
	}
	memset(peer, 0, sizeof(esp_now_peer_info_t));
	peer->channel = 0;
	peer->ifidx = WIFI_IF_STA;
	peer->encrypt = false;
	memcpy(peer->peer_addr, s_bell_mac, ESP_NOW_ETH_ALEN);
	ESP_ERROR_CHECK(esp_now_add_peer(peer));
	free(peer);
	return ESP_OK;
}

void esp_now_app_start(EventGroupHandle_t system_events)
{
	local_system_events = system_events;

	xEventGroupWaitBits(local_system_events, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

	esp_now_app_init();

	xTaskCreatePinnedToCore(&espnow_task, "espnow_task", ESPNOW_APP_TASK_STACK_SIZE, NULL, ESPNOW_APP_TASK_PRIORITY, NULL, ESPNOW_APP_TASK_CORE_ID);
}