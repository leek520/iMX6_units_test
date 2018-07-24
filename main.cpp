#include "common.h"

extern int gpio_export(unsigned int gpio);
extern int gpio_unexport(unsigned int gpio);
extern int gpio_set_direction(unsigned int gpio, unsigned int out_flag);
extern int gpio_set_value(unsigned int gpio, unsigned int value);
extern int gpio_get_value(unsigned int gpio);


void split(char str[], const char delims[], int *out, int *len)
{
    char *result = NULL;
    int cnt = 0;
    result = strtok( str, delims );
    while( result != NULL ) {
        //printf("%s\n",result);
        *(out++) = atoi(result);
        result = strtok( NULL, delims );
        cnt++;
    }
    *len = cnt;
}

static void gpio_test()
{
    //获取用户输入，如果为ESC，则退出该函数
    char str[20];
    int para[20];
    int len;
    /*1.*/
    gets(str);
    /*2.*/
    split(str, " ", para, &len);
    while (1) // Danger！
    {
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
                { gpio_test, "GPIO test" } ,
                { 0, 0}
            };

/******************************************************************************
* @brief:Main
* @author:leek
* @date 2018/07/24
*******************************************************************************/
int main(void)
{
    int i, idx;
    char str[20];
    printf("<***************************************>\n");
    printf("               iMX6 Test Program\n");
    printf("<***************************************>\n");

    /*测试菜单*/
    while(1)
    {
        printf("\nPlease select function : \n");
        for(i=0; CmdTip[i].fun!=0; i++)
            printf("%d : %s\n", i, CmdTip[i].tip);

        gets(str);
        idx = atoi(str);
        if(idx<i)
        {
            (*CmdTip[idx].fun)();
            sleep(20);
        }
    }
}
