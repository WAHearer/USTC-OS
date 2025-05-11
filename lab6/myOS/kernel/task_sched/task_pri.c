#include "../../include/task.h"
#include "../../include/myPrintk.h"


void rqPriInit(myTCB* idleTsk) {
     rq.idleTsk=idleTsk;
}

int rqPriIsEmpty(void) {
     return !rq.head&&!rq.tail;
}

myTCB * nextPriTsk(void) {
     myTCB *p=rq.head;
     int maxPri=-1;
     myTCB *maxPriTCB;
     while(p){ 
          if(p->pri>maxPri){
               maxPri=p->pri;
               maxPriTCB=p;
          }
          p=p->nextTCB;
     }
     return maxPriTCB;
}

void tskEnqueuePri(myTCB *tsk) {
     if(tsk->task_entrance==initTskBody)
          myPrintk(0x2,"YES\n");
     if(!rq.head)
          rq.head=rq.tail=tsk;
     else{
          rq.tail->nextTCB=tsk;
          rq.tail=tsk;
     }
     tsk->nextTCB=(void*)0;
     
}

void tskDequeuePri(myTCB *tsk) {
     
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

myTCB* schedulePri(void) {
     myTCB *nextTsk;
     nextTsk = nextPriTsk();
     return nextTsk;
}