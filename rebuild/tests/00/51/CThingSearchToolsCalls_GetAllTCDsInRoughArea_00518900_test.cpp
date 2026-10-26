#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00518900 CThingSearchTools::GetAllTCDsInRoughArea, 597 retail bytes.
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
 __forceinline void clear(){Finish=(T*)CopyTrivial(Finish,Finish,Start);}
};
struct CTCDriverBaseCalls {
 virtual void Slot0();
 virtual void Slot1();
 virtual void Slot2();
 virtual void Slot3();
 virtual void Slot4();
 virtual void Slot5();
 virtual void Slot6();
 virtual void Slot7();
 virtual void Slot8();
 virtual void Slot9();
 virtual void Slot10();
 virtual void Slot11();
 virtual void Slot12();
 virtual void Slot13();
 virtual void Slot14();
 virtual void Slot15();
 virtual void Slot16();
 virtual void Slot17();
 virtual void Slot18();
 virtual void Slot19();
 virtual void Slot20();
 virtual void Slot21();
 virtual void Slot22();
 virtual void Slot23();
 virtual void Slot24();
 virtual void Slot25();
 virtual void Slot26();
 virtual void Slot27();
 virtual bool IsDriverType(long)const; // Original virtual slot28, +0x70.
};
class CThing { public:
 unsigned char BeforeDriver[0x5c];CTCDriverBaseCalls* PTCDriver;unsigned char BeforeType[0x6c-0x60];unsigned long ThingTypeFlags;
 __forceinline bool HasDriverType(long type)const{CTCDriverBaseCalls* driver=PTCDriver;return driver && driver->IsDriverType(type);}
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
// Same-TU non-inlined STLport overflow model. Only GetAllTCDsInRoughArea
// is graded; this body gives RTM the original non-escaping value-reference fact.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingVector:RetailVector<const CThing*> {
 void _M_insert_overflow(const CThing**,const CThing* const&,const FalseType&,unsigned int,bool); // 0x0049c370
 __forceinline void push_back(const CThing* const& value){if(Finish!=End){new(Finish)const CThing*(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
};
#pragma auto_inline(off)
void ThingVector::_M_insert_overflow(const CThing** position,const CThing* const& value,const FalseType&,unsigned int count,bool atEnd){
 unsigned int size=Finish-Start;
 unsigned int capacity=size+(size<count?count:size);
 const CThing** storage=capacity?(const CThing**)malloc(capacity*sizeof(const CThing*)):0;
 const CThing** out=storage;
 for(const CThing** p=Start;p!=position;++p,++out)new(out)const CThing*(*p);
 for(unsigned int i=0;i<count;++i,++out)new(out)const CThing*(value);
 if(!atEnd)for(const CThing** p=position;p!=Finish;++p,++out)new(out)const CThing*(*p);
 if(Start)free(Start);
 Start=storage;Finish=out;End=storage+capacity;
}
#pragma auto_inline(on)
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
struct C2DExtentsI {long X,Y;};
class CMapwhoLevelInfo { public:long Size;C2DExtentsI Extents;long ScaleShift;long NoThings;};
class CThingMap { public:
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
class CThingFilter_IsInQuestOrNullQuest { public:void* QuestSectionName;bool operator()(const CThing&)const;}; // 0x006a7c60
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
 void GetAllTCDsInRoughArea(const C2DBoxI&,const Filters&,ThingVector&)const;
};
void CThingSearchTools_Calls::GetAllTCDsInRoughArea(const C2DBoxI& area,const Filters& filter,ThingVector& result)const {
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
        const CThing* typed=thing;
        if(typed->HasDriverType(25) && filter(*typed))result.push_back(typed);
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

long SearchCount_013b89a8,SearchWork_013b89ac;
unsigned long SearchID_013b89b0;
namespace NThingMap {long g_NoMapwhoLevels;}
CLayerInfo LayerInfo_0137dad8[2];
static NThingMap::CThingMap* Maps[2];
static unsigned MapCount;
static CThing Things[5];
static bool Available[5],Aware[5],ThirdPass[5];
static int FirstCalls,SecondCalls,ThirdCalls,AreaCalls,ClipCalls;
static int Score[5]={9,2,7,4,1};
static int Index(const CThing* p){int i=int(static_cast<const CThing*>(p)-Things);if(i<0||i>=5)exit(80);return i;}
void CTCDriverBaseCalls::Slot0(){}
void CTCDriverBaseCalls::Slot1(){}
void CTCDriverBaseCalls::Slot2(){}
void CTCDriverBaseCalls::Slot3(){}
void CTCDriverBaseCalls::Slot4(){}
void CTCDriverBaseCalls::Slot5(){}
void CTCDriverBaseCalls::Slot6(){}
void CTCDriverBaseCalls::Slot7(){}
void CTCDriverBaseCalls::Slot8(){}
void CTCDriverBaseCalls::Slot9(){}
void CTCDriverBaseCalls::Slot10(){}
void CTCDriverBaseCalls::Slot11(){}
void CTCDriverBaseCalls::Slot12(){}
void CTCDriverBaseCalls::Slot13(){}
void CTCDriverBaseCalls::Slot14(){}
void CTCDriverBaseCalls::Slot15(){}
void CTCDriverBaseCalls::Slot16(){}
void CTCDriverBaseCalls::Slot17(){}
void CTCDriverBaseCalls::Slot18(){}
void CTCDriverBaseCalls::Slot19(){}
void CTCDriverBaseCalls::Slot20(){}
void CTCDriverBaseCalls::Slot21(){}
void CTCDriverBaseCalls::Slot22(){}
void CTCDriverBaseCalls::Slot23(){}
void CTCDriverBaseCalls::Slot24(){}
void CTCDriverBaseCalls::Slot25(){}
void CTCDriverBaseCalls::Slot26(){}
void CTCDriverBaseCalls::Slot27(){}
bool CTCDriverBaseCalls::IsDriverType(long)const{return false;}
static int DriverCalls;
struct DriverMock:CTCDriverBaseCalls{long Type;virtual bool IsDriverType(long type)const{if(type!=25)exit(81);++DriverCalls;return type==Type;}};
bool CThingFilter_IsInQuestOrNullQuest::operator()(const CThing& p)const{++FirstCalls;return Available[Index(&p)];}
void CThingSearchTools_Calls::GetAllThingMapsInArea(const C2DBoxI&,RetailVector<NThingMap::CThingMap*>& out)const {
 ++AreaCalls;out.Start=Maps;out.Finish=Maps+MapCount;out.End=Maps+2;
}
C2DBoxI CThingSearchTools_Calls::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI& in)const{++ClipCalls;return in;}
#define CHECK(x) do{if(!(x)){printf("failed at %d\n",__LINE__);return 1;}}while(0)
static void ResetCalls(){FirstCalls=SecondCalls=ThirdCalls=AreaCalls=ClipCalls=0;}
int main(){
 CHECK(sizeof(CLayerInfo)==16 && offsetof(CLayerInfo,ScaleShift)==4);
 CHECK(offsetof(NThingMap::CThingMap,MapwhoLevelInfo)==4 && offsetof(NThingMap::CThingMap,Cells)==0x14 && offsetof(NThingMap::CThingMap,Area)==0x20);
 CHECK(offsetof(CThingSearchTools_Calls,AreaMaps)==0xc && sizeof(CThingSearchTools_Calls)==24);
 CThingSearchTools_Calls search;C2DBoxI area;area.TLX=area.TLY=0;area.BRX=area.BRY=2;
 Filters filters;ThingVector result;result.Start=result.Finish=result.End=0;
 SearchCount_013b89a8=5;SearchID_013b89b0=10;SearchWork_013b89ac=99;MapCount=0;
 search.GetAllTCDsInRoughArea(area,filters,result);
 CHECK(result.Start==result.Finish && SearchCount_013b89a8==6 && SearchID_013b89b0==11 && SearchWork_013b89ac==0 && search.AreaMaps.Finish==search.AreaMaps.Start);
 DriverMock drivers[5];
 CTCMapwho who[5];CMapwhoNode nodes[8];memset(who,0,sizeof(who));memset(nodes,0,sizeof(nodes));memset(Things,0,sizeof(Things));
 int i;for(i=0;i<5;++i){Things[i].ThingTypeFlags=31;drivers[i].Type=25;Things[i].PTCDriver=&drivers[i];((CTCBase*)&who[i])->ParentThing=Things+i;nodes[i].PMapwho=who+i;Available[i]=Aware[i]=true;ThirdPass[i]=true;}
 // Same mapwho appears in two cells and at a second spatial level.
 nodes[0].PNext=&nodes[1];nodes[1].PNext=&nodes[2];nodes[3].PNext=&nodes[4];
 nodes[5].PMapwho=&who[0];nodes[6].PMapwho=&who[1];nodes[7].PMapwho=&who[4];nodes[6].PNext=&nodes[7];
 CThingMapCell fine[4],coarse[1];fine[0].PFirstNode=&nodes[0];fine[1].PFirstNode=&nodes[3];fine[2].PFirstNode=0;fine[3].PFirstNode=&nodes[5];coarse[0].PFirstNode=&nodes[6];
 NThingMap::CMapwhoLevelInfo info[2];memset(info,0,sizeof(info));info[0].Extents.X=info[0].Extents.Y=2;info[1].Extents.X=info[1].Extents.Y=1;
 RetailVector<CThingMapCell> levels[2];levels[0].Start=fine;levels[0].Finish=levels[0].End=fine+4;levels[1].Start=coarse;levels[1].Finish=levels[1].End=coarse+1;
 NThingMap::CThingMap map;map.MapwhoLevelInfo.Start=info;map.MapwhoLevelInfo.Finish=map.MapwhoLevelInfo.End=info+2;map.Cells.Start=levels;map.Cells.Finish=map.Cells.End=levels+2;
 Maps[0]=Maps[1]=&map;MapCount=2;NThingMap::g_NoMapwhoLevels=2;LayerInfo_0137dad8[0].ScaleShift=0;LayerInfo_0137dad8[1].ScaleShift=1;
 ResetCalls();search.GetAllTCDsInRoughArea(area,filters,result);
 CHECK(result.Finish-result.Start==5 && FirstCalls==5 && SecondCalls==0 && ThirdCalls==0 && ClipCalls==2 && AreaCalls==1);
 for(i=0;i<5;++i){CHECK(who[i].SearchID==SearchID_013b89b0);CHECK(result.Start[i]==&Things[i]);}
 CHECK(search.AreaMaps.Finish==search.AreaMaps.Start && DriverCalls==5);
 // A second call appends, including capacity overflow, without deduplicating earlier output.
 search.GetAllTCDsInRoughArea(area,filters,result);CHECK(result.Finish-result.Start==10);
 for(i=0;i<10;++i)CHECK(result.Start[i]==&Things[i%5]);result.clear();
 // Null drivers and other driver types are excluded before any filter.
 Things[0].PTCDriver=0;drivers[1].Type=99;Available[2]=false;Available[3]=false;
 ResetCalls();search.GetAllTCDsInRoughArea(area,filters,result);
 CHECK(result.Finish-result.Start==1 && result.Start[0]==&Things[4] && FirstCalls==3 && SecondCalls==0 && ThirdCalls==0);result.clear();
 // A future stamp is neither processed nor lowered.
 who[4].SearchID=SearchID_013b89b0+10;ResetCalls();search.GetAllTCDsInRoughArea(area,filters,result);
 CHECK(result.Start==result.Finish && who[4].SearchID==SearchID_013b89b0+9);
 NThingMap::g_NoMapwhoLevels=0;ResetCalls();search.GetAllTCDsInRoughArea(area,filters,result);CHECK(result.Start==result.Finish && FirstCalls==0);
 NThingMap::g_NoMapwhoLevels=2;area.BRX=area.TLX=0;area.BRY=area.TLY=0;ResetCalls();search.GetAllTCDsInRoughArea(area,filters,result);
 CHECK(result.Start==result.Finish && FirstCalls==0 && search.AreaMaps.Finish==search.AreaMaps.Start);
 free(result.Start);puts("00518900_TEST PASS");return 0;
}