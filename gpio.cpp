#include "common.h"

#define GPIO_ROOT "/sys/class/gpio"
#define GPIO_EXPORT    GPIO_ROOT "/export"
#define GPIO_UNEXPORT  GPIO_ROOT "/unexport"
#define GPIO_DIRECTION GPIO_ROOT "/gpio%d/direction"
#define GPIO_ACTIVELOW GPIO_ROOT "/gpio%d/active_low"
#define GPIO_VALUE     GPIO_ROOT "/gpio%d/value"


#define MAX_BUF 64

int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_direction(unsigned int gpio, unsigned int out_flag);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio);

/******************************************************************************
* @brief:gpio_export
* @author:leek
* @date 2018/07/06
*******************************************************************************/
int gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(GPIO_EXPORT, O_WRONLY);
    if (fd < 0)
    {
        return fd;
    }
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

/******************************************************************************
* @brief:gpio_unexport
* @author:leek
* @date 2018/07/06
*******************************************************************************/
int gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(GPIO_UNEXPORT, O_WRONLY);
    if (fd < 0)
    {
        return fd;
    }
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

/******************************************************************************
* @brief:gpio_direction
* @author:leek
* @date 2018/07/06
*******************************************************************************/
int gpio_set_direction(unsigned int gpio, unsigned int out_flag)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), GPIO_DIRECTION, gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        return fd;
    }
    if (out_flag)
        write(fd, "out", 4);
    else
        write(fd, "in", 3);
    close(fd);
    return 0;
}

/******************************************************************************
* @brief:gpio_value
* @author:leek
* @date 2018/07/06
*******************************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), GPIO_VALUE, gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        return fd;
    }
    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);
    close(fd);

    return 0;
}

/******************************************************************************
* @brief:gpio_value
* @author:leek
* @date 2018/07/06
*******************************************************************************/
int gpio_get_value(unsigned int gpio)
{
    int fd;
    char buf[MAX_BUF];
    char value;

    snprintf(buf, sizeof(buf), GPIO_VALUE, gpio);
    fd = open(buf, O_RDONLY);
    if (fd < 0)
    {
        return fd;
    }

    if (read(fd, &value, sizeof(value)) != sizeof(value)) {
        close(fd);
        return -1;
    }
    if (close(fd) == -1)
        return -1;

    return value - '0';
}
