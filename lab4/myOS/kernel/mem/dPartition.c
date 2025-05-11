#include "../../include/myPrintk.h"
#include "../../include/mem.h"
//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition{
	unsigned long size;
} dPartition;	//共占4个字节

#define dPartition_size ((unsigned long)0x4)

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x)\n", dp, dp->size);
}

// EMB 是每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB{
	unsigned long size;
	unsigned long nextStart;
	unsigned long free;
} EMB;	//共占12个字节

unsigned long KMemHandler,UMemHandler;

#define EMB_size ((unsigned long)0xC)

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}


unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	*((dPartition*)start)=(dPartition){totalSize-dPartition_size-EMB_size};
	*((EMB*)(start+dPartition_size))=(EMB){totalSize-dPartition_size-EMB_size,0,1};
	return start;
}


void dPartitionWalkByAddr(unsigned long dp){
	showdPartition((dPartition*)dp);
	EMB* emb=(EMB*)(dp+dPartition_size);
	while(emb){
		showEMB(emb);
		emb=(EMB*)emb->nextStart;
	}
}


//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	dPartition *dpt=(dPartition*)dp;
	EMB* emb=(EMB*)(dp+dPartition_size);
	while(emb){
		if(emb->free&&emb->size>=size){
			if(emb->size-size>EMB_size){
				EMB* newemb=(EMB*)((unsigned long)emb+EMB_size+size);
				newemb->size=emb->size-EMB_size-size;
				newemb->nextStart=emb->nextStart;
				newemb->free=1;
				emb->nextStart=(unsigned long)newemb;
				emb->size=size;
				dpt->size-=EMB_size;
			}
			emb->free=0;
			dpt->size-=size;
			return (unsigned long)emb+EMB_size;
		}
		emb=(EMB*)(emb->nextStart);
	}
	return 0;
}


unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	dPartition *dpt=(dPartition*)dp;
	EMB *emb=(EMB*)(start-EMB_size);
	dpt->size+=emb->size;
	emb->free=1;
	if(emb->nextStart&&((EMB*)emb->nextStart)->free){
		emb->size+=EMB_size+((EMB*)emb->nextStart)->size;
		emb->nextStart=((EMB*)emb->nextStart)->nextStart;
		dpt->size+=EMB_size;
	}
	return 1;
}


// 进行封装，此处默认firstfit分配算法，当然也可以使用其他fit，不限制。
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}