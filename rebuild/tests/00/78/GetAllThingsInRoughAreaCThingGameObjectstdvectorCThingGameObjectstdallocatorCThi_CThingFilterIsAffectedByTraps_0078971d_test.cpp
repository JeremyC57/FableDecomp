// BEGIN exact source snapshot SHA256 8aea94e09ef85cfc31c6f0faa25e22e452e08d349d6aeca6586616270909cce7
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
struct CThing {
 unsigned char BeforeType[0x6c];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
struct CThingGameObject;
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
struct CThingFilter_IsNotList {void* NotList;void* NotList2;bool operator()(const CThing&)const;}; // 0x006a7eb0
struct CThingFilter_IsAbleToBeAttacked {__forceinline bool operator()(const CThing& p)const{return NCreatureMode::IsAbleToBeAttacked(p);}};
struct CThingFilter_HasTC {long Type;bool operator()(const CThing&)const;}; // 0x006a80a0
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

// END exact source snapshot
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
long SearchCount_013b89a8,SearchWork_013b89ac;
unsigned long SearchID_013b89b0;
namespace NThingMap {long g_NoMapwhoLevels;}
CLayerInfo LayerInfo_0137dad8[2];
static NThingMap::CThingMap* Maps[2];
static unsigned MapCount;
static CThingGameObject Things[5];
static bool Gates[5][5];
static int Calls[5];
static const void* FilterObjects[5];
static int FirstCalls,SecondCalls,ThirdCalls,AreaCalls,ClipCalls;
static int Score[5]={9,2,7,4,1};
static int Index(const CThing* p){int i=int(static_cast<const CThingGameObject*>(p)-Things);if(i<0||i>=5)exit(80);return i;}
bool CThingFilter_IsNotList::operator()(const CThing& p)const{++Calls[0];return Gates[0][Index(&p)];}
bool __fastcall NCreatureMode::IsAbleToBeAttacked(const CThing& p){++Calls[1];return Gates[1][Index(&p)];}
bool CThingFilter_HasTC::operator()(const CThing& p)const{++Calls[2];return !Gates[2][Index(&p)];}
bool CThingFilter_IsAffectedByTraps::operator()(const CThing& p)const{++Calls[3];return Gates[3][Index(&p)];}
CThingMapCell& NThingMap::CThingMap::GetCell(unsigned char l,unsigned long x,unsigned long y){return Cells[l][y*MapwhoLevelInfo[l].Extents.X+x];}

template<class T> T* RetailVector<T>::erase(T* first,T* last){unsigned count=last-first;T* tail=last;while(tail!=Finish)*first++=*tail++;Finish-=count;return first;}
void ThingVector::push_back(CThingGameObject* const& value){if(Finish!=End){new(Finish)CThingGameObject*(value);++Finish;}else _M_insert_overflow(Finish,value,FalseType(),1,true);}
void CThingSearchTools_Calls::GetAllThingMapsInArea(const C2DBoxI&,RetailVector<NThingMap::CThingMap*>& out)const {
 ++AreaCalls;out.Start=Maps;out.Finish=Maps+MapCount;out.End=Maps+2;
}
C2DBoxI CThingSearchTools_Calls::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI& in)const{++ClipCalls;return in;}
#define CHECK(x) do{if(!(x)){printf("failed at %d\n",__LINE__);return 1;}}while(0)
static void ResetCalls(){FirstCalls=SecondCalls=ThirdCalls=AreaCalls=ClipCalls=0;memset(Calls,0,sizeof(Calls));}
int main(){
 CHECK(sizeof(CLayerInfo)==16 && offsetof(CLayerInfo,ScaleShift)==4);
 CHECK(offsetof(NThingMap::CThingMap,MapwhoLevelInfo)==4 && offsetof(NThingMap::CThingMap,Cells)==0x14 && offsetof(NThingMap::CThingMap,Area)==0x20);
 CHECK(offsetof(CThingSearchTools_Calls,AreaMaps)==0xc && sizeof(CThingSearchTools_Calls)==24);
 CThingSearchTools_Calls search;C2DBoxI area;area.TLX=area.TLY=0;area.BRX=area.BRY=2;
 CThingFilter_IsNotList f0;CThingFilter_IsAbleToBeAttacked f1;CThingFilter_HasTC f2;CThingFilter_IsAffectedByTraps f3;Filters filters={&f0,&f1,&f2,&f3};ThingVector result;result.Start=result.Finish=result.End=0;
 SearchCount_013b89a8=5;SearchID_013b89b0=10;SearchWork_013b89ac=99;MapCount=0;
 search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.Start==result.Finish && SearchCount_013b89a8==6 && SearchID_013b89b0==11 && SearchWork_013b89ac==0 && search.AreaMaps.Finish==search.AreaMaps.Start);
 CTCMapwho who[5];CMapwhoNode nodes[8];memset(who,0,sizeof(who));memset(nodes,0,sizeof(nodes));memset(Things,0,sizeof(Things));
 for(int i=0;i<5;++i){Things[i].ThingTypeFlags=7;((CTCBase*)&who[i])->ParentThing=Things+i;nodes[i].PMapwho=who+i;for(int g=0;g<5;++g)Gates[g][i]=true;}
 // Same mapwho appears in two cells and at a second spatial level.
 nodes[0].PNext=&nodes[1];nodes[1].PNext=&nodes[2];nodes[3].PNext=&nodes[4];
 nodes[5].PMapwho=&who[0];nodes[6].PMapwho=&who[1];nodes[7].PMapwho=&who[4];nodes[6].PNext=&nodes[7];
 CThingMapCell fine[4],coarse[1];fine[0].PFirstNode=&nodes[0];fine[1].PFirstNode=&nodes[3];fine[2].PFirstNode=0;fine[3].PFirstNode=&nodes[5];coarse[0].PFirstNode=&nodes[6];
 NThingMap::CMapwhoLevelInfo info[2];memset(info,0,sizeof(info));info[0].Extents.X=info[0].Extents.Y=2;info[1].Extents.X=info[1].Extents.Y=1;
 RetailVector<CThingMapCell> levels[2];levels[0].Start=fine;levels[0].Finish=levels[0].End=fine+4;levels[1].Start=coarse;levels[1].Finish=levels[1].End=coarse+1;
 NThingMap::CThingMap map;map.MapwhoLevelInfo.Start=info;map.MapwhoLevelInfo.Finish=map.MapwhoLevelInfo.End=info+2;map.Cells.Start=levels;map.Cells.Finish=map.Cells.End=levels+2;
 Maps[0]=Maps[1]=&map;MapCount=2;NThingMap::g_NoMapwhoLevels=2;LayerInfo_0137dad8[0].ScaleShift=0;LayerInfo_0137dad8[1].ScaleShift=1;
 ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.Finish-result.Start==5 && Calls[0]==5 && Calls[1]==5 && Calls[2]==5 && Calls[3]==5 && ClipCalls==2 && AreaCalls==1);
 for(i=0;i<5;++i){CHECK(who[i].SearchID==SearchID_013b89b0);CHECK(result.Start[i]==&Things[i]);}
 CHECK(search.AreaMaps.Finish==search.AreaMaps.Start);
 // A second call appends, including capacity overflow, without deduplicating earlier output.
 search.GetAllThingsInRoughArea(area,filters,result);CHECK(result.Finish-result.Start==10);
 for(i=0;i<10;++i)CHECK(result.Start[i]==&Things[i%5]);result.clear();
 // Type gate and independent short-circuit filters exclude rows before insertion.
 Things[0].ThingTypeFlags=0;Gates[0][0]=false;Gates[0][1]=false;
 Gates[1][2]=false;Gates[2][3]=false;
 ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.Finish-result.Start==1 && result.Start[0]==&Things[4]);result.clear();
 for(int g=0;g<4;++g){
  Gates[g][4]=false;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
  CHECK(result.Start==result.Finish && Calls[g]>=1);
  int expected[5]={0,0,0,0,0};
  for(int item=0;item<5;++item){
   if((Things[item].ThingTypeFlags&CThingGameObject::ThingTypeMask)!=CThingGameObject::ThingTypeMask)continue;
   for(int stage=0;stage<4;++stage){++expected[stage];if(!Gates[stage][item])break;}
  }
  for(int stage=0;stage<4;++stage)CHECK(Calls[stage]==expected[stage]);
  Gates[g][4]=true;
 }
 // A future stamp is neither processed nor lowered.
 who[4].SearchID=SearchID_013b89b0+10;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.Start==result.Finish && who[4].SearchID==SearchID_013b89b0+9);
 NThingMap::g_NoMapwhoLevels=0;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);CHECK(result.Start==result.Finish && Calls[0]==0);
 NThingMap::g_NoMapwhoLevels=2;area.BRX=area.TLX=0;area.BRY=area.TLY=0;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.Start==result.Finish && Calls[0]==0 && search.AreaMaps.Finish==search.AreaMaps.Start);
 free(result.Start);puts("0078971d_TEST PASS");return 0;
}