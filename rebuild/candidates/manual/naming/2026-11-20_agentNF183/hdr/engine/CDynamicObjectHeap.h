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
#ifdef FABLE_RETAIL_CDYNAMICOBJECTHEAP_FREE_INLINE
 // NF183 opt-in (2026-11-20, q4502 / q4505; rows 0x00B49AA0 / 0x00B49A40 / 0x00B49FE0 / 0x00B47980): FableWin.pdb
 // ?Free@?$CDynamicObjectHeap@...@@QAEXPAX@Z, inline in retail: AllocCount decremented, the entry pushed on FreeList, and
 // RefCountedRelease (0x00F36CA0) when the count reaches 0 with release enabled (the landed rows' local models had this body).
 void Free(void* object){
  --AllocCount;((CObjectEntry*)object)->NextEntry=FreeList;FreeList=(CObjectEntry*)object;
  if(AllocCount==0&&RefCountReleaseEnabled)RefCountedRelease();
 }
#else
 void Free(void*);
#endif
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
