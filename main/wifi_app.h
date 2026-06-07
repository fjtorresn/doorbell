/*
 * wifi_app.h
 *
 *  Created on: 04-05-2026
 *      Author: Francisco
 */

 #include "esp_netif.h"

 // Wi-Fi configuration parameters
 #define WIFI_STA_POWER_SAVE        WIFI_PS_NONE
 #define MAX_SSID_LENGTH            32
 #define MAX_PASSWORD_LENGTH        64
 #define MAX_CONNECTION_RETRIES     5

 #define EXAMPLE_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
 #define EXAMPLE_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD

 #if CONFIG_ESP_STATION_EXAMPLE_WPA3_SAE_PWE_HUNT_AND_PECK
	#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
	#define EXAMPLE_H2E_IDENTIFIER ""
#elif CONFIG_ESP_STATION_EXAMPLE_WPA3_SAE_PWE_HASH_TO_ELEMENT
	#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
	#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#elif CONFIG_ESP_STATION_EXAMPLE_WPA3_SAE_PWE_BOTH
	#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
	#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#endif

#if CONFIG_ESP_WIFI_AUTH_OPEN
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
	#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif

 // network interface
 extern esp_netif_t* netif_sta;

 /**
  * Message IDs for Wi-Fi application tasks
  */
 typedef enum wifi_app_message
 {
    WIFI_APP_MSG_STA_CONNECTED_GOT_IP = 0
 } wifi_app_message_e;

 /**
  * Structure for the message queue
  */
 typedef struct wifi_app_queue_message
 {
    wifi_app_message_e msg_id;
 } wifi_app_queue_message_t;

 /**
  * Sends a message to the queue
  * @param msg_id The message ID to send
  * @return pdTRUE if the message was sent successfully, pdFALSE otherwise
  */
 BaseType_t wifi_app_send_message(wifi_app_message_e msg_id);

 /**
  * Starts the Wi-Fi RTOS task
  */
 void wifi_app_start(EventGroupHandle_t system_events);