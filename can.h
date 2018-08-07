#ifndef CAN_H
#define CAN_H

#include <net/if.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include<linux/sockios.h>


#include "common.h"

#define CAN_PORT    "can%d"
#define CAN_DOWN    "ifconfig can%d down"
#define CAN_UP      "ip link set can%d up type can bitrate %d"

int can_init(int port, int bitrate);
int can_send(int socket_fd, int can_id, int frame_type, char *send_buf, int len);
int can_receive(int socket_fd, char *rev_buf, int *len);
int set_filter(int socket_fd, int can_id, int frame_type);
void create_can0_task();
void create_can1_task();

#endif // CAN_H
