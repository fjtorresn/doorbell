/*
 * http_client.h
 *
 *  Created on: 21-05-2026
 *      Author: Francisco
 */

#ifndef MAIN_HTTP_CLIENT_H_
#define MAIN_HTTP_CLIENT_H_

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048
// #define HOST_URL
// "https://api.pushover.net/1/messages.json?token=aes6txyqsrr3yru47n63zo7zuguvuf&user=gdb4ptiuwzr8t83nbe84ebwg1rmnh9&title=Timbre&message=Timbre"
#define HOST_URL "http://192.168.1.20:80"

void http_client_start(EventGroupHandle_t system_events);

#endif /* MAIN_HTTP_CLIENT_H_ */