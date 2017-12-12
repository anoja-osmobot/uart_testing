#include "uart.h"
int main() {

	printf("!!! Make sure you have enabled UART1 (/dev/ttyO1) see the README.md how to do this. !!!\n");

	init_bbc_lib();
	uart_properties *uart = malloc(sizeof(uart_properties));
	uart->uart_id = uart1;
	uart->baudrate = B9600;

	uint8_t isOpen = uart_open(uart);
	int i = 0;

	if (isOpen == 0) {
		unsigned char receive[100];
		while(i++ < 10)
		{
			char buf[30];
			sprintf(buf, "foo %d", i);

			// Send data to uart1
			if (uart_send(uart, buf, strlen(buf) + 1) < 0) {
				printf("Could not send data to uart port");
				return -1;
			}

			usleep(100000);

			// Read data from uart1
			if (uart_read(uart, receive, 100) < 0) {
				printf("Could not read data from uart port");
				return -1;
			}

			printf("Read: %s\n",receive);
			usleep(50000);
		}
		uart_close(uart);
	}

	syslog(LOG_INFO, "%s", "Finished pwm example.");
	return 0;
}

