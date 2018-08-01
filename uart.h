#ifndef UART_H
#define UART_H

#include "common.h"

#define TIMEOUT_SEC  15  //接收超时
#define TIMEOUT_USEC 0

#define UART_ROOT "/dev/ttymxc%d"

int uart_open(const char *dev);
int uart_config(int fd, int baudrate, int databit, char *parioy, int stopbit, bool hardflow);
int uart_receive(int fd, char *r_buf, int len);
int uart_send(int fd, char *s_buf, int len);

#endif // UART_H
