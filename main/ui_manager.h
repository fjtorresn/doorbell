/*
 * ui_manager.h
 *
 *  Created on: 03-05-2026
 *      Author: Francisco
 */

#ifndef MAIN_UI_MANAGER_H_
#define MAIN_UI_MANAGER_H_

#define BLINK_OUTPUT_GPIO		25
#define LED_TIME                5000

#define BUTTON_INPUT_GPIO		18
#define DEBOUNCE_TIME_MS		300

#define ESP_INTR_FLAG_DEFAULT 	0

void ui_manager_start(EventGroupHandle_t system_events);


#endif /* MAIN_UI_MANAGER_H_ */
