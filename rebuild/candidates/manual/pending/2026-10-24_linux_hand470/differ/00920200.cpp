// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CanBeUsedAsCover, IsNotInList>& filter, vector<CThingObject*>& result) -- the cover search: walks every thing map overlapping the area, each mapwho layer and every cell of the scaled box, visits each mapwho once per search ID and appends the objects (type mask 0x807) that can be used as cover and are not already in the exclusion list
// Grade: DIFFER(640v640) -- scheduling only: at the node-loop exit retail reloads the spilled box bound into edi first, then esi/edx; ours reloads esi, edx, then edi (everything else, incl. registers and frame, matches)
// Leaf: ?GetAllThingsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00920200, 640 bytes (incl. padding), `__thiscall`, `ret 0xc`; one caller 0x0091F2A5.
// Label: the catalog name `GetAllThingsInRoughArea<CThingGameObject const, vector<CIntelligentPointer<CThing const> >,
//   CThingFilter_None>` is propagated: the body tests the CThingObject type mask (0x807), calls
//   CThingFilter_CanBeUsedAsCover::operator() 0x006A7AB0 and an STLport find 0x0091F680 over the second filter's vector
//   (accepting a thing that is NOT found), and appends raw 4-byte pointers (overflow 0x0091FFE0).
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, 0x006A7AB0, 0x0091F680,
//   vector<CThingObject*>::_M_insert_overflow 0x0091FFE0, memmove 0x00BFEAE6 (the inlined clear()).
// Shape: the landed sibling 0x007FDB10 (same template, other filters) verbatim apart from the type and the filter pair;
//   the not-in-list filter copies the thing pointer into its own local right before the find call (the find takes the
//   value by reference) and homes the iterator tag in the dead `area` argument slot.
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
 unsigned char BeforeComponents[0x20];unsigned long UsingComponents[9];InterfaceMap Components;unsigned char BeforeType[0x6c-0x4c];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
struct CThingObject;
// Same-TU non-inlined STLport overflow model. Only GetAllThingsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingVector:RetailVector<CThingObject*> {
 void _M_insert_overflow(CThingObject**,CThingObject* const&,const FalseType&,unsigned int,bool); // 0x0091ffe0
 __forceinline void push_back(CThingObject* const& value){if(Finish!=End){new(Finish)CThingObject*(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
};
#pragma auto_inline(off)
void ThingVector::_M_insert_overflow(CThingObject** position,CThingObject* const& value,const FalseType&,unsigned int count,bool atEnd){
 unsigned int size=Finish-Start;
 unsigned int capacity=size+(size<count?count:size);
 CThingObject** storage=capacity?(CThingObject**)malloc(capacity*sizeof(CThingObject*)):0;
 CThingObject** out=storage;
 for(CThingObject** p=Start;p!=position;++p,++out)new(out)CThingObject*(*p);
 for(unsigned int i=0;i<count;++i,++out)new(out)CThingObject*(value);
 if(!atEnd)for(CThingObject** p=position;p!=Finish;++p,++out)new(out)CThingObject*(*p);
 if(Start)free(Start);
 Start=storage;Finish=out;End=storage+capacity;
}
#pragma auto_inline(on)
struct CThingPhysical:CThing {enum {ThingTypeMask=3};};
struct CThingGameObject:CThingPhysical {enum {ThingTypeMask=7};};
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
struct CRandomAccessIteratorTag {CRandomAccessIteratorTag(){}};
// Same-TU stand-in for the STLport find 0x0091f680: it never reads the tag (so VC homes the tag temporary on the live
// `area` argument slot as retail does) and it ends in an external call, so the caller cannot assume the stand-in
// preserves ecx/edx (VC7.1 uses a same-TU callee's register usage; retail keeps `last` in edi across the call).
void FindTrace(const CThing**); // never defined: only makes the stand-in clobber the scratch registers
#pragma auto_inline(off)
__declspec(noinline) const CThing** __fastcall FindThing(const CThing** first,const CThing** last,const CThing* const& value,const CRandomAccessIteratorTag&) { // 0x0091f680
 while(first!=last&&*first!=value)++first;
 FindTrace(first);
 return first;
}
#pragma auto_inline(on)
struct CThingFilter_CanBeUsedAsCover {bool operator()(const CThing&)const;}; // 0x006a7ab0
struct CThingFilter_IsNotInList {
 RetailVector<const CThing*> List;
 __forceinline bool operator()(const CThing& thing)const{const CThing** last=List.Finish;const CThing* p=&thing;return FindThing(List.Start,last,p,CRandomAccessIteratorTag())==last;}
};
struct Filters {const CThingFilter_CanBeUsedAsCover* F1;const CThingFilter_IsNotInList* F2;
 __forceinline bool operator()(const CThingObject& p)const{return (*F1)(p)&&(*F2)(p);}
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
        if(IsThingOfType<CThingObject>(thing)){
         CThingObject* typed=static_cast<CThingObject*>(thing);
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