#include "../../include/task.h"
#include "../../include/myPrintk.h"


void rqRRInit(myTCB* idleTsk) {
     rq.idleTsk=idleTsk;
}

int rqRRIsEmpty(void) {
     return !rq.head&&!rq.tail;
}

myTCB * nextRRTsk(void) {
     return rq.head;
}

void tskEnqueueRR(myTCB *tsk) {
     
     if(!rq.head)
          rq.head=rq.tail=tsk;
     else{
          rq.tail->nextTCB=tsk;
          rq.tail=tsk;
     }
     tsk->nextTCB=(void*)0;
}

void tskDequeueRR(myTCB *tsk) {
     if(rq.head==tsk)
          rq.head=rq.head->nextTCB;
     else{
          myTCB *p=rq.head;
          while(p->nextTCB!=tsk){
               p=p->nextTCB;
          }
          p->nextTCB=tsk->nextTCB;
          if(rq.tail==tsk)
               rq.tail=p;
     }
}

myTCB* scheduleRR(int interruptCall) {
     myTCB *nextTsk;
     if(interruptCall){
        if(rq.head->cnt<5){
            rq.head->cnt++;
            return (void*)0;
        }
        rq.head->cnt=0;
        rq.tail->nextTCB=rq.head;
        rq.tail=rq.head;
        rq.head=rq.head->nextTCB;
        rq.head->nextTCB=(void*)0;
     }
     rq.head->cnt=0;
     nextTsk = nextRRTsk();
     return nextTsk;
}