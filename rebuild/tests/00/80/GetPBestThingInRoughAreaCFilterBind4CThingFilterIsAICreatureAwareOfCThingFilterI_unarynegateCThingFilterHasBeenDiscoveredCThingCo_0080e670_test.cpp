// BEGIN exact source snapshot SHA256 0d5c704b85df9d6dcda5c44218d957992d7856fc837e2804952e98189971c5d8
// 0080e670 CThingSearchTools::GetAllThingsInRoughArea, 622 retail bytes.
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
class CThing { public:
 unsigned char BeforeType[0x6c];unsigned long ThingTypeFlags;
 enum {ThingTypeMask=0};
};
template<class T> __forceinline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
class CThingCreatureBase;
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
struct ThingList {
 struct Node {Node* Next;Node* Previous;CThingCreatureBase* Value;};
 Node* Sentinel;
 struct iterator {
  Node* PNode;
  explicit iterator(Node* n):PNode(n){}
  iterator(const iterator& other):PNode(other.PNode){}
 };
 __forceinline iterator end()const{return iterator(Sentinel);}
 __forceinline iterator insert(iterator where,CThingCreatureBase* const& value){
  Node* position=where.PNode;Node* node=(Node*)malloc(sizeof(Node));
  new(&node->Value)CThingCreatureBase*(value);
  Node* previous=position->Previous;
  node->Next=position;node->Previous=previous;
  previous->Next=node;position->Previous=node;
  return iterator(node);
 }
 __forceinline void push_back(CThingCreatureBase* const& value){insert(end(),value);}
 // Test-access helpers do not participate in the graded leaf.
 unsigned size()const{unsigned n=0;for(Node* p=Sentinel->Next;p!=Sentinel;p=p->Next)++n;return n;}
 CThingCreatureBase* at(unsigned i)const{Node* p=Sentinel->Next;while(i--)p=p->Next;return p->Value;}
 void clear(){Node* p=Sentinel->Next;while(p!=Sentinel){Node* n=p->Next;free(p);p=n;}Sentinel->Next=Sentinel->Previous=Sentinel;}
};
class CThingPhysical:public CThing { public:enum {ThingTypeMask=3};};
class CThingGameObject:public CThingPhysical { public:enum {ThingTypeMask=7};};
class CThingCreatureBase:public CThingGameObject { public:enum {ThingTypeMask=15};};
class CThingAICreature:public CThingCreatureBase { public:enum {ThingTypeMask=31};};
class CThingObject:public CThingGameObject { public:enum {ThingTypeMask=0x807};};
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
class C3DVector;
class CThingFilter_IsNot { public:CThing* Thing;bool operator()(const CThing&)const;}; // 0x006a76a0
template<class T> class CIntelligentPointer;
class CThingFilter_IsNotList { public:RetailVector<const CThing*>* PNotList;RetailVector<CIntelligentPointer<CThing> >* PNotList2;bool operator()(const CThing&)const;}; // 0x006a7eb0
class CThingFilter_IsWithinArea { public:C3DVector* StartPos;C3DVector* EndPos;float Radius;bool IncludeThingRad;bool operator()(const CThing&)const;}; // 0x007fd830
namespace { struct CThingFilter_IsEvadeExplosion {C3DVector* StartPos;C3DVector* EndPos;bool operator()(const CThing&)const;}; } // 0x0080dc90
// Original std::unary_negate owns its predicate by value (PDB struct130456).
namespace std {
template<class Predicate> struct unary_negate {
 Predicate _Functor;
 explicit unary_negate(const Predicate& predicate):_Functor(predicate){}
 __forceinline bool operator()(const CThing& p)const{return !_Functor(p);}
};
}
template<class P1,class P2,class P3,class P4> class CFilter_Bind4 { public:
 P1* F1;P2* F2;P3* F3;P4* F4;
 __forceinline bool operator()(const CThingCreatureBase& p)const{return (*F1)(p)&&(*F2)(p)&&(*F3)(p)&&(*F4)(p);}
};
typedef CFilter_Bind4<CThingFilter_IsNot,CThingFilter_IsNotList,CThingFilter_IsWithinArea,std::unary_negate<CThingFilter_IsEvadeExplosion> > Filters;
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
        if(IsThingOfType<CThingCreatureBase>(thing)){
         CThingCreatureBase* typed=static_cast<CThingCreatureBase*>(thing);
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
static CThingCreatureBase Things[5];
static bool Gates[5][5];
static int Calls[5];
static const void* FilterObjects[5];
static int FirstCalls,SecondCalls,ThirdCalls,AreaCalls,ClipCalls;
static int Score[5]={9,2,7,4,1};
static int Index(const CThing* p){int i=int(static_cast<const CThingCreatureBase*>(p)-Things);if(i<0||i>=5)exit(80);return i;}
bool CThingFilter_IsNot::operator()(const CThing& p)const{if(this==FilterObjects[0]){++Calls[0];return Gates[0][Index(&p)];}exit(82);return false;}
bool CThingFilter_IsNotList::operator()(const CThing& p)const{if(this==FilterObjects[1]){++Calls[1];return Gates[1][Index(&p)];}exit(82);return false;}
bool CThingFilter_IsWithinArea::operator()(const CThing& p)const{if(this==FilterObjects[2]){++Calls[2];return Gates[2][Index(&p)];}exit(82);return false;}
bool CThingFilter_IsEvadeExplosion::operator()(const CThing& p)const{if(this==FilterObjects[3]){++Calls[3];return !Gates[3][Index(&p)];}exit(82);return false;}
void CThingSearchTools_Calls::GetAllThingMapsInArea(const C2DBoxI&,RetailVector<NThingMap::CThingMap*>& out)const {
 ++AreaCalls;out.Start=Maps;out.Finish=Maps+MapCount;out.End=Maps+2;
}
C2DBoxI CThingSearchTools_Calls::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI& in)const{++ClipCalls;return in;}
#define CHECK(x) do{if(!(x)){printf("failed at %d\n",__LINE__);return 1;}}while(0)
static void ResetCalls(){FirstCalls=SecondCalls=ThirdCalls=AreaCalls=ClipCalls=0;memset(Calls,0,sizeof(Calls));}
int main(){
 CHECK(sizeof(Filters)==16 && offsetof(Filters,F4)==12 && sizeof(std::unary_negate<CThingFilter_IsEvadeExplosion>)==8);
 CHECK(sizeof(ThingList::Node)==12 && sizeof(CLayerInfo)==16 && offsetof(CLayerInfo,ScaleShift)==4);
 CHECK(offsetof(NThingMap::CThingMap,MapwhoLevelInfo)==4 && offsetof(NThingMap::CThingMap,Cells)==0x14 && offsetof(NThingMap::CThingMap,Area)==0x20);
 CHECK(offsetof(CThingSearchTools_Calls,AreaMaps)==0xc && sizeof(CThingSearchTools_Calls)==24);
 CThingSearchTools_Calls search;C2DBoxI area;area.TLX=area.TLY=0;area.BRX=area.BRY=2;
 CThingFilter_IsNot f0;FilterObjects[0]=&f0; CThingFilter_IsNotList f1;FilterObjects[1]=&f1; CThingFilter_IsWithinArea f2;FilterObjects[2]=&f2; CThingFilter_IsEvadeExplosion f3={0,0};std::unary_negate<CThingFilter_IsEvadeExplosion> negative(f3);FilterObjects[3]=&negative._Functor;Filters filters={&f0,&f1,&f2,&negative};ThingList result;ThingList::Node sentinel;result.Sentinel=&sentinel;sentinel.Next=sentinel.Previous=&sentinel;
 SearchCount_013b89a8=5;SearchID_013b89b0=10;SearchWork_013b89ac=99;MapCount=0;
 search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.size()==0 && SearchCount_013b89a8==6 && SearchID_013b89b0==11 && SearchWork_013b89ac==0 && search.AreaMaps.Finish==search.AreaMaps.Start);
 CTCMapwho who[5];CMapwhoNode nodes[8];memset(who,0,sizeof(who));memset(nodes,0,sizeof(nodes));memset(Things,0,sizeof(Things));
 for(int i=0;i<5;++i){Things[i].ThingTypeFlags=15;((CTCBase*)&who[i])->ParentThing=Things+i;nodes[i].PMapwho=who+i;for(int g=0;g<5;++g)Gates[g][i]=true;}
 // Same mapwho appears in two cells and at a second spatial level.
 nodes[0].PNext=&nodes[1];nodes[1].PNext=&nodes[2];nodes[3].PNext=&nodes[4];
 nodes[5].PMapwho=&who[0];nodes[6].PMapwho=&who[1];nodes[7].PMapwho=&who[4];nodes[6].PNext=&nodes[7];
 CThingMapCell fine[4],coarse[1];fine[0].PFirstNode=&nodes[0];fine[1].PFirstNode=&nodes[3];fine[2].PFirstNode=0;fine[3].PFirstNode=&nodes[5];coarse[0].PFirstNode=&nodes[6];
 NThingMap::CMapwhoLevelInfo info[2];memset(info,0,sizeof(info));info[0].Extents.X=info[0].Extents.Y=2;info[1].Extents.X=info[1].Extents.Y=1;
 RetailVector<CThingMapCell> levels[2];levels[0].Start=fine;levels[0].Finish=levels[0].End=fine+4;levels[1].Start=coarse;levels[1].Finish=levels[1].End=coarse+1;
 NThingMap::CThingMap map;map.MapwhoLevelInfo.Start=info;map.MapwhoLevelInfo.Finish=map.MapwhoLevelInfo.End=info+2;map.Cells.Start=levels;map.Cells.Finish=map.Cells.End=levels+2;
 Maps[0]=Maps[1]=&map;MapCount=2;NThingMap::g_NoMapwhoLevels=2;LayerInfo_0137dad8[0].ScaleShift=0;LayerInfo_0137dad8[1].ScaleShift=1;
 ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.size()==5 && Calls[0]==5 && Calls[1]==5 && Calls[2]==5 && Calls[3]==5 && ClipCalls==2 && AreaCalls==1);
 for(i=0;i<5;++i){CHECK(who[i].SearchID==SearchID_013b89b0);CHECK(result.at(i)==&Things[i]);}
 CHECK(search.AreaMaps.Finish==search.AreaMaps.Start);
 // A second call appends distinct list nodes without deduplicating earlier output.
 search.GetAllThingsInRoughArea(area,filters,result);CHECK(result.size()==10);
 for(i=0;i<10;++i)CHECK(result.at(i)==&Things[i%5]);result.clear();
 // Type gate and independent short-circuit filters exclude rows before insertion.
 Things[0].ThingTypeFlags=0;Gates[0][0]=false;Gates[0][1]=false;
 Gates[1][2]=false;Gates[2][3]=false;
 ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.size()==1 && result.at(0)==&Things[4]);result.clear();
 for(int g=0;g<4;++g){
  Gates[g][4]=false;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
  CHECK(result.size()==0 && Calls[g]>=1);
  int expected[5]={0,0,0,0,0};
  for(int item=0;item<5;++item){
   if((Things[item].ThingTypeFlags&CThingCreatureBase::ThingTypeMask)!=CThingCreatureBase::ThingTypeMask)continue;
   for(int stage=0;stage<4;++stage){++expected[stage];if(!Gates[stage][item])break;}
  }
  for(int stage=0;stage<4;++stage)CHECK(Calls[stage]==expected[stage]);
  Gates[g][4]=true;
 }
 // A future stamp is neither processed nor lowered.
 who[4].SearchID=SearchID_013b89b0+10;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.size()==0 && who[4].SearchID==SearchID_013b89b0+9);
 NThingMap::g_NoMapwhoLevels=0;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);CHECK(result.size()==0 && Calls[0]==0);
 NThingMap::g_NoMapwhoLevels=2;area.BRX=area.TLX=0;area.BRY=area.TLY=0;ResetCalls();search.GetAllThingsInRoughArea(area,filters,result);
 CHECK(result.size()==0 && Calls[0]==0 && search.AreaMaps.Finish==search.AreaMaps.Start);
 result.clear();puts("0080e670_TEST PASS");return 0;
}