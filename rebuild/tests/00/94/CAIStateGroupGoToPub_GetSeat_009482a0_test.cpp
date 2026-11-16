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
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_CTCTHINGOWNER_CTOR_PASS23
#include "engine/fable_ai_action_enums.h"
// Selected production source, pasted verbatim for the self-contained fixture.
// 009482a0 CAIStateGroup_GoToPub::GetSeat
// Leaf: ?GetSeat@CAIStateGroup_GoToPub@@IAE_NPAVCThing@@PBVCCharString@@@Z
// Flags: none
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCTAVERNTABLE_SEAT
#define FABLE_RETAIL_CTCTAVERNTABLE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCTAVERNTABLE_FULL_LAYOUT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIBrain.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTavernTable.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/GFRandom.h"

bool CAIStateGroup_GoToPub::GetSeat(CThing* table,const CCharString* text)
{
 if(table&&text){
  PTable=CIntelligentPointer<CThingObject>(GFDebugPDynamicCast(table,(CThingObject*)0));
  CTCTavernTable* tc_tavern_table;
  if(PTable->GetTC(&tc_tavern_table)&&tc_tavern_table->GetSeat(GetCreature(),Pos,RHSet,ActionPointIndex,SeatIndex,text)){
   // Fidelity: named ordinary pointer with the original temporary lifetime.
   { CIntelligentPointer<CThing> tankard(tc_tavern_table->GetTankard(SeatIndex));PTankard=tankard; }
   return true;
  }
  ActionPointIndex=-1;
  return false;
 }
 std::list<CThingObject*> tables;
 CTCThingOwner* tc_thing_owner;
 if(IS_THING_ALIVE(PPub)&&PPub->GetTC(&tc_thing_owner)){
  const std::list<CIntelligentPointer<CThing> >& objects=tc_thing_owner->GetObjectList();
  for(std::list<CIntelligentPointer<CThing> >::const_iterator object_it=objects.begin();object_it!=objects.end();++object_it){
   if(IS_THING_ALIVE(*object_it)&&(*object_it)->HasTC(TCI_TAVERN_TABLE))tables.push_back(GFDebugPDynamicCast((*object_it).Get(),(CThingObject*)0));
  }
 }else return false;
 if(tables.empty())return false;
 PTable=CIntelligentPointer<CThingObject>((CThingObject*)0);
 while(!tables.empty()){
  long index=GFRandom(tables.size(),GetPWorldSeed());
  std::list<CThingObject*>::iterator table_it;
  for(table_it=tables.begin();table_it!=tables.end();++table_it){
   if(index==0){PTable=CIntelligentPointer<CThingObject>(*table_it);break;}
   --index;
  }
  CTCTavernTable* tc_tavern_table;
  if(PTable->GetTC(&tc_tavern_table)&&tc_tavern_table->GetSeat(GetCreature(),Pos,RHSet,ActionPointIndex,SeatIndex,0)){
   PTankard=CIntelligentPointer<CThing>(tc_tavern_table->GetTankard(SeatIndex));
   return true;
  }
  tables.erase(table_it);
 }
 ActionPointIndex=-1;
 return false;
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
static int born,died,tokensBorn,tokensDied,strings;
struct FixtureToken {public:CBaseObject*Owner;FixtureToken(CBaseObject*p):Owner(p){}};
static std::vector<FixtureToken*> Tokens;
static void Invalidate(CBaseObject*p){for(unsigned n=0;n<Tokens.size();++n)if(Tokens[n]->Owner==p)Tokens[n]->Owner=0;}
CBaseObject::~CBaseObject(){Invalidate(this);}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++born;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);++died;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return PData?reinterpret_cast<FixtureToken*>(PData)->Owner:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){FixtureToken*t=reinterpret_cast<FixtureToken*>(PData);if(t){bool found=false;for(unsigned n=0;n<Tokens.size();++n)if(Tokens[n]==t){Tokens.erase(Tokens.begin()+n);found=true;break;}assert(found);delete t;PData=0;++tokensDied;}if(p){t=new FixtureToken(p);PData=reinterpret_cast<CBaseObjectPointer*>(t);Tokens.push_back(t);++tokensBorn;}}
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
static void StringBorn(const char*){++strings;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;StringBorn(s);}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData){++PStringData->NoRefs;++strings;}}
CCharString::~CCharString(){if(PStringData){assert(strings>0);--strings;if(--PStringData->NoRefs==0)delete PStringData;}}
const char CCharString::NullChar=0;
CWorldInit::CWorldInit():Component(0),ScavengeWorld(0),PlayerManager(0),DefinitionManager(0),DisplayEngine(0){}
static CWorldInit WorldInit;static CWorldMap WorldMap;
namespace std {template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);}template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);}}
CWorld::CWorld(const CWorldInit&){PWorldMap_ptr=&WorldMap;}
CWorld::~CWorld(){}
static CGame*Game;static CMainGameComponent*Component;static CGameDefinitionManager*Definitions;static CPlayerManager*Players;static CNavigatorManager*Navigation;
CAIBrain::CAIBrain(const CMainGameComponent&c,CWorld&w,CThingManager&t,CPlayerManager&p,CThingAICreature&cr,CDefPointer<const CBrainDef>):Component(const_cast<CMainGameComponent*>(&c)),Creature(cr),ThingManager(&t),PlayerManager(&p),World(&w){}
CAIStateGroupBase::CAIStateGroupBase():PBrain(0){}
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
static void InitThing(CThing&t){memset(&t.UsingComponents,0,sizeof t.UsingComponents);t.Dead=false;t.ThingTypeFlags=31;}
template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static void AddTC(CThing&t,ETCInterfaceType id,CTCBase*p){if(!t.Components._M_start)SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(t.Components),4);t.Components.push_back(std::make_pair(id,p));unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
CTCThingOwner::CTCThingOwner(CThing&t):CTCBase(t),CallOnDie(false){}
CTCTavernTable::CTCTavernTable(CThing&t):CTCBase(t),BeingServed(false),NumPeopleSeated(0),SomeoneTalking(false),SomeoneLaughing(false),ActionPointSitStrlen(0),ActionPointGameMasterStrlen(0),ActionPointHeroStrlen(0),ActionPointTankardStrlen(0),ActionPointMaidStrlen(0),AvailableForUse(true){}
CTCTavernTable::~CTCTavernTable(){}
void CTCTavernTable::OnInitialActivate(){abort();}
class FixtureOwner:public CTCThingOwner{public:FixtureOwner(CThing&t):CTCThingOwner(t){}ETCInterfaceType GetInterfaceType()const{abort();}CCharString GetClassTypeName()const{abort();}};
class FixtureTable:public CTCTavernTable{public:FixtureTable(CThing&t):CTCTavernTable(t){}ETCInterfaceType GetInterfaceType()const{abort();}CCharString GetClassTypeName()const{abort();}};
CAIStateGroup_GoToPub::CAIStateGroup_GoToPub():SeatIndex(-1),ActionPointIndex(-1){}
bool CAIStateGroup_GoToPub::IsRunnable(){abort();}void CAIStateGroup_GoToPub::Update(){abort();}
struct Scenario {int mode,alive,owner,entries,dead,component,available,nullTankard;unsigned long seed;};
static Scenario scenario;
struct Context{
 CThingAICreature Me;CThingBuilding Pub;CThingObject A,B,C,Old,Tankard;
 CWorld World;CThingManager Things;CAIBrain Brain;FixtureOwner Owner;FixtureTable TA,TB,TC;CAIStateGroup_GoToPub Script;CCharString Name;unsigned long Seed;
 Context():World(WorldInit),Things(*Component,*Definitions,World,*Players,*Navigation,&Seed,0,0,0),Brain(*Component,World,Things,*Players,Me,CDefPointer<const CBrainDef>()),Owner(Pub),TA(A),TB(B),TC(C),Seed(scenario.seed){
  InitThing(Me);InitThing(Pub);InitThing(A);InitThing(B);InitThing(C);InitThing(Old);InitThing(Tankard);
  Pub.Dead=scenario.alive==0;if(scenario.owner)AddTC(Pub,TCI_THING_OWNER,&Owner);
  CThingObject* objects[]={&A,&B,&C};FixtureTable* components[]={&TA,&TB,&TC};
  Owner.GetObjectList().push_back(CIntelligentPointer<CThing>((CThing*)0));
  for(int i=0;i<3;++i){objects[i]->Dead=(scenario.dead&(1<<i))!=0;if(scenario.entries&(1<<i))AddTC(*objects[i],TCI_TAVERN_TABLE,(scenario.component&(1<<i))?components[i]:0);Owner.GetObjectList().push_back(CIntelligentPointer<CThing>(objects[i]));}
  Script.PBrain=&Brain;Script.PPub=CIntelligentPointer<CThingBuilding>(scenario.alive==2?0:&Pub);Script.PTable=CIntelligentPointer<CThingObject>(&Old);Script.PTankard=CIntelligentPointer<CThing>(&Old);Script.ActionPointIndex=777;Script.SeatIndex=888;Script.Pos=C3DVector(1,2,4);
 }
};
static Context*Current;
static int Events[64],EventCount;
static void Event(int e){assert(EventCount<64);Events[EventCount++]=e;}
unsigned long*CAIStateGroupBase::GetPWorldSeed()const{assert(this==&Current->Script);Event(10);return &Current->Seed;}
long __fastcall GFROR13(unsigned long n){Event(11);unsigned long v=n&0xffffffffUL;return (long)(((v>>13)|(v<<19))&0xffffffffUL);}
static int TableIndex(const CTCTavernTable*p){if(p==&Current->TA)return 0;if(p==&Current->TB)return 1;assert(p==&Current->TC);return 2;}
bool CTCTavernTable::GetSeat(CThing&creature,C3DVector&pos,CRightHandedSet&rh,long&ap,long&seat,const CCharString*name){int i=TableIndex(this);assert(&creature==&Current->Me&&&pos==&Current->Script.Pos&&&rh==&Current->Script.RHSet&&&ap==&Current->Script.ActionPointIndex&&&seat==&Current->Script.SeatIndex);assert(name==(scenario.mode==0?&Current->Name:0));Event(20+i);pos=C3DVector(8.0f+i,16,32);ap=100+i;seat=200+i;return(scenario.available&(1<<i))!=0;}
CThingObject*CTCTavernTable::GetTankard(long seat){int i=TableIndex(this);assert(seat==200+i);Event(30+i);return scenario.nullTankard?0:&Current->Tankard;}
static unsigned long OracleStep(unsigned long seed){unsigned long v=(seed*0x24a1UL+0x24dfUL)&0xffffffffUL;return((v>>13)|(v<<19))&0xffffffffUL;}
static void RunCase(){assert(Tokens.empty());int beforeBorn=born,beforeDied=died,beforeTB=tokensBorn,beforeTD=tokensDied;EventCount=0;
 {Context c;Current=&c;int expectedEvents[64],ne=0;CThingObject*last=&c.Old;int lastSeat=-1;bool success=false;unsigned long expectedSeed=scenario.seed;bool reset=false;
  if(scenario.mode==0){last=&c.A;reset=true;if((scenario.entries&1)&&(scenario.component&1)){expectedEvents[ne++]=20;lastSeat=0;success=(scenario.available&1)!=0;if(success)expectedEvents[ne++]=30;}}
  else if(scenario.alive==1&&scenario.owner){int pool[3],n=0;for(int i=0;i<3;++i)if((scenario.entries&(1<<i))&&!(scenario.dead&(1<<i)))pool[n++]=i;
   if(n){reset=true;while(n){expectedEvents[ne++]=10;expectedEvents[ne++]=11;expectedSeed=OracleStep(expectedSeed);int index=(int)(expectedSeed%(unsigned long)n);int selected=pool[index];last=selected==0?&c.A:selected==1?&c.B:&c.C;if(scenario.component&(1<<selected)){expectedEvents[ne++]=20+selected;lastSeat=selected;success=(scenario.available&(1<<selected))!=0;if(success){expectedEvents[ne++]=30+selected;break;}}for(int j=index;j+1<n;++j)pool[j]=pool[j+1];--n;}}
  }
  bool actual=c.Script.GetSeat(scenario.mode<2?&c.A:0,scenario.mode==0||scenario.mode==2?&c.Name:0);assert(actual==success&&c.Script.PTable.Get()==last&&c.Seed==expectedSeed);assert(EventCount==ne);for(int e=0;e<ne;++e)assert(Events[e]==expectedEvents[e]);
  assert(c.Script.ActionPointIndex==(success?100+lastSeat:reset?-1:777));assert(c.Script.SeatIndex==(lastSeat<0?888:200+lastSeat));assert(c.Script.Pos.X==(lastSeat<0?1.0f:8.0f+lastSeat));assert(c.Script.Pos.Y==(lastSeat<0?2.0f:16.0f)&&c.Script.Pos.Z==(lastSeat<0?4.0f:32.0f));
  CThing*expectedTankard=success?(scenario.nullTankard?(CThing*)0:&c.Tankard):&c.Old;assert(c.Script.PTankard.Get()==expectedTankard);
 }assert(Tokens.empty()&&born-beforeBorn==died-beforeDied&&tokensBorn-beforeTB==tokensDied-beforeTD&&!strings);
}
int main(){CGame game;Game=&game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);Component=&component;CGameDefinitionManager definitions;Definitions=&definitions;CPlayerManager players;Players=&players;CNavigatorManager navigation;Navigation=&navigation;unsigned cases=0;
for(int mode=0;mode<4;++mode)for(int entries=0;entries<8;++entries)for(int dead=0;dead<8;++dead)for(int available=0;available<8;++available)for(int nullTankard=0;nullTankard<2;++nullTankard){scenario.mode=mode;scenario.alive=1;scenario.owner=1;scenario.entries=entries;scenario.dead=dead;scenario.component=entries;scenario.available=available;scenario.nullTankard=nullTankard;scenario.seed=(unsigned long)(entries+dead+available);RunCase();++cases;}
for(int mode=0;mode<4;++mode)for(int alive=0;alive<3;++alive)for(int owner=0;owner<2;++owner){scenario.mode=mode;scenario.alive=alive;scenario.owner=owner;scenario.entries=7;scenario.dead=0;scenario.component=7;scenario.available=7;scenario.nullTankard=0;scenario.seed=0;RunCase();++cases;}
assert(cases==4120);puts("009482a0_TEST PASS");return 0;}