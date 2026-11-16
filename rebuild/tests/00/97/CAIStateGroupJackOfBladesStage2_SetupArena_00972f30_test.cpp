// Self-contained SetupArena fixture. Selected production body is pasted below.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
enum ESex{};enum EAIStateGroupType{};enum EScriptAIPriority{};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};enum EPlayerSpawnStatus{};enum ESaveType{};
namespace NCameraModes {enum EMode{};}
#endif
#include "engine/EPlayerMode.h"
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWIDESTRING
#define private public
#define protected public
#include "engine/fable_ai_action_enums.h"
// 00972f30 CAIStateGroup_JackOfBladesStage2::SetupArena
// Flags: /GS
// Leaf: ?SetupArena@CAIStateGroup_JackOfBladesStage2@@MAEXXZ
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_C2DCOORDI_COPY
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#include <stdio.h>
#define FABLE_RETAIL_JACK_SETUPARENA_PASS23
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CCameraManagerSetDef.h"
#include "engine/CGameCameraManager.h"
void CAIStateGroup_JackOfBladesStage2::SetupArena()
{
 CAIStateGroup_JackOfBladesBase::SetupArena();
 CThing* pMarkerThing=0;
 long markerNumber=0;
 char markerName[25];
 for(;;){
  ++markerNumber;
  pMarkerThing=0;
  sprintf(markerName,"jack_pillar%d",markerNumber);
  PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),100),CThingFilter_HasScriptName(markerName),&pMarkerThing);
  if(IS_THING_ALIVE(pMarkerThing))PillarMarkers.push_back(pMarkerThing);
  if(!IS_THING_ALIVE(pMarkerThing))break;
 }
 std::vector<CThing*> things;
 long pillar_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_FRESCO_DOME_PILLAR_01");
 PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),100),CThingFilter_HasDefGlobalIndex(pillar_def_index),things);
 for(std::vector<CThing*>::iterator it=things.begin();it!=things.end();it++){
  CThing* pillar=*it;
  Pillars.push_back(pillar);
 }
 pMarkerThing=0;
 PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),100),CThingFilter_HasScriptName("JACK_TO_LOOK"),&pMarkerThing);
 if(pMarkerThing)PCutsceneLookAtMarker=pMarkerThing;
 CGameCameraManager* pcamera_manager;
 if(GetWorld().GetGameCamera()->GetCameraManager(&pcamera_manager)){
  CDefPointer<const CCameraManagerSetDef> pcam_man_def;
  PeekDefinitionManager().GetDef("CAMERA_MANAGER_SET_JACK_BOSS_FIGHT",&pcam_man_def);
  pcamera_manager->SetCameraManagerSetDef(pcam_man_def);
 }
}

#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CWorldMap.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CThingManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CNavigatorManager.h"
#include "engine/CDefString.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCamera.h"
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
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
CWorldMap& CWorld::GetWorldMap() { abort(); }
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}

CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){}
CTCBase::CTCBase(CThing&t):ParentThing(&t){}CTCBase::~CTCBase(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
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
CThingSearchTools::CThingSearchTools(CThingManager&t,CWorldMap&w):WorldMap(w),PThingMapManager(0),ThingManager(t){}CThingSearchTools::~CThingSearchTools(){}

CIDrawWorld::~CIDrawWorld(){} CWorld::~CWorld(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
static int pointerBirths,pointerDeaths,stringCount;
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++pointerBirths;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);++pointerDeaths;}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;unsigned long size=(unsigned long)strlen(s);PStringData->Data.PData=new char[size+1];memcpy(PStringData->Data.PData,s,size+1);PStringData->Data.StringLength=size;PStringData->NoRefs=1;++stringCount;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->NoRefs==0){delete[]PStringData->Data.PData;delete PStringData;--stringCount;}}
const char CCharString::NullChar=0;
CAIStateGroupBase::CAIStateGroupBase():PBrain(0),Terminated(false){}
void CAIStateGroup_JackOfBladesBase::Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&){abort();}
EAIStateGroupType CAIStateGroup_JackOfBladesBase::GetStateGroupType()const{abort();}
void CAIStateGroup_JackOfBladesBase::Cleanup(){abort();}void CAIStateGroup_JackOfBladesBase::OnReplace(){abort();}void CAIStateGroup_JackOfBladesBase::OnActivate(){abort();}bool CAIStateGroup_JackOfBladesBase::IsTimingCritical()const{abort();}
void CAIStateGroup_JackOfBladesStage2::Cleanup(){abort();}void CAIStateGroup_JackOfBladesStage2::Update(){abort();}void CAIStateGroup_JackOfBladesStage2::OnActivate(){abort();}bool CAIStateGroup_JackOfBladesStage2::IsRunnable(){abort();}
void CAIGameCameraBase::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const{abort();}
bool CAIGameCameraBase::IsPosInViewingFrustrum(const C3DVector& pos) const{abort();}
bool CAIGameCameraBase::Is2DAreaInViewingFrustrum(const C3DVector& pos, float radius) const{abort();}
bool CAIGameCameraBase::IsVolumeInViewingFrustrum(const C3DVector& pos, float radius) const{abort();}
bool CAIGameCameraBase::IsLineOfSightToPos(const C3DVector& pos) const{abort();}
float CAIGameCameraBase::GetControlAngleXY() const{abort();}
void CAIGameCameraBase::SetChangeInControlDirection(){abort();}
bool CAIGameCameraBase::GetCameraManager(const CGameCameraManager** manager) const{abort();}
bool CAIGameCameraBase::GetCameraManager(CGameCameraManager** manager){abort();}
const C3DVector CAIGameCameraBase::PeekCurrentPos() const{abort();}
bool CAIGameCameraBase::IsFreeLookMode() const{abort();}
bool CAIGameCameraBase::IsOverridingMainAngleXY() const{abort();}
void CAIGameCameraBase::ScriptEnd(){abort();}
void CAIGameCameraBase::ScriptStart(){abort();}
void CAIGameCameraBase::ScriptMoveToPosAndLookAtPos(const C3DVector& pos, const C3DVector& lookAt, float speed){abort();}
void CAIGameCameraBase::ScriptMoveToPosAndLookAtThing(const CThingGameObject& thing, const C3DVector& pos, float speed){abort();}
void CAIGameCameraBase::ScriptMoveToThingAndLookAtPos(const CThingGameObject& thing, const C3DVector& offset, const C3DVector& lookAt, float speed, bool relative){abort();}
void CAIGameCameraBase::ScriptMoveToThingAndLookAtThing(const CThingGameObject& thing, const CThingGameObject& lookAt, const C3DVector& offset, float speed, bool relative){abort();}
void CAIGameCameraBase::ScriptSetRotationSpeed(float speed){abort();}
void CAIGameCameraBase::SetDebugCameraType(long type){abort();}
void CAIGameCameraBase::SetLookaroundVector(const C3DVector& vector){abort();}
void CAIGameCameraBase::CyclePlayerViewType(){abort();}

CAIBrain::CAIBrain(const CMainGameComponent&c,CWorld&w,CThingManager&t,CPlayerManager&p,CThingAICreature&cr,CDefPointer<const CBrainDef>):Component(const_cast<CMainGameComponent*>(&c)),Creature(cr),ThingManager(&t),PlayerManager(&p),World(&w){}
static CWorld* activeWorld;static CThingSearchTools* activeSearch;static CGameDefinitionManager* activeDefs;
static int setupCalls,searchCalls,cameraCalls,definitionCalls,setCalls;static bool haveCamera,haveDefinition;
static CCameraManagerSetDef* activeCameraDef;static CGameCameraManager* activeCamera;
struct FixtureThingInfo{CThing*thing;char name[32];unsigned long def;};static FixtureThingInfo fixtureThings[16];static int fixtureThingCount;
static const FixtureThingInfo&Info(const CThing&t){for(int i=0;i<fixtureThingCount;++i)if(fixtureThings[i].thing==&t)return fixtureThings[i];abort();}
static NThingMap::CThingMap* activeMap;
static C3DVector creaturePos(32,64,0);
CWorld&CAIStateGroupBase::GetWorld()const{return *activeWorld;}
const CThingSearchTools&CAIStateGroupBase::PeekThingSearchTools()const{return *activeSearch;}
const CGameDefinitionManager&CAIStateGroupBase::PeekDefinitionManager()const{return *activeDefs;}
CGameDefinitionManager&FABLE_FASTCALL CGameDefinitionManager::Get(){return *activeDefs;}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&n)const{assert(strcmp(n.GetAsCharArray(),"OBJECT_FRESCO_DOME_PILLAR_01")==0);return 71;}
void CAIStateGroup_JackOfBladesBase::SetupArena(){assert(searchCalls==0);++setupCalls;}
const C3DVector&CThing::GetPos()const{return creaturePos;}
unsigned long CThingSearchTools::NoLookups=0,CThingSearchTools::NoThingsScanned=0,CThingSearchTools::SearchID=0;
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI&a,std::vector<NThingMap::CThingMap*>&out)const{assert(a.TLX==-68&&a.TLY==-36&&a.BRX==132&&a.BRY==164);++searchCalls;out.clear();out.push_back(activeMap);}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&m,const C2DBoxI&)const{assert(&m==activeMap);return C2DBoxI(0,0,1,1);}
namespace NThingMap {long MaxLayers=1;const CLayerInfo LayerInfo[1]={{1,0}};CThingMap::~CThingMap(){}}
bool CThingFilter_HasScriptName::operator()(const CThing&t)const{return strcmp(Name.GetAsCharArray(),Info(t).name)==0;}
bool CThingFilter_HasDefGlobalIndex::operator()(const CThing&t)const{return DefGlobalIndex==Info(t).def;}
void CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>&d){assert(this==activeCamera&&definitionCalls==1);assert(d.Object==(haveDefinition?activeCameraDef:0));++setCalls;}
template<class T>bool CDefinitionManager::GetDef(const CCharString&n,CDefPointer<const T>*out)const{assert(this==activeDefs&&strcmp(n.GetAsCharArray(),"CAMERA_MANAGER_SET_JACK_BOSS_FIGHT")==0);++definitionCalls;assert(!out->Object);if(haveDefinition){out->Object=activeCameraDef;activeCameraDef->AddRef();return true;}return false;}
bool CResourceList::HandleFailedAllocation(unsigned long){abort();}

CTCMapwho::CTCMapwho(CThing&t):CTCBase(t),SearchID(0),InMapwho(false){}CTCMapwho::~CTCMapwho(){}
void CTCMapwho::OnDie(){abort();}ETCInterfaceType CTCMapwho::GetInterfaceType()const{abort();}CCharString CTCMapwho::GetClassTypeName()const{abort();}
CCamera::CCamera(const C3DVector&p,const C3DVector&f,const C3DVector&u,float h,float v):WorldPosition(p),HorizontalFOV(h),VerticalFOV(v){RHSet.Up=u;RHSet.Forward=f;}
namespace NCameraModes{CTools::CTools(const CGameCameraManager&c):CameraManager(const_cast<CGameCameraManager*>(&c)){memset(ScanThings,0,sizeof ScanThings);}}
CGameCameraManager::CGameCameraManager(const CWorld&w):Camera(C3DVector(0,0,0),C3DVector(0,1,0),C3DVector(0,0,1),1,1),OldCamera(C3DVector(0,0,0),C3DVector(0,1,0),C3DVector(0,0,1),1,1),World(w),CamTools(*this){}
CGameCameraManager::~CGameCameraManager(){}
bool CGameCameraManager::GetCameraManager(CGameCameraManager**out){++cameraCalls;if(haveCamera)*out=this;return haveCamera;}
void CCameraManagerSetDef::Transfer(CPersistContext&){abort();}void CCameraManagerSetDef::Copy(const CDefClassBase*){abort();}unsigned long CCameraManagerSetDef::GetSizeofClass(){return sizeof(*this);}
void CGameCameraManager::Update(){abort();}
CCamera CGameCameraManager::GetRenderCamera(float interpolation) const{abort();}
void CGameCameraManager::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const{abort();}
bool CGameCameraManager::IsPosInViewingFrustrum(const C3DVector& pos) const{abort();}
bool CGameCameraManager::Is2DAreaInViewingFrustrum(const C3DVector& pos, float radius) const{abort();}
bool CGameCameraManager::IsVolumeInViewingFrustrum(const C3DVector& pos, float radius) const{abort();}
bool CGameCameraManager::IsLineOfSightToPos(const C3DVector& pos) const{abort();}
float CGameCameraManager::GetControlAngleXY() const{abort();}
void CGameCameraManager::SetChangeInControlDirection(){abort();}
bool CGameCameraManager::GetCameraManager(const CGameCameraManager** manager) const{abort();}
const C3DVector CGameCameraManager::PeekCurrentPos() const{abort();}
float CGameCameraManager::GetAngleXY() const{abort();}
float CGameCameraManager::GetAngleYZ() const{abort();}
void CGameCameraManager::SetMainThing(const CThingCreatureBase* thing){abort();}
void CGameCameraManager::SetViewThing(const CThingGameObject* thing){abort();}

int main(){
 CGame game;CMainGameComponentInit init;CMainGameComponent component(game,init);CWorld world;CWorldMap map;world.PWorldMap_ptr=&map;CGameDefinitionManager defs;CPlayerManager players;CNavigatorManager navigation;unsigned long seed=0;CThingManager things(component,defs,world,players,navigation,&seed,0,0,0);CThingSearchTools search(things,map);activeWorld=&world;activeSearch=&search;activeDefs=&defs;
 CThingAICreature creature;CAIBrain brain(component,world,things,players,creature,CDefPointer<const CBrainDef>());CGameCameraManager camera(world);world.PCurrentGameCamera=&camera;activeCamera=&camera;CCameraManagerSetDef cameraDef;activeCameraDef=&cameraDef;cameraDef.IVRefCount=1;
 unsigned cases=0;
 for(int markers=0;markers<=4;++markers)for(int pillars=0;pillars<=3;++pillars)for(int stopDead=0;stopDead<2;++stopDead)for(int look=0;look<3;++look)for(int cameraMode=0;cameraMode<3;++cameraMode)for(int initial=0;initial<2;++initial){
  setupCalls=searchCalls=cameraCalls=definitionCalls=setCalls=0;haveCamera=cameraMode!=0;haveDefinition=cameraMode==2;fixtureThingCount=0;
  CThing objs[12];CTCMapwho*who[12];NThingMap::CMapwhoNode nodes[13];NThingMap::CThingMap grid;activeMap=&grid;
  int total=markers+stopDead+pillars+(look?1:0);
  for(int i=0;i<total;++i){objs[i].Dead=false;objs[i].ThingTypeFlags=1;who[i]=new CTCMapwho(objs[i]);nodes[i].PMapwho=who[i];nodes[i].PNext=i+1<total?&nodes[i+1]:0;nodes[i].PPrevious=0;nodes[i].PCell=0;FixtureThingInfo&r=fixtureThings[fixtureThingCount++];r.thing=&objs[i];r.name[0]=0;r.def=0;
   if(i<markers+stopDead)sprintf(r.name,"jack_pillar%d",i+1);
   if(stopDead&&i==markers)objs[i].Dead=true;
   if(i>=markers+stopDead&&i<markers+stopDead+pillars)r.def=71;
   if(look&&i==total-1){strcpy(r.name,"JACK_TO_LOOK");objs[i].Dead=look==2;}
  }
  // Duplicate the first mapwho node: the real search stamp must prevent duplicate result insertions.
  if(total){nodes[total]=nodes[0];nodes[total].PNext=0;nodes[total-1].PNext=&nodes[total];}
  NThingMap::CMapwhoLevelInfo level;level.Size=1;level.Extents.X=level.Extents.Y=1;level.ScaleShift=0;level.NoThings=total;grid.MapwhoLevelInfo.push_back(level);
  std::vector<NThingMap::CThingMapCell>cells;NThingMap::CThingMapCell cell;cell.PFirstNode=total?nodes:0;cells.push_back(cell);grid.Cells.push_back(cells);
  {CAIStateGroup_JackOfBladesStage2 state;state.PBrain=&brain;if(initial){state.PillarMarkers.push_back(&objs[11]);state.Pillars.push_back(&objs[11]);state.PCutsceneLookAtMarker=&objs[11];}state.SetupArena();assert(setupCalls==1&&searchCalls==markers+3&&cameraCalls==1);assert(state.PillarMarkers.size()==(unsigned)(markers+initial)&&state.Pillars.size()==(unsigned)(pillars+initial));
   for(int i=0;i<markers;++i)assert(state.PillarMarkers[i+initial].Get()==&objs[i]);for(int i=0;i<pillars;++i)assert(state.Pillars[i+initial].Get()==&objs[markers+stopDead+i]);assert(state.PCutsceneLookAtMarker.Get()==(look?&objs[total-1]:(initial?&objs[11]:0)));assert(definitionCalls==(haveCamera?1:0)&&setCalls==definitionCalls);assert(cameraDef.IVRefCount==1);
  }
  for(int i=0;i<total;++i)delete who[i];assert(stringCount==0);++cases;
 }
 assert(cases==720);puts("00972f30_TEST PASS");return 0;
}