/*
 * esp_now_app.h
 *
 *  Created on: 28-05-2026
 *      Author: Francisco
 */
#ifndef ESP_NOW_APP_H
#define ESP_NOW_APP_H

#include "esp_now.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define ESPNOW_MAXDELAY 512
#define ESPNOW_QUEUE_SIZE 6
#define WIFI_CHANNEL 0
#define ESPNOW_DATA_SIZE 4

typedef struct {
	uint8_t dest_mac[6];
	uint8_t payload[ESPNOW_DATA_SIZE]; // Real payload of ESPNOW data.
	int len;
} __attribute__((packed)) espnow_data_t;

typedef struct {
	uint8_t mac_addr[ESP_NOW_ETH_ALEN];
	esp_now_send_status_t status;
} espnow_send_event_t;

void esp_now_app_start(EventGroupHandle_t system_events);

#endif // ESP_NOW_APP_H