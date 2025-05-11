#include "../../include/myPrintk.h"


// eFPartition是表示整个内存的数据结构
typedef struct eFPartition{
	unsigned long totalN;
	unsigned long perSize;  // unit: byte	
}eFPartition;	// 占8个字节

#define eFPartition_size 8

void showeFPartition(struct eFPartition *efp){
	myPrintk(0x5,"eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x)\n", efp, efp->totalN, efp->perSize);
}

typedef struct EEB {
	unsigned long next_start;
	unsigned long free;
}EEB;	// 占8个字节

#define EEB_size 8

void showEEB(struct EEB *eeb){
	myPrintk(0x7,"EEB(start=0x%x, next=0x%x)\n", eeb, eeb->next_start);
}


void eFPartitionWalkByAddr(unsigned long efpHandler){
	showeFPartition((eFPartition*)efpHandler);
	EEB *eeb=(EEB*)(efpHandler+eFPartition_size);
	while(eeb){
		showEEB(eeb);
		eeb=(EEB*)eeb->next_start;
	}
}


unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n){
	if(perSize%4!=0)
		perSize=((int)perSize/4)*4+1;//4字节对齐
	return eFPartition_size+(EEB_size+perSize)*n;
}

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n){
	if(perSize%4!=0)
		perSize=((int)perSize/4)*4+1;//4字节对齐
	*((eFPartition*)start)=(eFPartition){n,perSize};
	EEB *pos=(EEB*)(start+eFPartition_size);
	for(int i=1;i<=n;i++){
		*pos=(EEB){pos+EEB_size+perSize,1};
		pos=(EEB*)pos->next_start;
	}
	return start;
}


unsigned long eFPartitionAlloc(unsigned long EFPHandler){
	eFPartition *efp=(eFPartition*)EFPHandler;
	EEB *eeb=(EEB*)(efp+eFPartition_size);
	while(eeb){
		if(eeb->free){
			efp->totalN--;
			eeb->free=0;
			return (unsigned long)eeb+EEB_size;
		}
		eeb=(EEB*)eeb->next_start;
	}
	return 0;
}


unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart){
	eFPartition *efp=(eFPartition*)EFPHandler;
	efp->totalN++;
	EEB *eeb=(EEB*)(mbStart-EEB_size);
	eeb->free=1;
}
