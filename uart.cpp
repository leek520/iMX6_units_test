#include "uart.h"

#include "common.h"

/*
    struct termio
    {
          unsigned short c_iflag; // 输入模式标志
          unsigned short c_oflag; // 输出模式标志
          unsigned short c_cflag; // 控制模式标志
          unsigned short c_lflag; // 本地模式标志
          unsigned char c_line; // line discipline
          unsigned char c_cc[NCC]; // control characters
    };
在这个结构中最为重要的是c_cflag，通过对它的赋值，用户可以设置波特率、字符大小、
数据位、停止位、奇偶校验位和硬件流控等。另外c_iflag 和c_cc 也是比较常用的标志
1.获取和设置终端属性
    int tcgetattr(int fd, struct termios *termios_p);
    int tcsetattr(int fd, int optional_actions, struct termios *termios_p);
    其中optional_actions决定什么时候起作用，可取如下值
        TCSANOW：不等数据传输完毕就立即改变属性。
        TCSADRAIN：等待所有数据传输结束才改变属性。
        TCSAFLUSH：清空输入输出缓冲区才改变属性。
    注意：当进行多重修改时，应当在这个函数之后再次调用 tcgetattr() 来检测是否所有修改都成功实现。
2.波特率函数
    获取波特率
    speed_t cfgetospeed(struct termios *termios_p);
    speed_t cfgetispeed(struct termios *termios_p);
    设置波特率
    int cfsetospeed(struct termios *termios_p, speed_t speed);
    int cfsetispeed(struct termios *termios_p, speed_t speed);
3.线路控制函数
    int tcdrain (int fd);    //等待所有写入fd中的数据输出
    int tcflush (int fd, int queue_selector);    //丢弃要写入fd，但尚未传输的数据，或者收到但是尚未读取的数据。
    取决于queue_selector 的值：
        TCIFLUSH 刷新收到的数据但是不读
        TCOFLUSH 刷新写入的数据但是不传送
        TCIOFLUSH 同时刷新收到的数据但是不读，并且刷新写入的数据但是不传送

    int tcflow (int fd, int action);    //挂起 fd 上的数据传输或接收。
    取决于 action 的值：
        TCOOFF 挂起输出
        TCOON 重新开始被挂起的输出
        TCIOFF 发送一个 STOP 字符，停止终端设备向系统传送数据
        TCION 发送一个 START 字符，使终端设备向系统传输数据
        打开一个终端设备时的默认设置是输入和输出都没有挂起。

5.进程组控制函数
    pid_t tcgetpgrp(int fd);    //获取前台进程组的进程组ID
    int tcsetpgrp(int fd, pid_t pgrpid);    //设置前台进程组的进程组ID
    pid_t tcgetsid(int fd);    //获取会话首进程的进程组ID

6.cfmakeraw
    设置终端的 Raw 模式 ，设置终端属性:
    termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP| INLCR | IGNCR | ICRNL | IXON);
    termios_p->c_oflag &= ~OPOST;
    termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    termios_p->c_cflag &= ~(CSIZE | PARENB);
    termios_p->c_cflag |= CS8;

7.模式
    Cbreak模式
        除了"Del"和"Ctrl"键外，接受其他所有字符输入，数都可以禁止行缓冲(line buffering)。控制字符将被终端驱动程序解释成其它字符。
    Raw模式
        可以禁止行缓冲(line buffering)，处理挂起（CTRLZ）、中断或退出（CTRLC）等控制字符时，将直接传送给程序去处理而不产生终端信号
        如果不是开发串口终端，只是收发数据的话就用RAW模式即可
*/


#define TIMEOUT_SEC  0  //接收超时
#define TIMEOUT_USEC 0
static speed_t baudrate_map(int b)
{
    speed_t retval;

    switch(b)
    {
        case 110:
            retval = B110;
            break;

        case 300:
            retval = B300;
            break;

        case 1200:
            retval = B1200;
            break;

        case 2400:
            retval = B2400;
            break;

        case 4800:
            retval = B4800;
            break;

        case 9600:
            retval = B9600;
            break;

        case 19200:
            retval = B19200;
            break;

        case 38400:
            retval = B38400;
            break;

        case 57600:
            retval = B57600;
            break;

        case 115200:
            retval = B115200;
            break;

#ifdef B230400
        case 230400:
            retval = B230400;
            break;
#endif

#ifdef B460800
        case 460800:
            retval = B460800;
            break;
#endif

#ifdef B500000
        case 500000:
            retval = B500000;
            break;
#endif

#ifdef B576000
        case 576000:
            retval = B576000;
            break;
#endif

#ifdef B921600
        case 921600:
            retval = B921600;
            break;
#endif

#ifdef B1000000
        case 1000000:
            retval = B1000000;
            break;
#endif

#ifdef B1152000
        case 1152000:
            retval = B1152000;
            break;
#endif

#ifdef B1500000
        case 1500000:
            retval = B1500000;
            break;
#endif

#ifdef B2000000
        case 2000000:
            retval = B2000000;
            break;
#endif

#ifdef B2500000
        case 2500000:
            retval = B2500000;
            break;
#endif

#ifdef B3000000
        case 3000000:
            retval = B3000000;
            break;
#endif

#ifdef B3500000
        case 3500000:
            retval = B3500000;
            break;
#endif

#ifdef B4000000
        case 4000000:
            retval = B4000000;
            break;
#endif

        default:
            retval = 0;
            break;
    }

    return(retval);
}

void set_baudrate(struct termios *ti, int baudrate)
{
    cfsetospeed(ti, baudrate_map(baudrate));
    cfsetispeed(ti, baudrate_map(baudrate));
}
void set_hardflow(struct termios *ti, bool flow_control)
{
    if (flow_control)   //进行硬件流控
        ti->c_cflag |= CRTSCTS;
    else                //无硬件流控
        ti->c_cflag &= ~CRTSCTS;

    //ti.c_cflag |= (IXON | IXOFF | IXANY); //软件流控

}
void set_databit(struct termios *ti, int databit)
{
    ti->c_cflag &= ~CSIZE;   //数据位的位掩码，先清空
    switch (databit)
    {
    case 5:
        ti->c_cflag |= CS5;
        break;
    case 6:
        ti->c_cflag |= CS6;
        break;
    case 7:
        ti->c_cflag |= CS7;
        break;
    default:
        ti->c_cflag |= CS8;
        break;
    }
}
void set_parioy(struct termios *ti, char *parioy)
{
    if (*parioy == 'O' | *parioy == 'o')
    {
        ti->c_cflag |= PARENB;
        ti->c_cflag |= PARODD;
        ti->c_iflag |= (INPCK | ISTRIP);
    }
    else if (*parioy == 'E' | *parioy == 'e')
    {
        ti->c_iflag |= (INPCK | ISTRIP);
        ti->c_cflag |= PARENB;
        ti->c_cflag &= ~PARODD;
    }
    else if (*parioy == 'N' | *parioy == 'n')
    {
        ti->c_cflag &= ~PARENB;
    }


}
void set_stopbit(struct termios *ti, int stopbit)
{
   if( stopbit == 1 )
       ti->c_cflag &= ~CSTOPB;
   else if ( stopbit == 2 )
       ti->c_cflag |= CSTOPB;
}

int uart_open(const char *dev)
{
    int fd;
    assert(dev);
    fd = open(dev, O_RDWR |  O_NOCTTY | O_NDELAY); //通知linix系统，这个程序不会成为这个端口的控制终端。
    if (fd < 0) {
        perror("Can't open uart port");
        return -1;
    }
    //设置串口阻塞，因为这里是以非阻塞(FNDELAY)形式打开的，所以第三个参数为0，后面会详细介绍fcntl函数
//    if(fcntl(fd,F_SETFL,0)<0)
//    {
//        perror("fcntl failed!");
//        return -1;
//    }

    return fd;
}

int uart_config(int fd, int baudrate, int databit, char *parioy, int stopbit, bool hardflow=false)
{
    struct termios options;

    if (tcgetattr(fd, &options) != 0)    //取出属性
    {
        perror("tcgetattr failed!");
        return -1;

    }
     /*设置字符大小*/
    options.c_cflag |= (CLOCAL | CREAD);    //通过位掩码的方式激活本地连接和接受使能选项
    //CLOCAL保证程序不占用串口
    //CREAD保证程序可以从串口中读取数据


    /*设置波特率*/
    set_baudrate(&options, baudrate);
    /*设置数据位*/
    set_databit(&options, databit);
    /*设置奇偶校验位*/
    set_parioy(&options, parioy);
    /*设置停止位*/
    set_stopbit(&options, stopbit);
    /*设置硬件流控制*/
    set_hardflow(&options, hardflow);


    options.c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);    //使串口工作在原始模式下
    options.c_oflag &= ~OPOST;      //表示数据经过处理后输出
    options.c_iflag &= ~(ICRNL | INPCK | ISTRIP | IXON | BRKINT );

    /*
     *ICANON：允许规范模式进行输入处理
     *ECHO：允许输入字符的本地回显
     *ECHOE：在接收EPASE时执行Backspace,Space,Backspace组合
     *ISIG：允许信号
     */

    /*设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时*/
    options.c_cc[VMIN] = 1;     //设置最小接受字符为1
    options.c_cc[VTIME] = 0;    //设置等待时间为0

    tcflush(fd ,TCIFLUSH);/*tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */

    /*激活配置使其生效*/
    if (tcsetattr(fd, TCSANOW, &options)<0) {   //立即生效
        return -1;
    }
    return fd;
}

/*
 * 安全写函数
 */

ssize_t safe_write(int fd, char *vptr, size_t n)
{
    size_t  nleft;
    ssize_t nwritten;
    char *ptr;

    ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
    if((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if(nwritten < 0&&errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/*
 * 安全读函数
 */
ssize_t safe_read(int fd,char *vptr,size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr=vptr;
    nleft=n;

    while(nleft > 0)
    {
        if((nread = read(fd,ptr,nleft)) < 0)
        {
            if(errno == EINTR)//被信号中断
                nread = 0;
            else
                return -1;
        }
        else
        if(nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return (n-nleft);
}

int uart_receive(int fd, char *r_buf, int len)
{
    fd_set rfds;
    struct timeval time;
    int ret;
    int cnt = 0;

    /*将读文件描述符加入描述符集合*/
    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);

    /*设置超时为15s*/
    time.tv_sec = TIMEOUT_SEC;
    time.tv_usec = TIMEOUT_USEC;

    /*实现多路IO*/

    ret = select(fd+1, &rfds ,NULL, NULL, &time);

    switch (ret) {
    case -1:
        fprintf(stderr,"select error!\n");
        return -1;
    case 0:
        fprintf(stderr, "time over!\n");
        return -1;
    default:
        cnt = read(fd, r_buf, len);
        if(cnt == -1)
        {
            fprintf(stderr, "safe read failed!\n");
            return -1;
        }
        return cnt;
    }
}
int uart_send(int fd, char *s_buf, int len)

{

    int ret = 0;
    ret = write(fd, s_buf, len);  //实际写入的长度

    if(ret == len){
        return ret;
    }
    else
    {
        tcflush(fd, TCOFLUSH);
        return -1;
    }

}
int uart_close(int fd)
{
    assert(fd);
    close(fd);
    return 0;
}

