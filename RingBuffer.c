#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap_4.h"
#include "LinkingList.h"
#include "RingBuffer.h"

static List_Header ring_buff_list = INIT_LIST_HEADER;

int RingBuffer_DataLength(RING_BUFF *buff)
{
    if(!buff) return 0;

    int data_len = buff->tail - buff->head;
    
    if(data_len > 0)
        return data_len;
        // return (data_len == ((buff->len) - 1))?((buff->len)):(data_len + 1);

    if(data_len < 0)
        return ((buff->len) + data_len);
        // return (data_len == -1)?((buff->len)):((buff->len) + data_len + 1);
    
    if(buff->EF) return (buff->len);

    return 0;
}

unsigned char RingBuffer_Byte_Get(RING_BUFF *buff)
{
	if(!buff)
		return -1;

    if(RingBuffer_DataLength(buff) == 0)
		return -1;

    unsigned char tmp = buff->data[buff->head];
    buff->head = (buff->head + 1) % (buff->len);

    if(buff->head == buff->tail) buff->EF = RING_BUFF_EMPTY;

    return tmp;
}

int RingBuffer_Byte_Set(RING_BUFF *buff, const unsigned char byte)
{
    if(!buff) return -1;

    // RX Buffer Full , Over Write Old Data
    if(RingBuffer_DataLength(buff) == (buff->len))
        buff->head = (buff->head + 1) % (buff->len);

    // Copy Data Register To Ring Buffer
    buff->data[buff->tail] = byte;

    buff->tail = (buff->tail + 1) % (buff->len);

    if(buff->tail == buff->head)
    	buff->EF = RING_BUFF_FULL;

    return 0;
}

RING_BUFF *RingBuffer_New(char *des, int len)
{
    if(!ring_buff_list) return NULL;
    if(!des || !len) return NULL;

    RING_BUFF *new_buff = (RING_BUFF *)extmalloc(sizeof(RING_BUFF));
    memset(new_buff, 0, sizeof(RING_BUFF));
    new_buff->len = len;
    new_buff->data = (unsigned char *)extmalloc(sizeof(unsigned char) * new_buff->len);
    memset(new_buff->data, 0, new_buff->len);

    // Recording !!
    RING_RECORD *new_record = (RING_RECORD *)extmalloc(sizeof(RING_RECORD));
    memset(new_record, 0, sizeof(RING_RECORD));

    if(des) sprintf(new_record->des, "%s", des);
    new_record->point = (void *)new_buff;
    List_Insert_Tail(ring_buff_list, (void *)new_record);
    
    return new_buff;
}

void RingBuffer_Init(void)
{
    List_Header_Init(&ring_buff_list);
}

