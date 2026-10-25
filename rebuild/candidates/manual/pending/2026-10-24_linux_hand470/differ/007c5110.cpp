// CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>& filter, list<CIntelligentPointer<CThingCreatureBase> >& result) -- the creature range search: walks every thing map overlapping the box, every mapwho level, row and cell, stamps each mapwho node with the search ID, keeps creatures (type mask 0xF) that are not the excluded thing and lie in the 2D range, and appends an intelligent pointer to each to the result list
// Grade: DIFFER(697v697) -- scheduling only: at the node-loop exit retail reloads the scaled box as esi (BRX), edx (TLX), ebx (BRY); ours reloads edx first (every other byte matches)
// Leaf: ?GetAllThingsInRoughArea@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C5110, 704 bytes (incl. padding), `__thiscall`, `ret 0xc`, `and esp,-8` frame
//   (the 8-byte CIntelligentPointer local).
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CThingFilter_IsNot 0x006A76A0,
//   CThingFilter_In2DRange 0x006A76B0, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10, malloc 0x00BFEA0E (the 16-byte list node), memmove 0x00BFEAE6 (the inlined AreaMaps clear()).
// Shape: the landed sibling 0x008786E0 (vector<CIntelligentPointer<const CThing> >, same aligned frame) with the result an
//   STLport list (insert(end(), pointer): node allocated, value copy-constructed under the placement-new null test, then
//   linked before the sentinel) and the two Bind2 filters called out of line through the filter's pointers.
// Layout: shared CThingSearchTools donor has a 16-byte debug vector at +0xc;
// retail uses the first three words (12-byte STLport vector). No trailing
// donor member is accessed. CThing's retail type flags are +0x6c (PDB +0x7c).
// Local helper/global names carrying addresses remain descriptive pending PDB proof.
#include "engine/CThingSearchTools.h"
#include "engine/CBaseObject.h"
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
struct CThing:CBaseObject {
 unsigned char BeforeType[0x6c-sizeof(CBaseObject)];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
struct CThingCreatureBase;
// Same-TU non-inlined STLport overflow model. Only GetAllThingsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
// CBaseIntelligentPointer/CBaseObjectPointer names and members recovered from PDB;
// a genuine virtual destructor supplies the vptr. No hand-written vtable slots.
struct CBaseObjectPointer {CBaseObject* object;unsigned long refcount;};
struct CBaseIntelligentPointer {
 CBaseObjectPointer* PData;
 CBaseIntelligentPointer(); // 0x00a01b10
 virtual ~CBaseIntelligentPointer(); // 0x00a01c10
 CBaseObject* GetPItem()const; // 0x00a01b50
 void SetPItem(CBaseObject*); // 0x00a01b90
};
template<class T> struct CIntelligentPointer:CBaseIntelligentPointer {
 CIntelligentPointer(T* p):CBaseIntelligentPointer(){SetPItem((CBaseObject*)p);}
 CIntelligentPointer(const CIntelligentPointer& p):CBaseIntelligentPointer(){SetPItem(p.GetPItem());}
 T* Get()const{return static_cast<T*>(GetPItem());}
};
struct CThingCreatureBase;
struct ThingList {
 struct Node {Node* Next;Node* Previous;CIntelligentPointer<CThingCreatureBase> Value;};
 Node* Sentinel;
 struct iterator {
  Node* PNode;
  explicit iterator(Node* n):PNode(n){}
  iterator(const iterator& other):PNode(other.PNode){}
 };
 __forceinline iterator end()const{return iterator(Sentinel);}
 __forceinline iterator insert(iterator where,const CIntelligentPointer<CThingCreatureBase>& value){
  Node* position=where.PNode;Node* node=(Node*)malloc(sizeof(Node));
  new(&node->Value)CIntelligentPointer<CThingCreatureBase>(value);
  Node* previous=position->Previous;
  node->Next=position;node->Previous=previous;
  previous->Next=node;position->Previous=node;
  return iterator(node);
 }
 __forceinline void push_back(const CIntelligentPointer<CThingCreatureBase>& value){insert(end(),value);}
};
struct CThingGameObject:CThing {enum {ThingTypeMask=7};};
struct CThingCreatureBase:CThingGameObject {enum {ThingTypeMask=15};};
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
struct CThingFilter_IsNot {const CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a76a0
struct CThingFilter_In2DRange {float X,Y,Range;bool operator()(const CThing&)const;}; // 0x006a76b0
struct Filters {const CThingFilter_IsNot* F1;const CThingFilter_In2DRange* F2;
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
        if(IsThingOfType<CThingCreatureBase>(thing)){
         CThingCreatureBase* typed=static_cast<CThingCreatureBase*>(thing);
         if(filter(*typed)){CIntelligentPointer<CThingCreatureBase> pointer(typed);result.push_back(pointer);}
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