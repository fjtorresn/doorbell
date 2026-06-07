/**
 * task_common.h
 * 
 * Created on: 05-07-2026
 *      Author: Francisco
 */

#ifndef MAIN_TASKS_COMMON_H_
#define MAIN_TASKS_COMMON_H_

// Wi-Fi app task
#define WIFI_APP_TASK_STACK_SIZE        4096
#define WIFI_APP_TASK_PRIORITY          5
#define WIFI_APP_TASK_CORE_ID           0

// UI manager task
#define UI_MANAGER_TASK_STACK_SIZE      2048
#define UI_MANAGER_TASK_PRIORITY        10
#define UI_MANAGER_TASK_CORE_ID         1

// HTTP client task
#define HTTP_CLIENT_TASK_STACK_SIZE     8192
#define HTTP_CLIENT_TASK_PRIORITY       5
#define HTTP_CLIENT_TASK_CORE_ID        1

// ESP-NOW app task
#define ESPNOW_APP_TASK_STACK_SIZE      2048
#define ESPNOW_APP_TASK_PRIORITY        6
#define ESPNOW_APP_TASK_CORE_ID         0

#define WIFI_CONNECTED_BIT              (1 << 0)
#define RING_PRESSED_BIT                (1 << 1)


 #endif /* MAIN_TASKS_COMMON_H_ */