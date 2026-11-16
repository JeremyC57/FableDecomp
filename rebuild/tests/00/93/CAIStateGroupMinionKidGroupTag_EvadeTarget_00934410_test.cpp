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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
// 00934410 CAIStateGroup_MinionKidGroupTag::EvadeTarget
// Flags: none
// Leaf: ?EvadeTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDGROUPTAG
#define FABLE_RETAIL_KID_EVADE_PASS23
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureGroup.h"
#include "engine/CAIStateGroup_MinionKidGroupTag.h"

#include "engine/CTCCreatureGroupTag.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFWorldDistance.h"
void CAIStateGroup_MinionKidGroupTag::EvadeTarget()
{
 if(GetTCGroupTag().GetTagger()==&GetCreature()) return;
 float distance_apart_sq=GFGet2DDistanceSquared(C2DVector(GetCreature().PeekPos()),C2DVector(GetTCGroupTag().GetTagger()->PeekPos()));
 if(distance_apart_sq<GFSqr(3.0f)) {
  CTCCreatureNavigation* pnav;
  GetCreature().GetTC(&pnav);
  C3DVector pos;
  if(!GFWorld2DDistanceSmallerThan(GetCreature().GetPos(),InitialTagPos,15.0f,false)) pos=InitialTagPos;
  else if(!pnav->GetNearestFleePosition(GetTCGroupTag().GetTagger()->PeekPos(),3.5f,pos)) pos=GetCreature().PeekPos();
  EAIActionReturn ret=AI_ACTION_FAILED;
  bool nav_route_calculated=false;
  if(AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),0x22,0.013888889f)==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
  do {
   ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,pos,1.0f,nav_route_calculated,AI_MOVEMENT_NON_CONSTANT_RUNNING,true,false,true,0,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS && GetTCGroupTag().GetTagger()!=&GetCreature());
 }else if(distance_apart_sq>GFSqr(4.5f)) {
  EAIActionReturn ret=AI_ACTION_FAILED;
  bool nav_route_calculated=false;
  if(AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),0x22,0.013888889f)==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
  do {
   ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),2.5f,nav_route_calculated,AI_MOVEMENT_NON_CONSTANT_RUNNING,true,false,true,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS && GetTCGroupTag().GetTagger()!=&GetCreature());
 }else{
  if(AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger(),0x22,0.013888889f)==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
  EAIActionReturn ret=AI_ACTION_IN_PROGRESS;
  while(ret==AI_ACTION_IN_PROGRESS && GetTCGroupTag().GetTagger()!=&GetCreature())
   ret=AIAction_LookAtPos(ACTION_REPEAT_CALLBACK,GetTCGroupTag().GetTagger()->GetPos(),1,0.013888889f);
  AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(),"BOY_PULLING_FACES_01",CSoundPair(0),false,true,false,0,0));
 }
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



#include "engine/CGroupMemberData.h"

struct Scenario {int distance,axis,home,flee,lookResult,moveCount,moveResult,tagAt,ownTag;};
struct TestState {int tokenNew,tokenDelete,baseNew,baseDelete,strings,constructed,destroyed,moves,looks,positions,flees,terminations,actions;bool navValue;};
static Scenario scenario;static TestState state;struct Context;static Context*Current;
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
class FixtureNavigation:public CTCCreatureNavigation{public:FixtureNavigation(CThing&t):CTCCreatureNavigation(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};

CTCCreatureNavigation::CTCCreatureNavigation(CThing&t):CTCBase(t){}
CTCCreatureNavigation::~CTCCreatureNavigation(){}
CTCCreatureGroup::CTCCreatureGroup(CThing&t):CTCBase(t),MemberCount(0),ActiveMemberCount(0),ShutdownOnEmpty(false){}
CTCCreatureGroup::~CTCCreatureGroup(){}
CTCCreatureGroupTag::CTCCreatureGroupTag(CThing&t):CTCCreatureGroup(t),TaggerUID(0),TaggedFrame(0){}
CTCCreatureGroupTag::~CTCCreatureGroupTag(){}
struct Context {
 CThingAICreature Me,Tagger;CWorld World;CThingManager Things;CAIBrain Brain;FixtureNavigation Nav;CTCCreatureGroupTag Group;CAIStateGroup_MinionKidGroupTag Script;unsigned long Seed;C3DVector Expected;
 Context():World(WorldInit),Things(*Component,*Definitions,World,*Players,*Navigation,&Seed,0,0,0),Brain(*Component,World,Things,*Players,Me,CDefPointer<const CBrainDef>()),Nav(Me),Group(Me),Seed(1){
  ThingCount=0;InitThing(Me);InitThing(Tagger);Script.PBrain=&Brain;Script.PTCGroupTag=&Group;AddTC(Me,TCI_NAVIGATION,&Nav);
  static const float distances[]={0.0f,2.5f,3.0f,3.5f,4.5f,5.0f};float d=distances[scenario.distance];
  TS(&Me).Pos=C3DVector(32,48,128);TS(&Tagger).Pos=C3DVector(32+(scenario.axis==0?d:0),48+(scenario.axis==1?d:0),-128);
  static const float homes[]={0.0f,14.5f,15.0f,16.0f};Script.InitialTagPos=TS(&Me).Pos+C3DVector(homes[scenario.home],0,64);
  Expected=scenario.home>=2?Script.InitialTagPos:scenario.flee?C3DVector(8,16,24):TS(&Me).Pos;
 }
};
static bool Same(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
const CThingAICreature*CTCCreatureGroupTag::GetTagger()const{assert(this==&Current->Group);return scenario.ownTag||(scenario.tagAt>0&&state.moves>=scenario.tagAt)?&Current->Me:&Current->Tagger;}
const C3DVector&CThing::GetPos()const{return TS(this).Pos;}
const C3DVector&CThingPhysical::PeekPos()const{return TS(this).Pos;}
bool CTCCreatureNavigation::GetNearestFleePosition(const C3DVector&center,float radius,C3DVector&pos)const{assert(this==&Current->Nav&&Same(center,TS(&Current->Tagger).Pos)&&radius==3.5f&&scenario.distance<2&&scenario.home<2&&!state.looks);++state.flees;if(scenario.flee)pos=C3DVector(8,16,24);return scenario.flee!=0;}
static void CheckLookSpeed(float speed){unsigned int bits;memcpy(&bits,&speed,4);assert(bits==0x3c638e39u);}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,long frames,float speed){assert(this==&Current->Script&&repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==&Current->Tagger&&frames==0x22&&!state.looks&&!state.moves&&!state.terminations);CheckLookSpeed(speed);++state.looks;return(EAIActionReturn)scenario.lookResult;}
static EAIActionReturn MoveResult(){assert(!state.terminations);++state.moves;return state.moves<=scenario.moveCount?AI_ACTION_IN_PROGRESS:(EAIActionReturn)scenario.moveResult;}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector pos,float proximity,bool&nav,EMovementType movement,bool a,bool b,bool c,const CThing*avoid,bool e,float speed){assert(this==&Current->Script&&repeat==ACTION_REPEAT_CALLBACK&&Same(pos,Current->Expected)&&proximity==1&&movement==AI_MOVEMENT_NON_CONSTANT_RUNNING&&a&&!b&&c&&!avoid&&e&&speed==0&&nav==state.navValue&&scenario.distance<2&&state.looks==1);nav=!nav;state.navValue=nav;return MoveResult();}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat,CIntelligentPointer<const CThing>thing,float proximity,bool&nav,EMovementType movement,bool a,bool b,bool c,bool e,float speed){assert(this==&Current->Script&&repeat==ACTION_REPEAT_CALLBACK&&thing.Get()==&Current->Tagger&&proximity==2.5f&&movement==AI_MOVEMENT_NON_CONSTANT_RUNNING&&a&&!b&&c&&e&&speed==0&&nav==state.navValue&&scenario.distance==5&&state.looks==1);nav=!nav;state.navValue=nav;return MoveResult();}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat repeat,C3DVector pos,long frames,float speed){assert(this==&Current->Script&&repeat==ACTION_REPEAT_CALLBACK&&Same(pos,TS(&Current->Tagger).Pos)&&frames==1&&scenario.distance>=2&&scenario.distance<=4&&state.looks==1);CheckLookSpeed(speed);++state.positions;return MoveResult();}
void CAIStateGroupBase::Terminate(){assert(this==&Current->Script&&!state.terminations&&scenario.lookResult==1&&state.looks==1&&!state.moves);++state.terminations;}
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase&thing,const CCharString&name,const CSoundPair&sound,bool a,bool b,bool c,long d,long e){assert(&thing==&Current->Me&&strcmp(name.GetAsCharArray(),"BOY_PULLING_FACES_01")==0&&!sound.FirstSound&&!sound.LastSound&&!a&&b&&!c&&!d&&!e&&!state.terminations&&scenario.distance>=2&&scenario.distance<=4);++state.constructed;CurrentAction=this;}
EAIActionReturn CAIStateGroupBase::AIAction_PerformAction(const CCreatureActionBase&action){assert(this==&Current->Script&&&action==CurrentAction&&!state.actions&&state.constructed==1);++state.actions;return AI_ACTION_FAILED;}

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
void CAIStateGroup_MinionKidGroupTag::OnActivate() { abort(); }
void CAIStateGroup_MinionKidGroupTag::Cleanup() { abort(); }
bool CAIStateGroup_MinionKidGroupTag::IsRunnable() { abort(); }
void CAIStateGroup_MinionKidGroupTag::Update() { abort(); }
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
ETCInterfaceType CTCCreatureGroup::GetInterfaceType()const { abort(); }
CCharString CTCCreatureGroup::GetClassTypeName()const { abort(); }
bool CTCCreatureGroup::AddNewMember(const CThingAICreature*) { abort(); }
bool CTCCreatureGroup::RemoveMember(const CThingAICreature*) { abort(); }
bool CTCCreatureGroup::DeactivateMember(const CThingAICreature*) { abort(); }
bool CTCCreatureGroup::ActivateMember(const CThingAICreature*) { abort(); }
ETCInterfaceType CTCCreatureGroupTag::GetInterfaceType()const { abort(); }
CCharString CTCCreatureGroupTag::GetClassTypeName()const { abort(); }
bool CTCCreatureGroupTag::RemoveMember(const CThingAICreature*) { abort(); }
bool CTCCreatureGroupTag::DeactivateMember(const CThingAICreature*) { abort(); }

static void RunCase(){assert(Tokens.empty());memset(&state,0,sizeof state);{Context c;Current=&c;c.Script.EvadeTarget();int active=!scenario.ownTag;int stopped=active&&scenario.lookResult==1;int middle=scenario.distance>=2&&scenario.distance<=4;int moves=active&&!stopped?scenario.moveCount+1:0;if(moves&&scenario.tagAt>0&&scenario.tagAt<moves)moves=scenario.tagAt;assert(state.looks==active&&state.terminations==stopped&&state.moves==moves&&state.positions==(middle?moves:0));assert(state.flees==(active&&scenario.distance<2&&scenario.home<2?1:0));int actions=active&&!stopped&&middle;assert(state.actions==actions&&state.constructed==actions&&state.destroyed==actions&&!state.strings);}assert(Tokens.empty()&&state.baseNew==state.baseDelete&&state.tokenNew==state.tokenDelete);}
int main(){CGame game;Game=&game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);Component=&component;CGameDefinitionManager definitions;Definitions=&definitions;CPlayerManager players;Players=&players;CNavigatorManager navigation;Navigation=&navigation;unsigned cases=0;
for(int distance=0;distance<6;++distance)for(int axis=0;axis<2;++axis)for(int home=0;home<4;++home)for(int flee=0;flee<2;++flee)for(int look=0;look<5;++look)for(int own=0;own<2;++own){memset(&scenario,0,sizeof scenario);scenario.distance=distance;scenario.axis=axis;scenario.home=home;scenario.flee=flee;scenario.lookResult=look;scenario.ownTag=own;scenario.moveCount=2;scenario.moveResult=2;RunCase();++cases;}
for(int distance=0;distance<6;++distance)for(int progress=0;progress<4;++progress)for(int result=0;result<4;++result)for(int tag=0;tag<6;++tag){memset(&scenario,0,sizeof scenario);scenario.distance=distance;scenario.lookResult=2;scenario.moveCount=progress;scenario.moveResult=result;scenario.tagAt=tag;scenario.flee=1;RunCase();++cases;}
assert(cases==1536u);puts("00934410_TEST PASS");return 0;}