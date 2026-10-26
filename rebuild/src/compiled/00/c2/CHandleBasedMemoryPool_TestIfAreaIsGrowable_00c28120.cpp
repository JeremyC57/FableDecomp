// CHandleBasedMemoryPool::TestIfAreaIsGrowable(CHeader*,int) -- test adjacent movable blocks.
// Retail 00c28120,210 bytes. Genuine virtual CanMoveResource native slot1.
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
 int TestIfAreaIsGrowable(CHeader*,int);
 int GrowFreeArea(CHeader*&);
 void RemoveFreeArea(CHeader*); // 00c27f50
 CHeader* FindFreeArea(int); // 00c27fd0
 void InsertFreeArea(CHeader*); // 00c27ef0
 void MoveArea(CHeader*,CHeader*); // 00c28580
 CHeader* CombineAdjacentAreas(CHeader*); // 00c282e0
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CHeader)==32);
FABLE_STATIC_ASSERT(offsetof(CHeader,PrevContiguous)==24);
FABLE_STATIC_ASSERT(offsetof(CHeader,NextContiguous)==28);
#endif

int CHandleBasedMemoryPool::TestIfAreaIsGrowable(CHeader* area,int required) {
 char* start=(char*)(area+1);
 char* end=start+area->Size+area->Padding;
 if(end-start>=required)return 1;
 CHeader* header=area->PrevContiguous;
 while(header) {
  if(header->Status!='N' && (header->Locked || !CanMoveResource(header->Resource,header->Handle)))break;
  start=(char*)(header+1);
  if(end-start>=required)return 1;
  header=header->PrevContiguous;
 }
 header=area->NextContiguous;
 while(header) {
  if(header->Status!='N' && (header->Locked || !CanMoveResource(header->Resource,header->Handle)))break;
  if((char*)(header+1)+header->Size+header->Padding-start>=required)return 1;
  header=header->NextContiguous;
 }
 return 0;
}