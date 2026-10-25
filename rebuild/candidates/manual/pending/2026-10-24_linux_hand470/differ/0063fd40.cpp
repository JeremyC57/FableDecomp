// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase>& filter, list<CThing*>& result) -- the world-map circle search: walks every thing map overlapping the area, each mapwho layer and each cell of the scaled box, visits every mapwho once per search ID and appends (push_back) the things whose physics footprint overlaps the filter circle and that pass the polymorphic world-map filter
// Grade: DIFFER(661v661) -- scheduling only: at the node-loop exit retail reloads the spilled scaled-box bounds as esi (BRX), edi (BRY), edx (TLX), then ebp (map); ours reloads edx, esi, edi, ebp (all other bytes match)
// Leaf: ?GetAllThingsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063FD40, 661 bytes (incl. padding), `__thiscall`, `ret 0xc`; one caller 0x0063EA1C.
// Label: the catalog name `ForEachInRoughArea<CThingPhysical, CFilter_Bind3<SolidToThing, PhysicsFlagsSolidToThing,
//   OverlappingCircle>, CThingFindHeightZBelowAt>` is propagated: the body is a GetAllThingsInRoughArea over plain CThing
//   (mask 0, the folded `xor cl,cl` test) with the CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase>
//   filter (its out-of-line operator() is the landed 0x0063F2C0 in this TU) appending to an STLport list<CThing*>.
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CTCPhysicsBase::
//   GetMapwhoPosition 0x006AFCB0 (C3DVector by hidden pointer), CWorldMap::CThingFilterBase::operator() (virtual slot 0),
//   malloc 0x00BFEA0E (the 12-byte list node), memmove 0x00BFEAE6 (the inlined clear()).
// Shape: the landed list sibling 0x0080ECC0 with the landed 0x0063F2C0 circle filter inlined; the squared distance is
//   one expression here (retail squares dy first, unlike the two-statement out-of-line copy).
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
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingList {
 struct Node {Node* Next;Node* Previous;CThing* Value;};
 Node* Sentinel;
 struct iterator {
  Node* PNode;
  explicit iterator(Node* n):PNode(n){}
  iterator(const iterator& other):PNode(other.PNode){}
 };
 __forceinline iterator end()const{return iterator(Sentinel);}
 __forceinline iterator insert(iterator where,CThing* const& value){
  Node* position=where.PNode;Node* node=(Node*)malloc(sizeof(Node));
  new(&node->Value)CThing*(value);
  Node* previous=position->Previous;
  node->Next=position;node->Previous=previous;
  previous->Next=node;position->Previous=node;
  return iterator(node);
 }
 __forceinline void push_back(CThing* const& value){insert(end(),value);}
 // Test-access helpers do not participate in the graded leaf.
 unsigned size()const{unsigned n=0;for(Node* p=Sentinel->Next;p!=Sentinel;p=p->Next)++n;return n;}
 CThing* at(unsigned i)const{Node* p=Sentinel->Next;while(i--)p=p->Next;return p->Value;}
 void clear(){Node* p=Sentinel->Next;while(p!=Sentinel){Node* n=p->Next;free(p);p=n;}Sentinel->Next=Sentinel->Previous=Sentinel;}
};
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
struct C3DVector {float X,Y,Z;};
class CTCPhysicsBase {
public:
 unsigned char _pad00[0x34];
 float Radius; // +0x34
 C3DVector GetMapwhoPosition()const; // 0x006afcb0
};
struct CThingWithPhysics:CThing {}; // retail CThing PhysicsTC +0x60 lives inside BeforeType
__forceinline const CTCPhysicsBase* GetPhysicsTC(const CThing& thing){return *(CTCPhysicsBase* const*)((const char*)&thing+0x60);}
class CThingFilter_OverlappingCircle {
public:
 C3DVector SpherePos; // +0x00
 float Radius;        // +0x0c
 __forceinline bool Overlaps(const C3DVector& pos,const CTCPhysicsBase* physics)const{
  float dx=pos.X-SpherePos.X;
  float dy=pos.Y-SpherePos.Y;
  float r=physics->Radius+Radius;
  if(dx*dx+dy*dy<r*r)return true;
  return false;
 }
 __forceinline bool operator()(const CThing& thing)const{
  const CTCPhysicsBase* physics=GetPhysicsTC(thing);
  if(physics)return Overlaps(physics->GetMapwhoPosition(),physics);
  return false;
 }
};
class CWorldMap {
public:
 class CThingFilterBase {
 public:
  CThingFilterBase();
  virtual bool operator()(const CThing& thing)const; // slot 0
 };
};
struct Filters {const CThingFilter_OverlappingCircle* F1;const CWorldMap::CThingFilterBase* F2;
 __forceinline bool operator()(const CThing& p)const{return (*F1)(p)&&(*F2)(p);}
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
 void GetAllThingsInRoughArea(const C2DBoxI&,const Filters&,ThingList&)const;
};
void CThingSearchTools_Calls::GetAllThingsInRoughArea(const C2DBoxI& area,const Filters& filter,ThingList& result)const {
 NThingMap::CThingMap* map;
 GetAllThingMapsInArea(area,AreaMaps);
 ++SearchCount_013b89a8;SearchWork_013b89ac=0;++SearchID_013b89b0;
 for(NThingMap::CThingMap** m=AreaMaps.Start;m!=AreaMaps.Finish;++m) {
  map=*m;C2DBoxI bounds=ClipAndConvertWorldMapAreaToLocalArea(*map,area);
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
         if(filter(*typed))result.insert(result.end(),typed);
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