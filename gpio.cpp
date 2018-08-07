#include "gpio.h"

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
int gpio_set_direction(unsigned int gpio, const char *dir)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), GPIO_DIRECTION, gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        return fd;
    }

    write(fd, dir, strlen(dir)+1);

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

/******************************************************************************
* @brief:gpio_set_edge
* @author:leek
* @date 2018/08/02
*******************************************************************************/
int gpio_set_edge(unsigned int gpio, const char *edge)
{
    int fd, len;
    char buf[MAX_BUF];


    len = snprintf(buf, sizeof(buf), GPIO_EDGE, gpio);


    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        printf ("Failed set GPIO-%d edge\n", gpio);
        return fd;
    }

    write(fd, edge, strlen(edge) + 1);
    close(fd);

    return 0;
}
/******************************************************************************
* @brief:gpio_fd_open
* @author:leek
* @date 2018/08/02
*******************************************************************************/
int gpio_fd_open(unsigned int gpio, unsigned int dir)
{
    int fd, len;
    char buf[MAX_BUF];


    len = snprintf(buf, sizeof(buf), GPIO_VALUE, gpio);


    fd = open(buf, dir | O_NONBLOCK );

    if (fd < 0) {
        perror("gpio/fd_open");
    }
    return fd;
}
/* gpio fd close */
int gpio_fd_close(int fd)
{
    return close(fd);
}

/******************************************************************************
* @brief:gpio_interrup_task
* @author:leek
* @date 2018/08/02
*******************************************************************************/
void signal_handler(int sig)
{
    printf( "signal_handler!\n" );

}

/******************************************************************************
* @brief:gpio_interrup_task
* @author:leek
* @date 2018/08/02
*******************************************************************************/
void * gpio_interrup_task(void *arg)
{
    int gpio_inter = 25;

    int gpio_fd;

    signal(SIGINT, signal_handler);
    /*GPIO-34, SODIMM-135, Input, Interrupt*/
    gpio_export(gpio_inter);
    gpio_set_direction(gpio_inter, IN);
    gpio_set_edge(gpio_inter, RISING_EDGE);

    gpio_fd = gpio_fd_open(gpio_inter, O_RDONLY);

    while(1) {


    }
}


/******************************************************************************
* @brief:create_gpio_interrup_task
* @author:leek
* @date 2018/08/02
*******************************************************************************/
int create_gpio_interrup_task()
{
    int ret;
    pthread_t pid;
    pthread_attr_t attr;

    ret = pthread_attr_init(&attr);         //初始化线程属性结构
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   //设置attr结构为分离
    //创建线程，返回线程号给pid1,线程属性设置为attr的属性，线程函数入口为task1，参数为NULL

    ret = pthread_create(&pid, &attr, gpio_interrup_task, NULL);

    if (ret != 0)
        return -1;

    pthread_attr_destroy(&attr);

}
