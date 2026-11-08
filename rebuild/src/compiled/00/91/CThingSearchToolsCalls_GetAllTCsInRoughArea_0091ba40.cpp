// 0091ba40 CThingSearchTools::GetAllTCsInRoughArea, 683 retail bytes.
// Layout: shared CThingSearchTools donor has a 16-byte debug vector at +0xc;
// retail uses the first three words (12-byte STLport vector). No trailing
// donor member is accessed. CThing's retail type flags are +0x6c (PDB +0x7c).
// Local helper/global names carrying addresses remain descriptive pending PDB proof.
#include "fable_compiler.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCBase.h"
#include <stddef.h>
#include <new>
struct FalseType {FalseType(){}};
extern "C" void* __cdecl memmove(void*,const void*,size_t); // 0x00bfeae6
FABLE_FORCEINLINE void* CopyTrivial(const void* first,const void* last,void* result) {
 return last==first ? result : (char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> struct RetailVector {
 T* Start;T* Finish;T* End;
 struct iterator {
  T* Ptr;
  explicit iterator(T* p):Ptr(p){}
  iterator(const iterator& other):Ptr(other.Ptr){}
  FABLE_FORCEINLINE T& operator*()const{return *Ptr;}
  FABLE_FORCEINLINE iterator& operator++(){++Ptr;return *this;}
  FABLE_FORCEINLINE bool operator!=(const iterator& other)const{return Ptr!=other.Ptr;}
  FABLE_FORCEINLINE iterator& operator+=(int n){Ptr+=n;return *this;}
  FABLE_FORCEINLINE iterator operator+(int n)const{iterator r(*this);r+=n;return r;}
 };
 FABLE_FORCEINLINE iterator begin()const{return iterator(Start);}
 FABLE_FORCEINLINE iterator end()const{return iterator(Finish);}
 FABLE_FORCEINLINE T& operator[](unsigned int n)const{return Start[n];}
 void _M_insert_overflow(T*,const T&,const FalseType&,unsigned int,bool); // 0x0049c370 for CThing*
 FABLE_FORCEINLINE void push_back(const T& value){if(Finish!=End){new(Finish)T(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
 FABLE_FORCEINLINE void clear(){Finish=(T*)CopyTrivial(Finish,Finish,Start);}
};
struct InterfaceEntry {long Type;void* Component;};
struct InterfaceMap {
 InterfaceEntry* Start;InterfaceEntry* Finish;
 FABLE_FORCEINLINE void* Find(long type)const{
  InterfaceEntry* first=Start;InterfaceEntry* last=Finish;long count=last-first;
  while(count>0){long half=count/2;InterfaceEntry* middle=first+half;
   if(middle->Type<type){first=middle+1;count-=half+1;}else count=half;
  }
  return ((first==last || type<first->Type) ? last : first)->Component;
 }
};
class CThing { public:
 unsigned char BeforeComponents[0x20];unsigned long UsingComponents[9];InterfaceMap Components;
 unsigned char BeforeType[0x6c-0x4c];unsigned long ThingTypeFlags;
 enum{ThingTypeMask=0};
 FABLE_FORCEINLINE bool HasComponent(long type)const{return (UsingComponents[type/32]&(1UL<<(type%32)))!=0;}
};
class CTCPerceiveThing;
template<class T> FABLE_FORCEINLINE bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
// Same-TU non-inlined STLport overflow model. Only GetAllTCsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingList {
 struct Node {Node* Next;Node* Previous;CTCPerceiveThing* Value;};
 Node* Sentinel;
 struct iterator {
  Node* PNode;
  explicit iterator(Node* n):PNode(n){}
  iterator(const iterator& other):PNode(other.PNode){}
 };
 FABLE_FORCEINLINE iterator end()const{return iterator(Sentinel);}
 FABLE_FORCEINLINE iterator insert(iterator where,CTCPerceiveThing* const& value){
  Node* position=where.PNode;Node* node=(Node*)malloc(sizeof(Node));
  new(&node->Value)CTCPerceiveThing*(value);
  Node* previous=position->Previous;
  node->Next=position;node->Previous=previous;
  previous->Next=node;position->Previous=node;
  return iterator(node);
 }
 FABLE_FORCEINLINE void push_back(CTCPerceiveThing* const& value){insert(end(),value);}
 // Test-access helpers do not participate in the graded leaf.
 unsigned size()const{unsigned n=0;for(Node* p=Sentinel->Next;p!=Sentinel;p=p->Next)++n;return n;}
 CTCPerceiveThing* at(unsigned i)const{Node* p=Sentinel->Next;while(i--)p=p->Next;return p->Value;}
 void clear(){Node* p=Sentinel->Next;while(p!=Sentinel){Node* n=p->Next;free(p);p=n;}Sentinel->Next=Sentinel->Previous=Sentinel;}
};
class CThingCreatureBase:public CThing { public:};
class CThingAICreature:public CThingCreatureBase { public:};
class C2DBoxI { public:
 long TLX,TLY,BRX,BRY;
 C2DBoxI(){}
 C2DBoxI(const C2DBoxI& v):TLX(v.TLX),TLY(v.TLY),BRX(v.BRX),BRY(v.BRY){}
 FABLE_FORCEINLINE long Width()const{return BRX-TLX;}
 FABLE_FORCEINLINE long Height()const{return BRY-TLY;}
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
 FABLE_FORCEINLINE CThingMapCell& GetCell(unsigned char layer,unsigned long x,unsigned long y) {
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
class CThingFilter_IsPerceivingThing { public:CThing* PerceivedThing;bool operator()(const CThing&)const;}; // 0x006a80e0
class CThingFilter_IsNot { public:CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a76a0
struct Filters {CThingFilter_IsPerceivingThing* F1;CThingFilter_IsNot* F2;FABLE_FORCEINLINE bool operator()(const CThing& p)const{return (*F1)(p)&&(*F2)(p);}};
struct CThingSearchTools_Calls {
 CWorldMap* WorldMap;
 CThingMapManager* PThingMapManager;
 CThingManager* ThingManager;
 mutable RetailVector<NThingMap::CThingMap*> AreaMaps;
 void GetAllThingMapsInArea(const C2DBoxI&,RetailVector<NThingMap::CThingMap*>&)const;
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI&)const;
 FABLE_FORCEINLINE C2DBoxI GetMapwhoArea(const NThingMap::CThingMap&,long layer,const C2DBoxI& box)const {
  long shift=LayerInfo_0137dad8[layer].ScaleShift;
  C2DBoxI out;out.TLX=box.TLX>>shift;out.TLY=box.TLY>>shift;
  out.BRX=((box.BRX-1)>>shift)+1;out.BRY=((box.BRY-1)>>shift)+1;return out;
 }
 void GetAllTCsInRoughArea(const C2DBoxI&,const Filters&,ThingList&)const;
};
void CThingSearchTools_Calls::GetAllTCsInRoughArea(const C2DBoxI& area,const Filters& filter,ThingList& result)const {
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
        if(thing && (thing->UsingComponents[1]&(1UL<<20))){
         CTCPerceiveThing* typed=static_cast<CTCPerceiveThing*>(thing->Components.Find(52));
         if(filter(*thing))result.insert(result.end(),typed);
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