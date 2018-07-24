#include "common.h"

extern int gpio_export(unsigned int gpio);
extern int gpio_unexport(unsigned int gpio);
extern int gpio_set_direction(unsigned int gpio, unsigned int out_flag);
extern int gpio_set_value(unsigned int gpio, unsigned int value);
extern int gpio_get_value(unsigned int gpio);

static void gpio_test()
{
    //获取用户输入，如果为ESC，则退出该函数
    while(1) {
        int led = IMX_GPIO_NR(1,25);
        gpio_export(led);
        gpio_set_direction(led, 1);
        gpio_set_value(led, 1);
        gpio_get_value(led);
    }
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
static void ethernet_test()
{


}

void Temp_function()
{
    printf("\nPlease input 1-11 to select test!!!\n");
}


struct {
    void (*fun)(void);
    char *tip;
}CmdTip[] = {
                { Temp_function, "Please input 1-11 to select test" } ,
                { gpio_test, "Please input 1-11 to select test" } ,
                { 0, 0}
            };

/******************************************************************************
* @brief:Main
* @author:leek
* @date 2018/07/24
*******************************************************************************/
int Main(void)
{
    int i, idx;

    printf("<***************************************>\n");
    printf("               iMX6 Test Program\n");
    printf("<***************************************>\n");

    /*测试菜单*/
    while(1)
    {
        printf("\nPlease select function : \n");
        for(i=0; CmdTip[i].fun!=0; i++)
            printf("%d : %s\n", i, CmdTip[i].tip);

        scanf("%s", idx) ;
        if(idx<i)
        {
            (*CmdTip[idx].fun)();
            sleep(20);
        }
    }
}
