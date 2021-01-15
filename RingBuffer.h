#ifndef __LIB_RINGBUFFER_H
#define __LIB_RINGBUFFER_H

#define RING_BUFF_SIZE          10
#define RING_BUFF_EMPTY         0
#define RING_BUFF_FULL          1
typedef struct __ring_buff
{
    unsigned char *data;
    unsigned int len;
    unsigned int head;
    unsigned int tail;
    unsigned char EF; // Empty ? Full ? Flag
} RING_BUFF;

#define RING_RECORD_STR_LEN     32
typedef struct __ring_record
{
    char des[RING_RECORD_STR_LEN];
    void *point;
} RING_RECORD;

void RingBuffer_Init(void);
RING_BUFF *RingBuffer_New(char *des, int len);
int RingBuffer_DataLength(RING_BUFF *rbuff);
unsigned char RingBuffer_Byte_Get(RING_BUFF *buff);
int RingBuffer_Byte_Set(RING_BUFF *buff, const unsigned char byte);

#endif
