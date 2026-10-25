// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind4<CanBeUsedAsCover, not1(IsOccupiableOccupied), IsNot, In2DRange>& filter, vector<CThing*>& result) -- the advance-in-cover search: walks every thing map overlapping the area, each mapwho layer and each cell of the scaled box, visits every mapwho once per search ID and appends the things that can be used as cover, are not occupied, are not the excluded thing and lie in 2D range
// Leaf: ?GetAllThingsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009793D0, 640 bytes (incl. padding), `__fastcall`, `ret 0xc`; one caller
//   CAIStateGroup_CreatureAdvanceInCover::GetCoverThingList 0x00978FD0 (landed: builds this CFilterBind4 and a C2DBoxI).
// Label: the catalog name `GetPBestThingInRoughArea<CFilter_Bind4<IsAICreatureAwareOf, IsUnconscious, IsAnAlly, ...>,
//   CThingCompare_Nearest, ...>` is propagated: the body is a GetAllThingsInRoughArea over plain CThing (mask 0, the
//   folded `xor dl,dl` test) calling CThingFilter_CanBeUsedAsCover 0x006A7AB0, the negated CThingFilter_IsOccupiableOccupied
//   0x006A8A30, CThingFilter_IsNot 0x006A76A0 and CThingFilter_In2DRange 0x006A76B0, appending to a vector<CThing*>.
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, the four filters,
//   vector<CThing*>::_M_insert_overflow 0x00436AC0, memmove 0x00BFEAE6 (the inlined clear()).
// Shape: the landed sibling 0x007A3CA0 (same search, other filters); the pushed pointer is a named local assigned before
//   the filter chain (its store sits before the first filter call, as in the landed 0x007FDB10).
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
struct CThing {
 unsigned char BeforeType[0x6c];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
struct CThing;
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
struct CThingPhysical:CThing {enum {ThingTypeMask=3};};
struct CThingGameObject:CThingPhysical {enum {ThingTypeMask=7};};
struct CThingCreatureBase:CThingGameObject {enum {ThingTypeMask=15};};
struct CThingAICreature:CThingCreatureBase {enum {ThingTypeMask=31};};
struct CThingObject:CThingGameObject {enum {ThingTypeMask=0x807};};
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
struct CThingFilter_CanBeUsedAsCover {bool operator()(const CThing&)const;}; // 0x006a7ab0
struct CThingFilter_IsOccupiableOccupied {bool operator()(const CThing&)const;}; // 0x006a8a30
struct CNotOccupied { // std::unary_negate<CThingFilter_IsOccupiableOccupied>
 CThingFilter_IsOccupiableOccupied Predicate;
 __forceinline bool operator()(const CThing& p)const{return !Predicate(p);}
};
struct CThingFilter_IsNot {CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a76a0
struct CThingFilter_In2DRange {float Pos[3];float Range;bool operator()(const CThing&)const;}; // 0x006a76b0
struct Filters {
 const CThingFilter_CanBeUsedAsCover* F0;
 const CNotOccupied* F1;
 const CThingFilter_IsNot* F2;
 const CThingFilter_In2DRange* F3;
 __forceinline bool operator()(const CThing& p)const{return (*F0)(p)&&(*F1)(p)&&(*F2)(p)&&(*F3)(p);}
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
    for(long y=scaled.TLY;y<scaled.BRY;++y) {
     CThingMapCell* cell=&map->GetCell((unsigned char)layer,scaled.TLX,y);
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
 AreaMaps.clear();
}