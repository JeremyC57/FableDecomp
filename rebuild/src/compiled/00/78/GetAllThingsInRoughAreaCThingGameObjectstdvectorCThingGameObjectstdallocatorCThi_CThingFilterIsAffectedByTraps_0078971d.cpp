// 0078971d CThingSearchTools::GetAllThingsInRoughArea, 390 retail bytes.
// Layout: shared CThingSearchTools donor has a 16-byte debug vector at +0xc;
// retail uses the first three words (12-byte STLport vector). No trailing
// donor member is accessed. CThing's retail type flags are +0x6c (PDB +0x7c).
// Local helper/global names carrying addresses remain descriptive pending PDB proof.
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
 T* erase(T*,T*); // 0x00436a80 for AreaMaps
 __forceinline void clear(){erase(Start,Finish);}
};
class CThing { public:
 unsigned char BeforeType[0x6c];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
class CThingGameObject;
// Same-TU non-inlined STLport overflow model. Only GetAllThingsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingVector:RetailVector<CThingGameObject*> {
 void _M_insert_overflow(CThingGameObject**,CThingGameObject* const&,const FalseType&,unsigned int,bool); // 0x00788ffc
 void push_back(CThingGameObject* const&); // 0x0078915c
};
#pragma auto_inline(off)
void ThingVector::_M_insert_overflow(CThingGameObject** position,CThingGameObject* const& value,const FalseType&,unsigned int count,bool atEnd){
 unsigned int size=Finish-Start;
 unsigned int capacity=size+(size<count?count:size);
 CThingGameObject** storage=capacity?(CThingGameObject**)malloc(capacity*sizeof(CThingGameObject*)):0;
 CThingGameObject** out=storage;
 for(CThingGameObject** p=Start;p!=position;++p,++out)new(out)CThingGameObject*(*p);
 for(unsigned int i=0;i<count;++i,++out)new(out)CThingGameObject*(value);
 if(!atEnd)for(CThingGameObject** p=position;p!=Finish;++p,++out)new(out)CThingGameObject*(*p);
 if(Start)free(Start);
 Start=storage;Finish=out;End=storage+capacity;
}
#pragma auto_inline(on)
class CThingPhysical:public CThing { public:enum {ThingTypeMask=3};};
class CThingGameObject:public CThingPhysical { public:enum {ThingTypeMask=7};};
class CThingCreatureBase:public CThingGameObject { public:enum {ThingTypeMask=15};};
class CThingAICreature:public CThingCreatureBase { public:enum {ThingTypeMask=31};};
class CThingObject:public CThingGameObject { public:enum {ThingTypeMask=0x807};};
class C2DBoxI { public:
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
class CMapwhoLevelInfo { public:long Size;C2DExtentsI Extents;long ScaleShift;long NoThings;};
class CThingMap { public:
 long Padding;RetailVector<CMapwhoLevelInfo> MapwhoLevelInfo;
 CThingMapManager* Manager;RetailVector<RetailVector<CThingMapCell> > Cells;
 C2DBoxI Area;long NoThings;
 CThingMapCell& GetCell(unsigned char,unsigned long,unsigned long); // 0x00436830
};
}
struct CLayerInfo {long Size;long ScaleShift;unsigned long UsedColour,UnusedColour;};
extern long SearchCount_013b89a8;
extern long SearchWork_013b89ac;
extern unsigned long SearchID_013b89b0;
namespace NThingMap {extern long g_NoMapwhoLevels;} // 0x013ba848
extern CLayerInfo LayerInfo_0137dad8[]; // Original variable spelling unrecovered.
namespace NCreatureMode {bool __fastcall IsAbleToBeAttacked(const CThing&);} // 0x0085fb30
class CThingFilter_IsNotList { public:void* NotList;void* NotList2;bool operator()(const CThing&)const;}; // 0x006a7eb0
struct CThingFilter_IsAbleToBeAttacked {__forceinline bool operator()(const CThing& p)const{return NCreatureMode::IsAbleToBeAttacked(p);}};
class CThingFilter_HasTC { public:long Type;bool operator()(const CThing&)const;}; // 0x006a80a0
struct CThingFilter_IsAffectedByTraps {bool operator()(const CThing&)const;}; // 0x007892ea
struct Filters {CThingFilter_IsNotList* F0;CThingFilter_IsAbleToBeAttacked* F1;CThingFilter_HasTC* F2;CThingFilter_IsAffectedByTraps* F3;
 __forceinline bool operator()(const CThingGameObject& p)const{return (*F0)(p)&&(*F1)(p)&&!(*F2)(p)&&(*F3)(p);}
};
struct CThingSearchTools_Calls {
 CWorldMap* WorldMap;
 CThingMapManager* PThingMapManager;
 CThingManager* ThingManager;
 mutable RetailVector<NThingMap::CThingMap*> AreaMaps;
 void GetAllThingMapsInArea(const C2DBoxI&,RetailVector<NThingMap::CThingMap*>&)const;
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI&)const;
 C2DBoxI GetMapwhoArea(const NThingMap::CThingMap&,long,const C2DBoxI&)const; // 0x00436500
 void GetAllThingsInRoughArea(const C2DBoxI&,const Filters&,ThingVector&)const;
};
// Actual out-of-line helper body exposes its non-escaping result and input references.
// This helper is a semantic dependency; only the assigned search leaf is graded.
#pragma auto_inline(off)
C2DBoxI CThingSearchTools_Calls::GetMapwhoArea(const NThingMap::CThingMap&,long layer,const C2DBoxI& b)const{
 long shift=LayerInfo_0137dad8[layer].ScaleShift;C2DBoxI r;r.TLX=b.TLX>>shift;r.TLY=b.TLY>>shift;r.BRX=((b.BRX-1)>>shift)+1;r.BRY=((b.BRY-1)>>shift)+1;return r;
}
#pragma auto_inline(on)
void CThingSearchTools_Calls::GetAllThingsInRoughArea(const C2DBoxI& area,const Filters& filter,ThingVector& result)const {
 GetAllThingMapsInArea(area,AreaMaps);
 ++SearchCount_013b89a8;SearchWork_013b89ac=0;++SearchID_013b89b0;
 for(RetailVector<NThingMap::CThingMap*>::iterator m=AreaMaps.begin();m!=AreaMaps.end();++m) {
  NThingMap::CThingMap* map=*m;C2DBoxI bounds=ClipAndConvertWorldMapAreaToLocalArea(*map,area);
  for(long layer=0;layer<NThingMap::g_NoMapwhoLevels;++layer) {
   const C2DBoxI scaled=GetMapwhoArea(*map,layer,bounds);
   const long width=scaled.Width();
   if(width!=0 && scaled.Height()!=0) {
    for(long y=scaled.TLY;y<scaled.BRY;++y) {
     CThingMapCell* cell=&map->GetCell((unsigned char)layer,scaled.TLX,y);
     if(scaled.TLX<scaled.BRX){long remaining=width;do {
      for(CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
       CTCMapwho* who=node->PMapwho;
       if(who->SearchID<SearchID_013b89b0) {
        who->SearchID=SearchID_013b89b0;
        CThing* thing=((CTCBase*)who)->ParentThing;
        if(IsThingOfType<CThingGameObject>(thing)){
         CThingGameObject* typed=static_cast<CThingGameObject*>(thing);if(filter(*typed))result.push_back(typed);
        }
       }
      }
      ++cell;
     }while(--remaining);}
    }
   }
  }
 }
 AreaMaps.clear();
}