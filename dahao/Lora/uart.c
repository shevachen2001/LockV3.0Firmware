#include "Lora\uart.h"


#define MAX_TEST_DATA_BYTES     (64U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 64                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 16                           /**< UART RX buffer size. */

#define STRING_END_CHARACTER      '\0'

#define CHAR_CARRIAGE_RETURN      '\r'
#define CHAR_NEW_LINE             '\n'

#define NUMBER_SYSTEM_BINARY      2
#define NUMBER_SYSTEM_DECIMAL     10
#define NUMBER_SYSTEM_HEXADECIMAL 16

#define PRINT_BUFFER_SIZE         32U

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

/****************************************************************************************************
**Function: convertToBase
**Author: RahulR
**Description: This function is used to convert an integer into specified type in the argument.
**Input: 
**  1. uint32_t num => Number to be converted
**  2. int32_t base => Type of base (binary, decimal or hexadecimal)
**Output: None
****************************************************************************************************/
static uint8_t *convertToBase(uint32_t num, int32_t base) 
{
    static uint8_t numberSys[]= "0123456789ABCDEF";
    static uint8_t buffer[PRINT_BUFFER_SIZE + 1]; // 1 added for holding \0 character
    uint8_t *ptr = NULL;

    ptr = &buffer[PRINT_BUFFER_SIZE];
    *ptr = STRING_END_CHARACTER;

    do
    {
        *--ptr = numberSys[num % base];
        num /= base;
    }while(num != 0);

    return(ptr);
}

/****************************************************************************************************
**Function: consoleLog
**Author: RahulR
**Description: This function is used to print on serial console.
**Input: Variable list of arguments
**Output: None
****************************************************************************************************/
void consoleLog(uint8_t* format, ...) 
{ 
    uint8_t *traverse; 
    uint32_t integer; 
    uint8_t *string = NULL; 
	
    // Initializing consoleLog's arguments 
    va_list arg; 
    va_start(arg, format);
	
    for(traverse = format; *traverse != '\0'; traverse++) 
    { 
        while(*traverse != '%') 
        {
            app_uart_put(*traverse);
            if(*traverse != STRING_END_CHARACTER)
            {
                traverse++;
            }
        }

        if(*traverse != STRING_END_CHARACTER)
        {
            traverse++; 
        }
        // Fetching and executing arguments
        switch(*traverse) 
        {
            // Fetch char argument
            case 'c': integer = va_arg(arg, int32_t);
                app_uart_put(integer);
                break; 
            // Fetch unsigned decimal argument
            case 'u': integer = va_arg(arg, uint32_t);
                uartSendstring(convertToBase(integer, NUMBER_SYSTEM_DECIMAL));
                break;
            // Fetch signed decimal argument
            case 'd': integer = va_arg(arg, int32_t);
                uartSendstring(convertToBase(integer, NUMBER_SYSTEM_DECIMAL));
                break;
            // Fetch binary representation		
            case 'b': integer = va_arg(arg, uint32_t);
						    uartSendstring(convertToBase(integer, NUMBER_SYSTEM_BINARY));
						    break;
            // Fetch string argument
            case 's': string = va_arg(arg, uint8_t *);
                uartSendstring(string); 
                break;
            // Fetch hexadecimal representation			
            case 'x': integer = va_arg(arg, uint32_t);
                uartSendstring(convertToBase(integer, NUMBER_SYSTEM_HEXADECIMAL));
                break;
        }
    }
	
    // Closing argument list to necessary clean-up
    va_end(arg);

    // Send carriage return and new line characters
    app_uart_put(CHAR_CARRIAGE_RETURN);
    app_uart_put(CHAR_NEW_LINE);
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

