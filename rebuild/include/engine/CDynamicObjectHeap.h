#ifndef FABLE_ENGINE_CDYNAMICOBJECTHEAP_H
#define FABLE_ENGINE_CDYNAMICOBJECTHEAP_H
#include "rebuild_abi.h"
// Both original PDB complete retail records. Debug adds heap-only guard bytes to entries.
class CDynamicObjectHeapBase {
public:
 CDynamicObjectHeapBase();
 virtual ~CDynamicObjectHeapBase();
 static void FABLE_FASTCALL RefCountedReleaseAllHeaps();
 void RefCountedRelease();
 virtual void Release()=0;
protected:
 long AllocCount,TotalAllocatorBlocks;
 bool RefCountReleaseEnabled;
 static CDynamicObjectHeapBase* HeapList;
 CDynamicObjectHeapBase* HeapNext;
};
template<long ObjectSize,long BlockCount> class CDynamicObjectHeap:public CDynamicObjectHeapBase {
public:
 CDynamicObjectHeap(bool);
 virtual ~CDynamicObjectHeap();
 void* Alloc(long debug_check_size){
  CObjectEntry* entry=FreeList;
  if(!entry){if(AllowHeapToGrow){CreateMoreBlocks();entry=FreeList;}else return 0;}
  ++AllocCount;FreeList=entry->NextEntry;return entry;
 }
 void Free(void*);
private:
 virtual void Release();
 long GetTotalBytesAllocated()const;
 long GetTotalBytesFree()const;
 void CreateMoreBlocks();
 struct CObjectEntry {CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};
 struct CObjectAllocatorBlockEntry {CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};
 CObjectEntry* FreeList;
 CObjectAllocatorBlockEntry StartBlock;
 bool AllowHeapToGrow;
};
#endif
