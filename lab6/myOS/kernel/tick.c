#include "../include/task.h"
extern void oneTickUpdateWallClock(void);       //TODO: to be generalized

void (*tick_hook)(void) = 0;

int tick_number = 0;
void tick(void){
     tick_number++;	

     oneTickUpdateWallClock();

     if(sched_type==2)
          scheduleRR(1);

     if(tick_hook) tick_hook();  //user defined   
}
