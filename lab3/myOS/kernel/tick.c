#include "wallClock.h"
int system_ticks;
int HH,MM,SS;
//NOTE:你可以自行定义接口来辅助实现


void tick(void){
	//1tick=10ms
	system_ticks++;
	int flag=0;
	if(system_ticks==100){
		system_ticks=0;
		SS++;
		flag=1;
	}
	if(SS==60){
		SS=0;
		MM++;
	}
	if(MM==60){
		MM=0;
		HH++;
	}
	if(HH==24){
		HH=0;
	}
	if(flag)
		setWallClock(HH,MM,SS);
	return;
}

