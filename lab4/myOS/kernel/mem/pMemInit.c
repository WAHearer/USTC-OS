#include "../../include/myPrintk.h"
#include "../../include/mem.h"
unsigned long pMemStart;  // 可用的内存的起始地址
unsigned long pMemSize;  // 可用的大小

void memTest(unsigned long start, unsigned long grainSize){
	if(start<=1024*1024){
		myPrintk(0x7,"illegal starting address.\n");
		return;
	}
	if(grainSize<4){
		myPrintk(0x7,"grain size is too small.");
		return;
	}
	unsigned long base=start;
	unsigned short *p;
	unsigned short temp;
	while(1){
		p=(unsigned short*)base;
		temp=*p;
		*p=0xAA55;
		if(*p!=0xAA55){
			*p=temp;
			base+=grainSize;
			continue;
		}
		*p=0x55AA;
		if(*p!=0x55AA){
			*p=temp;
			base+=grainSize;
			continue;
		}
		*p=temp;
		p=(unsigned short*)(base+grainSize-2);
		temp=*p;
		*p=0xAA55;
		if(*p!=0xAA55){
			*p=temp;
			base+=grainSize;
			continue;
		}
		*p=0x55AA;
		if(*p!=0x55AA){
			*p=temp;
			base+=grainSize;
			continue;
		}
		*p=temp;
		break;
	}
	pMemStart=base;
	while(1){
		p=(unsigned short*)base;
		temp=*p;
		*p=0xAA55;
		if(*p!=0xAA55){
			*p=temp;
			break;
		}
		*p=0x55AA;
		if(*p!=0x55AA){
			*p=temp;
			break;
		}
		*p=temp;
		p=(unsigned short*)(base+grainSize-2);
		temp=*p;
		*p=0xAA55;
		if(*p!=0xAA55){
			*p=temp;
			break;
		}
		*p=0x55AA;
		if(*p!=0x55AA){
			*p=temp;
			break;
		}
		*p=temp;
		pMemSize+=grainSize;
		base+=grainSize;
	}
	myPrintk(0x7,"MemStart: %x  \n", pMemStart);
	myPrintk(0x7,"MemSize:  %x  \n", pMemSize);
	
}

extern unsigned long _end;
void pMemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}
	
	KMemHandler=dPartitionInit(pMemStart,0x400000-pMemStart);
	UMemHandler=dPartitionInit(0x400000,pMemStart+pMemSize-0x400000);
}
