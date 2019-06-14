#ifndef _UART_H
#define _UART_H

#include "app_uart.h"
#include "app_error.h"
#include "bsp.h"

void uartSendstring(uint8_t *p);
void uart_event_handle(app_uart_evt_t * p_event); 
void uart_init(void);
void uart_error_handle(app_uart_evt_t * p_event);

#endif

