#include "common.h"

extern int gpio_export(unsigned int gpio);
extern int gpio_unexport(unsigned int gpio);
extern int gpio_set_direction(unsigned int gpio, unsigned int out_flag);
extern int gpio_set_value(unsigned int gpio, unsigned int value);
extern int gpio_get_value(unsigned int gpio);

static void gpio_test()
{
    int led = IMX_GPIO_NR(1,25);
    gpio_export(led);
    gpio_set_direction(led, 1);
    gpio_set_value(led, 1);
    gpio_get_value(led);
}


static void gpio_led_test()
{


}

static void uart_test()
{


}

static void adc_test()
{


}
int main(int argc, char *argv[])
{
    int i;
    /*测试菜单*/

    printf("hello word!\n");
    return 0;
}
