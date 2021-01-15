#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "heap_4.h"
#include "LinkingList.h"

#include "SWT.h"

// #include "UART.h"
// #define SWT_PRINT(...)               UART_Print(__VA_ARGS__)
// #define SWT_PRINT(...)           UART_Print(__VA_ARGS__)

#define SWT_PRINT(...)
#define SWT_PRINT(...)

static unsigned char kp_show_id = 0;
static List_Header timer_list = INIT_LIST_HEADER;

// -------- System Tick Config ---
#include "stm32f4xx_hal.h"
static uint32_t SWT_GetTick(void)
{
    return HAL_GetTick();
}
// ---------------------

void SWT_Init(void)
{
    List_Header_Init(&timer_list);
    if(!timer_list) SWT_PRINT("SWT Init Error\r\n");
}

void SWT_Thread(void)
{
    int list_amt = List_Header_Length(timer_list);

    int t_idx = 0;
    while(t_idx < list_amt) {
        SWT *c_tm = List_Get_Index(timer_list, t_idx);

        if(!c_tm) break;

        if(TIMER_IS_START(c_tm->state) && ((SWT_GetTick() - c_tm->tick) > c_tm->tout))
        {
            // Change State First , Because in tout_act() Have Change State Possible
            if(c_tm->type == TIMER_TYPE_ONCE)
                c_tm->state = TIMER_TIMESUP;
            else if(c_tm->type == TIMER_TYPE_CYCLE)
                c_tm->tick = SWT_GetTick();
            else SWT_PRINT("Timer Thread Type Error\r\n");

            if(c_tm->tout_act) {
                // SWT_PRINT("Timer %d Timesup !! Call Back Func Run\r\n", c_tm->t_id);
                c_tm->tout_act(); 
                break;
            }
        }
        else if(TIMER_IS_REMOVE(c_tm->state))
        {
            SWT_PRINT("Ready To Remove : %s\r\n", c_tm->des);
            if(List_Remove_Index(timer_list, t_idx)) {
                SWT_PRINT("List Remove Error \r\n");
            }
            else {
            	extfree(c_tm);
                list_amt--;
                SWT_PRINT("List Remove OK , Ready Free\r\n");
                continue;
            }
        }
        t_idx++;
    }

    return;
}

int SWT_Register(SWT new)
{
    // New Timer Check
    if(!new.des || !new.tout_act) return -1;
    else if(strlen(new.des) > 32) return -2;

    SWT *new_timer = (SWT *)extmalloc(sizeof(SWT));
    memcpy(new_timer, &new, sizeof(SWT));

    // This Timer Exist
    if(new.t_id != SWTID_INIT) {
        SWT_Edit_By_ID(new);
        if(TIMER_IS_START(new.state))
            SWT_Start_By_ID(new.t_id);
        else if(TIMER_IS_STOP(new.state))
            SWT_Stop_By_ID(new.t_id);
        else ;

        return new.t_id;
    }
    
    // Find t_id
    unsigned int t_id = 0;
    for(int t_idx = 0; t_idx < List_Header_Length(timer_list); ++t_idx)
        if(((SWT *)List_Get_Index(timer_list, t_idx))->t_id == t_id)
            t_id++;
    
    new_timer->t_id = t_id;
    new_timer->tick = SWT_GetTick();
    new_timer->state = new.state;

    int rc = List_Insert_Tail(timer_list, new_timer);

    if(rc < 0) return rc;

    return new_timer->t_id;
}

static SWT *search_timer_by_id_point(List_Header head, int t_id)
{
    SWT *r_timer = NULL;

    for(int t_idx = 0; t_idx < List_Header_Length(head); ++t_idx) {
        r_timer = (SWT *)List_Get_Index(head, t_idx);
        if(r_timer->t_id == t_id) break;
        else r_timer = NULL;
    }

    return r_timer;
}

static int search_timer_by_id_index(List_Header head, int t_id)
{
    SWT *r_timer = NULL;

    for(int t_idx = 0; t_idx < List_Header_Length(head); ++t_idx) {
        r_timer = (SWT *)List_Get_Index(head, t_idx);
        if(r_timer->t_id == t_id) return t_idx;
    }

    return -1;
}

static SWT *search_timer_by_des_point(List_Header head, char *description)
{
    SWT *r_timer = NULL;

    for(int t_idx = 0; t_idx < List_Header_Length(head); ++t_idx) {
        r_timer = (SWT *)List_Get_Index(head, t_idx);
        if(!strncmp(r_timer->des, description, TIMER_DESCRIPTION_LEN)) break;
        else r_timer = NULL;
    }

    return r_timer;
}

static int search_timer_by_des_index(List_Header head, char *description)
{
    SWT *r_timer = NULL;

    for(int t_idx = 0; t_idx < List_Header_Length(head); ++t_idx) {
        r_timer = (SWT *)List_Get_Index(head, t_idx);
        if(!strncmp(r_timer->des, description, TIMER_DESCRIPTION_LEN)) return t_idx;
    }

    return -1;
}

int SWT_Unregister_By_ID(int t_id)
{
    if(t_id == SWTID_INIT) return -1;

    SWT *rm_timer = search_timer_by_id_point(timer_list, t_id);
    if(!rm_timer) return -1;

    // Set Remove Flag , Waiting SWT_Thread() Remove
    rm_timer->state = TIMER_REMOVE;

    return 0;
}

int SWT_Unregister_All(void)
{
    for(int t_idx = 0; t_idx < List_Header_Length(timer_list); ++t_idx)
        ((SWT *)List_Get_Index(timer_list, t_idx))->state = TIMER_REMOVE;

    return 0;
}

int SWT_Unregister_By_Description(char *description)
{
    if(!description) return -1;

    SWT *rm_timer = search_timer_by_des_point(timer_list, description);
    if(!rm_timer) return -1;

    // Set Remove Flag , Waiting SWT_Thread() Remove
    rm_timer->state = TIMER_REMOVE;

    return 0;
}

int SWT_Edit_By_SWT(SWT ch_timer)
{
    if(ch_timer.t_id == SWTID_INIT) return -1;

    SWT *modify = search_timer_by_id_point(timer_list, ch_timer.t_id);
    if(modify) {

        if(ch_timer.type <= TIMER_TYPE_ONCE)
            modify->type = ch_timer.type;

        if(ch_timer.tout)
            modify->tout = ch_timer.tout;

        if(ch_timer.des)
            memcpy(modify->des, ch_timer.des, TIMER_DESCRIPTION_LEN);

        if(ch_timer.tout_act)
            modify->tout_act = ch_timer.tout_act;

        return 0;
    }

    return -1;
}

int SWT_Edit_By_ID(SWT ch_timer)
{
    if(ch_timer.t_id == SWTID_INIT) return -1;

    SWT *modify = search_timer_by_id_point(timer_list, ch_timer.t_id);
    if(modify) {

        if(ch_timer.type <= TIMER_TYPE_ONCE)
            modify->type = ch_timer.type;

        if(ch_timer.tout)
            modify->tout = ch_timer.tout;

        if(ch_timer.des)
            memcpy(modify->des, ch_timer.des, TIMER_DESCRIPTION_LEN);

        if(ch_timer.tout_act)
            modify->tout_act = ch_timer.tout_act;

        return 0;
    }

    return -1;
}

int SWT_Edit_By_Description(SWT ch_timer)
{
    if(!ch_timer.des) return -1;

    SWT *modify = search_timer_by_des_point(timer_list, ch_timer.des);
    if(modify) {

        if(ch_timer.type <= TIMER_TYPE_ONCE)
            modify->type = ch_timer.type;

        if(ch_timer.tout)
            modify->tout = ch_timer.tout;

        if(ch_timer.des)
            memcpy(modify->des, ch_timer.des, TIMER_DESCRIPTION_LEN);

        if(ch_timer.tout_act)
            modify->tout_act = ch_timer.tout_act;

        return 0;
    }

    return -1;
}

int SWT_Start_All(void)
{
    for(int t_idx = 0; t_idx < List_Header_Length(timer_list); ++t_idx) {
        SWT *c_tm = (SWT *)List_Get_Index(timer_list, t_idx);
        if(!c_tm) continue;
        c_tm->state = TIMER_START;
        c_tm->tick = SWT_GetTick();
    }

    return 0;
}

int SWT_Start_By_ID(int t_id)
{
    if(t_id == SWTID_INIT) return -1;

    SWT *s_timer = search_timer_by_id_point(timer_list, t_id);

    if(!s_timer) return -1;

    s_timer->state = TIMER_START;
    s_timer->tick = SWT_GetTick();

    return 0;
}
int SWT_Start_By_SWT(SWT tm)
{
    if(tm.t_id == SWTID_INIT) return -1;

    SWT *s_timer = search_timer_by_id_point(timer_list, tm.t_id);

    if(!s_timer) return -1;

    s_timer->state = TIMER_START;
    s_timer->tick = SWT_GetTick();

    return 0;
}

int SWT_Config_Start(SWT tm, unsigned char type, unsigned int tout)
{
    if(tm.t_id == SWTID_INIT) return -1;

    SWT *s_timer = search_timer_by_id_point(timer_list, tm.t_id);

    if(!s_timer) return -1;

    s_timer->tout = tout;
    s_timer->type = type;
    s_timer->state = TIMER_START;
    s_timer->tick = SWT_GetTick();

    return 0;
}

int SWT_Stop_All(void)
{
    for(int t_idx = 0; t_idx < List_Header_Length(timer_list); ++t_idx) {
        SWT *c_tm = (SWT *)List_Get_Index(timer_list, t_idx);
        if(!c_tm) continue;
        c_tm->state = TIMER_STOP;
    }

    return 0;
}

int SWT_Stop_By_ID(int t_id)
{
    if(t_id == SWTID_INIT) return -1;

    SWT *s_timer = search_timer_by_id_point(timer_list, t_id);

    if(!s_timer) return -1;

    s_timer->state = TIMER_STOP;

    return 0;
}
int SWT_Stop_By_SWT(SWT tm)
{
    if(tm.t_id == SWTID_INIT) return -1;

    SWT *s_timer = search_timer_by_id_point(timer_list, tm.t_id);

    if(!s_timer) return -1;

    s_timer->state = TIMER_STOP;

    return 0;
}

int SWT_Start_By_Des(char *des)
{
    SWT *s_timer = search_timer_by_des_point(timer_list, des);

    if(!s_timer) return -1;

    s_timer->state = TIMER_START;
    s_timer->tick = SWT_GetTick();

    return 0;
}

int SWT_Stop_By_Des(char *des)
{
    SWT *s_timer = search_timer_by_des_point(timer_list, des);

    if(!s_timer) return -1;

    s_timer->state = TIMER_STOP;

    return 0;
}

int SWT_State_By_ID(int t_id)
{
    if(t_id == SWTID_INIT) return -1;

    SWT *s_timer = search_timer_by_id_point(timer_list, t_id);

    if(s_timer) return s_timer->state;

    return -1;
}

int SWT_State_By_Des(char *des)
{
    SWT *s_timer = search_timer_by_des_point(timer_list, des);

    if(s_timer) return s_timer->state;

    return -1;
}

SWT *SWT_Info_Get(unsigned int idx)
{
    if(idx >= List_Header_Length(timer_list)) return NULL;

    return List_Get_Index(timer_list, idx);
}

int SWT_Amount(void)
{
    if(!timer_list) return -1;

    return List_Header_Length(timer_list);
}
