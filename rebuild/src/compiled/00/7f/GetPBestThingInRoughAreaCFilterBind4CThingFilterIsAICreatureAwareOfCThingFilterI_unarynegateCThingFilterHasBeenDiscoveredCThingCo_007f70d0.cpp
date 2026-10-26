// 007f70d0 CThingSearchTools::GetPBestThingInRoughArea, 613 retail bytes.
// Layout: shared CThingSearchTools donor has a 16-byte debug vector at +0xc;
// retail uses the first three words (12-byte STLport vector). No trailing
// donor member is accessed. CThing's retail type flags are +0x6c (PDB +0x7c).
// Local helper/global names carrying addresses remain descriptive pending PDB proof.
#include "engine/CThingSearchTools.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCBase.h"
#include <stddef.h>
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
 };
 __forceinline iterator begin()const{return iterator(Start);}
 __forceinline iterator end()const{return iterator(Finish);}
 __forceinline T& operator[](unsigned int n)const{return Start[n];}
 __forceinline void clear(){Finish=(T*)CopyTrivial(Finish,Finish,Start);}
};
class CThing { public:
 unsigned char BeforeType[0x6c];unsigned long ThingTypeFlags;
 __forceinline bool IsRequestedType()const{return (ThingTypeFlags&31)==31;}
};
class CThingCreatureBase:public CThing { public:};
class CThingAICreature:public CThingCreatureBase { public:};
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
class CMapwhoLevelInfo { public:long Size;long Width,Height;long ScaleShift;long NoThings;};
class CThingMap { public:
 long Padding;RetailVector<CMapwhoLevelInfo> MapwhoLevelInfo;
 CThingMapManager* Manager;RetailVector<RetailVector<CThingMapCell> > Cells;
 C2DBoxI Area;long NoThings;
 __forceinline CThingMapCell* GetCell(unsigned char layer,long x,long y)const {
  return &Cells[layer][y*MapwhoLevelInfo[layer].Width+x];
 }
};
}
struct CLayerInfo {long Size;long ScaleShift;unsigned long UsedColour,UnusedColour;};
extern long SearchCount_013b89a8;
extern long SearchWork_013b89ac;
extern unsigned long SearchID_013b89b0;
namespace NThingMap {extern long g_NoMapwhoLevels;} // 0x013ba848
extern CLayerInfo LayerInfo_0137dad8[]; // Original variable spelling unrecovered.
class CThingFilter_IsAvailableForCreatureInteraction { public:CThingAICreature* SourceThing;long Type;bool operator()(const CThingAICreature&)const;}; // 0x006a7430
class CThingFilter_In2DRangeLimit { public:float Pos[3];float MinRange,MaxRange;bool operator()(const CThing&)const;}; // 0x006a7700
class CThingFilter_CanSeeEachOther { public:CThingCreatureBase* Creature;bool operator()(const CThingCreatureBase&)const;}; // 0x006a78d0
class CThingFilter_IsUnconscious { public:bool operator()(const CThing&)const;}; // 0x006a73e0
struct Filters {
 CThingFilter_IsAvailableForCreatureInteraction* F1;
 CThingFilter_In2DRangeLimit* F2;
 CThingFilter_CanSeeEachOther* F3;
 CThingFilter_IsUnconscious* F4;
 __forceinline bool operator()(const CThingAICreature& p)const{return (*F1)(p)&&(*F2)(p)&&(*F3)(p)&&!(*F4)(p);}
};
class CThingCompare_Nearest { public:float Pos[3];template<class T> T* operator()(T*,T*)const;}; // 0x0065da97
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
 bool GetPBestThingInRoughArea(const C2DBoxI&,const Filters&,const CThingCompare_Nearest&,CThingAICreature**)const;
};
bool CThingSearchTools_Calls::GetPBestThingInRoughArea(const C2DBoxI& area,const Filters& filter,const CThingCompare_Nearest& compare,CThingAICreature** result)const {
 CThingAICreature* best=0;
 GetAllThingMapsInArea(area,AreaMaps);
 ++SearchCount_013b89a8;SearchWork_013b89ac=0;++SearchID_013b89b0;
 for(RetailVector<NThingMap::CThingMap*>::iterator m=AreaMaps.begin();m!=AreaMaps.end();++m) {
  NThingMap::CThingMap* map=*m;C2DBoxI bounds=ClipAndConvertWorldMapAreaToLocalArea(*map,area);
  for(long layer=0;layer<NThingMap::g_NoMapwhoLevels;++layer) {
   C2DBoxI scaled=GetMapwhoArea(*map,layer,bounds);
   if(scaled.Width()!=0 && scaled.Height()!=0) {
    for(long y=scaled.TLY;y<scaled.BRY;++y) {
     CThingMapCell* cell=map->GetCell((unsigned char)layer,scaled.TLX,y);
     for(long x=scaled.TLX;x<scaled.BRX;++x,++cell) {
      for(CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
       CTCMapwho* who=node->PMapwho;
       if(who->SearchID<SearchID_013b89b0) {
        who->SearchID=SearchID_013b89b0;
        CThingAICreature* thing=static_cast<CThingAICreature*>(((CTCBase*)who)->ParentThing);
        if(thing->IsRequestedType() && filter(*thing))best=compare(best,thing);
       }
      }
     }
    }
   }
  }
 }
 AreaMaps.clear();*result=best;return best!=0;
}