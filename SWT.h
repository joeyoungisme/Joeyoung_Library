#ifndef __FW_SW_TIMER_H
#define __FW_SW_TIMER_H

#ifndef DEBUG_TITLE
#define DEBUG_TITLE               "SWT"
#endif

// ------------- System Tick Relate -----------
#define MS_PER_SYSTK              (1)
#define SWT_MS(x)                 (x / MS_PER_SYSTK)
#define SWT_SEC(x)                ((x * 1000) / MS_PER_SYSTK)
// --------------------------------------------

#define TIMER_INIT                0
#define TIMER_START               1
#define TIMER_TIMESUP             2
#define TIMER_STOP                4
#define TIMER_REMOVE              0xFF

#define TIMER_IS_START(x)         (x == TIMER_START)
#define TIMER_IS_STOP(x)          (x == TIMER_STOP)
#define TIMER_IS_REMOVE(x)        (x == TIMER_REMOVE)

#define TIMER_TYPE_CYCLE          0
#define TIMER_TYPE_ONCE           1

#define SWTID_INIT                (-1)
#define TIMER_TICK_INIT           0

#define TIMER_DESCRIPTION_LEN     32
typedef struct __timer_node {
    unsigned char state;
    unsigned char type;
    unsigned int t_id;
    unsigned int tick;
    unsigned int tout;
    char des[TIMER_DESCRIPTION_LEN];
    void (*tout_act)(void);
} SWT;

void SWT_Init(void);
void SWT_Thread(void);

SWT *SWT_Info_Get(unsigned int idx);
int SWT_Amount(void);

// Return Timer ID , For Search Timer Used
int SWT_Register(SWT);
int SWT_Unregister_By_ID(int timer_id);
int SWT_Unregister_By_Description(char *description);
int SWT_Unregister_All(void);

int SWT_Edit_By_SWT(SWT ch_timer);
int SWT_Edit_By_ID(SWT ch_timer);
int SWT_Edit_By_Description(SWT);

int SWT_Config_Start(SWT, unsigned char, unsigned int);

int SWT_Start_By_ID(int t_id);
int SWT_Stop_By_ID(int t_id);
int SWT_Start_By_Des(char *des);
int SWT_Stop_By_Des(char *des);
int SWT_Start_By_SWT(SWT);
int SWT_Stop_By_SWT(SWT);

int SWT_Start_All(void);
int SWT_Stop_All(void);

int SWT_State_By_Des(char *des);
int SWT_State_By_ID(int t_id);

#endif
