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
#define FABLE_RETAIL_CTCSOLIDTOTHING_CTOR_PASS23
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_CTOR
#include "engine/EMessageEventType.h"
// 0090e5d0 CAIStateGroup_BanditGossip::Update() -- pairs gossip partners, approaches and performs a bandit interaction.
// Inline-budget: native FindMostRecentMessage0090EBE0 expands the real Bind3 predicate; RTM trace scores it78 against default74. T80 is the first row-budget probe.
// Retail:0x0090E5D0,1367 bytes,thiscall,complete body.
// Flags: /d2inlT80
// Leaf: ?Update@CAIStateGroup_BanditGossip@@MAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
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
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ORIGINAL_GUARD
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_CTHINGFILTER_ISVALIDGOSSIPEE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTCSOLIDTOTHING_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CTHINGFILTER_IN2DRANGE_CTOR
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCSolidToThing.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsValidGossipee.h"
#include "engine/CCombatActionBase.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CCreatureAction_SheatheWeapons.h"
#include "engine/CTCGossip.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_BanditGossip.h"

// Native IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >,007304C0..007304E4,36 bytes.
// Its original size compilation returns bool in AL; the complete real predicate stays visible for the hot wait-loop expansion.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline bool FABLE_FASTCALL IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >(const CIntelligentPointer<CThingAICreature>& thing)
{
    if(thing==0||thing->IsDead())return true;
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CAIStateGroup_BanditGossip::Update()
{
    WasCarryingWeapon=false;
    Critical=false;
    CIntelligentPointer<CThingAICreature> other_person=0;
    CTCGossip* gossip=0;
    CTCGossip* other_gossip=0;
    GetCreature().GetTC(&gossip);
    bool submissive;
    const CMessageEvent* event=GetWorld().GetMessageEventManager().FindMostRecentMessage(
        GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(),16.0f)),
            CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature()))));
    if(event){
        other_person=event->GetPOriginator<CThingAICreature>();
        GetWorld().GetMessageEventManager().RemoveMessage(event);
        if(!IS_THING_ALIVE(other_person)){Terminate();return;}
        other_person->GetTC(&other_gossip);
        gossip->Gossipee=other_person;
        gossip->MyTurnToSpeak=true;
        other_gossip->Gossipee=&GetCreature();
        other_gossip->MyTurnToSpeak=false;
        submissive=false;
    }else{
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,CMainGameComponent::GetConstantFPS()*4,
            GetCreature().PeekPos(),&GetCreature(),0,16.0f,16.0f,0);
        long wait_till_frame=CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*4;
        while(IS_THING_DEAD(gossip->Gossipee)){
            if(AIAction_DoNothing()==AI_ACTION_TERMINATE_PROCESS||CWorld::GetFrame()>wait_till_frame){Terminate();return;}
        }
        other_person=gossip->Gossipee;
        other_person->GetTC(&other_gossip);
        submissive=true;
    }
    if(other_person.Get()==0){Terminate();return;}
    if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,other_person,17,0.0027777f)!=AI_ACTION_FINISHED){Terminate();return;}
    EAIActionReturn ret=AI_ACTION_FINISHED;
    bool nav_route_calculated;
    do{
        ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,other_person,2.0f,nav_route_calculated,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
    }while(ret==AI_ACTION_IN_PROGRESS);
    if(ret!=AI_ACTION_FINISHED){Terminate();return;}
    if(CTCCarrying::IsThingCarryingWeapon(GetCreature())){
        if(AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()),false)!=AI_ACTION_FINISHED){Terminate();return;}
        WasCarryingWeapon=true;
    }
    if(IS_THING_DEAD(other_person)){Terminate();return;}
    CTCSolidToThing* ptc_solid_to_thing=0;
    if(!GetCreature().GetTC(&ptc_solid_to_thing)){
        CCharString solid_name("CTCSolidToThing");
        ptc_solid_to_thing=static_cast<CTCSolidToThing*>(GetCreature().AddTC(solid_name,false,0));
    }
    ptc_solid_to_thing->AddThingNotSolidTo(other_person.Get());
    char* interactions[3]={"VILLAGER_INTERACTION_BANDIT_PUSHING","VILLAGER_INTERACTION_BANDIT_HOLDING","VILLAGER_INTERACTION_BANDIT_TELLING_OFF"};
    CCharString interaction(interactions[GFRandom(3,GetPWorldSeed())]);
    Critical=true;
    AIAction_InteractWith(ACTION_REPEAT_UNTIL_FINISHED,other_person,interaction,100,submissive);
    Critical=false;
    GetCreature().RemoveTC(TCI_SOLID_TO_THING);
    if(WasCarryingWeapon){
        if(AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()),false)!=AI_ACTION_FINISHED){Terminate();return;}
        WasCarryingWeapon=false;
    }
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

struct TestState {
 int frames,fps,arrival,waitTerminate,waiting,moveProgress,moveFinal,lookResult,sheatheResult,unsheatheResult,interactionResult;
 int killRemove,killMove,killSheathe,initialPartner,initialDead,wantIncoming,carrying,initialSolid,randomSeed;
 int tokenNew,tokenDelete,baseNew,baseDelete,strings,requests,removed,waitCalls,looks,moves,sheathed,unsheathed,interacted,solidAdds,solidRemoved,constructed,destroyed,randomCalls;
 int kindTests,rangeTests,validTests,stage;
 bool preparing,terminated,solidCtorSeen,solidNameAlive,missingSolid;
};
static TestState state;
struct Context;static Context*Current;
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
static void StringBorn(const char*s){++state.strings;if(strcmp(s,"CTCSolidToThing")==0){assert(state.missingSolid&&!state.solidCtorSeen);state.solidCtorSeen=true;state.solidNameAlive=true;}}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;StringBorn(s);}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData){++PStringData->NoRefs;++state.strings;}}
CCharString::~CCharString(){if(PStringData){assert(state.strings>0);--state.strings;if(strcmp(GetAsCharArray(),"CTCSolidToThing")==0)state.solidNameAlive=false;if(--PStringData->NoRefs==0)delete PStringData;}}
const char CCharString::NullChar=0;
CWorldInit::CWorldInit():Component(0),ScavengeWorld(0),PlayerManager(0),DefinitionManager(0),DisplayEngine(0){}
static CWorldInit WorldInit;static CWorldMap WorldMap;
namespace std {template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);}template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);}}
CWorld::CWorld(const CWorldInit&){PWorldMap_ptr=&WorldMap;}
CWorld::~CWorld(){}
static CGame*Game;static CMainGameComponent*Component;static CGameDefinitionManager*Definitions;static CPlayerManager*Players;static CNavigatorManager*Navigation;
CAIBrain::CAIBrain(const CMainGameComponent&c,CWorld&w,CThingManager&t,CPlayerManager&p,CThingAICreature&cr,CDefPointer<const CBrainDef>):Component(const_cast<CMainGameComponent*>(&c)),Creature(cr),ThingManager(&t),PlayerManager(&p),World(&w){}
CAIStateGroupBase::CAIStateGroupBase():PBrain(0){}
CAIStateGroup_MinionGossip::CAIStateGroup_MinionGossip(){}
CAIStateGroup_BanditGossip::CAIStateGroup_BanditGossip(){}
CTCSolidToThing::CTCSolidToThing(CThing&t):CTCBase(t){}
CCreatureActionBase::~CCreatureActionBase(){++state.destroyed;}
CCombatActionBase::~CCombatActionBase(){}
static const CCreatureActionBase*CurrentAction;static int ActionKind;
struct ThingState{const CThing*Thing;C3DVector Pos;bool Valid;};static ThingState ThingStates[8];static int ThingCount;
static ThingState&TS(const CThing*t){for(int i=0;i<ThingCount;++i)if(ThingStates[i].Thing==t)return ThingStates[i];assert(ThingCount<8);ThingState&s=ThingStates[ThingCount++];s.Thing=t;s.Pos=C3DVector(0,0,0);s.Valid=true;return s;}
template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static void AddTC(CThing&t,ETCInterfaceType id,CTCBase*p){if(!t.Components._M_start)SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(t.Components),17);bool exists=false;for(unsigned k=0;k<t.Components.size();++k)if(t.Components[k].first==id){t.Components[k].second=p;exists=true;}if(!exists)t.Components.push_back(std::make_pair(id,p));for(unsigned k=1;k<t.Components.size();++k)for(unsigned j=k;j>0&&t.Components[j].first<t.Components[j-1].first;--j){std::pair<ETCInterfaceType,CTCBase*> tmp=t.Components[j];t.Components[j]=t.Components[j-1];t.Components[j-1]=tmp;}unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
static void InitThing(CThing&t){memset(&t.UsingComponents,0,sizeof t.UsingComponents);t.Dead=false;t.ThingTypeFlags=31;TS(&t);}
static void RemoveSlot(CThing&t,ETCInterfaceType id){for(unsigned i=0;i<t.Components.size();++i)if(t.Components[i].first==id)t.Components[i].second=0;unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]&=~(1UL<<(n%(8*sizeof(unsigned long))));}
class FixtureSolid:public CTCSolidToThing{public:FixtureSolid(CThing&t):CTCSolidToThing(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
struct Context {
 CThingAICreature Me,Other,Far,Wrong,Invalid;
 CWorld World;
 CThingManager Things;CAIBrain Brain;CMessageEventManager Manager;
 CTCGossip SelfGossip,OtherGossip;
 CTCSolidToThing*Solid;
 CAIStateGroup_BanditGossip Script;
 unsigned long Seed;
 Context():World(WorldInit),Things(*Component,*Definitions,World,*Players,*Navigation,&Seed,0,0,0),Brain(*Component,World,Things,*Players,Me,CDefPointer<const CBrainDef>()),Manager(World),SelfGossip(Me),OtherGossip(Other),Solid(0),Seed((unsigned long)state.randomSeed){ThingCount=0;World.PMessageEventManager_Data=&Manager;Script.PBrain=&Brain;InitThing(Me);InitThing(Other);InitThing(Far);InitThing(Wrong);InitThing(Invalid);AddTC(Me,TCI_GOSSIP,&SelfGossip);AddTC(Other,TCI_GOSSIP,&OtherGossip);AddTC(Far,TCI_GOSSIP,&OtherGossip);AddTC(Wrong,TCI_GOSSIP,&OtherGossip);AddTC(Invalid,TCI_GOSSIP,&OtherGossip);TS(&Far).Pos.X=32.0f;TS(&Other).Pos.X=4.0f;Wrong.ThingTypeFlags=0;TS(&Invalid).Valid=false;Other.Dead=state.initialDead!=0;SelfGossip.MyTurnToSpeak=false;OtherGossip.MyTurnToSpeak=true;Script.WasCarryingWeapon=true;Script.Critical=true;state.missingSolid=!state.initialSolid;if(state.initialSolid){Solid=new FixtureSolid(Me);AddTC(Me,TCI_SOLID_TO_THING,Solid);}if(state.initialPartner)SelfGossip.Gossipee=&Other;}
 ~Context(){delete Solid;}
};
CMessageEventManager::CMessageEventManager(const CWorld&w):World(w){}
CTCGossip::CTCGossip(CThing&t):CTCBase(t),MyTurnToSpeak(false){}
ETCInterfaceType CTCGossip::GetInterfaceType()const{return TCI_GOSSIP;}
CCharString CTCGossip::GetClassTypeName()const{return CCharString("CTCGossip");}
CMessageEvent*CMessageEventManager::AddMessage(EMessageEventType type,unsigned long life,const C3DVector&pos,CThing*originator,CThing*target,float sight,float hearing,long data){
 if(!state.preparing){assert(!state.terminated&&type==MESSAGE_EVENT_REQUEST_GOSSIP&&life==(unsigned long)(state.fps*4)&&originator==&Current->Me&&!target&&pos.X==0.0f&&pos.Y==0.0f&&pos.Z==0.0f&&sight==16.0f&&hearing==16.0f&&data==0);++state.requests;state.waiting=1;}
 CMessageEvent event;event.Type=type;event.Pos=pos;event.SightRadius=sight;event.HearingRadius=hearing;event.OriginatorUID=event.TargetUID=0;event.PTargetFaction=0;event.TimeStamp=state.frames;event.Lifespan=life;event.ExtraData=data;event.MessageID=0;event.ReactionSpeechType=(EReactionSpeechType)0;event.POriginator=originator;event.PTarget=target;Messages.push_front(event);return &Messages.front();
}
void CMessageEventManager::RemoveMessage(const CMessageEvent*event){assert(!state.terminated&&event&&event->POriginator.Get()==&Current->Other);++state.removed;for(std::list<CMessageEvent>::iterator i=Messages.begin();i!=Messages.end();++i)if(&*i==event){Messages.erase(i);if(state.killRemove==1)Current->Other.Dead=true;else if(state.killRemove==2)Invalidate(&Current->Other);return;}assert(false);}
bool CMessageEventFilter_IsOfType::operator()(const CMessageEvent&event)const{assert(Type==MESSAGE_EVENT_REQUEST_GOSSIP);++state.kindTests;return event.Type==Type;}
bool CThingFilter_In2DRange::operator()(const CThing&thing)const{assert(Pos.X==0.0f&&Pos.Y==0.0f&&Pos.Z==0.0f&&Range==16.0f);++state.rangeTests;float x=Pos.X-TS(&thing).Pos.X,y=Pos.Y-TS(&thing).Pos.Y;return x*x+y*y<=Range*Range;}
CThingFilter_IsValidGossipee::CThingFilter_IsValidGossipee(const CThingAICreature&thing):Thing(thing),PFaction(0),PTCVillage(0){assert(&thing==&Current->Me);}
bool CThingFilter_IsValidGossipee::operator()(const CThingAICreature&thing)const{assert(&Thing==&Current->Me);++state.validTests;return &thing!=&Thing&&TS(&thing).Valid&&(thing.UsingComponents._M_w[TCI_GOSSIP/(8*sizeof(unsigned long))]&(1UL<<(TCI_GOSSIP%(8*sizeof(unsigned long)))));}
CTCBase*CThing::AddTC(const CCharString&name,bool flag,const CTCParamsBase*param){assert(this==&Current->Me&&state.solidCtorSeen&&state.solidNameAlive&&strcmp(name.GetAsCharArray(),"CTCSolidToThing")==0&&!flag&&!param&&!Current->Solid);Current->Solid=new FixtureSolid(*this);::AddTC(*this,TCI_SOLID_TO_THING,Current->Solid);return Current->Solid;}
void CThing::RemoveTC(ETCInterfaceType type){assert(this==&Current->Me&&type==TCI_SOLID_TO_THING&&state.interacted==1&&Current->Solid);++state.solidRemoved;delete Current->Solid;Current->Solid=0;RemoveSlot(*this,TCI_SOLID_TO_THING);}
void CTCSolidToThing::AddThingNotSolidTo(const CThing*thing){assert(this==Current->Solid&&thing==&Current->Other&&!state.solidNameAlive);++state.solidAdds;NotSolidToThings.push_back(CIntelligentPointer<const CThing>(thing));}
CWorld&CAIStateGroupBase::GetWorld()const{return Current->World;}
unsigned long*CAIStateGroupBase::GetPWorldSeed()const{assert(!state.terminated);return &Current->Seed;}
long FABLE_FASTCALL GFROR13(unsigned long value){unsigned long expected=(unsigned long)state.randomSeed*0x24a1ul+0x24dful;assert(value==expected&&state.randomSeed>=0&&state.randomSeed<=2&&!state.randomCalls);++state.randomCalls;unsigned int word=(unsigned int)value;unsigned int rotated=(word>>13)|(word<<19);assert(rotated<0x80000000u);return (long)rotated;}
EAIActionReturn CAIStateGroupBase::AIAction_DoNothing(){assert(!state.terminated&&state.requests==1&&!state.looks);++state.waitCalls;++state.frames;if(state.arrival>=0&&state.frames>=state.arrival){Current->SelfGossip.Gossipee=&Current->Other;Current->Other.Dead=false;}return state.waitCalls==state.waitTerminate?AI_ACTION_TERMINATE_PROCESS:AI_ACTION_FINISHED;}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,long frames,float speed){assert(!state.terminated&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&thing.Get()==&Current->Other&&frames==17&&speed==0.0027777f&&!state.looks);++state.looks;return(EAIActionReturn)state.lookResult;}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,float distance,bool&nav,EMovementType movement,bool a,bool b,bool c,bool d,float last){assert(!state.terminated&&state.looks==1&&state.lookResult==AI_ACTION_FINISHED&&repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==&Current->Other&&distance==2.0f&&movement==AI_MOVEMENT_WALKING&&a&&!b&&c&&d&&last==0.0f);nav=state.moves%2!=0;++state.moves;if(state.moves<=state.moveProgress)return AI_ACTION_IN_PROGRESS;if(state.killMove==1)Current->Other.Dead=true;else if(state.killMove==2)Invalidate(&Current->Other);return(EAIActionReturn)state.moveFinal;}
bool CTCCarrying::IsThingCarryingWeapon(const CThing&thing){assert(&thing==&Current->Me&&state.moves==state.moveProgress+1&&state.moveFinal==AI_ACTION_FINISHED);return state.carrying!=0;}
CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons(CThingCreatureBase&thing):CCombatActionBase(){assert(&thing==&Current->Me&&state.carrying&&!state.terminated);CurrentAction=this;ActionKind=1;++state.constructed;}
CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons(CThingCreatureBase&thing):CCombatActionBase(){assert(&thing==&Current->Me&&state.carrying&&state.interacted==1&&state.solidRemoved==1&&!state.terminated);CurrentAction=this;ActionKind=2;++state.constructed;}
EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&action,bool wait){assert(!state.terminated&&!wait);if(ActionKind==1){++state.sheathed;if(state.killSheathe==1)Current->Other.Dead=true;else if(state.killSheathe==2)Invalidate(&Current->Other);return(EAIActionReturn)state.sheatheResult;}assert(ActionKind==2&&Current->Script.WasCarryingWeapon);++state.unsheathed;return(EAIActionReturn)state.unsheatheResult;}
EAIActionReturn CAIStateGroupBase::AIAction_InteractWith(EAIActionRepeat repeat,CIntelligentPointer<CThingAICreature>other,CCharString interaction,int priority,bool submissive){assert(!state.terminated&&Current->Script.Critical&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&other.Get()==&Current->Other&&priority==100&&submissive==(state.removed==0)&&state.solidAdds==1&&state.randomCalls==1);static const char*names[]={"VILLAGER_INTERACTION_BANDIT_PUSHING","VILLAGER_INTERACTION_BANDIT_HOLDING","VILLAGER_INTERACTION_BANDIT_TELLING_OFF"};static const int choices[]={2,0,1};assert(strcmp(interaction.GetAsCharArray(),names[choices[state.randomSeed]])==0);++state.interacted;return(EAIActionReturn)state.interactionResult;}

const C3DVector&CThing::GetPos()const{return TS(this).Pos;}
const C3DVector&CThingPhysical::PeekPos()const{return TS(this).Pos;}
void CAIStateGroupBase::Terminate(){assert(!state.terminated);state.terminated=true;}
long FABLE_FASTCALL CWorld::GetFrame(){return state.frames;}
long CMainGameComponent::ConstantFPS=5;

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
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
bool CAIStateGroup_MinionGossip::IsTimingCritical()const { abort(); }
void CAIStateGroup_MinionGossip::InitStateGroup(const CWorld&,CThingAICreature&) { abort(); }
void CAIStateGroup_MinionGossip::Update() { abort(); }
bool CAIStateGroup_MinionGossip::IsRunnable() { abort(); }
void CAIStateGroup_MinionGossip::OnReplace() { abort(); }
bool CAIStateGroup_MinionGossip::IsStillValid()const { abort(); }
long CAIStateGroup_MinionGossip::GetTimeToInitiate()const { abort(); }
long CAIStateGroup_MinionGossip::GetTimeToRespond()const { abort(); }
bool CAIStateGroup_MinionGossip::UseLineOfSight() { abort(); }
void CAIStateGroup_BanditGossip::OnReplace() { abort(); }
long CAIStateGroup_BanditGossip::GetTimeToInitiate()const { abort(); }
long CAIStateGroup_BanditGossip::GetTimeToRespond()const { abort(); }
bool CAIStateGroup_BanditGossip::UseLineOfSight() { abort(); }
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
void CCombatActionBase::PerformAction() { abort(); }
CCreatureActionBase* CCombatActionBase::Clone() const { abort(); }
void CCreatureAction_SheatheWeapons::Initialise() { abort(); }
void CCreatureAction_SheatheWeapons::PerformAction() { abort(); }
CCreatureActionBase* CCreatureAction_SheatheWeapons::Clone() const { abort(); }
int main(){CGame game;Game=&game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);Component=&component;CGameDefinitionManager definitions;Definitions=&definitions;CPlayerManager players;Players=&players;CNavigatorManager navigation;Navigation=&navigation;unsigned cases=0;int coverage[10]={0,0,0,0,0,0,0,0,0,0};
 static const int results[]={2,3,1,0};
 for(int seed=0;seed<4352;++seed){assert(Tokens.empty());memset(&state,0,sizeof(state));state.frames=100;state.fps=5;state.wantIncoming=seed%2;state.killRemove=seed/2%3;state.initialPartner=seed/5%2;state.initialDead=seed/7%2;state.arrival=seed/9%3==0?120:seed/9%3==1?121:-1;state.waitTerminate=seed/13%3==0?1:seed/13%3==1?3:0;state.lookResult=seed/17%5;state.moveProgress=seed/19%3;state.moveFinal=results[seed/23%4];state.carrying=seed/29%2;state.sheatheResult=results[seed/31%4];state.unsheatheResult=results[seed/37%4];state.killMove=seed/41%3;state.killSheathe=seed/43%3;state.interactionResult=seed/47%5;state.initialSolid=seed/53%2;state.randomSeed=seed/59%3;
  if(seed>=4096){int edge=seed-4096;state.wantIncoming=edge%2;state.killRemove=0;state.initialPartner=1;state.initialDead=0;state.arrival=120;state.waitTerminate=0;state.lookResult=2;state.moveFinal=2;state.carrying=edge/2%2;state.sheatheResult=2;state.unsheatheResult=results[edge/4%4];state.killMove=0;state.killSheathe=0;state.randomSeed=edge/16%3;}
  {Context context;Current=&context;state.preparing=true;C3DVector zero;zero.X=zero.Y=zero.Z=0.0f;
   if(state.wantIncoming)context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Other,0,16,16,0);
   context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Invalid,0,16,16,0);
   context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Far,0,16,16,0);
   context.Manager.AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP,20,zero,&context.Wrong,0,16,16,0);
   context.Manager.AddMessage((EMessageEventType)0,20,zero,&context.Other,0,16,16,0);
   state.preparing=false;context.Script.Update();assert(state.terminated&&!context.Script.Critical&&state.strings==0&&state.constructed==state.destroyed);
   assert(state.removed==state.wantIncoming&&state.requests==1-state.wantIncoming&&state.kindTests>=4&&state.rangeTests>=2&&state.validTests>=1);
   if(state.removed&&(!state.initialDead&&!state.killRemove)){assert(context.SelfGossip.MyTurnToSpeak&&!context.OtherGossip.MyTurnToSpeak&&context.OtherGossip.Gossipee.Get()==&context.Me);++coverage[0];}
   if(state.removed&&(state.initialDead||state.killRemove)){assert(!state.looks&&!state.moves&&!state.interacted&&!context.Script.WasCarryingWeapon);++coverage[1];}
   if(!state.removed){if(state.looks){assert((state.initialPartner&&!state.initialDead)||state.arrival==120);assert(context.SelfGossip.Gossipee.Get()==&context.Other||state.killMove==2||state.killSheathe==2);++coverage[2];}else{assert(state.waitCalls>0&&state.frames<=121);++coverage[3];}}
   if(state.moves)assert(state.lookResult==AI_ACTION_FINISHED&&state.moves==state.moveProgress+1);
   if(state.interacted){assert(state.solidRemoved==1&&state.randomCalls==1);++coverage[4];if(state.carrying){assert(state.sheathed==1&&state.sheatheResult==AI_ACTION_FINISHED&&state.unsheathed==1);assert(context.Script.WasCarryingWeapon==(state.unsheatheResult!=AI_ACTION_FINISHED));++coverage[5];}else assert(!state.sheathed&&!state.unsheathed&&!context.Script.WasCarryingWeapon);if(!state.initialSolid)assert(state.solidCtorSeen);else assert(!state.solidCtorSeen);++coverage[7+state.randomSeed];}
   else{assert(!state.solidRemoved&&!state.randomCalls);if(state.sheathed&&state.sheatheResult==AI_ACTION_FINISHED)assert(context.Script.WasCarryingWeapon);else assert(!context.Script.WasCarryingWeapon);if(state.killMove==2||state.killSheathe==2)++coverage[6];}
  }
  assert(Tokens.empty()&&state.tokenNew==state.tokenDelete&&state.baseNew==state.baseDelete&&state.strings==0);++cases;
 }

 unsigned searches=0;for(int count=0;count<=32;++count){CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >map;SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(map),33);for(int n=0;n<count;++n)map.push_back(std::make_pair((ETCInterfaceType)(n/2*3),(CTCBase*)0));for(int key=0;key<100;++key){ETCInterfaceType k=(ETCInterfaceType)key;int expected=0;while(expected<count&&map[expected].first<k)++expected;assert(map.LowerBound(k)==map.begin()+expected);++searches;}}assert(searches==3300u);
 memset(&state,0,sizeof(state));
 {CThingAICreature creature;CIntelligentPointer<CThingAICreature> empty=0;assert(IS_THING_DEAD(empty)&&!IS_THING_ALIVE(empty));CIntelligentPointer<CThingAICreature> first=&creature;assert(!IS_THING_DEAD(first)&&IS_THING_ALIVE(first));{const CIntelligentPointer<CThingAICreature>&reference=first;CIntelligentPointer<CThingAICreature> second(reference);assert(second.Get()==&creature&&first.Get()==&creature);creature.Dead=true;assert(IS_THING_DEAD(second)&&!IS_THING_ALIVE(second));creature.Dead=false;Invalidate(&creature);assert(!second.Get()&&!first.Get()&&IS_THING_DEAD(second));}assert(IS_THING_DEAD(first));}
 assert(Tokens.empty()&&state.tokenNew==state.tokenDelete&&state.baseNew==state.baseDelete);
 assert(cases==4352u);for(int n=0;n<10;++n)assert(coverage[n]>0);puts("0090e5d0_TEST PASS");return 0;}