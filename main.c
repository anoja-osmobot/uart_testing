#include "uart.h"
#include "gpio.h"
#define LEDGPIO 68;

int main() {
    init_bbc_lib();
    gpio_properties *gpio = malloc(sizeof(gpio_properties));
    gpio->nr = LEDGPIO;
    gpio->direction = OUTPUT_PIN;
    int isOpen_ = gpio_open(gpio);
    gpio_set_value(gpio, 0);
    gpio_set_value(gpio, 1);

    uart_properties *uart = malloc(sizeof(uart_properties));
    uart->uart_id = uart1;
    uart->baudrate = B9600;
    unsigned char receive[200];
    uint8_t isOpen = uart_open(uart);

    if (isOpen == 0) {
      char result[]= {0x3a, 0x01, 0x00, 0x01, 0x01, 0xff, 0x0d, 0x0a};
    /*  result[0] = 0x3a; // This number indicates the start of the message to the host.
      result[1] = 0x01; // Should unclude Destination Address here
      result[2] = 0x00; // Local Address
      result[3] = 0x01; // Returns the request that the hub sent
      result[4] = 0x01;
      result[5] = 0xFF;
      result[6] = 0X0d; // CR for end of the message
      result[7] = 0x0a; // LF for end of the message*/
     // printf("%d",sizeof(result));

      if (uart_send(uart, result, sizeof(result)+1) < 0) {
          printf("Could not send %x to uart port",result);
          return -1;
      }
     // printf("Send %x to uart port\n",result);
	usleep(10000);
//	uart_close(uart);
	gpio_set_value(gpio,0);
//	usleep(10000);
//	isOpen = uart_open(uart);
//	if(isOpen == 0){      
	//printf("\nSent\n");
      //sleep(5);
     sleep(2);
	memset(receive, 0x00, 200*sizeof(receive[0]));
      if (uart_read(uart, receive, 140) < 0) {
          printf("Could not read data from uart port");
          return -1;
      }
      //printf("Read*****\n%s\n*******",receive);
    // gpio_set_value(gpio, 1);
    uart_close(uart);
    }
        printf("Done with the loop\n");
//}
    return 0;
}

