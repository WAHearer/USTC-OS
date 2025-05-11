#include "../include/task.h"
#include "../include/myPrintk.h"

void schedule(void);
void destroyTsk(int takIndex);

unsigned long pMemHandler;

myTCB tcbPool[TASK_NUM];//进程池的大小设置

myTCB* idleTsk;                /* idle 任务 */
myTCB* currentTsk;             /* 当前任务 */
myTCB* firstFreeTsk;           /* 下一个空闲的 TCB */

#define TSK_RDY 0        //表示当前进程已经进入就绪队列中
#define TSK_WAIT -1      //表示当前进程还未进入就绪队列中
#define TSK_RUNING 1     //表示当前进程正在运行
#define TSK_NONE 2       //表示进程池中的TCB为空未进行分配

#define FCFS 0
#define PRI 1
#define RR 2
int sched_type=FCFS;

rdyQueue rq;

//tskEmpty进程（无需填写）
void tskEmpty(void){
}

//tskIdleBdy进程（无需填写）
void tskIdleBdy(void) {
     while(1){
          schedule();
     }
}

//初始化栈空间（不需要填写）
void stack_init(unsigned long **stk, void (*task)(void)){
     *(*stk)-- = (unsigned long) 0x08;       //高地址
     *(*stk)-- = (unsigned long) task;       //EIP
     *(*stk)-- = (unsigned long) 0x0202;     //FLAG寄存器

     *(*stk)-- = (unsigned long) 0xAAAAAAAA; //EAX
     *(*stk)-- = (unsigned long) 0xCCCCCCCC; //ECX
     *(*stk)-- = (unsigned long) 0xDDDDDDDD; //EDX
     *(*stk)-- = (unsigned long) 0xBBBBBBBB; //EBX

     *(*stk)-- = (unsigned long) 0x44444444; //ESP
     *(*stk)-- = (unsigned long) 0x55555555; //EBP
     *(*stk)-- = (unsigned long) 0x66666666; //ESI
     *(*stk)   = (unsigned long) 0x77777777; //EDI

}

void tskStart(myTCB *tsk){
     tsk->TSK_State = TSK_RDY;
     if(sched_type==FCFS)
          tskEnqueueFCFS(tsk);
     else if(sched_type==PRI)
          tskEnqueuePri(tsk);
     else if(sched_type==RR)
          tskEnqueueRR(tsk);
}


//TODO:以takIndex为关键字，在进程池中寻找并销毁takIndex对应的进程（需要填写）
void destroyTsk(int takIndex) {//在进程中寻找TSK_ID为takIndex的进程，并销毁该进程
     for(int i=0;i<TASK_NUM;i++){
          if(tcbPool[i].TSK_ID==takIndex&&tcbPool[i].TSK_State!=TSK_NONE){
               tcbPool[i].TSK_State=TSK_NONE;
               return;
          }
     }
}

unsigned long **prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;

//切换上下文（无需填写）
void context_switch(myTCB *prevTsk, myTCB *nextTsk) {
     prevTSK_StackPtr = &(prevTsk->stkTop);
     currentTsk = nextTsk;
     nextTSK_StackPtr = nextTsk->stkTop;
     CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}


void schedule(void){
     myTCB *nextTsk;
     if(sched_type==FCFS)
          nextTsk=scheduleFCFS();
     else if(sched_type==PRI)
          nextTsk=schedulePri();
     else if(sched_type==RR)
          nextTsk=scheduleRR(0);
     context_switch(currentTsk,nextTsk);
}

unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;
void startMultitask(void) {
    BspContext = BspContextBase + STACK_SIZE -1;
    prevTSK_StackPtr = &BspContext;
     if(sched_type==FCFS)
          currentTsk = nextFCFSTsk();
     else if(sched_type==PRI)
          currentTsk = nextPriTsk();
     else if(sched_type==RR)
          currentTsk = nextRRTsk();
    nextTSK_StackPtr = currentTsk->stkTop;
    CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}

void setSchedType(int type){
    if(type>2||type<0){
        myPrintk(0x2,"Error: sched_type is out of range\n");
        return;
    }
    sched_type=type;
}

int createTsk(void (*tskBody)(void)){
     for(int i=0;i<TASK_NUM;i++){
          if(tcbPool[i].TSK_State==TSK_NONE){
               tcbPool[i].task_entrance=tskBody;
               tcbPool[i].pri=0;
               stack_init(&tcbPool[i].stkTop,tskBody);
               tskStart(&tcbPool[i]);
               return tcbPool[i].TSK_ID;
          }
     }
}
int createTskWithPri(void (*tskBody)(void),int pri){
     for(int i=0;i<TASK_NUM;i++){
          if(tcbPool[i].TSK_State==TSK_NONE){
               tcbPool[i].task_entrance=tskBody;
               tcbPool[i].pri=pri;
               stack_init(&tcbPool[i].stkTop,tskBody);
               tskStart(&tcbPool[i]);
               return tcbPool[i].TSK_ID;
          }
     }
}

void TaskManagerInit(void) {
    // 初始化进程池（所有的进程状态都是TSK_NONE）
     int i;
     myTCB * thisTCB;
     for(i=0;i<TASK_NUM;i++){//对进程池tcbPool中的进程进行初始化处理
          thisTCB = &tcbPool[i];
          thisTCB->TSK_ID = i;
          thisTCB->stkTop = thisTCB->stack+STACK_SIZE-1;//将栈顶指针复位
          thisTCB->TSK_State = TSK_NONE;//表示该进程池未分配，可用
          thisTCB->task_entrance = tskEmpty;
          thisTCB->pri=0;
          thisTCB->cnt=0;
     }
     //创建idle任务
     idleTsk = &tcbPool[0];
     stack_init(&(idleTsk->stkTop),tskIdleBdy);
     idleTsk->task_entrance = tskIdleBdy;
     idleTsk->nextTCB = (void *)0;
     idleTsk->TSK_State = TSK_RDY;
     if(sched_type==FCFS)
          rqFCFSInit(idleTsk);
     else if(sched_type==PRI)
          rqPriInit(idleTsk);
     else if(sched_type==RR)
          rqRRInit(idleTsk);
     //创建init任务
     createTsk(initTskBody);
     //进入多任务状态
     myPrintk(0x2,"START MULTITASKING......\n");
     startMultitask();
     myPrintk(0x2,"STOP MULTITASKING......SHUT DOWN\n");
}

void tskEnd(void){
     if(sched_type==FCFS)
          tskDequeueFCFS(currentTsk);
     else if(sched_type==PRI)
          tskDequeuePri(currentTsk); 
     else if(sched_type==RR)
          tskDequeueRR(currentTsk);     
     //由于TCB结束，我们将进程池中对应的TCB也删除
     destroyTsk(currentTsk->TSK_ID);
     //TCB结束后，我们需要进行一次调度
     schedule();
}