#include "adc.h"

int adc_get_value(unsigned int adc_in)
{
    int fd;
    char buf[MAX_BUF];
    char str[10];
    int value;

    snprintf(buf, sizeof(buf), ADC_VALUE, adc_in);
    //fprintf(stderr, "%s\n", buf);
    fd = open(buf, O_RDONLY);
    if (fd < 0)
    {
        return fd;
    }

    while(read(fd,str,sizeof(str))>0);
    if (close(fd) == -1)
        return -1;


    //字符数组转int
    sscanf(str,"%d",&value);

    return value;
}
