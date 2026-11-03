#ifndef FABLE_ENGINE_CMEMORYALLOCATOR_H
#define FABLE_ENGINE_CMEMORYALLOCATOR_H
// Declaration-only API projection of the original class: no object construction, sizeof, or field access. This is not a storage-layout definition.
class CMemoryAllocator {
protected:
 static CMemoryAllocator* PMemoryAllocator;
 static void __fastcall Create();
 static void __fastcall EnterSynchronisation();
 static void __fastcall LeaveSynchronisation();
 void* InternalAllocate(long,unsigned long,bool);
public:
 static void* __fastcall Allocate(unsigned long size,unsigned long flags) {
  if(!PMemoryAllocator)Create();
  EnterSynchronisation();
  void* p=PMemoryAllocator->InternalAllocate(size,flags,false);
  LeaveSynchronisation();return p;
 }
};
#endif
