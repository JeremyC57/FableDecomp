#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
static void*(*SystemMalloc)(size_t)=&malloc;
static void(*SystemFree)(void*)=&free;
static void*LiveMemory[256];static int LiveCount=0,Allocations=0,Deallocations=0;
static void*TestMalloc(size_t n){assert(n>0&&LiveCount<256);void*p=SystemMalloc(n);assert(p);LiveMemory[LiveCount++]=p;++Allocations;return p;}
static void TestFree(void*p){assert(p);int n;for(n=0;n<LiveCount&&LiveMemory[n]!=p;++n){}assert(n<LiveCount);LiveMemory[n]=LiveMemory[--LiveCount];++Deallocations;SystemFree(p);}
#define malloc TestMalloc
#define free TestFree
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_containers.h"
#undef malloc
#undef free
// Controlled external overflow callee: actual append contract, guarded empty range arithmetic.
namespace std {
template<class T,class A> void vector<T,A>::_M_insert_overflow(iterator position,const T&x,const __true_type&,size_type fillLength,bool atEnd){
 assert(position==this->_M_finish&&fillLength==1&&atEnd);size_type oldSize=this->_M_start?(size_type)(this->_M_finish-this->_M_start):0;size_type capacity=this->_M_start?(size_type)(this->_M_end_of_storage._M_data-this->_M_start):0;size_type len=oldSize?oldSize*2:1;T*start=this->_M_end_of_storage.allocate(len);for(size_type i=0;i<oldSize;++i)_Construct(start+i,this->_M_start[i]);_Construct(start+oldSize,x);for(size_type i=0;i<oldSize;++i)this->_M_start[i].~T();if(this->_M_start)this->_M_end_of_storage.deallocate(this->_M_start,capacity);this->_M_start=start;this->_M_finish=start+oldSize+1;this->_M_end_of_storage._M_data=start+len;
}
template<class T,class A> void vector<T,A>::_M_insert_overflow(iterator position,const T&x,const __false_type&,size_type fillLength,bool atEnd){
 assert(position==this->_M_finish&&fillLength==1&&atEnd);size_type oldSize=this->_M_start?(size_type)(this->_M_finish-this->_M_start):0;size_type capacity=this->_M_start?(size_type)(this->_M_end_of_storage._M_data-this->_M_start):0;size_type len=oldSize?oldSize*2:1;T*start=this->_M_end_of_storage.allocate(len);for(size_type i=0;i<oldSize;++i)_Construct(start+i,this->_M_start[i]);_Construct(start+oldSize,x);for(size_type i=0;i<oldSize;++i)this->_M_start[i].~T();if(this->_M_start)this->_M_end_of_storage.deallocate(this->_M_start,capacity);this->_M_start=start;this->_M_finish=start+oldSize+1;this->_M_end_of_storage._M_data=start+len;
}
}
#include "engine/ETCInterfaceType.h"
#include "engine/CBaseObject.h"
class CTCBoastingPosition;
struct Event {int Kind,Position;long ID,Cost;};
static Event Expected[40];static int EventCount=0,NextEvent=0,ParentUpdated=0,ParentRendered=0,WorldPeeks=0,FrameGets=0;
static int PointerNew=0,PointerDelete=0;
static void Expect(int kind,int position,long id=0,long cost=0){assert(EventCount<40);Expected[EventCount].Kind=kind;Expected[EventCount].Position=position;Expected[EventCount].ID=id;Expected[EventCount++].Cost=cost;}
static void Observe(int kind,int position,long id=0,long cost=0){assert(NextEvent<EventCount);Event&e=Expected[NextEvent++];assert(e.Kind==kind&&e.Position==position&&e.ID==id&&e.Cost==cost&&!ParentUpdated&&!ParentRendered&&!WorldPeeks&&!FrameGets);}
class CThing:public CBaseObject {public:CTCBoastingPosition*Position;CThing():Position(0){}template<class T>bool GetTC(T**out){assert(Position);*out=static_cast<T*>(Position);return true;}void SetAsFrameUpdated(){assert(!ParentUpdated&&!ParentRendered&&NextEvent==EventCount);++ParentUpdated;}void SetAsRenderedThisFrame(unsigned long flags){assert(ParentUpdated==1&&!ParentRendered&&flags==4);++ParentRendered;}};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"
class CBaseObjectPointer {public:CBaseObject*Owner;CBaseObjectPointer(CBaseObject*p):Owner(p){}};
CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++PointerNew;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);++PointerDelete;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return PData?PData->Owner:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){delete PData;PData=p?new CBaseObjectPointer(p):0;}
class CTCBase{};
#include "engine/CVectorMap.h"
template<class K,class V>class FixtureMap:public CVectorMap<K,V>{public:FixtureMap(){this->_M_start=this->_M_end_of_storage.allocate(33);this->_M_finish=this->_M_start;this->_M_end_of_storage._M_data=this->_M_start+33;}};
class CTCBoastingPosition {public:int Number;bool PositionActivated,BoastOn;long BoastIndex,Cost;CTCBoastingPosition():Number(-1),PositionActivated(false),BoastOn(false),BoastIndex(-1),Cost(-1){}bool HasBoast()const{return BoastOn;}long GetBoastIndex()const{return BoastIndex;}bool IsPositionActivated()const{return PositionActivated;}
 void DeactivatePosition(){Observe(1,Number);PositionActivated=false;BoastOn=false;}
 void ActivatePosition(long id,long cost){Observe(2,Number,id,cost);PositionActivated=BoastOn=true;BoastIndex=id;Cost=cost;}
 void RemoveBoast(){Observe(3,Number);BoastOn=false;}
};
class CQuestManager {public:class CBoast {long ID,Cost;public:CBoast(long id,long cost):ID(id),Cost(cost){}long GetID()const{return ID;}long GetCost()const{return Cost;}};unsigned long FrameBoastsLastUpdated;std::list<CBoast>Boasts;static CQuestManager*Instance;static CQuestManager&Get(){assert(Instance);return *Instance;}unsigned long GetFrameBoastsLastUpdated()const{return FrameBoastsLastUpdated;}const std::list<CBoast>&GetBoasts()const{return Boasts;}};
CQuestManager*CQuestManager::Instance=0;
class CWorld {public:static long FABLE_FASTCALL GetFrame(){assert(WorldPeeks==1&&!FrameGets&&!ParentUpdated);++FrameGets;return 200;}};
class CTCBoastingPodium {public:CThing*ParentThing;CWorld*World;FixtureMap<long,CIntelligentPointer<CThing> >BoastingPositions;long LastPositionsWithBoastCount;unsigned long FrameBoastListFrom;bool BoastingActive;long GetNumberOfActiveBoastingPositions()const{long n=0;for(unsigned i=0;i<BoastingPositions.size();++i)if(BoastingPositions[i].second.Get()->Position->IsPositionActivated())++n;return n;}const CWorld&PeekWorld()const{assert(!WorldPeeks&&!FrameGets&&NextEvent==EventCount);++WorldPeeks;return *World;}CThing&GetParentThing(){return *ParentThing;}void FrameUpdate();};
// Native std::vector<long>::push_back,00485FF3..00486024,49 bytes; full original shared body.
// The actual external instantiation uses size optimization and a frame pointer.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
namespace std {
template<> void vector<long>::push_back(const long& x)
{
    if(this->_M_finish!=this->_M_end_of_storage._M_data){
        _Construct(this->_M_finish,x);++this->_M_finish;
    }else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CTCBoastingPodium::FrameUpdate()
{
    if(FrameBoastListFrom==0||CQuestManager::Get().GetFrameBoastsLastUpdated()>=FrameBoastListFrom){
        const std::list<CQuestManager::CBoast>& boasts=CQuestManager::Get().GetBoasts();
        long number_of_boasts=boasts.size();
        BoastingActive=number_of_boasts>0;
        if(number_of_boasts>GetNumberOfActiveBoastingPositions()){
            for(long c1=0;c1<BoastingPositions.size();++c1){
                CTCBoastingPosition* ptc_boasting_position;
                BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                ptc_boasting_position->DeactivatePosition();
            }
            LastPositionsWithBoastCount=0;
            std::vector<long> position_indexes;
            switch(number_of_boasts){
                case 1:position_indexes.push_back(1);break;
                case 2:position_indexes.push_back(2);break;
                case 3:position_indexes.push_back(1);position_indexes.push_back(3);break;
                case 4:position_indexes.push_back(2);position_indexes.push_back(4);break;
            }
            std::list<CQuestManager::CBoast>::const_iterator boast_it=boasts.begin();
            for(long c1=0;c1<position_indexes.size();++c1){
                CVectorMap<long,CIntelligentPointer<CThing> >::iterator lower_bound=BoastingPositions.LowerBound(position_indexes[c1]);
                CVectorMap<long,CIntelligentPointer<CThing> >::iterator upper_bound=BoastingPositions.UpperBound(position_indexes[c1]);
                for(CVectorMap<long,CIntelligentPointer<CThing> >::iterator position_it=lower_bound;position_it!=upper_bound;++position_it){
                    CTCBoastingPosition* ptc_boasting_position;
                    position_it->second->GetTC(&ptc_boasting_position);
                    const CQuestManager::CBoast& boast=*boast_it;
                    ptc_boasting_position->ActivatePosition(boast.GetID(),boast.GetCost());
                    ++LastPositionsWithBoastCount;
                    ++boast_it;
                }
            }
        }else if(number_of_boasts>LastPositionsWithBoastCount){
            std::list<const CQuestManager::CBoast*> newboasts;
            {
            for(std::list<CQuestManager::CBoast>::const_iterator boast_it=boasts.begin(),end_it=boasts.end();boast_it!=end_it;++boast_it){
                bool is_new=true;
                for(long c1=0;c1<BoastingPositions.size();++c1){
                    CTCBoastingPosition* ptc_boasting_position;
                    BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                    if(ptc_boasting_position->HasBoast()&&boast_it->GetID()==ptc_boasting_position->GetBoastIndex()){
                        is_new=false;
                        break;
                    }
                }
                if(is_new)newboasts.push_back(&*boast_it);
            }
            }
            std::list<const CQuestManager::CBoast*>::iterator boast_it=newboasts.begin();
            for(long c1=0;c1<BoastingPositions.size();++c1){
                if(boast_it==newboasts.end())break;
                CTCBoastingPosition* ptc_boasting_position;
                BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                if(ptc_boasting_position->IsPositionActivated()&&!ptc_boasting_position->HasBoast()){
                    ptc_boasting_position->ActivatePosition((*boast_it)->GetID(),(*boast_it)->GetCost());
                    ++boast_it;
                    ++LastPositionsWithBoastCount;
                }
            }
        }else if(number_of_boasts==0){
            for(long c1=0;c1<BoastingPositions.size();++c1){
                CTCBoastingPosition* ptc_boasting_position;
                BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                if(ptc_boasting_position->IsPositionActivated())ptc_boasting_position->RemoveBoast();
            }
            LastPositionsWithBoastCount=0;
        }
        FrameBoastListFrom=PeekWorld().GetFrame();
    }
    GetParentThing().SetAsFrameUpdated();
    GetParentThing().SetAsRenderedThisFrame(4);
}

struct PositionState {bool Active,Has;long ID,Cost;};
static void DirectHelpers(){
 for(int count=0;count<=32;++count){FixtureMap<long,CIntelligentPointer<CThing> >map;FixtureMap<ETCInterfaceType,CTCBase*>components;for(int n=0;n<count;++n){map.push_back(std::make_pair((long)(n/2*3),CIntelligentPointer<CThing>((CThing*)0)));components.push_back(std::make_pair((ETCInterfaceType)(n/2*3),(CTCBase*)0));}for(long key=0;key<100;++key){int lower=0,upper=0;while(lower<count&&map[lower].first<key)++lower;while(upper<count&&map[upper].first<=key)++upper;ETCInterfaceType k=(ETCInterfaceType)key;assert(map.LowerBound(key)==map.begin()+lower&&map.UpperBound(key)==map.begin()+upper&&components.LowerBound(k)==components.begin()+lower);}}
 assert(LiveCount==0&&PointerNew==PointerDelete);
 for(int count=0;count<32;++count){std::vector<long>v;for(long n=0;n<count;++n)v.push_back(n*3);long fallback=7;v.push_back(count?v[0]:fallback);assert(v.size()==(unsigned)(count+1));for(int n=0;n<count;++n)assert(v[n]==n*3);assert(v[count]==(count?0:7));}
 assert(LiveCount==0);
}
int main(){DirectHelpers();unsigned cases=0;int coverage[8]={0,0,0,0,0,0,0,0};
 static const long Groups[7]={1,2,2,3,3,4,4};
 for(int seed=0;seed<16384;++seed){assert(LiveCount==0);Allocations=Deallocations=PointerNew=PointerDelete=0;EventCount=NextEvent=ParentUpdated=ParentRendered=WorldPeeks=FrameGets=0;
  const int count=seed%5;const int positions=seed/5%8;const long initialLast=seed/40%6;const unsigned long initialFrame=seed/240%4==0?0:seed/240%4==1?99:seed/240%4==2?100:101;const unsigned long managerFrame=seed/960%3==0?98:seed/960%3==1?100:102;const bool initialActive=seed/2880%2!=0;
  PositionState oracle[7];bool change=initialFrame==0||managerFrame>=initialFrame;long expectedLast=initialLast;unsigned long expectedFrame=initialFrame;bool expectedActive=initialActive;
  {CThing owner,objects[7];CTCBoastingPosition slots[7];CWorld world;CQuestManager manager;CQuestManager::Instance=&manager;manager.FrameBoastsLastUpdated=managerFrame;CTCBoastingPodium podium;podium.ParentThing=&owner;podium.World=&world;podium.LastPositionsWithBoastCount=initialLast;podium.FrameBoastListFrom=initialFrame;podium.BoastingActive=initialActive;
   int activeCount=0;for(int i=0;i<positions;++i){slots[i].Number=i;slots[i].PositionActivated=(seed/(i+3)+i)%3!=0;slots[i].BoastOn=slots[i].PositionActivated&&((seed/(i+5))%2!=0);slots[i].BoastIndex=100+(seed/(i+7)+i)%6;slots[i].Cost=40+i;objects[i].Position=&slots[i];oracle[i].Active=slots[i].PositionActivated;oracle[i].Has=slots[i].BoastOn;oracle[i].ID=slots[i].BoastIndex;oracle[i].Cost=slots[i].Cost;if(oracle[i].Active)++activeCount;podium.BoastingPositions.push_back(std::make_pair(Groups[i],CIntelligentPointer<CThing>(&objects[i])));}
   for(int j=0;j<count;++j)manager.Boasts.push_back(CQuestManager::CBoast(100+j,20+j*2));
   if(change){expectedActive=count>0;expectedFrame=200;if(count>activeCount){++coverage[0];for(int i=0;i<positions;++i){Expect(1,i);oracle[i].Active=oracle[i].Has=false;}expectedLast=0;int next=0;long chosen[2];int selected=0;if(count==1){chosen[selected++]=1;}if(count==2){chosen[selected++]=2;}if(count==3){chosen[selected++]=1;chosen[selected++]=3;}if(count==4){chosen[selected++]=2;chosen[selected++]=4;}for(int group=0;group<selected;++group)for(int i=0;i<positions;++i)if(Groups[i]==chosen[group]){assert(next<count);Expect(2,i,100+next,20+next*2);oracle[i].Active=oracle[i].Has=true;oracle[i].ID=100+next;oracle[i].Cost=20+next*2;++next;++expectedLast;}if(!positions)++coverage[5];}
    else if(count>initialLast){++coverage[1];int pending[5],n=0;for(int boast=0;boast<count;++boast){bool found=false;for(int i=0;i<positions;++i)if(oracle[i].Has&&oracle[i].ID==100+boast)found=true;if(!found)pending[n++]=boast;else ++coverage[6];}int next=0;for(int i=0;i<positions&&next<n;++i)if(oracle[i].Active&&!oracle[i].Has){int b=pending[next++];Expect(2,i,100+b,20+b*2);oracle[i].Active=oracle[i].Has=true;oracle[i].ID=100+b;oracle[i].Cost=20+b*2;++expectedLast;}if(next<n)++coverage[7];}
    else if(count==0){++coverage[2];for(int i=0;i<positions;++i)if(oracle[i].Active){Expect(3,i);oracle[i].Has=false;}expectedLast=0;}
    else ++coverage[3];
   }else ++coverage[4];
   podium.FrameUpdate();assert(NextEvent==EventCount&&ParentUpdated==1&&ParentRendered==1&&WorldPeeks==(int)change&&FrameGets==(int)change);assert(podium.LastPositionsWithBoastCount==expectedLast&&podium.FrameBoastListFrom==expectedFrame&&podium.BoastingActive==expectedActive);for(int i=0;i<positions;++i)assert(slots[i].PositionActivated==oracle[i].Active&&slots[i].BoastOn==oracle[i].Has&&slots[i].BoastIndex==oracle[i].ID&&slots[i].Cost==oracle[i].Cost);assert(manager.Boasts.size()==(unsigned)count);
  }
  assert(LiveCount==0&&Allocations==Deallocations&&PointerNew==PointerDelete);++cases;
 }
 assert(cases==16384u);for(int c=0;c<8;++c)assert(coverage[c]>0);puts("00813d00_TEST PASS");return 0;}
