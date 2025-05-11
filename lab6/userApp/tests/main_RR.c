#include "../myOS/userInterface.h"   //interface from kernel
#include "shell.h"
#include "memTestCase.h"

void myTsk0(void);
void myTsk1(void);
void myTsk2(void);
void startShell(void);
void initShell(void);
void memTestCaseInit(void);
int createTsk(void (*tskBody)(void));
void tskEnd(void);

void wallClock_hook_main(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
	sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x7E,24,72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
    	setWallClockHook(&wallClock_hook_main);
}

// init task 入口
void myMain(void) {
     clear_screen();
     doSomeTestBefore();
     myPrintf(0x07, "********************************\n");
	 myPrintf(0x07, "*         INIT   INIT !        *\n");
	 myPrintf(0x07, "********************************\n");
     setSchedType(1);
     createTskWithPri(myTsk0,1);
     createTskWithPri(myTsk1,3);
     createTskWithPri(myTsk2,2);
     tskEnd();
}
