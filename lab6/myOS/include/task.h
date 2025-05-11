#ifndef __TASK_H__
#define __TASK_H__

#ifndef USER_TASK_NUM
#include "../../userApp/userApp.h"
#endif

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define initTskBody myMain         // connect initTask with myMain

#define STACK_SIZE 512            // definition of STACK_SIZE

void initTskBody(void);

void CTX_SW(void*prev_stkTop, void*next_stkTop);

//#error "TODO: 为 myTCB 增补合适的字段"
typedef struct myTCB {
     unsigned long *stkTop;        /* 栈顶指针 */
     unsigned long stack[STACK_SIZE];      /* 开辟了一个大小为STACK_SIZE的栈空间 */  
     unsigned long TSK_State;   /* 进程状态 */
     unsigned long TSK_ID;      /* 进程ID */ 
     void (*task_entrance)(void);  /*进程的入口地址*/
     struct myTCB * nextTCB;           /*下一个TCB*/
     int pri;
     int cnt;
} myTCB;

typedef struct rdyQueue{
     myTCB * head;
     myTCB * tail;
     myTCB * idleTsk;
} rdyQueue;
extern rdyQueue rq;

extern myTCB tcbPool[TASK_NUM];//进程池的大小设置

extern myTCB* idleTsk;                /* idle 任务 */
extern myTCB* currentTsk;             /* 当前任务 */
extern myTCB* firstFreeTsk;           /* 下一个空闲的 TCB */
extern int sched_type;

void TaskManagerInit(void);

void rqFCFSInit(myTCB* idleTsk);
int rqFCFSIsEmpty(void);
myTCB * nextFCFSTsk(void);
void tskEnqueueFCFS(myTCB *tsk);
void tskDequeueFCFS(myTCB *tsk);
myTCB* scheduleFCFS(void);

void rqPriInit(myTCB* idleTsk);
int rqPriIsEmpty(void);
myTCB * nextPriTsk(void);
void tskEnqueuePri(myTCB *tsk);
void tskDequeuePri(myTCB *tsk);
myTCB* schedulePri(void);

void rqRRInit(myTCB* idleTsk);
int rqRRIsEmpty(void);
myTCB * nextRRTsk(void);
void tskEnqueueRR(myTCB *tsk);
void tskDequeueRR(myTCB *tsk);
myTCB* scheduleRR(int interruptCall);

#endif
