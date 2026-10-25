// CThingSearchTools::GetAllTCsInRoughArea(const C2DBoxI& area, const CThingFilter_IsInQuestOrNullQuest& filter, vector<CTCInterface61*>& result) -- appends the interface-61 (0x3d) component of every thing linked into a mapwho cell overlapping the area (each thing once per search) that has the component and is in the quest (or no quest)
// Grade: DIFFER(686v686) -- scheduling only: at the level-loop exit retail reloads edi (the map iterator) before esi (this), ours reloads esi first (the order flips against 0x0084BA50, which differs only in the filter)
// Leaf: ?GetAllTCsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00518340, 688 bytes (incl. padding), `__thiscall`, `ret 0xc`; one caller 0x0050AADB.
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CThingFilter_IsInQuestOrNullQuest
//   0x006A7C60, vector<T*>::_M_insert_overflow 0x00514F40, memmove 0x00BFEAE6.
// Label: the catalog name `GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<IsPerceivingThing, IsNot>, list<...> >` is
//   propagated: this instance looks up interface 0x3d (bit 29 of the second component word), takes the single
//   IsInQuestOrNullQuest filter by reference and appends to a vector; the component class is named descriptively.
// Shape: as 0x0084BA50 (the landed 0x0091BA40 search with a vector push_back and its same-TU overflow stand-in).
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
struct InterfaceEntry {long Type;void* Component;};
struct InterfaceMap {
 InterfaceEntry* Start;InterfaceEntry* Finish;
 __forceinline void* Find(long type)const{
  InterfaceEntry* first=Start;InterfaceEntry* last=Finish;long count=last-first;
  while(count>0){long half=count/2;InterfaceEntry* middle=first+half;
   if(middle->Type<type){first=middle+1;count-=half+1;}else count=half;
  }
  return ((first==last || type<first->Type) ? last : first)->Component;
 }
};
struct CThing {
 unsigned char BeforeComponents[0x20];unsigned long UsingComponents[9];InterfaceMap Components;
 unsigned char BeforeType[0x6c-0x4c];unsigned long ThingTypeFlags;
 enum{ThingTypeMask=0};
 __forceinline bool HasComponent(long type)const{return (UsingComponents[type/32]&(1UL<<(type%32)))!=0;}
};
struct CTCInterface61;
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
// Same-TU non-inlined STLport overflow model. Only GetAllTCsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingVector:RetailVector<CTCInterface61*> {
 void _M_insert_overflow(CTCInterface61**,CTCInterface61* const&,const FalseType&,unsigned int,bool); // 0x00514f40
 __forceinline void push_back(CTCInterface61* const& value){if(Finish!=End){new(Finish)CTCInterface61*(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
};
#pragma auto_inline(off)
void ThingVector::_M_insert_overflow(CTCInterface61** position,CTCInterface61* const& value,const FalseType&,unsigned int count,bool atEnd){
 unsigned int size=Finish-Start;
 unsigned int capacity=size+(size<count?count:size);
 CTCInterface61** storage=capacity?(CTCInterface61**)malloc(capacity*sizeof(CTCInterface61*)):0;
 CTCInterface61** out=storage;
 for(CTCInterface61** p=Start;p!=position;++p,++out)new(out)CTCInterface61*(*p);
 for(unsigned int i=0;i<count;++i,++out)new(out)CTCInterface61*(value);
 if(!atEnd)for(CTCInterface61** p=position;p!=Finish;++p,++out)new(out)CTCInterface61*(*p);
 if(Start)free(Start);
 Start=storage;Finish=out;End=storage+capacity;
}
#pragma auto_inline(on)
struct CThingCreatureBase:CThing {};
struct CThingAICreature:CThingCreatureBase {};
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
struct CThingFilter_IsInQuestOrNullQuest {const void* QuestSectionName;bool operator()(const CThing&)const;}; // 0x006a7c60
typedef CThingFilter_IsInQuestOrNullQuest Filters;
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
 void GetAllTCsInRoughArea(const C2DBoxI&,const Filters&,ThingVector&)const;
};
void CThingSearchTools_Calls::GetAllTCsInRoughArea(const C2DBoxI& area,const Filters& filter,ThingVector& result)const {
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
        if(thing && (thing->UsingComponents[1]&(1UL<<29))){
         CTCInterface61* typed=static_cast<CTCInterface61*>(thing->Components.Find(61));
         if(filter(*thing))result.push_back(typed);
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