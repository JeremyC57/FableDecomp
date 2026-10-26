// CHandleBasedMemoryPool::GrowFreeArea(CHeader*&) -- relocate an adjacent block into a free area.
// Retail00c286f0,231 bytes. Labels incorrectly call this PartialDefragment.
// c27f50 RemoveFreeArea,c27fd0 FindFreeArea,c27ef0 InsertFreeArea,
// c28580 MoveArea,c282e0 CombineAdjacentAreas, as established by their bodies.
// Actual donor header retained for reference; native virtual slots differ: retail
// MoveResource at0, CanMoveResource at1; donor slot0 is deleting destructor.
#define CHandleBasedMemoryPool CHandleBasedMemoryPoolDonor
#include "engine/CHandleBasedMemoryPool.h"
#undef CHandleBasedMemoryPool
struct CHeader {
 void* Resource; int Handle,Size; short Padding; unsigned char Status,Locked;
 CHeader** RefAvailable; CHeader *NextAvailable,*PrevContiguous,*NextContiguous;
};
class CHandleBasedMemoryPool { public:
 virtual void MoveResource(void*,int,void*,void*,unsigned int); // native slot0
 virtual int CanMoveResource(void*,int); // native slot1; full EAX tested
 // Native data agrees with donor through FailureHandler; four native-only
 // unreconciled bytes precede FreeLists, independently used at c27ef0/c28050.
 void* Buffer;void* UnalignedBuffer;int PoolSize,AllocatedSize,NumAllocations,LockCount,LargestFreeArea;
 unsigned int MemoryFlags;bool MultiThreaded,ShutdownFlag,EnableDefrag,EnableFullDefragIfPartialFails;
 unsigned int MaxFreeSizeBeforeDefrag,MaxFreeSizeBeforeFailure;int MaxBlocksToTryOnPartialDefrag;
 CFailedAllocationHandler* FailureHandler;unsigned char NativePoolExtension[4];CHeader* FreeLists[256];
 int TestIfAreaIsGrowable(CHeader*,int);
 int GrowFreeArea(CHeader*&);
 void RemoveFreeArea(CHeader*); // 00c27f50
 CHeader* FindFreeArea(int); // 00c27fd0
 __declspec(noinline) void InsertFreeArea_00c27ef0(CHeader*); // 00c27ef0
 void MoveArea(CHeader*,CHeader*); // 00c28580
 CHeader* CombineAdjacentAreas(CHeader*); // 00c282e0
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CHeader)==32);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,FreeLists)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CHeader,PrevContiguous)==24);
FABLE_STATIC_ASSERT(offsetof(CHeader,NextContiguous)==28);
#endif

// Genuine adjacent callee implementation; compiler can see its register effects.
// Mirrors independently landed c27ef0 and its retail body; no shortened fixture stub.
void CHandleBasedMemoryPool::InsertFreeArea_00c27ef0(CHeader* area) {
 area->Size+=area->Padding;
 int size=area->Size;unsigned int bucket=(unsigned int)size>>9;
 area->Padding=0;if(bucket>=255)bucket=255;
 CHeader** head=&FreeLists[bucket];area->RefAvailable=head;
 CHeader* old=*head;area->NextAvailable=old;
 if(old)old->RefAvailable=&area->NextAvailable;
 *head=area;if(LargestFreeArea<area->Size)LargestFreeArea=area->Size;
}
int CHandleBasedMemoryPool::GrowFreeArea(CHeader*& area) {
 CHeader* p=area->NextContiguous;
 if(p && !p->Locked && CanMoveResource(p->Resource,p->Handle)) {
  RemoveFreeArea(area);
  CHeader* freeArea=FindFreeArea(p->Size);
  area->Status='N';
  InsertFreeArea_00c27ef0(area);
  if(freeArea) {
   MoveArea(p,freeArea);
   CombineAdjacentAreas(p);
   return 1;
  }
 }
 p=area->PrevContiguous;
 if(p && !p->Locked && CanMoveResource(p->Resource,p->Handle)) {
  RemoveFreeArea(area);
  CHeader* freeArea=FindFreeArea(p->Size);
  area->Status='N';
  InsertFreeArea_00c27ef0(area);
  if(freeArea) {
   MoveArea(p,freeArea);
   area=CombineAdjacentAreas(p);
   return 1;
  }
 }
 return 0;
}