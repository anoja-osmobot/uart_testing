#ifndef CORE_H_
#define CORE_H_

#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/wait.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define SLOTS	"/sys/devices/platform/bone_capemgr/slots"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

typedef struct {
	int id;
	char *board_name;
	char *part_number;
	char *version;
	char *manufacturer;
	char *file_name;
} overlay;


extern int load_device_tree_overlay(overlay* ol);
extern int init_bbc_lib();
#endif /* CORE_H_ */

