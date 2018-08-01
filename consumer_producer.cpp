#include "consumer_producer.h"

/* 初始化缓冲区结构 */
void init(struct prodcons *b)
{
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->notempty, NULL);
    pthread_cond_init(&b->notfull, NULL);
    b->readpos = 0;
    b->writepos = 0;
}
/* 将产品放入缓冲区,这里是存入一个整数*/
void producer_write(struct prodcons *b, int data)
{
    pthread_mutex_lock(&b->lock);
    /* 等待缓冲区未满*/
    if ((b->writepos + 1) % BUFFER_SIZE == b->readpos)
    {
        pthread_cond_wait(&b->notfull, &b->lock);
    }
    /* 写数据,并移动指针 */
    b->buffer[b->writepos] = data;
    b->writepos++;
    if (b->writepos >= BUFFER_SIZE)
        b->writepos = 0;
    /* 设置缓冲区非空的条件变量*/
    pthread_cond_signal(&b->notempty);  //用于解除某一个等待线程的阻塞状态
    pthread_mutex_unlock(&b->lock);
}
/* 从缓冲区中取出整数*/
int consumer_read(struct prodcons *b)
{
    int data;
    pthread_mutex_lock(&b->lock);
    /* 等待缓冲区非空*/
    if (b->writepos == b->readpos)
    {
        pthread_cond_wait(&b->notempty, &b->lock);
    }
    /* 读数据,移动读指针*/
    data = b->buffer[b->readpos];
    b->readpos++;
    if (b->readpos >= BUFFER_SIZE)
        b->readpos = 0;
    /* 设置缓冲区未满的条件变量*/
    pthread_cond_signal(&b->notfull);
    pthread_mutex_unlock(&b->lock);
    return data;
}


#define OVER ( - 1)
struct prodcons buffer;
void *producer(void *data)
{
    int n;
    for (n = 0; n < 10000; n++)
    {
        printf("%d --->\n", n);
        producer_write(&buffer, n);
    }
    producer_write(&buffer, OVER);
    return NULL;
}

void *consumer(void *data)
{
    int d;
    while (1)
    {
        d = consumer_read(&buffer);
        if (d == OVER)
            break;
        printf("---> %d \n", d);
    }
    return NULL;
}


/* 测试:生产者线程将1 到10000 的整数送入缓冲区,消费者线程从缓冲区中获取整数,两者都打印信息*/
int c_p_test(void)
{
    pthread_t th_a, th_b;
    void *retval;
    init(&buffer);
    /* 创建生产者和消费者线程*/
    pthread_create(&th_a, NULL, producer, 0);
    pthread_create(&th_b, NULL, consumer, 0);
    /* 等待两个线程结束*/
    pthread_join(th_a, &retval);
    pthread_join(th_b, &retval);
    return 0;
}
