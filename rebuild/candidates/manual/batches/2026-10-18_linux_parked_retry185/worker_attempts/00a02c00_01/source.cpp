// CHandleBasedMemoryPool::GrowFreeArea(CHeader*&) PDB00B6DB90. Leaf: GrowFreeArea
#define CHandleBasedMemoryPool CHandleBasedMemoryPoolDonor
#include "engine/CHandleBasedMemoryPool.h"
#undef CHandleBasedMemoryPool
struct BaseClass {BaseClass();virtual ~BaseClass();}; // genuine99A2D0/99A2E0
struct BaseNonCopyable:BaseClass {BaseNonCopyable();virtual ~BaseNonCopyable();}; // genuine99A2F0/99A300
struct Header {Header();void*Resource;long Handle,Size;short Padding;unsigned char Status,Locked;Header**RefAvailable;Header*NextAvailable,*PrevContiguous,*NextContiguous;}; //PDBB6CBD0, native32 no repository header
struct CHandleBasedMemoryPool:BaseNonCopyable {
 CHandleBasedMemoryPool();virtual ~CHandleBasedMemoryPool();
 virtual void MoveResource(void*,long,void*,void*,unsigned long);
 virtual bool CanMoveResource(void*,long);
 virtual void ValidateResource(void*);
 void CombineAreas(Header*,Header*);Header*FindFreeArea(long);void MoveArea(Header*,Header*);void CombineAdjacentAreas(Header*&);
 bool GrowFreeArea(Header*&);
};
// NativeVT129C964 4 genuine slots A03AA0/A02120/A03A40/A03A50; RTTI1356A1C hierarchy HandlePool->BaseNonCopyable->BaseClass.
// ConstructorB6DF10. CanMoveResource fablewinPDB319A010 bool(void*,long), nonconst.
// CombineAreas A01E40/PDBB6CCD0, FindFreeArea A01ED0/B6CDA0, MoveArea A028F0/B6D830, CombineAdjacentAreas A02470/B6D3A0, all void except Find.
bool CHandleBasedMemoryPool::GrowFreeArea(Header*&area){
 Header*next=area->NextContiguous;
 if(next){if(next->RefAvailable){CombineAreas(area,next);return true;}
 if(!next->Locked&&CanMoveResource(next->Resource,next->Handle)){
 area->Locked=1;Header*free=FindFreeArea(next->Size);area->Locked=0;
 if(free&&free!=next&&free!=area){MoveArea(next,free);CombineAdjacentAreas(next);area=next;return true;}
 }}
 next=area->PrevContiguous;
 if(next){if(next->RefAvailable){CombineAreas(next,area);area=next;return true;}
 if(!next->Locked&&CanMoveResource(next->Resource,next->Handle)){
 area->Locked=1;Header*free=FindFreeArea(next->Size);area->Locked=0;
 if(free&&free!=next&&free!=area){MoveArea(next,free);CombineAdjacentAreas(next);area=next;return true;}
 }}return false;
}
