#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include "gpio.h"
#define LEDGPIO 68;
/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B9600   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO1" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

main()
{
    init_bbc_lib();
    gpio_properties *gpio = malloc(sizeof(gpio_properties));
    gpio->nr = LEDGPIO;
    gpio->direction = OUTPUT_PIN;
    int isOpen_ = gpio_open(gpio);
    gpio_set_value(gpio, 0);
    gpio_set_value(gpio, 1);

    int fd, c, res;
    struct termios oldtio, newtio;
    char buf[255];
    // Load the pin configuration
    int ret = system("echo BB-UART1 > /sys/devices/platform/bone_capemgr/slots");
    /* Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C. */
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;

    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    // NMEA command to ouput all sentences
    // Note that this code & format values in manual are hexadecimal
    char result[8];
    result[0] = 0x3a; // This number indicates the start of the message to the host.
    result[1] = 0x01; // Should unclude Destination Address here
    result[2] = 0x00; // Local Address
    result[3] = 0x01; // Returns the request that the hub sent
    result[4] = 0x01;
    result[5] = 0xFF;
    result[6] = 0X0d; // CR for end of the message
    result[7] = 0x0a; // LF for end of the message*/

    write(fd, result, 8);
    /* terminal settings done, now handle input*/
    gpio_set_value(gpio, 0);
    while (TRUE) {     /* loop continuously */
        /*  read blocks program execution until a line terminating character is
            input, even if more than 255 chars are input. If the number
            of characters read is smaller than the number of chars available,
            subsequent reads will return the remaining chars. res will be set
            to the actual number of characters actually read */
        res = read(fd, buf, 135);
        buf[res] = 0;             /* set end of string, so we can printf */
        printf("%s", buf, res);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
}

