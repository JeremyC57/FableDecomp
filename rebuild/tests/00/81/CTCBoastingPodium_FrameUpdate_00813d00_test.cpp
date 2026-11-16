// Self-contained real-object behavior test; external methods are doubles in this file.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum EPlayerMode{};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};enum EPlayerSpawnStatus{};enum ESaveType{};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBOASTINGPODIUM_CTOR_PASS23
#define FABLE_RETAIL_CQUESTMANAGER_CTOR_PASS23
#define private public
#define protected public
// 00813d00 CTCBoastingPodium::FrameUpdate() -- updates boasting positions from the quest manager's boast list.
// Inline-budget: original ordinary LowerBound scores59 at the native expanded group loop and71 at the native called new-boast loop; default74 expands both. T70 is the first measured row-budget probe.
// Retail:0x00813D00,1372 bytes including the native switch table; code returns at0081424A.
// Flags: /d2inlT70
// Leaf: ?FrameUpdate@CTCBoastingPodium@@UAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
#define FABLE_RETAIL_CQUESTMANAGER_PODIUM
#define FABLE_RETAIL_CTCBOASTINGPOSITION_PODIUM
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBoastingPosition.h"
#include "engine/CTCBoastingPodium.h"
#include "engine/CQuestManager.h"
#include "engine/CWorld.h"

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

#include "engine/CCamera.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CGameScriptInterface.h"

struct Event {int Kind,Position;long ID,Cost;};
static Event Expected[40];static int EventCount=0,NextEvent=0,ParentUpdated=0,ParentRendered=0,WorldPeeks=0,FrameGets=0;
static int PointerNew=0,PointerDelete=0;
static void Expect(int kind,int position,long id=0,long cost=0){assert(EventCount<40);Expected[EventCount].Kind=kind;Expected[EventCount].Position=position;Expected[EventCount].ID=id;Expected[EventCount++].Cost=cost;}
static void Observe(int kind,int position,long id=0,long cost=0){assert(NextEvent<EventCount);Event&e=Expected[NextEvent++];assert(e.Kind==kind&&e.Position==position&&e.ID==id&&e.Cost==cost&&!ParentUpdated&&!ParentRendered&&!WorldPeeks&&!FrameGets);}

static CWorld* ActiveWorld;static CThing* ActiveOwner;
static CTCBoastingPosition* SlotPointers[7];
static int SlotNumber(const CTCBoastingPosition*p){for(int i=0;i<7;++i)if(SlotPointers[i]==p)return i;assert(false);return -1;}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
CCharString::CCharString():PStringData(0){} CCharString::CCharString(const CCharString&x):PStringData(x.PStringData){} CCharString::~CCharString(){}
CWideString::CWideString():PStringData(0){} CWideString::CWideString(const CWideString&x):PStringData(x.PStringData){} CWideString::~CWideString(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++PointerNew;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){PData=0;++PointerDelete;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
CTCBase::CTCBase(CThing&t):ParentThing(&t){} CTCBase::~CTCBase(){}
CTCBoastingPodium::CTCBoastingPodium(CThing&t):CTCBase(t){}
CTCBoastingPosition::CTCBoastingPosition(CThing&t):CTCBase(t){} CTCBoastingPosition::~CTCBoastingPosition(){}
CIDrawWorld::~CIDrawWorld(){} CWorld::~CWorld(){}
template<class T>CTBaseSingletonNoDefaultAlloc<T>::~CTBaseSingletonNoDefaultAlloc(){}
template<>CCountedPointer<CQuestManager> CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass=CCountedPointer<CQuestManager>();
CQuestManager::CQuestManager(CWorld&w,CScriptInfoManager&,CGameScriptInterface&){}
CQuestManager::CBoast::CBoast(const CBoast&x):ID(x.ID),Cost(x.Cost),Reward(x.Reward),ScriptName(x.ScriptName),Text(x.Text),Title(x.Title),Evil(x.Evil),Failed(x.Failed),Completed(x.Completed),GroupIndex(x.GroupIndex){}
CScriptInfoManager::CScriptInfoManager():CurrentScriptID(0){} CScriptInfoManager::~CScriptInfoManager(){}
const CWorld&CTCBase::PeekWorld()const{assert(!WorldPeeks&&!FrameGets&&NextEvent==EventCount);++WorldPeeks;return *ActiveWorld;}
long FABLE_FASTCALL CWorld::GetFrame(){assert(WorldPeeks==1&&!FrameGets&&!ParentUpdated);++FrameGets;return 200;}
void CThing::SetAsFrameUpdated(){assert(this==ActiveOwner&&!ParentUpdated&&!ParentRendered&&NextEvent==EventCount);++ParentUpdated;}
void CThing::SetAsRenderedThisFrame(unsigned long flags){assert(this==ActiveOwner&&ParentUpdated==1&&!ParentRendered&&flags==4);++ParentRendered;}
long CTCBoastingPodium::GetNumberOfActiveBoastingPositions()const{long n=0;for(unsigned i=0;i<BoastingPositions.size();++i){CTCBoastingPosition*p=0;assert(BoastingPositions[i].second->GetTC(&p));if(p->IsPositionActivated())++n;}return n;}
void CTCBoastingPosition::DeactivatePosition(){Observe(1,SlotNumber(this));PositionActivated=false;BoastOn=false;}
void CTCBoastingPosition::ActivatePosition(long id,long cost){Observe(2,SlotNumber(this),id,cost);PositionActivated=BoastOn=true;BoastIndex=id;Cost=cost;}
void CTCBoastingPosition::RemoveBoast(){Observe(3,SlotNumber(this));BoastOn=false;}
class FixturePodium:public CTCBoastingPodium{public:FixturePodium(CThing&t):CTCBoastingPodium(t){}ETCInterfaceType GetInterfaceType()const{abort();}CCharString GetClassTypeName()const{abort();}};
class FixturePosition:public CTCBoastingPosition{public:FixturePosition(CThing&t):CTCBoastingPosition(t){}};
template<class K,class V>class FixtureMap:public CVectorMap<K,V>{public:FixtureMap(){this->_M_start=this->_M_end_of_storage.allocate(33);this->_M_finish=this->_M_start;this->_M_end_of_storage._M_data=this->_M_start+33;}};
template<class K,class V>static void SeedMap(CVectorMap<K,V>&m){assert(!m._M_start);m._M_start=m._M_end_of_storage.allocate(33);m._M_finish=m._M_start;m._M_end_of_storage._M_data=m._M_start+33;}
static void AddPosition(CThing&t,CTCBoastingPosition&p){memset(&t.UsingComponents,0,sizeof t.UsingComponents);const unsigned n=TCI_BOASTING_POSITION;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));SeedMap(t.Components);t.Components.push_back(std::make_pair(TCI_BOASTING_POSITION,static_cast<CTCBase*>(&p)));}

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
ETCInterfaceType CTCBoastingPosition::GetInterfaceType()const { abort(); }
CCharString CTCBoastingPosition::GetClassTypeName()const { abort(); }
void CTCBoastingPosition::OnInitialActivate() { abort(); }
bool CTCBoastingPosition::HasFrameUpdate()const { abort(); }
void CTCBoastingPosition::FrameUpdate() { abort(); }
bool CTCBoastingPosition::IsComponentSerialisable()const { abort(); }
void CWorld::PostLoadInit() { abort(); }
void CWorld::LoadWorldFromFile(CAFile& file, long version) { abort(); }
CThingManager& CWorld::GetThingManager() { abort(); }
const CThingManager& CWorld::PeekThingManager() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const { abort(); }
void CWorld::PostInit(CDisplayEngine& displayEngine) { abort(); }
C3DVector CWorld::GetGameLODCentre() const { abort(); }
void CWorld::Init() { abort(); }
void CWorld::Uninitialise() { abort(); }
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) { abort(); }
CWorldMap& CWorld::GetWorldMap() { abort(); }
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) { abort(); }
bool CEnginePersistentPrimitiveBase::WasRendered() const { abort(); }
struct PositionState {bool Active,Has;long ID,Cost;};
static void DirectHelpers(){
 for(int count=0;count<=32;++count){FixtureMap<long,CIntelligentPointer<CThing> >map;FixtureMap<ETCInterfaceType,CTCBase*>components;for(int n=0;n<count;++n){map.push_back(std::make_pair((long)(n/2*3),CIntelligentPointer<CThing>((CThing*)0)));components.push_back(std::make_pair((ETCInterfaceType)(n/2*3),(CTCBase*)0));}for(long key=0;key<100;++key){int lower=0,upper=0;while(lower<count&&map[lower].first<key)++lower;while(upper<count&&map[upper].first<=key)++upper;ETCInterfaceType k=(ETCInterfaceType)key;assert(map.LowerBound(key)==map.begin()+lower&&map.UpperBound(key)==map.begin()+upper&&components.LowerBound(k)==components.begin()+lower);}}
 assert(PointerNew==PointerDelete);
 for(int count=0;count<32;++count){std::vector<long>v;for(long n=0;n<count;++n)v.push_back(n*3);long fallback=7;v.push_back(count?v[0]:fallback);assert(v.size()==(unsigned)(count+1));for(int n=0;n<count;++n)assert(v[n]==n*3);assert(v[count]==(count?0:7));}
 
}
int main(){DirectHelpers();unsigned cases=0;int coverage[8]={0,0,0,0,0,0,0,0};
 static const long Groups[7]={1,2,2,3,3,4,4};
 for(int seed=0;seed<16384;++seed){PointerNew=PointerDelete=0;EventCount=NextEvent=ParentUpdated=ParentRendered=WorldPeeks=FrameGets=0;
  const int count=seed%5;const int positions=seed/5%8;const long initialLast=seed/40%6;const unsigned long initialFrame=seed/240%4==0?0:seed/240%4==1?99:seed/240%4==2?100:101;const unsigned long managerFrame=seed/960%3==0?98:seed/960%3==1?100:102;const bool initialActive=seed/2880%2!=0;
  PositionState oracle[7];bool change=initialFrame==0||managerFrame>=initialFrame;long expectedLast=initialLast;unsigned long expectedFrame=initialFrame;bool expectedActive=initialActive;
  {CThing owner,objects[7];ActiveOwner=&owner;CTCBoastingPosition slot0(objects[0]),slot1(objects[1]),slot2(objects[2]),slot3(objects[3]),slot4(objects[4]),slot5(objects[5]),slot6(objects[6]);CTCBoastingPosition*slots[7]={&slot0,&slot1,&slot2,&slot3,&slot4,&slot5,&slot6};for(int z=0;z<7;++z)SlotPointers[z]=slots[z];CWorld world;ActiveWorld=&world;CScriptInfoManager scriptInfo;CGameScriptInterface game;CQuestManager manager(world,scriptInfo,game);CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass.Data=&manager;manager.FrameBoastsLastUpdated=managerFrame;FixturePodium podium(owner);SeedMap(podium.BoastingPositions);podium.LastPositionsWithBoastCount=initialLast;podium.FrameBoastListFrom=initialFrame;podium.BoastingActive=initialActive;
   int activeCount=0;for(int i=0;i<positions;++i){slots[i]->PositionActivated=(seed/(i+3)+i)%3!=0;slots[i]->BoastOn=slots[i]->PositionActivated&&((seed/(i+5))%2!=0);slots[i]->BoastIndex=100+(seed/(i+7)+i)%6;slots[i]->Cost=40+i;AddPosition(objects[i],*slots[i]);oracle[i].Active=slots[i]->PositionActivated;oracle[i].Has=slots[i]->BoastOn;oracle[i].ID=slots[i]->BoastIndex;oracle[i].Cost=slots[i]->Cost;if(oracle[i].Active)++activeCount;podium.BoastingPositions.push_back(std::make_pair(Groups[i],CIntelligentPointer<CThing>(&objects[i])));}
   for(int j=0;j<count;++j){CQuestManager::CBoast b;b.ID=100+j;b.Cost=20+j*2;manager.Boasts.push_back(b);}
   if(change){expectedActive=count>0;expectedFrame=200;if(count>activeCount){++coverage[0];for(int i=0;i<positions;++i){Expect(1,i);oracle[i].Active=oracle[i].Has=false;}expectedLast=0;int next=0;long chosen[2];int selected=0;if(count==1){chosen[selected++]=1;}if(count==2){chosen[selected++]=2;}if(count==3){chosen[selected++]=1;chosen[selected++]=3;}if(count==4){chosen[selected++]=2;chosen[selected++]=4;}for(int group=0;group<selected;++group)for(int i=0;i<positions;++i)if(Groups[i]==chosen[group]){assert(next<count);Expect(2,i,100+next,20+next*2);oracle[i].Active=oracle[i].Has=true;oracle[i].ID=100+next;oracle[i].Cost=20+next*2;++next;++expectedLast;}if(!positions)++coverage[5];}
    else if(count>initialLast){++coverage[1];int pending[5],n=0;for(int boast=0;boast<count;++boast){bool found=false;for(int i=0;i<positions;++i)if(oracle[i].Has&&oracle[i].ID==100+boast)found=true;if(!found)pending[n++]=boast;else ++coverage[6];}int next=0;for(int i=0;i<positions&&next<n;++i)if(oracle[i].Active&&!oracle[i].Has){int b=pending[next++];Expect(2,i,100+b,20+b*2);oracle[i].Active=oracle[i].Has=true;oracle[i].ID=100+b;oracle[i].Cost=20+b*2;++expectedLast;}if(next<n)++coverage[7];}
    else if(count==0){++coverage[2];for(int i=0;i<positions;++i)if(oracle[i].Active){Expect(3,i);oracle[i].Has=false;}expectedLast=0;}
    else ++coverage[3];
   }else ++coverage[4];
   podium.FrameUpdate();assert(NextEvent==EventCount&&ParentUpdated==1&&ParentRendered==1&&WorldPeeks==(int)change&&FrameGets==(int)change);assert(podium.LastPositionsWithBoastCount==expectedLast&&podium.FrameBoastListFrom==expectedFrame&&podium.BoastingActive==expectedActive);for(int i=0;i<positions;++i)assert(slots[i]->PositionActivated==oracle[i].Active&&slots[i]->BoastOn==oracle[i].Has&&slots[i]->BoastIndex==oracle[i].ID&&slots[i]->Cost==oracle[i].Cost);assert(manager.Boasts.size()==(unsigned)count);CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass.Data=0;
  }
  assert(PointerNew==PointerDelete);++cases;
 }
 assert(cases==16384u);for(int c=0;c<8;++c)assert(coverage[c]>0);puts("00813d00_TEST PASS");return 0;}