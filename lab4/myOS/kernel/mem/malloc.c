#include "../../include/mem.h"

unsigned long kmalloc(unsigned long size){
    return dPartitionAlloc(KMemHandler,size);

}

unsigned long kfree(unsigned long start){
    return dPartitionFree(KMemHandler,start);
}

unsigned long malloc(unsigned long size){
    // 调用实现的dPartition或者是ePartition的alloc
    return dPartitionAlloc(UMemHandler,size);

}

unsigned long free(unsigned long start){
    // 调用实现的dPartition或者是ePartition的free
    return dPartitionFree(UMemHandler,start);
}

