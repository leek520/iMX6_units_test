#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>      /*标准输入输出的定义*/
#include <fcntl.h>      /*文件控制定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <string.h>
#include <errno.h>      /*错误号定义*/
#include <assert.h>
#include <unistd.h>     /*UNIX 标准函数定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <sys/ioctl.h>
#include <sys/types.h>


#include <pthread.h>
#define IMX_GPIO_NR(bank, nr)        (((bank) - 1) * 32 + (nr))

#define MAX_BUF 100

#endif // COMMON_H

