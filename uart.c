#include "uart.h"


int uart_open(uart_properties *uart) {
        FILE *slots;
        char buf[30] = "/dev/ttyO";
        char port_nr[2];
        sprintf(port_nr, "%d", uart->uart_id);
        strcat(buf,port_nr);
        struct termios uart_port;

        slots = fopen(SLOTS, "w");
        if(slots == NULL) printf("slots didn't open\n");
        fseek(slots,0,SEEK_SET);

        fprintf(slots, "BB-UART%i", uart->uart_id+1);
        fflush(slots);
        fclose(slots);

        uart->fd = open(buf, O_RDWR | O_NOCTTY);
        if(uart->fd < 0) printf("port failed to open\n");

        bzero(&uart_port,sizeof(uart_port));

        uart_port.c_cflag = uart->baudrate | CS8 | CLOCAL | CREAD;
        uart_port.c_iflag = IGNPAR | ICRNL;
        uart_port.c_oflag = 0;
        uart_port.c_lflag = 0;

        uart_port.c_cc[VTIME] = 0;
        uart_port.c_cc[VMIN]  = 1;

        //clean the line and set the attributes
        tcflush(uart->fd,TCIFLUSH);
        tcsetattr(uart->fd,TCSANOW,&uart_port);
        return 0;
}

int uart_send(uart_properties *uart, char *tx, int length) {
        if (write(uart->fd, tx, length) == -1) {
                syslog(LOG_ERR, "Could not write %s to uart %i", tx, uart->uart_id);
                return -1;
        }
        syslog(LOG_INFO, "Wrote %s to uart %i", tx, uart->uart_id);
        return 0;
}

int uart_read(uart_properties *uart,unsigned char *rx, int length) {
        int count;
	//usleep(500);
        if( (count = read(uart->fd,(void*)rx,length)) < 0) {
           	printf("Count is %d\n",count);
            return -1;
        }
        printf("Reading_rx %d: %x\n",count,rx);
        for(int i=0;i<count;i++){
                printf("%x ",rx[i]);
        }
        return count;
}

int uart_close(uart_properties *uart) {
        close(uart->fd);
        return 0;
}
