// Self-contained full-body test: real shared class models and controlled external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESex{};enum EAIStateGroupType{};enum EScriptAIPriority{};enum EReactionSpeechType{FixtureReactionZero};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};enum EPlayerSpawnStatus{};enum ESaveType{};enum EEnginePrimitiveStatIndex{};
#endif
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define private public
#define protected public

#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_CTOR_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
// 00915350 CAIStateGroup_KidHurtSelfRunToMother::Update
// Flags: none
// Leaf: ?Update@CAIStateGroup_KidHurtSelfRunToMother@@UAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#define FABLE_RETAIL_KID_MOTHER_REFERENCE_PASS23
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/CThingFilter_CanSeeEachOther.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/GFWorldDistance.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

void CAIStateGroup_KidHurtSelfRunToMother::Update()
{
    EAIActionReturn ret=AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),"BOY_TRIP_FALL_OVER_01",CSoundPair(0),true,true,false,0,0),false);
    if(ret==AI_ACTION_TERMINATE_PROCESS || ret==AI_ACTION_FAILED){Terminate();return;}
    CTCCreatureModeManager* ptc_cmm;
    GetCreature().GetTC(&ptc_cmm);
    ptc_cmm->AddMode(20);
    AddedSadMode=true;
    if(AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),"BOY_TRIP_GET_UP_01",CSoundPair(0),false,true,false,0,0),false)!=AI_ACTION_FINISHED){Terminate();return;}
    if(GFRandom(100,GetPWorldSeed())<75){Terminate();return;}
    if(IS_THING_ALIVE(GetCreature().PeekMotherCreature()) && GFWorld2DDistanceSmallerThan(GetCreature().GetPos(),GetCreature().PeekMotherCreature()->GetPos(),80.0f,false)){
        bool finished_nav2=false;
        CThingFilter_CanSeeEachOther can_see_each_other(GetCreature());
        bool sent_message=false;
        while((ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,GetCreature().PeekMotherCreature(),1.0f,finished_nav2,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f))==AI_ACTION_IN_PROGRESS){
            if(!sent_message && can_see_each_other(*GetCreature().PeekMotherCreature())){SendMessageToMother();sent_message=true;}
        }
        if(ret!=AI_ACTION_FINISHED){Terminate();return;}
        if(!sent_message){SendMessageToMother();sent_message=true;}
        for(long c1=0;c1<5 && ret!=AI_ACTION_TERMINATE_PROCESS;++c1){
            long frame_to_stop_looking=CWorld::GetFrame()+2*CMainGameComponent::GetConstantFPS()+GFRandom(30,GetPWorldSeed());
            while(frame_to_stop_looking>CWorld::GetFrame() && ret!=AI_ACTION_TERMINATE_PROCESS)
                ret=AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetCreature().PeekMotherCreature(),1,0.0027777f);
            ret=PerformRandomLengthCrying();
        }
    }else ret=PerformRandomLengthCrying();
    Terminate();
}

#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CThingManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CNavigatorManager.h"
#include "engine/CDefString.h"

#include "engine/CWorldInit.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CCamera.h"


struct Scenario {int fallResult,getUpResult,randomGate,motherKind,moveProgress,moveFinal,visibleAt,cryTerminate,lookTerminate,delay;};
struct TestState {int tokenNew,tokenDelete,baseNew,baseDelete,strings,constructed,destroyed,actions,modes,moves,sight,messages,looks,cries,randomCalls,frames,terminations;unsigned long priorSeed;bool navValue;};
static TestState state;static Scenario scenario;struct Context;static Context*Current;
struct FixtureToken {public:CBaseObject*Owner;FixtureToken(CBaseObject*p):Owner(p){}};
static std::vector<FixtureToken*> Tokens;
static void Invalidate(CBaseObject*p){for(unsigned n=0;n<Tokens.size();++n)if(Tokens[n]->Owner==p)Tokens[n]->Owner=0;}
CBaseObject::~CBaseObject(){Invalidate(this);}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++state.baseNew;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);++state.baseDelete;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return PData?reinterpret_cast<FixtureToken*>(PData)->Owner:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){FixtureToken*t=reinterpret_cast<FixtureToken*>(PData);if(t){bool found=false;for(unsigned n=0;n<Tokens.size();++n)if(Tokens[n]==t){Tokens.erase(Tokens.begin()+n);found=true;break;}assert(found);delete t;PData=0;++state.tokenDelete;}if(p){t=new FixtureToken(p);PData=reinterpret_cast<CBaseObjectPointer*>(t);Tokens.push_back(t);++state.tokenNew;}}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}

CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){}
CTCBase::CTCBase(CThing&t):ParentThing(&t){}CTCBase::~CTCBase(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CDefinitionManager::~CDefinitionManager(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CClassFactory::CClassFactory(unsigned char){}CClassFactory::~CClassFactory(){}
CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){}void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
CThingManager::CThingManager(const CMainGameComponent&c,const CGameDefinitionManager&d,CWorld&w,CPlayerManager&p,CNavigatorManager&n,unsigned long*seed,unsigned long,unsigned long,unsigned long):CClassFactory(0),Component(c),DefinitionManager(d),World(w),WorldMap(*w.PWorldMap_ptr),WorldSeed(seed),PlayerManager(p),NavigationManager(n){}CThingManager::~CThingManager(){}


CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::CBaseObject(const CBaseObject&):IntelligentPointer(0){}
CIDrawWorld::~CIDrawWorld(){}
static void StringBorn(const char*){++state.strings;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;StringBorn(s);}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData){++PStringData->NoRefs;++state.strings;}}
CCharString::~CCharString(){if(PStringData){assert(state.strings>0);--state.strings;if(--PStringData->NoRefs==0)delete PStringData;}}
const char CCharString::NullChar=0;
CWorldInit::CWorldInit():Component(0),ScavengeWorld(0),PlayerManager(0),DefinitionManager(0),DisplayEngine(0){}
static CWorldInit WorldInit;static CWorldMap WorldMap;
namespace std {template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);}template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);}}
CWorld::CWorld(const CWorldInit&){PWorldMap_ptr=&WorldMap;}
CWorld::~CWorld(){}
static CGame*Game;static CMainGameComponent*Component;static CGameDefinitionManager*Definitions;static CPlayerManager*Players;static CNavigatorManager*Navigation;
CAIBrain::CAIBrain(const CMainGameComponent&c,CWorld&w,CThingManager&t,CPlayerManager&p,CThingAICreature&cr,CDefPointer<const CBrainDef>):Component(const_cast<CMainGameComponent*>(&c)),Creature(cr),ThingManager(&t),PlayerManager(&p),World(&w){}
CAIStateGroupBase::CAIStateGroupBase():PBrain(0){}
CCreatureActionBase::~CCreatureActionBase(){++state.destroyed;}
static const CCreatureActionBase*CurrentAction;
struct ThingState{const CThing*Thing;C3DVector Pos;bool Valid;};static ThingState ThingStates[8];static int ThingCount;
static ThingState&TS(const CThing*t){for(int i=0;i<ThingCount;++i)if(ThingStates[i].Thing==t)return ThingStates[i];assert(ThingCount<8);ThingState&s=ThingStates[ThingCount++];s.Thing=t;s.Pos=C3DVector(0,0,0);s.Valid=true;return s;}
template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static void AddTC(CThing&t,ETCInterfaceType id,CTCBase*p){if(!t.Components._M_start)SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(t.Components),17);bool exists=false;for(unsigned k=0;k<t.Components.size();++k)if(t.Components[k].first==id){t.Components[k].second=p;exists=true;}if(!exists)t.Components.push_back(std::make_pair(id,p));for(unsigned k=1;k<t.Components.size();++k)for(unsigned j=k;j>0&&t.Components[j].first<t.Components[j-1].first;--j){std::pair<ETCInterfaceType,CTCBase*> tmp=t.Components[j];t.Components[j]=t.Components[j-1];t.Components[j-1]=tmp;}unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
static void InitThing(CThing&t){memset(&t.UsingComponents,0,sizeof t.UsingComponents);t.Dead=false;t.ThingTypeFlags=31;TS(&t);}
static void RemoveSlot(CThing&t,ETCInterfaceType id){for(unsigned i=0;i<t.Components.size();++i)if(t.Components[i].first==id)t.Components[i].second=0;unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]&=~(1UL<<(n%(8*sizeof(unsigned long))));}
class FixtureModes:public CTCCreatureModeManager{public:FixtureModes(CThing&t):CTCCreatureModeManager(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};

CTCCreatureModeManager::CTCCreatureModeManager(CThing&t):CTCBase(t){}
CTCCreatureModeManager::~CTCCreatureModeManager(){}
CTCCreatureModeManager::CModeBase::~CModeBase(){}
struct Context {
 CThingAICreature Me,Mother;CWorld World;CThingManager Things;CAIBrain Brain;FixtureModes Modes;CAIStateGroup_KidHurtSelfRunToMother Script;unsigned long Seed;
 Context():World(WorldInit),Things(*Component,*Definitions,World,*Players,*Navigation,&Seed,0,0,0),Brain(*Component,World,Things,*Players,Me,CDefPointer<const CBrainDef>()),Modes(Me),Seed(123){
  ThingCount=0;InitThing(Me);InitThing(Mother);Script.PBrain=&Brain;Script.AddedSadMode=false;AddTC(Me,TCI_ENTITY_MODE_MANAGER,&Modes);state.priorSeed=Seed;
  if(scenario.motherKind!=0)Me.MotherCreature=&Mother;
  Mother.Dead=scenario.motherKind==1;
  TS(&Mother).Pos=C3DVector(scenario.motherKind==3?80.0f:scenario.motherKind==4?81.0f:4.0f,0.0f,4096.0f);
  if(scenario.motherKind==5)Invalidate(&Mother);
 }
};
bool CTCCreatureModeManager::AddMode(unsigned long mode){assert(this==&Current->Modes&&mode==20&&state.actions==1&&state.destroyed==1&&!state.terminations);++state.modes;return false;}
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase&thing,const CCharString&name,const CSoundPair&sound,bool movement,bool physics,bool last,long first,long second){
 assert(&thing==&Current->Me&&!state.terminations&&state.constructed==state.destroyed&&sound.FirstSound==0&&sound.LastSound==0&&physics&&!last&&!first&&!second);
 if(!state.actions){assert(movement&&strcmp(name.GetAsCharArray(),"BOY_TRIP_FALL_OVER_01")==0&&!state.modes);}
 else{assert(!movement&&state.actions==1&&state.modes==1&&Current->Script.AddedSadMode&&strcmp(name.GetAsCharArray(),"BOY_TRIP_GET_UP_01")==0);}
 CurrentAction=this;++state.constructed;
}
EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&action,bool wait){assert(&action==CurrentAction&&!wait&&!state.terminations&&state.constructed==state.destroyed+1);++state.actions;return(EAIActionReturn)(state.actions==1?scenario.fallResult:scenario.getUpResult);}
unsigned long* CAIStateGroupBase::GetPWorldSeed()const{assert(this==&Current->Script&&!state.terminations&&state.actions==2);return&Current->Seed;}
long FABLE_FASTCALL GFROR13(unsigned long value){assert((unsigned int)value==(unsigned int)(state.priorSeed*0x24a1ul+0x24dful));unsigned long next=state.randomCalls?scenario.delay:scenario.randomGate;state.priorSeed=next;++state.randomCalls;return(long)next;}
const C3DVector&CThing::GetPos()const{return TS(this).Pos;}
const C3DVector&CThingPhysical::PeekPos()const{return TS(this).Pos;}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,float distance,bool&nav,EMovementType movement,bool a,bool b,bool c,bool d,float last){
 assert(!state.terminations&&repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==&Current->Mother&&distance==1.0f&&movement==AI_MOVEMENT_RUNNING&&a&&!b&&c&&d&&last==0.0f&&nav==state.navValue);
 nav=!nav;state.navValue=nav;++state.moves;return state.moves<=scenario.moveProgress?AI_ACTION_IN_PROGRESS:(EAIActionReturn)scenario.moveFinal;
}
bool CThingFilter_CanSeeEachOther::operator()(const CThingCreatureBase&other)const{assert(&Creature==&Current->Me&&&other==&Current->Mother&&!state.messages&&!state.terminations&&state.moves<=scenario.moveProgress);++state.sight;return state.moves>=scenario.visibleAt;}
void CAIStateGroup_KidHurtSelfRunToMother::SendMessageToMother(){assert(this==&Current->Script&&!state.terminations&&!state.messages&&state.moves>0);++state.messages;}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,long frames,float speed){
 assert(!state.terminations&&repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==&Current->Mother&&frames==1&&state.messages==1);unsigned int bits;memcpy(&bits,&speed,4);assert(bits==0x3b360a13u);++state.looks;++state.frames;return scenario.lookTerminate&&state.looks==scenario.lookTerminate?AI_ACTION_TERMINATE_PROCESS:AI_ACTION_FINISHED;
}
EAIActionReturn CAIStateGroup_KidHurtSelfRunToMother::PerformRandomLengthCrying(){assert(this==&Current->Script&&!state.terminations);++state.cries;return state.cries==scenario.cryTerminate?AI_ACTION_TERMINATE_PROCESS:AI_ACTION_FINISHED;}
void CAIStateGroupBase::Terminate(){assert(this==&Current->Script&&!state.terminations);++state.terminations;}
long FABLE_FASTCALL CWorld::GetFrame(){return state.frames;}
long CMainGameComponent::ConstantFPS=5;

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
void CThingAICreature::Save(CCharString& out) { abort(); }
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
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
void CClassFactory::AddNewClassType(const CClassTypeInit&) { abort(); }
void CClassFactory::OnCreateClass(CClass&) { abort(); }
void CClassFactory::OnDeleteClass(CClass&) { abort(); }
void CThingManager::OnCreateClass(CClass&) { abort(); }
void CThingManager::OnDeleteClass(CClass&) { abort(); }
void CGameDefinitionManager::InitAndCompile() { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
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
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { abort(); }
void CAIStateGroupBase::OnActivate() { abort(); }
bool CAIStateGroupBase::InitCreaturePosition() { abort(); }
bool CAIStateGroupBase::AllowPriorityOverlap() { abort(); }
bool CAIStateGroupBase::IsStillValid() const { abort(); }
const CCharString* CAIStateGroupBase::GetDebugText() const { abort(); }
bool CAIStateGroupBase::IsTimingCritical() const { abort(); }
EAIStateGroupType CAIStateGroupBase::GetStateGroupType() const { abort(); }
void CAIStateGroupBase::Cleanup() { abort(); }
void CAIStateGroupBase::OnReplace() { abort(); }
bool CAIStateGroupBase::IsRunnable() { abort(); }
void CAIStateGroupBase::InitStateGroup(const CWorld& world, CThingAICreature& creature) { abort(); }
void CAIStateGroup_KidHurtSelfRunToMother::OnActivate() { abort(); }
bool CAIStateGroup_KidHurtSelfRunToMother::IsRunnable() { abort(); }
void CAIStateGroup_KidHurtSelfRunToMother::Cleanup() { abort(); }
void CCreatureActionBase::EditUpdate(float) { abort(); }
void CCreatureActionBase::FinishAction() { abort(); }
bool CCreatureActionBase::IsPlayingAnimation() const { abort(); }
long CCreatureActionBase::GetAnimDelayTime() const { abort(); }
long CCreatureActionBase::GetAnimationIndex() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionInTime() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionOutTime() const { abort(); }
unsigned long CCreatureActionBase::GetAnimExtraFlags() const { abort(); }
void CCreatureActionBase::Initialise() { abort(); }
bool CCreatureActionBase::IsStrikeEvent(long) const { abort(); }
bool CCreatureActionBase::IsStrikeEventActive() const { abort(); }
bool CCreatureActionBase::IsStrikeAction() const { abort(); }
bool CCreatureActionBase::IsPartOfComboChain() const { abort(); }
bool CCreatureActionBase::IsStrikeEventFinishedBeingActive() const { abort(); }
bool CCreatureActionBase::IsVitalStrike() const { abort(); }
bool CCreatureActionBase::IsKnockdownStrike() const { abort(); }
bool CCreatureActionBase::IsBlockCounter() const { abort(); }
bool CCreatureActionBase::IsMeleeFlourish() const { abort(); }
bool CCreatureActionBase::IsActionWithStrikeEventHelperName() const { abort(); }
CCharString CCreatureActionBase::GetStrikeEventHelperName() const { abort(); }
bool CCreatureActionBase::IsOverrideableByActionOfSamePriority() const { abort(); }
void CCreatureActionBase::OnFrameUpdate(float) { abort(); }
bool CCreatureActionBase::IsQueuable() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruption() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const C3DVector &) const { abort(); }
bool CCreatureActionBase::IsBlockable() const { abort(); }
bool CCreatureActionBase::IsAffectedByHaste() const { abort(); }
bool CCreatureActionBase::IsAffectedByHeroStats() const { abort(); }
bool CCreatureActionBase::IsPhysicsPushableByHero() const { abort(); }
bool CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance() const { abort(); }
bool CCreatureActionBase::IsAlwaysUseAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsClearingMovementVector() const { abort(); }
bool CCreatureActionBase::IsToHaveEmptyHands() const { abort(); }
bool CCreatureActionBase::IsToReplaceItemIfRemoved() const { abort(); }
bool CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); }
bool CCreatureActionBase::IsPreventingQuickAccessItemUsage() const { abort(); }
void CCreatureActionBase::SetHitter(const CThing &) { abort(); }
void CCreatureActionBase::SetHitDirection(C3DVector) { abort(); }
void CCreatureActionBase::SetHitPosition(C3DVector) { abort(); }
CTCWeaponTrail * CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long &) const { abort(); }
float CCreatureActionBase::GetTotalTimeSeconds() const { abort(); }
bool CCreatureActionBase::IsToEvadeAllHits() const { abort(); }
bool CCreatureActionBase::IsToCauseRecoil() const { abort(); }
bool CCreatureActionBase::IsAbleToBeAttacked() const { abort(); }
bool CCreatureActionBase::IsPlayHitResponse() const { abort(); }
bool CCreatureActionBase::IsPreventingNormalMovement() const { abort(); }
bool CCreatureActionBase::IsPreventingLookTurning() const { abort(); }
void CCreatureActionBase::OnBlowBlocked(bool) { abort(); }
void CCreatureActionBase::OnFollowUpActionFailedToAdd() { abort(); }
void CCreatureActionBase::UpdateActionRunningTime() { abort(); }
bool CCreatureActionBase::IsDecapitatingAttack() const { abort(); }
bool CCreatureActionBase::PreventSneaking() const { abort(); }
void CCreatureActionBase::FrameUpdate() { abort(); }
bool CCreatureActionBase::IsUsingAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsUsingForcedMovement() const { abort(); }
bool CCreatureActionBase::IsMovementUsingPhysics() const { abort(); }
void CCreatureActionBase::UpdateActionMoveToPos() { abort(); }
void CCreatureActionBase::InitialiseActionMovement() { abort(); }
float CCreatureActionBase::GetFrameUpdateTime() const { abort(); }
float CCreatureActionBase::GetActionSpeedParameter() const { abort(); }
float CCreatureActionBase::GetActionStartRunningTimeSeconds() const { abort(); }
bool CCreatureActionBase::OverrideFaceWithDefault() const { abort(); }
bool CCreatureActionBase::AllowsCombinationAnims() const { abort(); }
bool CCreatureActionBase::CanAnimationTransitionOut() const { abort(); }
void CCreatureActionBase::OnAnimationLoaded() { abort(); }
bool CCreatureActionBase::WaitForMeshToLoad() const { abort(); }
void CCreatureAction_PlayAnimation::PerformAction() { abort(); }
void CCreatureAction_PlayAnimation::FrameUpdate() { abort(); }
bool CCreatureAction_PlayAnimation::IsUsingAnimationMovement() const { abort(); }
bool CCreatureAction_PlayAnimation::IsMovementUsingPhysics() const { abort(); }
void CCreatureAction_PlayAnimation::FinishAction() { abort(); }
bool CCreatureAction_PlayAnimation::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); }
CCreatureActionBase* CCreatureAction_PlayAnimation::Clone() const { abort(); }

static void RunCase(){assert(Tokens.empty());memset(&state,0,sizeof state);state.frames=100;
 {Context context;Current=&context;context.Script.Update();assert(state.terminations==1&&state.strings==0&&state.constructed==state.destroyed);
  bool stopped=scenario.fallResult==AI_ACTION_TERMINATE_PROCESS||scenario.fallResult==AI_ACTION_FAILED;
  assert(state.actions==(stopped?1:2)&&state.modes==(stopped?0:1)&&context.Script.AddedSadMode==!stopped);
  if(stopped||scenario.getUpResult!=AI_ACTION_FINISHED){assert(!state.randomCalls&&!state.moves&&!state.looks&&!state.cries);}
  else if(scenario.randomGate<75){assert(state.randomCalls==1&&!state.moves&&!state.looks&&!state.cries);}
  else if(scenario.motherKind!=2){assert(state.randomCalls==1&&!state.moves&&!state.messages&&!state.looks&&state.cries==1);}
  else{
   assert(state.moves==scenario.moveProgress+1);
   bool early=scenario.moveProgress>=scenario.visibleAt;
   assert(state.sight==(early?scenario.visibleAt:scenario.moveProgress));
   assert(state.messages==((early||scenario.moveFinal==AI_ACTION_FINISHED)?1:0));
   if(scenario.moveFinal!=AI_ACTION_FINISHED)assert(!state.looks&&!state.cries&&state.randomCalls==1);
   else{
    int cries=scenario.cryTerminate?scenario.cryTerminate:5;assert(state.cries==cries&&state.randomCalls==cries+1);
    int duration=10+scenario.delay;int expectedLooks=duration*cries;
    if(scenario.lookTerminate&&scenario.lookTerminate<=expectedLooks){int cycle=(scenario.lookTerminate-1)/duration;expectedLooks=duration*cycle+(scenario.lookTerminate-1)%duration+1+duration*(cries-cycle-1);}
    assert(state.looks==expectedLooks&&state.frames==100+expectedLooks);
   }
  }
 }
 assert(Tokens.empty()&&state.baseNew==state.baseDelete&&state.tokenNew==state.tokenDelete&&!state.strings);
}
int main(){CGame game;Game=&game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);Component=&component;CGameDefinitionManager definitions;Definitions=&definitions;CPlayerManager players;Players=&players;CNavigatorManager navigation;Navigation=&navigation;unsigned cases=0;
 // Full action-return and boundary grid; unrelated inputs held at the successful path.
 for(int fall=0;fall<5;++fall)for(int up=0;up<5;++up)for(int gate=0;gate<4;++gate)for(int mother=0;mother<6;++mother){memset(&scenario,0,sizeof scenario);scenario.fallResult=fall;scenario.getUpResult=up;static const int gates[]={0,74,75,99};scenario.randomGate=gates[gate];scenario.motherKind=mother;scenario.moveProgress=2;scenario.moveFinal=2;scenario.visibleAt=1;RunCase();++cases;}
 // Navigation, one-time message and early termination boundaries independently combined.
 for(int progress=0;progress<4;++progress)for(int result=0;result<4;++result)for(int visible=1;visible<=5;++visible)for(int crying=0;crying<=5;++crying)for(int delay=0;delay<2;++delay){memset(&scenario,0,sizeof scenario);static const int results[]={0,1,2,3};scenario.fallResult=scenario.getUpResult=2;scenario.randomGate=75;scenario.motherKind=2;scenario.moveProgress=progress;scenario.moveFinal=results[result];scenario.visibleAt=visible;scenario.cryTerminate=crying;scenario.delay=delay?29:0;scenario.lookTerminate=(visible%2)?1:0;RunCase();++cases;}
 assert(cases==1560u);
 puts("00915350_TEST PASS");return 0;
}