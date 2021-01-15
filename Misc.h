#ifndef __MISC_MISC_H__
#define __MISC_MISC_H__


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#define ARRAY_SIZE(x)                           (sizeof(x)/sizeof(x[0]))

// Anytype To String
#define LLONG_MAX                               ((long long)9223372036854775807)
#define LONG_MAX                                ((long)2147483547)
#define ANYTYPE_PRECISION                       5
#define PRECISION_MAX                           5

#define typename(x)     __extension__ _Generic((x), \
    signed char*         : TYPENAME_INT8, \
    unsigned char*       : TYPENAME_UINT8, \
    short*               : TYPENAME_INT16, \
    unsigned short*      : TYPENAME_UINT16, \
    int*                 : TYPENAME_INT32, \
    long int*            : TYPENAME_INT32, \
    unsigned int*        : TYPENAME_UINT32, \
    long unsigned int*   : TYPENAME_UINT32, \
    float*               : TYPENAME_FLOAT, \
    double*              : TYPENAME_DOUBLE, \
    default              : TYPENAME_AMOUNT)
#define ANYTYPE_VAR(x)   (void *)x, typename(x)

enum {
    TYPENAME_BOOL = 0,
    TYPENAME_INT8,
    TYPENAME_UINT8,
    TYPENAME_INT16,
    TYPENAME_UINT16,
    TYPENAME_INT32,
    TYPENAME_UINT32,
    TYPENAME_FLOAT,
    TYPENAME_DOUBLE,
    TYPENAME_AMOUNT
};

#define MAKEWORD(H, L)      ((WORD)(((BYTE)(L)) | ((WORD)((BYTE)(H))) << 8))
#define MAKELONG(H, L)      ((DWORD)(((WORD)(L)) | ((DWORD)((WORD)(H))) << 16))
#define MAKEUINT64(H, L)    ((uint64_t)(((uint32_t)(L)) | ((uint64_t)((uint32_t)(H))) << 32))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define TIMECMP_YEAR        0x01
#define TIMECMP_MON         0x02
#define TIMECMP_MDAY        0x04
#define TIMECMP_HOUR        0x08
#define TIMECMP_MIN         0x10
#define TIMECMP_SEC         0x20

#define YEAR_CMP(x, y) \
    ((x).tm_year == (y).tm_year)?(0):(((x).tm_year > (y).tm_year)?(1):(-1))
#define MON_CMP(x, y) \
    ((x).tm_mon == (y).tm_mon)?(0):(((x).tm_mon > (y).tm_mon)?(1):(-1))
#define MDAY_CMP(x, y) \
    ((x).tm_mday == (y).tm_mday)?(0):(((x).tm_mday > (y).tm_mday)?(1):(-1))
#define HOUR_CMP(x, y) \
    ((x).tm_hour == (y).tm_hour)?(0):(((x).tm_hour > (y).tm_hour)?(1):(-1))
#define MIN_CMP(x, y) \
    ((x).tm_min == (y).tm_min)?(0):(((x).tm_min > (y).tm_min)?(1):(-1))
#define SEC_CMP(x, y) \
    ((x).tm_sec == (y).tm_sec)?(0):(((x).tm_sec > (y).tm_sec)?(1):(-1))

//*****************************************************************************
//
// Prototypes.
//
//*****************************************************************************

#include <time.h>
#include <stdbool.h>

// Add By Joeyoung - 2018 05 15
void lowercase(char *str, int len);
void uppercase(char *str, int len);
uint32_t hexbuf2str(char *des_str, uint8_t *hexbuf, int);
uint32_t str2hex32(char *buff, uint32_t len);
float str2float(char *buff, uint32_t len);
uint32_t hexstr2byte(uint8_t *des_buff, char *src_str, int len);
char *AnytypeToString(char *, void *, int , int , int);
void hexstr2bytearray(uint8_t *des, uint32_t len, char *str);
void byte_reverse(unsigned char *buff, int len);
uint8_t *byte_compare(unsigned char *src, int srclen, const unsigned char *key, const int keylen);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __DRIVERS_WATCHDOG_H__
