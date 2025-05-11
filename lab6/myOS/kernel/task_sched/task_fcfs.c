#include "../../include/task.h"
#include "../../include/myPrintk.h"


void rqFCFSInit(myTCB* idleTsk) {//对rqFCFS进行初始化处理
     rq.idleTsk=idleTsk;
}

int rqFCFSIsEmpty(void) {//当head和tail均为NULL时，rqFCFS为空
     return !rq.head&&!rq.tail;
}

myTCB * nextFCFSTsk(void) {//获取下一个Tsk
     return rq.head;
}

void tskEnqueueFCFS(myTCB *tsk) {//将tsk入队rqFCFS
     
     if(!rq.head)
          rq.head=rq.tail=tsk;
     else{
          rq.tail->nextTCB=tsk;
          rq.tail=tsk;
     }
     tsk->nextTCB=(void*)0;
}

void tskDequeueFCFS(myTCB *tsk) {//rqFCFS出队
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

myTCB* scheduleFCFS(void) {
     myTCB *nextTsk;
     nextTsk = nextFCFSTsk();
     return nextTsk;
}