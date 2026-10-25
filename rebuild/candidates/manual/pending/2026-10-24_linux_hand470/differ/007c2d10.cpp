// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind3<CThingFilter_IsNot, ShotsToCheckForCollisionWith, CThingFilter_SolidToThing>& filter, vector<CThing*>& result) -- the shot-collision search: walks every thing map overlapping the area, each mapwho layer and each cell of the scaled box, visits every mapwho once per search ID and appends the things that are not excluded, do not evade all hits, interact or collide with things, are a creature/object or driven by a type-0x69 driver, and are not the source thing
// Grade: DIFFER(704v704) -- scheduling only: at the level-loop exit retail reloads edi (the map iterator) before esi (this), ours reloads esi first (every other byte matches)
// Leaf: ?GetAllThingsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C2D10, 704 bytes (incl. padding), `__thiscall`, `ret 0xc`; one caller 0x007C0137.
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CThingFilter_IsNot 0x006A76A0,
//   NCreatureMode::IsToEvadeAllHits 0x0085FA00 (__fastcall), CTCPhysicsBase virtual +0xf0 / +0xe4, the driver's
//   GetInterfaceType (+0x54), CThingFilter_SolidToThing 0x00661640, vector<CThing*>::_M_insert_overflow 0x00436AC0,
//   memmove 0x00BFEAE6.
// Shape: the landed sibling 0x007FDB10 loop with the landed 0x007C2800 Bind3 filter bodies inlined (that out-of-line
//   copy says this search inlines the same body).
#include "engine/CThingSearchTools.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCBase.h"
#include <stddef.h>
#include <new>
struct FalseType {FalseType(){}};
extern "C" void* __cdecl memmove(void*,const void*,size_t); // 0x00bfeae6
__forceinline void* CopyTrivial(const void* first,const void* last,void* result) {
 return last==first ? result : (char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> struct RetailVector {
 T* Start;T* Finish;T* End;
 struct iterator {
  T* Ptr;
  explicit iterator(T* p):Ptr(p){}
  iterator(const iterator& other):Ptr(other.Ptr){}
  __forceinline T& operator*()const{return *Ptr;}
  __forceinline iterator& operator++(){++Ptr;return *this;}
  __forceinline bool operator!=(const iterator& other)const{return Ptr!=other.Ptr;}
  __forceinline iterator& operator+=(int n){Ptr+=n;return *this;}
  __forceinline iterator operator+(int n)const{iterator r(*this);r+=n;return r;}
 };
 __forceinline iterator begin()const{return iterator(Start);}
 __forceinline iterator end()const{return iterator(Finish);}
 __forceinline T& operator[](unsigned int n)const{return Start[n];}
 void _M_insert_overflow(T*,const T&,const FalseType&,unsigned int,bool); // 0x0049c370 for CThing*
 __forceinline void push_back(const T& value){if(Finish!=End){new(Finish)T(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
 __forceinline void clear(){Finish=(T*)CopyTrivial(Finish,Finish,Start);}
};
struct CTCCreatureModeManager {
 struct CModeBase {virtual ~CModeBase();virtual unsigned long GetType()const=0;};
 CModeBase& GetCurrentMode()const; // 0x0073a6e0
};
struct InterfaceEntry {long Type;CTCCreatureModeManager* Component;};
struct InterfaceMap {
 InterfaceEntry* Start;InterfaceEntry* Finish;
 const InterfaceEntry* LowerBound(const long*)const; // 0x004365b0
};
struct CThing {
 unsigned char BeforeComponents[0x20];unsigned long UsingComponents[9];InterfaceMap Components;unsigned char BeforeDriver[0x5c-0x4c];
 struct CTCBase_* PTCDriver; // +0x5c CTCDriverBase*
 struct CTCPhysicsBase_* PhysicsTC; // +0x60
 unsigned char BeforeType[0x6c-0x64];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
// Same-TU non-inlined STLport overflow model. Only GetAllThingsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingVector:RetailVector<CThing*> {
 void _M_insert_overflow(CThing**,CThing* const&,const FalseType&,unsigned int,bool); // 0x00436ac0
 __forceinline void push_back(CThing* const& value){if(Finish!=End){new(Finish)CThing*(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
};
#pragma auto_inline(off)
void ThingVector::_M_insert_overflow(CThing** position,CThing* const& value,const FalseType&,unsigned int count,bool atEnd){
 unsigned int size=Finish-Start;
 unsigned int capacity=size+(size<count?count:size);
 CThing** storage=capacity?(CThing**)malloc(capacity*sizeof(CThing*)):0;
 CThing** out=storage;
 for(CThing** p=Start;p!=position;++p,++out)new(out)CThing*(*p);
 for(unsigned int i=0;i<count;++i,++out)new(out)CThing*(value);
 if(!atEnd)for(CThing** p=position;p!=Finish;++p,++out)new(out)CThing*(*p);
 if(Start)free(Start);
 Start=storage;Finish=out;End=storage+capacity;
}
#pragma auto_inline(on)
struct C2DBoxI {
 long TLX,TLY,BRX,BRY;
 C2DBoxI(){}
 C2DBoxI(const C2DBoxI& v):TLX(v.TLX),TLY(v.TLY),BRX(v.BRX),BRY(v.BRY){}
 __forceinline long Width()const{return BRX-TLX;}
 __forceinline long Height()const{return BRY-TLY;}
};
struct CMapwhoNode {CTCMapwho* PMapwho;CMapwhoNode* PNext;CMapwhoNode* PPrevious;CThingMapCell* PCell;};
struct CThingMapCell {CMapwhoNode* PFirstNode;};
namespace NThingMap {
struct C2DExtentsI {long X,Y;};
struct CMapwhoLevelInfo {long Size;C2DExtentsI Extents;long ScaleShift;long NoThings;};
struct CThingMap {
 long Padding;RetailVector<CMapwhoLevelInfo> MapwhoLevelInfo;
 CThingMapManager* Manager;RetailVector<RetailVector<CThingMapCell> > Cells;
 C2DBoxI Area;long NoThings;
 __forceinline CThingMapCell& GetCell(unsigned char layer,unsigned long x,unsigned long y) {
  const CMapwhoLevelInfo& info=MapwhoLevelInfo[layer];
  return Cells[layer][y*info.Extents.X+x];
 }
};
}
struct CLayerInfo {long Size;long ScaleShift;unsigned long UsedColour,UnusedColour;};
extern long SearchCount_013b89a8;
extern long SearchWork_013b89ac;
extern unsigned long SearchID_013b89b0;
namespace NThingMap {extern long g_NoMapwhoLevels;} // 0x013ba848
extern CLayerInfo LayerInfo_0137dad8[]; // Original variable spelling unrecovered.
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCBase_ { // CTCBase as seen through the thing's driver pointer
 FILL10(0) FILL10(1)
 virtual void v20();
 virtual long GetInterfaceType() const; // slot 21 (+0x54)
};
struct CTCPhysicsBase_ {
 FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4)
 virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
 virtual void v55(); virtual void v56();
 virtual bool GetCollidesWithThings() const;         // slot 57 (+0xe4)
 virtual bool GetCollidesWithThingsOneWay() const;   // slot 58 (+0xe8)
 virtual void v59();
 virtual bool GetInteractsWithThings() const;        // slot 60 (+0xf0)
};
namespace NCreatureMode {
 bool __fastcall IsToEvadeAllHits(const CThing& thing); // 0x0085fa00
}
struct CThingFilter_IsNot {CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a76a0
struct CThingFilter_SolidToThing {CThing* SourceThing;void* PTCSolidToThing;bool operator()(const CThing&)const;}; // 0x00661640
namespace {
struct CThingFilter_ShotsToCheckForCollisionWith {
 __forceinline bool operator()(const CThing& thing)const{
  unsigned long flags=thing.ThingTypeFlags;
  CTCPhysicsBase_* physics=thing.PhysicsTC;
  if(NCreatureMode::IsToEvadeAllHits(thing))return false;
  if(!physics)return false;
  if(!physics->GetInteractsWithThings()&&!physics->GetCollidesWithThings())return false;
  if(flags&0xC)return true;
  CTCBase_* driver=thing.PTCDriver;
  return driver&&driver->GetInterfaceType()==0x69;
 }
};
}
struct Filters {const CThingFilter_IsNot* F1;const CThingFilter_ShotsToCheckForCollisionWith* F2;const CThingFilter_SolidToThing* F3;
 __forceinline bool operator()(const CThing& p)const{return (*F1)(p)&&(*F2)(p)&&(*F3)(p);}
};
struct CThingSearchTools_Calls {
 CWorldMap* WorldMap;
 CThingMapManager* PThingMapManager;
 CThingManager* ThingManager;
 mutable RetailVector<NThingMap::CThingMap*> AreaMaps;
 void GetAllThingMapsInArea(const C2DBoxI&,RetailVector<NThingMap::CThingMap*>&)const;
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI&)const;
 __forceinline C2DBoxI GetMapwhoArea(const NThingMap::CThingMap&,long layer,const C2DBoxI& box)const {
  long shift=LayerInfo_0137dad8[layer].ScaleShift;
  C2DBoxI out;out.TLX=box.TLX>>shift;out.TLY=box.TLY>>shift;
  out.BRX=((box.BRX-1)>>shift)+1;out.BRY=((box.BRY-1)>>shift)+1;return out;
 }
 void GetAllThingsInRoughArea(const C2DBoxI&,const Filters&,ThingVector&)const;
};
void CThingSearchTools_Calls::GetAllThingsInRoughArea(const C2DBoxI& area,const Filters& filter,ThingVector& result)const {
 GetAllThingMapsInArea(area,AreaMaps);
 ++SearchCount_013b89a8;SearchWork_013b89ac=0;++SearchID_013b89b0;
 for(RetailVector<NThingMap::CThingMap*>::iterator m=AreaMaps.begin();m!=AreaMaps.end();++m) {
  NThingMap::CThingMap* map=*m;C2DBoxI bounds=ClipAndConvertWorldMapAreaToLocalArea(*map,area);
  for(long layer=0;layer<NThingMap::g_NoMapwhoLevels;++layer) {
   C2DBoxI scaled=GetMapwhoArea(*map,layer,bounds);
   if(scaled.Width()!=0 && scaled.Height()!=0) {
    long y=scaled.TLY;
    if(y<scaled.BRY) {
     long offset=(unsigned char)layer*sizeof(NThingMap::CMapwhoLevelInfo);
     long cellsOffset=(unsigned char)layer*sizeof(RetailVector<CThingMapCell>);
     for(;y<scaled.BRY;++y) {
     long rowWidth=((NThingMap::CMapwhoLevelInfo*)((char*)map->MapwhoLevelInfo.Start+offset))->Extents.X;
     RetailVector<CThingMapCell>& rowCells=*(RetailVector<CThingMapCell>*)((char*)map->Cells.Start+cellsOffset);
     CThingMapCell* cell=&rowCells[y*rowWidth+scaled.TLX];
     for(long x=scaled.TLX;x<scaled.BRX;++x,++cell) {
      for(CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
       CTCMapwho* who=node->PMapwho;
       if(who->SearchID<SearchID_013b89b0) {
        who->SearchID=SearchID_013b89b0;
        CThing* thing=((CTCBase*)who)->ParentThing;
        if(IsThingOfType<CThing>(thing)){
         CThing* typed=thing;
         if(filter(*typed))result.push_back(typed);
        }
       }
      }
     }
    }
   }
   }
  }
 }
 AreaMaps.clear();
}