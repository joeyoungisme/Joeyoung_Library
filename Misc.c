//------------------------------------------

//------------------------------------------
// To do list :
//
// 1. 
//------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ustdlib.h"
#include "Misc.h"

int time_compare(struct tm t1, struct tm t2)
{
    int res = 0;

    res = HOUR_CMP(t1, t2);
    if(res) return res;

    res = MIN_CMP(t1, t2);
    if(res) return res;

    res = SEC_CMP(t1, t2);
    return res;
}

int time_par_compare(uint8_t par, struct tm t1, struct tm t2)
{
    int res = 0;

    for(int index = 0; index < 6; ++index) {
        uint8_t curr_cmp = par & (0x01 << index);
        if(!curr_cmp) continue;

        switch(curr_cmp) {
            case TIMECMP_YEAR:
                res = YEAR_CMP(t1, t2);
            break;
            
            case TIMECMP_MON:
                res = MON_CMP(t1, t2);
            break;
            
            case TIMECMP_MDAY:
                res = MDAY_CMP(t1, t2);
            break;
            
            case TIMECMP_HOUR:
                res = HOUR_CMP(t1, t2);
            break;
            
            case TIMECMP_MIN:
                res = MIN_CMP(t1, t2);
            break;
            
            case TIMECMP_SEC:
                res = SEC_CMP(t1, t2);
            break;
        }

        if(res) break;
    }

    return res;
}

float U32ToFloat(unsigned long ulVal)
{
  float *pf = (float*)&ulVal;
  return *pf;    
}

double U64ToDouble(unsigned long long ullVal)
{
  double *pf = (double*)&ullVal;
  return *pf;    
}

unsigned long FloatToU32(float fVal)
{
/*
  // This does not work!
  unsigned long ul = 0;
  float *pf;
  pf = (float*) (&ul);
  
  *pf = fVal;
  return ul;
*/
  unsigned long *pul = (unsigned long*)&fVal;
  return *pul;    
}

//*****************************************************************************
//
//!IntToFloatStr
//
//*****************************************************************************
bool IntToFloatStr(float Xangle, int* pcbuf)
{
    float temp;

    temp = Xangle;

    if(Xangle>=0)
    {
     pcbuf[0] = (int32_t)temp ;
     pcbuf[1] = (int32_t)((temp -(int32_t)temp )*1000) ;
    }
    else
    {
     temp = 0-temp; 
     pcbuf[0] = (int32_t)temp ;
     pcbuf[1] = (int32_t)((temp -(int32_t)temp )*1000) ;
     return true ;
    }

    return false ;
}
void uppercase(char *str, int len)
{
    for(int idx = 0; idx < len; ++idx)
        if(str[idx] >= 'a' && str[idx] <= 'z')
            str[idx] -= 0x20;
}

void lowercase(char *str, int len)
{
    for(int idx = 0; idx < len; ++idx)
        if(str[idx] >= 'A' && str[idx] <= 'Z')
            str[idx] += 0x20;
}

uint32_t hexstr2byte(uint8_t *des_buff, char *src_str, int len)
{
    if(!des_buff || !src_str) return 0;

    // Check Str 
    for(int idx = 0; idx < len; ++idx) {
        if(src_str[idx] >= '0' && src_str[idx] <= '9') continue;
        else if(src_str[idx] >= 'a' && src_str[idx] <= 'f') continue;
        else if(src_str[idx] >= 'A' && src_str[idx] <= 'F') continue;
        else return 0;
    }

    // "1" => 0x10
    int res_len = 0;
    for(int idx = 0; idx < len; ++idx)
    {
        uint8_t curr_val = 0;

        if(src_str[idx] >= '0' && src_str[idx] <= '9')
            curr_val = src_str[idx] - '0';
        else if(src_str[idx] >= 'a' && src_str[idx] <= 'f')
            curr_val = src_str[idx] - 'a' + 10;
        else if(src_str[idx] >= 'A' && src_str[idx] <= 'F')
            curr_val = src_str[idx] - 'A' + 10;

        if(!(idx % 2)) des_buff[res_len] = curr_val << 4;
        else des_buff[res_len++] += curr_val;
    }
    return res_len;
}

uint32_t hexbuf2str(char *des_str, uint8_t *hexbuf, int len)
{
    char *convert_point = des_str;

    // Hex To String
    for(int index = 0; index < len; ++index) {
        unsigned char temp_char;

        temp_char = (hexbuf[index] >> 4) & 0x0F;
        if(temp_char >= 0 && temp_char <= 9)
            temp_char += '0';
        else
            temp_char += ('A' - 10);
        
        *convert_point++ = temp_char;

        temp_char = (hexbuf[index] & 0x0F);
        if(temp_char >= 0 && temp_char <= 9)
            temp_char += '0';
        else
            temp_char += ('A' - 10);
        
        *convert_point++ = temp_char;
    }
    *convert_point = '\0';

    return 0;
}

uint32_t str2hex32(char *buff, uint32_t len)
{
    // Max Len = 0xFFFFFFFF
    if(len > 8) return 0;

    uint32_t ret_val = 0;

    for(int index = len - 1; index >= 0; --index) {
        if(*buff >= '0' && *buff <= '9')
            ret_val += (uint32_t)(*buff - '0') << (index * 4);
        else if(*buff >= 'A' && *buff <= 'F')
            ret_val += (uint32_t)(*buff - 'A' + 10) << (index * 4);
        else if(*buff >= 'a' && *buff <= 'f')
            ret_val += (uint32_t)(*buff - 'a' + 10) << (index * 4);

        buff++;
    }

    return ret_val;
}

float str2float(char *buff, uint32_t len)
{
    char *small_num = buff;
    uint8_t num_count = 0;

    while(*small_num++ != '.') num_count++;

    buff[num_count] = '\0';

    float big_val = (float)atoi(buff);


    small_num[6] = '\0';

    float small_val = (float)atoi(small_num);

    for(int index = 0; index < 6; ++index)
        small_val /= 10.0;

    return big_val + small_val;
}


// buf      : String buffer allocated by user
// fVal     : Long long value assigned by user 
// Precision: 0 ~ 5 assigned by user
// Return   : -1 (false), > 0 (Success, return string len) 
static int LLongToStr(char *buf, long long llVal)
{
    if(!buf) return -1;

    int count = 0;

    if(llVal < 0) {
        *buf++ = '-';
        llVal = (long long)llVal * -1;
        count++;
    }

    //  Why 20 char?? Because LLONG_MAX = 9223372036854775807
    char tempbuf[20];
    int index = 0;
    unsigned long long ullVal = (unsigned long long)llVal;
    do{
        tempbuf[index++] = (ullVal % 10) + 48;
        count++;
    }while(ullVal /= 10);

    // string reverse (tempbuf)
    while(index > 0)
        *buf++ = tempbuf[--index];

    buf[count] = '\0';

    return count;
}

// buf      : String buffer allocated by user
// fVal     : Float value assigned by user 
// Precision: 0 ~ 5 assigned by user
// Return   : -1 (false), > 0 (Success, return string len) 
int FloatToStr(char *buf, float fVal, int precision)
{
    if(!buf) return -1;
    if((precision > PRECISION_MAX) || (((long)fVal) > LONG_MAX)) {
        usprintf(buf, "Err");
        return -1;
    }

    int count = 0;

    if(fVal < 0) {
        *buf++ = '-';
        fVal *= -1.0;
        count++;
    }

    // 4 out 5 in section
    float tmpval = 0.5;
    for(int index = 0; index < precision; ++index)
        tmpval /= 10.0;
    fVal += tmpval;

    // Get Double Value part of integer
    long part_of_integer = (long)fVal;
    fVal -= (float)part_of_integer;
    
    uint8_t integer_only = 0;
    char part_of_point[PRECISION_MAX+1] = {0};
    if(fVal == tmpval) integer_only = 1;
    else {
        // after point number -> string
        for(int index = 0; index < precision; ++index) {
            fVal *= 10.0;
            part_of_point[index] = ((int)fVal) + '0';
            part_of_point[index + 1] = '\0';
            fVal -= (float)((long)fVal);
        }
    }

    // merge integer & point
    int res = 0;
    res = usprintf((char *)buf, "%lu", part_of_integer);
    if(res > 0) count += res;

    if(integer_only) return count;

    res = usprintf((char *)buf + res, ".%s", part_of_point);
    if(res > 0) count += res;

    return count;
}

// buf      : String buffer allocated by user
// fVal     : Double value assigned by user 
// Precision: 0 ~ 5 assigned by user
// Return   : -1 (false), > 0 (Success, return string len) 
static int DoubleToStr(char *buf, double dVal, int precision)
{
    if(!buf) return -1;
    if((precision > PRECISION_MAX) || ((long long)dVal > LLONG_MAX)) {
        usprintf(buf, "Err");
        return -1;
    }

    int count = 0;

    if(dVal < 0) {
        *buf++ = '-';
        dVal *= -1.0;
        count++;
    }

    // 4 out 5 in section
    double tmpval = 0.5;
    for(int index = 0; index < precision; ++index)
        tmpval /= 10.0;
    dVal += tmpval;

    // Get Double Value part of integer
    long long part_of_integer = (long long)dVal;
    dVal -= part_of_integer;
    
    // after point number -> string
    char part_of_point[PRECISION_MAX+1];
    for(int index = 0; index < precision; ++index) {
        dVal *= 10.0;
        part_of_point[index] = ((int)dVal) + 48;
        part_of_point[index + 1] = '\0';
        dVal -= (double)((long long)dVal);
    }

    // merge integer & point
    int res = 0;
    res = LLongToStr(buf, part_of_integer);
    if(res > 0) count += res;
    res = usprintf((char *)buf + res, ".%s", part_of_point);
    if(res > 0) count += res;

    return count;
}

// buf      : String buffer allocated by user
// anytype  : use ANYTYPE_VAR(x) macro
// data_type: use ANYTYPE_VAR(x) macro
// operate  : operate < 0 is Div, >= 0 is Mul
// Return   : Success : Char * (string) , Failed : NULL
// char *AnytypeToString(char *str, anytype var, int data_type, int operate)
char *AnytypeToString(char *str, void *var, int data_type, int operate, int negative)
{
    if(!str) return NULL;

    uint32_t str_len = 0;

    switch(data_type) {
        case TYPENAME_INT8:
            if(operate < 0) *(char *)var /= (operate * -1);
            else *(char *)var *= operate;

            if(negative) *(char *)var *= -1;
            str_len = usprintf((char *)str, "%d", *(char *)var);
        break;
        case TYPENAME_UINT8:
            if(operate < 0) *(unsigned char *)var /= (operate * -1);
            else *(unsigned char *)var *= operate;

            // if(negative) ...
            str_len = usprintf((char *)str, "%u", *(unsigned char *)var);
        break;
        case TYPENAME_INT16: 
            if(operate < 0) *(short *)var /= (operate * -1);
            else *(short *)var *= operate;

            if(negative) *(short *)var *= -1;
            str_len = usprintf((char *)str, "%d", *(short *)var);
        break;
        case TYPENAME_UINT16:
            if(operate < 0) *(unsigned short *)var /= (operate * -1);
            else *(unsigned short *)var *= operate;

            // if(negative) *(char *)var *= -1;
            str_len = usprintf((char *)str, "%u", *(unsigned short *)var);
        break;
        case TYPENAME_INT32:
            if(operate < 0) *(int *)var /= (operate * -1);
            else *(int *)var *= operate;

            if(negative) *(int *)var *= -1;
            str_len = usprintf((char *)str, "%d", *(int *)var);
        break;
        case TYPENAME_UINT32:
            if(operate < 0) *(unsigned int *)var /= (operate * -1);
            else *(unsigned int *)var *= operate;

            // if(negative) *(char *)var *= -1;
            str_len = usprintf((char *)str, "%u", *(unsigned int *)var);
        break;
        case TYPENAME_FLOAT:
            if(operate < 0)
                *(float *)var /= (float)(operate * -1);
            else
                *(float *)var *= (float)operate;

            if(negative) *(float *)var *= -1;
            str_len = FloatToStr(str, *(float *)var, ANYTYPE_PRECISION);
        break;
        case TYPENAME_DOUBLE:
            if(operate < 0)
                *(double *)var /= (double)(operate * -1);
            else
                *(double *)var *= (double)operate;

            if(negative) *(double *)var *= -1;
            str_len = DoubleToStr(str, *(double *)var, ANYTYPE_PRECISION);
        break;
        default:
            str[str_len++] = 'E';
            str[str_len++] = 'r';
            str[str_len++] = 'r';
            str[str_len++] = 'T';
            str[str_len++] = 'y';
            str[str_len++] = 'p';
            str[str_len++] = 'e';
        break;
    }

    str[str_len] = '\0';

    return str;
}

void hexstr2bytearray(uint8_t *des, uint32_t len, char *str)
{
    if(!des || !len || !str) return;

    for(int idx = 0; idx < (len * 2); ++idx)
    {
        if(!str[idx]) return;

        char curr_char = str[idx];

        if(curr_char >= '0' && curr_char <= '9')
        {
            curr_char -= '0';
        }
        else if(curr_char >= 'A' && curr_char <= 'F')
        {
            curr_char -= ('A' - 10);
        }
        else if(curr_char >= 'a' && curr_char <= 'f')
        {
            curr_char -= ('a' - 10);
        }

        if(idx % 2) des[idx/2] += curr_char & 0x0F;
        else des[idx/2] = (curr_char & 0x0F) << 4;
    }
}

void byte_reverse(unsigned char *buff, int len)
{
    if(!buff || !len) return;

    for(int idx = 0; idx < (len/2); ++idx)
    {
        uint8_t tmp = buff[idx];
        buff[idx] = buff[len - idx -1];
        buff[len - idx - 1] = tmp;
    }
    return;
}

uint8_t *byte_compare(unsigned char *src, int srclen, const unsigned char *key, const int keylen)
{
    if(!src || !srclen || !key || !keylen) return NULL;

    int keyidx = 0;
    for(int idx = 0; idx < (srclen - keylen); ++idx)
    {
        while(src[idx + keyidx] == key[keyidx])
        {
            if(++keyidx == keylen) return (src + idx);
        }

        keyidx = 0;
    }

    return NULL;
}
