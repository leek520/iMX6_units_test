#ifndef GPIO_H
#define GPIO_H

#include "common.h"

#define GPIO_ROOT "/sys/class/gpio"
#define GPIO_EXPORT    GPIO_ROOT "/export"
#define GPIO_UNEXPORT  GPIO_ROOT "/unexport"
#define GPIO_DIRECTION GPIO_ROOT "/gpio%d/direction"
#define GPIO_ACTIVELOW GPIO_ROOT "/gpio%d/active_low"
#define GPIO_VALUE     GPIO_ROOT "/gpio%d/value"
#define GPIO_EDGE      GPIO_ROOT "/gpio%d/edge"


#define IN           "in"
#define OUT          "out"
#define RISING_EDGE  "rising"
#define FALLING_EDGE "falling"
#define BOTH_EDGE    "both"

#define LED 25


int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_direction(unsigned int gpio, const char *dir);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio);
int gpio_set_edge(unsigned int gpio, const char *edge);
int gpio_fd_open(unsigned int gpio, unsigned int dir);

#endif // GPIO_H
