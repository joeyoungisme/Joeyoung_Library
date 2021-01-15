#ifndef __LIB_HEAP_4_H
#define __LIB_HEAP_4_H

#define extmalloc               pvPortMalloc
#define extfree                 vPortFree

#define extmemfreesize			xPortGetFreeHeapSize
#define extminmemfreesize		xPortGetMinimumEverFreeHeapSize

void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );

size_t xPortGetFreeHeapSize( void );
size_t xPortGetMinimumEverFreeHeapSize( void );

#ifdef DEBUG_VERSION
typedef struct __mem_log {
    void *addr;
    unsigned short len;
    char str[18];
} MEM_LOG;

struct __mem_log *debugInfo(int idx);
char *debugMemList(int idx);
void debugFree( void *pv );
void *debugMalloc( size_t xWantedSize , char *des);
#else
#define MEM_LOG                 void
#define debugInfo(stuff)        NULL
#define debugMemList(stuff)     NULL
#define debugFree(stuff)        extfree(stuff)
#define debugMalloc(stuff, str) extmalloc(stuff)
#endif

#endif
