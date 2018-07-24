#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


#define IMX_GPIO_NR(bank, nr)        (((bank) - 1) * 32 + (nr))

#endif // COMMON_H
