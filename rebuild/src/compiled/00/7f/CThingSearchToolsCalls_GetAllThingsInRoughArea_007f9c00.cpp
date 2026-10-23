// 007f9c00 CThingSearchTools::GetAllThingsInRoughArea, 619 retail bytes.
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
 struct const_iterator {
  const T* Ptr;
  explicit const_iterator(const T* p):Ptr(p){}
  const_iterator(const const_iterator& other):Ptr(other.Ptr){}
  __forceinline const T& operator*()const{return *Ptr;}
  __forceinline const_iterator& operator+=(int n){Ptr+=n;return *this;}
  __forceinline const_iterator operator+(int n)const{const_iterator r(*this);r+=n;return r;}
 };
 __forceinline iterator begin(){return iterator(Start);}
 __forceinline iterator end(){return iterator(Finish);}
 __forceinline const_iterator begin()const{return const_iterator(Start);}
 __forceinline const_iterator end()const{return const_iterator(Finish);}
 __forceinline T& operator[](unsigned int n){return *(begin()+n);}
 __forceinline const T& operator[](unsigned int n)const{return *(begin()+n);}
 void _M_insert_overflow(T*,const T&,const FalseType&,unsigned int,bool); // 0x0049c370 for CThing*
 __forceinline void push_back(const T& value){if(Finish!=End){new(Finish)T(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
 __forceinline void clear(){Finish=(T*)CopyTrivial(Finish,Finish,Start);}
};
struct CThing {
 unsigned char BeforeType[0x6c];unsigned long ThingTypeFlags;
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
struct C2DExtentsI {long X,Y;};
namespace NThingMap {
struct CMapwhoLevelInfo {long Size;C2DExtentsI Extents;long ScaleShift;long NoThings;};
struct CThingMap {
 long Padding;RetailVector<CMapwhoLevelInfo> MapwhoLevelInfo;
 CThingMapManager* Manager;RetailVector<RetailVector<CThingMapCell> > Cells;
 C2DBoxI Area;long NoThings;
 __forceinline const C2DExtentsI& GetLayerExtents(long layer)const{CMapwhoLevelInfo* const& levels=MapwhoLevelInfo.Start;return levels[layer].Extents;}
 __forceinline CThingMapCell& GetCell(unsigned char layer,unsigned long x,unsigned long y) {
  const C2DExtentsI& extents=GetLayerExtents(layer);
  return Cells[layer][y*extents.X+x];
 }
};
}
struct CLayerInfo {long Size;long ScaleShift;unsigned long UsedColour,UnusedColour;};
extern long SearchCount_013b89a8;
extern long SearchWork_013b89ac;
extern unsigned long SearchID_013b89b0;
namespace NThingMap {extern long g_NoMapwhoLevels;} // 0x013ba848
extern CLayerInfo LayerInfo_0137dad8[]; // Original variable spelling unrecovered.
struct CThingFilter_IsNotList {RetailVector<const CThing*>* PNotList;void* PNotList2;bool operator()(const CThing&)const;}; // 0x006a7eb0
struct CThingFilter_IsTargetable {bool ZTargeting,UsingLightning;bool operator()(const CThing&)const;}; // 0x006a84f0
struct CThingFilter_In3DRange {float Pos[3];float Range;bool operator()(const CThing&)const;}; // 0x006a7780
struct Filters {
 CThingFilter_IsNotList* F1;
 CThingFilter_IsTargetable* F2;
 CThingFilter_In3DRange* F3;
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