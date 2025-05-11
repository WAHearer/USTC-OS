#include "vga.h"
//NOTE:实现了时钟设置和获取的接口，如何在tick函数中使用它们，随着时钟中断更新墙钟？
void setWallClock(int HH,int MM,int SS){
	char *p=(char *)0xb8f90;
	*p=HH/10%10+'0';
	*(p+1)=0x07;
	*(p+2)=HH%10+'0';
	*(p+3)=0x07;
	*(p+4)=':';
	*(p+5)=0x07;
	*(p+6)=MM/10%10+'0';
	*(p+7)=0x07;
	*(p+8)=MM%10+'0';
	*(p+9)=0x07;
	*(p+10)=':';
	*(p+11)=0x07;
	*(p+12)=SS/10%10+'0';
	*(p+13)=0x07;
	*(p+14)=SS%10+'0';
	*(p+15)=0x07;
}

void getWallClock(int *HH,int *MM,int *SS){
	char *p=(char *)0xb8f90;
	*HH=(*p-'0')*10+*(p+2)-'0';
	*MM=(*(p+6)-'0')*10+*(p+8)-'0';
	*SS=(*(p+12)-'0')*10+*(p+14)-'0';
}
