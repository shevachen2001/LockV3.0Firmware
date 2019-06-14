#include "Lora\uart.h"


#define MAX_TEST_DATA_BYTES     (64U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 64                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 16                           /**< UART RX buffer size. */

uint8_t   uartBuff[64]={0};  
uint16_t  uartLenx =0;  
uint8_t   uartNodat=0;

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void uart_event_handle(app_uart_evt_t * p_event)  
{
    if (p_event->evt_type == APP_UART_DATA_READY)  
    {  
        app_uart_get(&uartBuff[uartLenx++]); 
			  uartNodat=0;
		}
}


void uartSendstring(uint8_t *p)
{
	 while(*p)
	 {
		  app_uart_put(*p++);
	 }
}


void uart_init(void)
{
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
    {
         RX_PIN_NUMBER,
         TX_PIN_NUMBER,
         RTS_PIN_NUMBER,
         CTS_PIN_NUMBER,
         APP_UART_FLOW_CONTROL_DISABLED,
         false,
         UART_BAUDRATE_BAUDRATE_Baud115200
    };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
			                   uart_event_handle,
                         //uart_error_handle,
                         APP_IRQ_PRIORITY_LOW,
                         err_code);

    APP_ERROR_CHECK(err_code);
}

