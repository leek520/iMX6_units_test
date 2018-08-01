#ifndef CONSUMER_PRODUCER_H
#define CONSUMER_PRODUCER_H

#include "common.h"


#define BUFFER_SIZE 16 // 缓冲区数量
// 缓冲区相关数据结构
struct prodcons
 {

    int buffer[BUFFER_SIZE]; /* 实际数据存放的数组*/
    pthread_mutex_t lock; /* 互斥体lock 用于对缓冲区的互斥操作 */
    int readpos, writepos; /* 读写指针*/
    pthread_cond_t notempty; /* 缓冲区非空的条件变量 */
    pthread_cond_t notfull; /* 缓冲区未满的条件变量 */
 };

int c_p_test(void);

#endif // CONSUMER_PRODUCER_H
