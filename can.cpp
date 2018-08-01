#include "can.h"

/*
baudrate = (can_baudrate_t*)&ifr.ifr_ifru;
*baudrate = new_baudrate;
ret = ioctl(can_fd,SIOCSCANBAUDRATE, &ifr);
if (ret) {
    perror("ioctl:SIOCSCANBAUDRATE");
    gotoabort;
}
*/
/*
标准帧:就使用 can_id 的低 11 位；
扩展帧:就使用 0～28 位。
can_id 的第 29、30、31 位是帧的标志位，用来定义帧的类型
// special address description flags for the CAN_ID
#define CAN_EFF_FLAG 0x80000000U // EFF/SFF is set in the MSB
#define CAN_RTR_FLAG 0x40000000U // remote transmission request
#define CAN_ERR_FLAG 0x20000000U // error message frame

// valid bits in CAN ID for frame formats
#define CAN_SFF_MASK 0x000007FFU // standard frame format (SFF)
#define CAN_EFF_MASK 0x1FFFFFFFU // extended frame format (EFF)
#define CAN_ERR_MASK 0x1FFFFFFFU // omit EFF, RTR, ERR flags

struct can_filter rfilter[2];
rfilter[0].can_id = 0x123;
rfilter[0].can_mask = CAN_SFF_MASK; //#define CAN_SFF_MASK 0x000007FFU
rfilter[1].can_id = 0x200;
rfilter[1].can_mask = 0x700;
setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));//设置规则


setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0); //禁用过滤规则

通过错误掩码可以实现对错误帧的过滤，例如：
can_err_mask_t err_mask = ( CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF );
setsockopt(s, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, err_mask, sizeof(err_mask));

*/


void set_bitrate(int port, int bitrate)
{
    char buf_d[MAX_BUF], buf_u[MAX_BUF];
    //先关闭can，才能进行配置
    snprintf(buf_d, sizeof(buf_d), CAN_DOWN, port);
    system(buf_d);

    //配置比特率并打开can设备
    snprintf(buf_u, sizeof(buf_u), CAN_UP, port, bitrate);
    system(buf_u);
}
int set_filter(int socket_fd, int can_id, int frame_type)
{
    int s = socket_fd;
    int ret;
    struct can_filter filter;

    filter.can_id = can_id | frame_type;
    filter.can_mask = CAN_SFF_MASK;

    ret = setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER,&filter, sizeof(filter));
    if (ret < 0) {
        perror("setsockopt failed");
        return -1;
    }
    return 0;
}
int can_init(int port, int bitrate)
{
    char buf[5];
    int s, ret;
    struct sockaddr_can addr;
    struct ifreq ifr;

    //1.配置can
    set_bitrate(port, bitrate);
    //2.创建socket监听设备
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);//创建 SocketCAN 套接字
    if (s < 0) {
        perror("socket PF_CAN failed");
        return -1;
    }

    snprintf(buf, sizeof(buf), CAN_PORT, port);

    strcpy(ifr.ifr_name, buf);
    ret = ioctl(s, SIOCGIFINDEX, &ifr);//指定 can0 设备
    if (ret < 0) {
        perror("ioctl failed");
        return -1;
    }
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(s, (struct sockaddr *)&addr, sizeof(addr)); //将套接字与 can 绑定
    if (ret < 0) {
        perror("bind failed");
        close(s);
        return -1;
    }

    return s;
}

int can_receive(int socket_fd, char *rev_buf, int *len)
{

    fd_set rfds;
    struct timeval time;
    int ret;

    int s = socket_fd;
    int nbytes;
    struct can_frame frame;
    memset(frame.data, 0, CAN_MAX_DLEN);
    /*将读文件描述符加入描述符集合*/
    FD_ZERO(&rfds);
    FD_SET(s,&rfds);

    /*设置超时为1s*/
    time.tv_sec = 0;
    time.tv_usec = 0;

    /*实现多路IO*/

    ret = select(s+1, &rfds ,NULL, NULL, &time);

    switch (ret) {
    case -1:
        fprintf(stderr,"select error!\n");
        return -1;
    case 0:
        //fprintf(stderr, "time over!\n");
        return -1;
    default:
        nbytes = read(s, &frame, sizeof(frame)); //接收报文
        //显示报文
        if(nbytes < 0)
        {
            printf("Receive Error frame\n");
            return -1;
        }

//        if ((frame.can_id & 0x00FEF700) != 0x00FEF700)
//            return -1;
        printf("Rec: fd=%d id=0x%X dlc=%d\n", socket_fd, frame.can_id,frame.can_dlc);
        memcpy(rev_buf, frame.data, frame.can_dlc);

        *len = frame.can_dlc;

        return nbytes;
    }

}

int can_send(int socket_fd, int can_id, int frame_type, char *send_buf, int len)
{
    int s = socket_fd;
    int nbytes;
    struct can_frame frame;
    memset(frame.data, 0, CAN_MAX_DLEN);
    //生成报文
    frame.can_id = can_id | frame_type;
    frame.can_dlc = len;
    memcpy(frame.data, send_buf, len);
    //发送报文

    nbytes = write(s, &frame, sizeof(frame)); //发送frame
    if(nbytes != sizeof(frame))
    {
        printf("Send Error frame\n");
        return -1; //发送错误，退出
    }else{
        printf("Send: fd=%d id=0x%X dlc=%d\n", socket_fd, frame.can_id,frame.can_dlc);
    }
    return nbytes;
}

pthread_mutex_t g_can0_mutex;
pthread_mutex_t g_can1_mutex;
int g_can0_fd;
int g_can1_fd;

void * can0_Rx_task(void  *arg)
{
    static int rcnt=0;
    int ret = 0;
    int len;
    char rev_buf[CAN_MAX_DLEN+1] = "";
    int fd = g_can0_fd;

    pthread_mutex_lock(&g_can1_mutex);
    //设置过滤规则
    struct can_filter rfilter;
    rfilter.can_id = 0x0C000004;
    rfilter.can_mask = CAN_EFF_MASK;        //只接收ID=0x0c000004的扩展帧
    setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));//设置规则
    pthread_mutex_unlock(&g_can1_mutex);

    //循环接收
    while (1) {
        pthread_mutex_lock(&g_can0_mutex);

        ret = can_receive(fd, rev_buf, &len);

        pthread_mutex_unlock(&g_can0_mutex);

        sleep(1);
        if (ret <= 0) continue;
        rcnt++;
        printf("Receive: fd = %d cnt = %d\n", fd, rcnt);
        printf("data: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X \n",
               rev_buf[0], rev_buf[1], rev_buf[2] , rev_buf[3],
               rev_buf[4], rev_buf[5], rev_buf[6], rev_buf[7]);
        memset(rev_buf, 0, sizeof(rev_buf));        //一定要清空

    }

    pthread_exit( (void  *)1);

}

void * can0_Tx_task(void  *arg)
{
    static int scnt=0;
    int ret;
    char send_buf[8] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
    int can_id = 0x0C000003;
    int frame_type = CAN_EFF_FLAG;      //发送扩展帧

    int fd = g_can0_fd;
    while (1) {
        pthread_mutex_lock(&g_can0_mutex);

        ret = can_send(fd, can_id, frame_type, send_buf, 8);

        pthread_mutex_unlock(&g_can0_mutex);

        sleep(1);
        if (ret <= 0) continue;
        scnt++;
        printf("Send: fd = %d cnt = %d\n", fd, scnt);

    }

    pthread_exit((void  *)2);
}

void create_can0_task()
{

    pthread_t pid0, pid1;
    pthread_attr_t attr;

    pthread_attr_init(&attr);         //初始化线程属性结构
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   //设置attr结构为分离
    //创建线程，返回线程号给pid1,线程属性设置为attr的属性，线程函数入口为task1，参数为NULL

    pthread_create(&pid0, &attr, can0_Tx_task, NULL);

    pthread_create(&pid1, &attr, can0_Rx_task, NULL);

}


void * can1_Rx_task(void  *arg)
{
    static int rcnt=0;
    int ret = 0;
    int len;
    char rev_buf[CAN_MAX_DLEN+1] = "";
    int fd = g_can1_fd;

    pthread_mutex_lock(&g_can1_mutex);
    //设置过滤规则
    struct can_filter rfilter;
    rfilter.can_id = 0x0C000001;
    rfilter.can_mask = CAN_EFF_MASK;
    setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));//设置规则
    pthread_mutex_unlock(&g_can1_mutex);
    //循环接收
    while (1) {
        pthread_mutex_lock(&g_can1_mutex);

        ret = can_receive(fd, rev_buf, &len);

        pthread_mutex_unlock(&g_can1_mutex);

        sleep(1);
        if (ret <= 0) continue;
        rcnt++;
        printf("Receive: fd = %d cnt = %d\n", fd, rcnt);
        printf("data: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X \n",
               rev_buf[0], rev_buf[1], rev_buf[2] , rev_buf[3],
               rev_buf[4], rev_buf[5], rev_buf[6], rev_buf[7]);
        memset(rev_buf, 0, sizeof(rev_buf));        //一定要清空

    }

    pthread_exit( (void  *)1);

}

void * can1_Tx_task(void  *arg)
{
    static int scnt=0;
    int ret;
    char send_buf[8] = {0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1};
    int can_id = 0x0C000004;
    int frame_type = CAN_EFF_FLAG;      //扩展帧

    int fd = g_can1_fd;
    while (1) {
        pthread_mutex_lock(&g_can1_mutex);

        ret = can_send(fd, can_id, frame_type, send_buf, 8);

        pthread_mutex_unlock(&g_can1_mutex);

        sleep(1);
        if (ret <= 0) continue;
        scnt++;
        printf("Send: fd = %d cnt = %d\n", fd, scnt);

    }

    pthread_exit((void  *)2);
}

void create_can1_task()
{

    pthread_t pid0, pid1;
    pthread_attr_t attr;

    pthread_attr_init(&attr);         //初始化线程属性结构
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   //设置attr结构为分离
    //创建线程，返回线程号给pid1,线程属性设置为attr的属性，线程函数入口为task1，参数为NULL

    pthread_create(&pid0, &attr, can1_Tx_task, NULL);

    pthread_create(&pid1, &attr, can1_Rx_task, NULL);

}
