#include "uart.h"
#include "StateArrays.h"
//from RSComms.c
#define MSG_PREPAD 4 // This is the number of bytes coming before the content of a message
#define MAX_CONTENTLEN 128 // This is the longest message that we are thinking of sending for now (can contain entire state array)
#define MSG_POSTPAD 3 // This is the number of bytes coming after the content of a message
#define TX_LENGTH MSG_PREPAD + MAX_CONTENTLEN + MSG_POSTPAD // Assuming same for nodes/hubs, if different can move into the #ifdef
#define RX_LENGTH TX_LENGTH // Assuming same for nodes/hubs, if different can move into the #ifdef
#define CMD_SENDSTATE 1 // Return the entire state
#define CMD_SENDREADINGS 2 // Return just the time-dependent variables
#define CMD_READREGISTERS 3 // Return bytes corresponding to the supplied list of addresses
#define CMD_WRITEREGISTERS 4 // Set bytes corresponding to the supplied list of addresses and values sent in (address, value) pairs
unsigned int RSMsgLen = 135;
//from Osmobot6main.c
unsigned char rec_buffer[STATELEN] = {0};

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
void printDeviceReading(unsigned char *buf){
    printf("\n");
    printf("_FIRMVERS:%d.%d\n",rec_buffer[_FIRMVERS+1],rec_buffer[_FIRMVERS]);
    printf("_HARDVERS:%d.%d\n",rec_buffer[_HARDVERS+1],rec_buffer[_HARDVERS]);
    printf("_NODEID:%d|%d|%d\n",rec_buffer[_NODEID+2],rec_buffer[_NODEID+1],rec_buffer[_NODEID]);
    printf("_NODEADD:%d\n",rec_buffer[_NODEADD]);
    printf("_SLIDEID:%d+4\n",rec_buffer[_SLIDEID]);
    printf("_ADCAVG:%d\n",rec_buffer[_ADCAVG]);
    printf("_LSAVG:%d\n",rec_buffer[_LSAVG]);
    printf("_AMBCORRECT:%d\n",rec_buffer[_AMBCORRECT]);
    printf("_LSINTSTEPS:%d\n",rec_buffer[_LSINTSTEPS]);
    printf("_LSGAINS:%d\n",rec_buffer[_LSGAINS]);

    printf("_UPTIME:%d\n",rec_buffer[PERSISTLEN + 0]);
    printf("_ADC0:%d %d\n",rec_buffer[PERSISTLEN + 5],rec_buffer[PERSISTLEN + 4]);
    printf("_ADC1:%d %d\n",rec_buffer[PERSISTLEN + 7],rec_buffer[PERSISTLEN + 6]);
    printf("_ADC2:%d %d\n",rec_buffer[PERSISTLEN + 9],rec_buffer[PERSISTLEN + 8]);
    printf("_ADC3:%d %d\n",rec_buffer[PERSISTLEN + 11],rec_buffer[PERSISTLEN + 10]);
    printf("_ADC4:%d\n",rec_buffer[PERSISTLEN + 12]);
    printf("_ADC5:%d\n",rec_buffer[PERSISTLEN + 14]);
    printf("_ADC6:%d\n",rec_buffer[PERSISTLEN + 16]);
    printf("_ADC7:%d\n",rec_buffer[PERSISTLEN + 18]);
    printf("_LS0:%d %d\n",rec_buffer[PERSISTLEN + 27],rec_buffer[PERSISTLEN + 26],rec_buffer[PERSISTLEN + 25],rec_buffer[PERSISTLEN + 24],rec_buffer[PERSISTLEN + 23],rec_buffer[PERSISTLEN + 22],rec_buffer[PERSISTLEN + 21],rec_buffer[PERSISTLEN + 20]);
    printf("_LS2:%d %d\n",rec_buffer[PERSISTLEN + 35],rec_buffer[PERSISTLEN + 34],rec_buffer[PERSISTLEN + 33],rec_buffer[PERSISTLEN + 32],rec_buffer[PERSISTLEN + 31],rec_buffer[PERSISTLEN + 30],rec_buffer[PERSISTLEN + 29],rec_buffer[PERSISTLEN + 28]);
    printf("_LS3:%d\n",rec_buffer[PERSISTLEN + 43],rec_buffer[PERSISTLEN + 42],rec_buffer[PERSISTLEN + 41],rec_buffer[PERSISTLEN + 40],rec_buffer[PERSISTLEN + 39],rec_buffer[PERSISTLEN + 38],rec_buffer[PERSISTLEN + 37],rec_buffer[PERSISTLEN + 36]);
    printf("_LS4:%d\n",rec_buffer[PERSISTLEN + 42]);
    printf("_LS5:%d\n",rec_buffer[PERSISTLEN + 50]);
    printf("_LS6:%d\n",rec_buffer[PERSISTLEN + 58]);
    printf("_LS7:%d\n",rec_buffer[PERSISTLEN + 64]);
    printf("_LS8:%d\n",rec_buffer[PERSISTLEN + 72]);


    printf("_CRC: %d\n",rec_buffer[MSG_PREPAD+STATELEN]);//4+128
    printf("_CR: %d\n",rec_buffer[MSG_PREPAD+STATELEN+1]);//4+128+1
    printf("_LF:%d\n",rec_buffer[MSG_PREPAD+STATELEN+2]);//4+128+2
}

void processRxRSMessage( unsigned char *receive)
{
    memset( &rec_buffer, 0x00, sizeof(rec_buffer));
    memcpy( &rec_buffer, &receive[MSG_PREPAD], RSMsgLen);
    switch( receive[3] ){
        case CMD_SENDSTATE:
              // sendToServer(rec_buffer, sizeof(rec_buffer)); // Forward whole state array to the server
            printDeviceReading(rec_buffer);
            break;

        default:
            break;
    }

}


int uart_read(uart_properties *uart,unsigned char *rx, int length) {
        int count;
        if( (count = read(uart->fd,(void*)rx,length)) < 0) {
           	printf("Count is %d\n",count);
            return -1;
        }
        printf("Reading_rx %d: %x\n",count,rx);
        /*for(int i=0;i<length;i++){
                printf("%x ",rx[i]);
        }*/
        while(*rx==0x3a) rx++;
        processRxRSMessage(rx);
        return count;
}

int uart_close(uart_properties *uart) {
        close(uart->fd);
        return 0;
}

