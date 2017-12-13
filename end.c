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
   // gpio_set_value(gpio, 1);

    uart_properties *uart = malloc(sizeof(uart_properties));
    uart->uart_id = uart1;
    uart->baudrate = B9600;
    unsigned char receive[20];
    uint8_t isOpen = uart_open(uart);
    char result[]= {0x3a, 0x01, 0x00, 0x01, 0x01, 0xff, 0x0d, 0x0a};

    if (isOpen == 0) {
      while(1){
	     gpio_set_value(gpio, 1);
          if (uart_send(uart, result, sizeof(result)+1) < 0) {
              printf("Could not send %x to uart port",result);
              return -1;
          }
          usleep(10000);
          gpio_set_value(gpio,0);
          sleep(2);
          memset(receive, 0x00, 20*sizeof(receive[0]));
          if (uart_read(uart, receive, 18) < 0) {
              printf("Could not read data from uart port");
              return -1;
          }
          sleep(5);
        }
      uart_close(uart);
    }
    printf("Done with the loop\n");
    return 0;
  }

