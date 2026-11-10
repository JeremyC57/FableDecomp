#ifndef FABLE_ENGINE_CMEMORYALLOCATOR_H
#define FABLE_ENGINE_CMEMORYALLOCATOR_H
// Declaration-only API projection of the original class: no object construction, sizeof, or field access. This is not a storage-layout definition.
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
#include "rebuild_abi.h"
#endif
class CMemoryAllocator {
protected:
 static CMemoryAllocator* PMemoryAllocator;
 static void __fastcall Create();
 static void __fastcall EnterSynchronisation();
 static void __fastcall LeaveSynchronisation();
 void* InternalAllocate(long,unsigned long,bool);
public:
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
 // Original FableWin Get018a4e40; OnFailedAllocation030bf3a0.
 static CMemoryAllocator* FABLE_FASTCALL Get(){return PMemoryAllocator;}
 void OnFailedAllocation();
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
 // Opt-in (chunk 892, row 0x00B482A0): Ego_r.pdb ?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z (0x009F2BE0, landed) and
 // ?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z (0x009F2B20, landed), public static __fastcall. Declarations only.
 static void* FABLE_FASTCALL AllocateAligned(unsigned long size,long alignment);
 static void FABLE_FASTCALL DeallocateAligned(void* p);
#endif
 static void* __fastcall Allocate(unsigned long size,unsigned long flags) {
  if(!PMemoryAllocator)Create();
  EnterSynchronisation();
  void* p=PMemoryAllocator->InternalAllocate(size,flags,false);
  LeaveSynchronisation();return p;
 }
};
#endif
