#ifndef GPIO_H
#define GPIO_H

#include "common.h"

#define GPIO_ROOT "/sys/class/gpio"
#define GPIO_EXPORT    GPIO_ROOT "/export"
#define GPIO_UNEXPORT  GPIO_ROOT "/unexport"
#define GPIO_DIRECTION GPIO_ROOT "/gpio%d/direction"
#define GPIO_ACTIVELOW GPIO_ROOT "/gpio%d/active_low"
#define GPIO_VALUE     GPIO_ROOT "/gpio%d/value"


#define LED 25


int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_direction(unsigned int gpio, unsigned int out_flag);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio);

#endif // GPIO_H
