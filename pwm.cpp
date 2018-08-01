#include "pwm.h"

/******************************************************************************
* @brief:backlight_set_value
* @author:leek
* @date 2018/07/30
*******************************************************************************/
int backlight_set_value(int value)
{
    int fd;

    fd = open(BACKLIGHT_VALUE, O_WRONLY);
    if (fd < 0)
    {
        return fd;
    }
    char str = (char)(value + '0');

    write(fd, &str, 1);

    close(fd);
    return 0;
}

/******************************************************************************
* @brief:backlight_get_max
* @author:leek
* @date 2018/07/30
*******************************************************************************/
int backlight_get_max()
{
    int fd;
    char value;

    fd = open(BACKLIGHT_MAX, O_RDONLY);
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


/* PWM export */
int pwm_export(unsigned int pwm)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), PWM_EXPORT, pwm);

    fd = open(buf, O_WRONLY);

    if (fd < 0)
    {
        return fd;
    }

    write(fd, "0", 2);
    close(fd);
    return 0;
}

/* PWM unexport */
int pwm_unexport(unsigned int pwm)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), PWM_EXPORT, pwm);

    fd = open(buf, O_WRONLY);

    if (fd < 0)
    {
        return fd;
    }

    write(fd, "1", 2);
    close(fd);
    return 0;

}


/* PWM configuration */
int pwm_config(unsigned int pwm, unsigned int period, unsigned int duty_cycle)
{
    int fd,len,len_p,len_d;
    char buf[MAX_BUF];
    char buf_p[MAX_BUF];
    char buf_d[MAX_BUF];

    len = snprintf(buf, sizeof(buf), PWM_PERIOD, pwm);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    len_p = snprintf(buf_p, sizeof(buf_p), "%d", period);
    /* set pwm period */
    write(fd, buf_p, len_p);

    len = snprintf(buf, sizeof(buf), PWM_CYCLE, pwm);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    len_d = snprintf(buf_d, sizeof(buf_d), "%d", duty_cycle);
    write(fd, buf_d, len_d);

    close(fd);
    return 0;
}


/* PWM enable */
int pwm_enable(unsigned int pwm)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), PWM_ENABLE, pwm);

    fd = open(buf, O_WRONLY);

    if (fd < 0)
    {
        return fd;
    }

    write(fd, "1", 2);
    close(fd);
    return 0;
}


/* PWM disable */
int pwm_disable(unsigned int pwm)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), PWM_ENABLE, pwm);

    fd = open(buf, O_WRONLY);

    if (fd < 0)
    {
        return fd;
    }

    write(fd, "0", 2);
    close(fd);
    return 0;
}

