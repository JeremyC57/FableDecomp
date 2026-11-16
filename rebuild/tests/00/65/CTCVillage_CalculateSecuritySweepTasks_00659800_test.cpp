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
#include "engine/ETCInterfaceType.h"
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
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_CTOR_PASS23
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_CTCTHINGOWNER_CTOR_PASS23
#include "engine/fable_ai_action_enums.h"
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
// Selected production source, pasted verbatim.
// 00659800 CTCVillage::CalculateSecuritySweepTasks
// Leaf: ?CalculateSecuritySweepTasks@CTCVillage@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_STLP_FIND_IF_GUARD_BODY_PASS23

#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_STLP_BINARY_COMPOSE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_COUNT_IF_CALLED_FASTCALL_PASS23
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#define FABLE_RETAIL_SECURITY_SWEEP_LIFETIMES_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_SCOPED_PTR_RESET
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_GETSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_VILLAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MEMBERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_CWORLD_GAMETIME
#define FABLE_RETAIL_CTCBASE_GETWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVILLAGETASKPATROL_CTOR
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS_CTOR
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CVillageTaskSecuritySweep.h"
#include "engine/CMainGameComponent.h"
#include <math.h>
#include "engine/CThingCompare_Nearest.h"
#include "engine/HasGuardProperty.h"
#include "stlport/fable_count_if.h"
#include "stlport/fable_algorithm.h"
void CTCVillage::CalculateSecuritySweepTasks()
{
    long num_guards;
    CThing* pguard;
    {
                num_guards = std::count_if(VillagePeople.begin(), VillagePeople.end(), HasGuardProperty());
        std::list<CIntelligentPointer<CThingAICreature> >::iterator guard_it =
            std::find_if(VillagePeople.begin(), VillagePeople.end(), HasGuardProperty());
        if (guard_it == VillagePeople.end())
            return;
        pguard = guard_it->Get();
    }
    CTCCreatureNavigation* ptc_navigation = 0;
    pguard->GetTC(&ptc_navigation);
    CThingPlayerCreature* pplayer = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!pplayer)
        return;
    float radius_of_catchment_circle;
    const C3DVector* exit_position;
    {
    const C3DVector& player_position = pplayer->GetPos();
    CThing* nearest_exit = 0;
    PositionPlayerWasInWhenSecuritySweepCalculated = player_position;
    CMainGameComponent::Get()->GetWorld().PeekMyPlayer()->GetPControlledCreature();
    GetWorld().PeekThingSearchTools().GetPBestThing(
        std::compose2(std::logical_or<bool>(), CThingFilter_HasTC(TCI_REGION_EXIT), CThingFilter_HasTC(TCI_TELEPORTER)),
        CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearest_exit);
    if (!nearest_exit)
        return;
    radius_of_catchment_circle = InitialSecuritySweepRadius -
        (float)((PeekWorld().GetFrame() - FrameStartedSecuritySweep) / CMainGameComponent::GetConstantFPS()) * 0.4f;
    {
    CThingAICreature* nearest_guard;
    if (GetWorld().PeekThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_GUARD),
            CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearest_guard)) {
        float guard_dist = (PositionPlayerWasInWhenSecuritySweepCalculated - nearest_guard->GetPos()).GetMagnitude2D();
        if (radius_of_catchment_circle > guard_dist) {
            InitialSecuritySweepRadius -= radius_of_catchment_circle - guard_dist;
            radius_of_catchment_circle = guard_dist;
        }
    }
    }
    const float minimumRadius = 2.0f;
    if (SecuritySweepUrgent)
        radius_of_catchment_circle = minimumRadius;
    if (radius_of_catchment_circle < minimumRadius)
        radius_of_catchment_circle = minimumRadius;
    exit_position = &nearest_exit->GetPos();
    }
    C3DVector guard_vector = *exit_position - PositionPlayerWasInWhenSecuritySweepCalculated;
    float distance_to_exit = guard_vector.GetSquaredMagnitude2D();
    if (distance_to_exit < radius_of_catchment_circle * radius_of_catchment_circle)
        radius_of_catchment_circle = (float)sqrt(distance_to_exit) * 0.8f;
    guard_vector.Normalise();
    guard_vector *= radius_of_catchment_circle;
    float angle_per_turn = 1.0f / num_guards;
    for (long c1 = 0; c1 < num_guards; ++c1) {
        float current_angle_mod = 0.0f;
        C3DVector t_guard_vector = guard_vector;
        C3DVector pos_to_go_to;
        while (!ptc_navigation->GetNearestNavigablePosTo(t_guard_vector + PositionPlayerWasInWhenSecuritySweepCalculated, 5.0f, pos_to_go_to)) {
            t_guard_vector = guard_vector;
            if (current_angle_mod >= 0.0f) {
                current_angle_mod += angle_per_turn * 0.2f;
                if (current_angle_mod >= angle_per_turn)
                    goto nextGuard;
            }
            else
                current_angle_mod *= -1.0f;
            t_guard_vector.RotateXY(current_angle_mod);
        }
        SecuritySweepTasks.push_back(CCountedPointer<CVillageTaskSecuritySweep>(
            new CVillageTaskSecuritySweep(pos_to_go_to, 5.0f, PositionPlayerWasInWhenSecuritySweepCalculated, 0.25f)));
    nextGuard:
        guard_vector.RotateXY(angle_per_turn);
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
// Unused owning fields stay null; their external deleters assert that contract.
namespace boost {
template<> inline void checked_delete<CEnvironment>(CEnvironment*p){assert(!p);}
template<> inline void checked_delete<CGameTimeManager>(CGameTimeManager*p){assert(!p);}
template<> inline void checked_delete<CAIGameCameraBase>(CAIGameCameraBase*p){assert(!p);}
template<> inline void checked_delete<CGameScriptInterface>(CGameScriptInterface*p){assert(!p);}
template<> inline void checked_delete<CCombatManager>(CCombatManager*p){assert(!p);}
template<> inline void checked_delete<CFactionManager>(CFactionManager*p){assert(!p);}
template<> inline void checked_delete<CBulletTimeManager>(CBulletTimeManager*p){assert(!p);}
template<> inline void checked_delete<CScriptConversationManager>(CScriptConversationManager*p){assert(!p);}
}
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
CThingManager::CThingManager(const CMainGameComponent&c,const CGameDefinitionManager&d,CWorld&w,CPlayerManager&p,CNavigatorManager&n,unsigned long*seed,unsigned long,unsigned long,unsigned long):CClassFactory(0),Component(c),DefinitionManager(d),World(w),WorldMap(*w.PWorldMap),WorldSeed(seed),PlayerManager(p),NavigationManager(n){}CThingManager::~CThingManager(){}


CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::CBaseObject(const CBaseObject&):IntelligentPointer(0){}
CIDrawWorld::~CIDrawWorld(){}
static void StringBorn(const char*){++strings;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;StringBorn(s);}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData){++PStringData->NoRefs;++strings;}}
CCharString::~CCharString(){if(PStringData){assert(strings>0);--strings;if(--PStringData->NoRefs==0)delete PStringData;}}
const char CCharString::NullChar=0;
CWorldInit::CWorldInit():Component(0),ScavengeWorld(0),PlayerManager(0),DefinitionManager(0),DisplayEngine(0){}
static CWorldInit WorldInit;
namespace std {template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);}template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);}}
CWorld::CWorld(const CWorldInit&i):Component(i.Component),PlayerManager(i.PlayerManager),DefinitionManager(*i.DefinitionManager){PWorldMap.reset(new CWorldMap);Paused=false;}
CWorld::~CWorld(){}
static CGame*Game;static CMainGameComponent*Component;static CGameDefinitionManager*Definitions;static CPlayerManager*Players;static CNavigatorManager*Navigation;
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
unsigned long GFastOneOverSqrtTable[128];
// External operations record the receiver/arguments; actual shared owning containers execute normally.
struct Scenario {int guards,player,exit,nearest,urgent,initial,elapsed,partial,guardDistance,exitDistance,exitZ,failures,vary;};
static Scenario scenario;
struct FixtureState {int phase,predicate,lists,controlled,nav,guard,attempt,created,destroyed,inner,outer;float radius,step;};
static FixtureState fs;
static CWorld* ActiveWorld;static CPlayer* ActivePlayer;static CThingPlayerCreature* ActiveHero;static CTCVillage* ActiveVillage;static CTCCreatureNavigation* ActiveNav;
static std::list<CClass*> ScanThings;
struct PositionRecord {const CThing* thing;C3DVector position;bool guard;};static PositionRecord Positions[12];static int PositionCount;
static PositionRecord& Rec(const CThing*p){for(int i=0;i<PositionCount;++i)if(Positions[i].thing==p)return Positions[i];abort();}
static C3DVector Center(32,64,4);
static bool Close(float a,float b){return fabs(a-b)<=0.001f;}
static void InitThing(CThing&t,C3DVector pos,bool guard){memset(&t.UsingComponents,0,sizeof t.UsingComponents);t.Dead=false;t.InLimbo=false;t.Activated=true;t.IncludeInThingScans=true;t.ThingTypeFlags=31;assert(PositionCount<12);PositionRecord&r=Positions[PositionCount++];r.thing=&t;r.position=pos;r.guard=guard;}
static void Flag(CThing&t,ETCInterfaceType type){unsigned id=(unsigned)type;t.UsingComponents._M_w[id/(8*sizeof(unsigned long))]|=1UL<<(id%(8*sizeof(unsigned long)));}
static void AddNavigation(CThing&t,CTCBase*p){typedef std::pair<ETCInterfaceType,CTCBase*> Entry;assert(!t.Components._M_start);t.Components._M_start=std::allocator<Entry>().allocate(1);t.Components._M_finish=t.Components._M_start;t.Components._M_end_of_storage._M_data=t.Components._M_start+1;t.Components.push_back(std::make_pair(TCI_NAVIGATION,p));Flag(t,TCI_NAVIGATION);}
CPlayerManager::CPlayerManager(CMainGameComponent&c,const CGameDefinitionManager&d):Component(&c),DefinitionManager(const_cast<CGameDefinitionManager*>(&d)){}
CThingPlayerCreature::CThingPlayerCreature(){}
CThingSearchTools::CThingSearchTools(CThingManager&t,CWorldMap&m):WorldMap(m),PThingMapManager(0),ThingManager(t){}
CThingSearchTools::~CThingSearchTools(){assert(AreaMaps.empty());}
CTCVillage::CTCVillage(CThing&t):CTCBase(t){}
CTCVillage::~CTCVillage(){}
class FixtureVillage:public CTCVillage {public:FixtureVillage(CThing&t):CTCVillage(t){}ETCInterfaceType GetInterfaceType()const{abort();}CCharString GetClassTypeName()const{abort();}};
CTCCreatureNavigation::CTCCreatureNavigation(CThing&t):CTCBase(t){}
CTCCreatureNavigation::~CTCCreatureNavigation(){}
class FixtureNavigation:public CTCCreatureNavigation {public:FixtureNavigation(CThing&t):CTCCreatureNavigation(t){}ETCInterfaceType GetInterfaceType()const{abort();}CCharString GetClassTypeName()const{abort();}};
bool HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature> p)const{assert(fs.phase==0&&p.Get());++fs.predicate;return Rec(p.Get()).guard;}
namespace std {
template<class I,class P>typename iterator_traits<I>::difference_type FABLE_FASTCALL count_if(I first,I last,P pred){typename iterator_traits<I>::difference_type n=0;for(;first!=last;++first)if(pred(*first))++n;return n;}
// Genuine selected shared find_if/__find_if bodies execute end to end here.
}
CPlayerManager& CTCBase::GetPlayerManager(){assert(this==ActiveVillage&&fs.phase==0);fs.phase=1;return*Players;}
CPlayer&CPlayerManager::GetMainPlayer(){assert(this==::Players&&fs.phase==1);fs.phase=2;return*ActivePlayer;}
CThingPlayerCreature*CPlayer::GetPControlledCreature()const{assert(this==ActivePlayer);++fs.controlled;if(fs.controlled==1){assert(fs.phase==2);fs.phase=3;return scenario.player?ActiveHero:0;}assert(fs.controlled==2&&fs.phase==5);fs.phase=6;return ActiveHero;}
const CPlayer*CWorld::PeekMyPlayer()const{assert(this==ActiveWorld&&fs.phase==4);fs.phase=5;return ActivePlayer;}
CWorld& CTCBase::GetWorld()const{assert(this==ActiveVillage&&(fs.phase==6||fs.phase==10));++fs.phase;return*ActiveWorld;}
const CWorld& CTCBase::PeekWorld()const{assert(this==ActiveVillage&&fs.phase==8);fs.phase=9;return*ActiveWorld;}
long FABLE_FASTCALL CWorld::GetFrame(){assert(fs.phase==9);fs.phase=10;return 100+scenario.elapsed*25+scenario.partial;}
const std::list<CClass*>&CThingSearchTools::PeekClassList()const{assert(this==ActiveWorld->PThingSearchTools.get());++fs.lists;if(fs.lists==1){assert(fs.phase==7);fs.phase=8;}else{assert(fs.lists==2&&fs.phase==11);fs.phase=12;}return ScanThings;}
const C3DVector&CThing::GetPos()const{PositionRecord&r=Rec(this);if(this==ActiveHero){assert(fs.phase==3);fs.phase=4;}else if(fs.phase==12&&!r.guard){assert(r.position.X==Center.X+scenario.exitDistance&&r.position.Z==Center.Z+scenario.exitZ);fs.phase=13;}else assert(fs.phase==8||fs.phase==12);return r.position;}
static int RequiredFailures(int guard){return scenario.vary?(scenario.failures+guard)%6:scenario.failures;}
static C3DVector ExpectedVector(float angle){double a=(double)angle*6.2831853071795864769;double length=sqrt((double)scenario.exitDistance*scenario.exitDistance+(double)scenario.exitZ*scenario.exitZ);double x=(double)scenario.exitDistance*fs.radius/length,z=(double)scenario.exitZ*fs.radius/length;return C3DVector((float)(x*cos(a)),(float)(x*sin(a)),(float)z);}
bool CTCCreatureNavigation::GetNearestNavigablePosTo(const C3DVector&candidate,float radius,C3DVector&out)const{assert(this==ActiveNav&&fs.phase==13&&radius==5.0f&&fs.guard<scenario.guards&&fs.attempt<5);float angle=fs.guard*fs.step+fs.attempt*fs.step*0.2f;C3DVector v=ExpectedVector(angle);assert(Close(candidate.X,Center.X+v.X)&&Close(candidate.Y,Center.Y+v.Y)&&Close(candidate.Z,Center.Z+v.Z));++fs.nav;bool success=fs.attempt>=RequiredFailures(fs.guard);out=C3DVector(64.0f+fs.guard,128.0f+fs.attempt,2);++fs.attempt;return success;}
void C3DVector::RotateXY(float angle){assert(fs.phase==13);if(angle==fs.step){++fs.outer;++fs.guard;fs.attempt=0;}else{assert(fs.attempt>0&&fs.attempt<5&&fabs(angle-fs.attempt*fs.step*0.2f)<0.000001);++fs.inner;}double a=(double)angle*6.2831853071795864769;float x=(float)(X*cos(a)-Y*sin(a));float y=(float)(X*sin(a)+Y*cos(a));X=x;Y=y;}
CVillageTaskBase::CVillageTaskBase(){}CVillageTaskBase::~CVillageTaskBase(){}
CVillageTaskSecuritySweep::CVillageTaskSecuritySweep(const C3DVector&p,float radius,const C3DVector&look,float angle):PositionToGoTo(p),RadiusAroundThePositionToStay(radius),PositionToLookTowards(look),AngleToLookTowards(angle),HasBecomeInvalid(false){assert(fs.phase==13&&fs.guard<scenario.guards&&fs.attempt==RequiredFailures(fs.guard)+1&&RequiredFailures(fs.guard)<5);assert(p.X==64+fs.guard&&p.Y==128+RequiredFailures(fs.guard)&&p.Z==2&&radius==5&&look.X==Center.X&&look.Y==Center.Y&&look.Z==Center.Z&&angle==0.25f);++fs.created;}
CVillageTaskSecuritySweep::~CVillageTaskSecuritySweep(){++fs.destroyed;}
C3DVector CVillageTaskSecuritySweep::GetPositionToGoTo(){return PositionToGoTo;}
float CVillageTaskSecuritySweep::GetRadiusAroundThePositionToStay(){return RadiusAroundThePositionToStay;}
C3DVector CVillageTaskSecuritySweep::GetPositionToLookTowards(){return PositionToLookTowards;}
float CVillageTaskSecuritySweep::GetAngleToLookTowards(){return AngleToLookTowards;}
EVillageTaskType CVillageTaskSecuritySweep::GetTaskType()const{return VILLAGE_TASK_SECURITY_SWEEP;}
bool CVillageTaskSecuritySweep::IsValid()const{return !HasBecomeInvalid;}
bool CVillageTaskSecuritySweep::IsTaskAvailable()const{return CreatureAssignedToTask.Get()==0;}
bool CVillageTaskSecuritySweep::IsCreatureAssignedToTask(const CThing&)const{abort();}
void CVillageTaskSecuritySweep::AssignCreatureToTask(const CThing&){abort();}
bool CVillageTaskSecuritySweep::IsThingInvolvedWithTask(const CThing&)const{abort();}
void CVillageTaskSecuritySweep::RemoveThingFromTask(const CThing&,bool&){abort();}
CMainGameComponent*CMainGameComponent::PMainGameComponent=0;long CMainGameComponent::ConstantFPS=25;
struct Context {
 CThing Owner;CThingPlayerCreature Hero;CThingAICreature Guard[4],Other,Exit;FixtureNavigation Nav;FixtureVillage Village;
 Context():Nav(Guard[0]),Village(Owner){PositionCount=0;InitThing(Owner,C3DVector(0,0,0),false);InitThing(Hero,Center,false);InitThing(Other,C3DVector(512,512,0),false);InitThing(Exit,Center+C3DVector((float)scenario.exitDistance,0,(float)scenario.exitZ),false);
  Village.VillagePeople.push_back(CIntelligentPointer<CThingAICreature>(&Other));
  for(int i=0;i<4;++i){InitThing(Guard[i],Center+C3DVector((float)(scenario.guardDistance+i*4),0,128),true);if(i<scenario.guards){Village.VillagePeople.push_back(CIntelligentPointer<CThingAICreature>(&Guard[i]));if(scenario.nearest)Flag(Guard[i],TCI_GUARD);ScanThings.push_back(&Guard[i]);}}
  // InitThing clears presence; initialize the first guard's real navigation registry after it.
  if(scenario.guards)AddNavigation(Guard[0],&Nav);
  if(scenario.exit)Flag(Exit,scenario.exit==1?TCI_REGION_EXIT:TCI_TELEPORTER);ScanThings.push_back(&Exit);ScanThings.push_back(&Other);
  Village.FrameStartedSecuritySweep=100;Village.InitialSecuritySweepRadius=(float)scenario.initial;Village.SecuritySweepUrgent=scenario.urgent!=0;Village.PositionPlayerWasInWhenSecuritySweepCalculated=C3DVector(-1,-2,-4);
  ActiveHero=&Hero;ActiveVillage=&Village;ActiveNav=&Nav;
 }
 ~Context(){ScanThings.clear();}
};
static void RunCase(){assert(Tokens.empty()&&ScanThings.empty());int beforeBorn=born,beforeDied=died,beforeTB=tokensBorn,beforeTD=tokensDied;memset(&fs,0,sizeof fs);int expectedTasks=0,expectedNav=0,expectedInner=0;
 {Context c;float initial=(float)scenario.initial;bool active=scenario.guards&&scenario.player&&scenario.exit;
  if(active){float r=initial-(float)scenario.elapsed*0.4f;if(scenario.nearest&&r>scenario.guardDistance){initial-=r-scenario.guardDistance;r=(float)scenario.guardDistance;}if(scenario.urgent||r<2)r=2;if(scenario.exitDistance<r)r=(float)scenario.exitDistance*0.8f;fs.radius=r;fs.step=1.0f/scenario.guards;
   for(int i=0;i<scenario.guards;++i){int f=RequiredFailures(i);if(f<5)++expectedTasks;expectedNav+=f<5?f+1:5;expectedInner+=f<5?f:4;}
  }
  c.Village.CalculateSecuritySweepTasks();
  assert(fs.predicate==(scenario.guards?scenario.guards+3:2));assert(fs.controlled==(scenario.guards?(scenario.player?2:1):0));assert(fs.lists==(scenario.guards&&scenario.player?(scenario.exit?2:1):0));
  assert(fs.nav==expectedNav&&fs.inner==expectedInner&&fs.outer==(active?scenario.guards:0)&&fs.created==expectedTasks&&fs.destroyed==0);assert(c.Village.InitialSecuritySweepRadius==initial);C3DVector snapshot=c.Village.PositionPlayerWasInWhenSecuritySweepCalculated;assert(snapshot.X==(scenario.guards&&scenario.player?Center.X:-1)&&snapshot.Y==(scenario.guards&&scenario.player?Center.Y:-2)&&snapshot.Z==(scenario.guards&&scenario.player?Center.Z:-4));
  int tasks=0;std::list<CCountedPointer<CVillageTaskSecuritySweep> >::iterator it=c.Village.SecuritySweepTasks.begin();for(int i=0;i<scenario.guards;++i)if(active&&RequiredFailures(i)<5){assert(it!=c.Village.SecuritySweepTasks.end());CVillageTaskSecuritySweep*p=it->Get();C3DVector pos=p->GetPositionToGoTo(),look=p->GetPositionToLookTowards();assert(pos.X==64+i&&pos.Y==128+RequiredFailures(i)&&pos.Z==2&&look.X==Center.X&&look.Y==Center.Y&&look.Z==Center.Z&&p->GetRadiusAroundThePositionToStay()==5&&p->GetAngleToLookTowards()==0.25f&&p->GetTaskType()==VILLAGE_TASK_SECURITY_SWEEP&&p->IsValid()&&p->IsTaskAvailable());assert(it->GetReferenceCount()==1);++it;++tasks;}assert(it==c.Village.SecuritySweepTasks.end()&&tasks==expectedTasks);
 }assert(fs.destroyed==expectedTasks&&Tokens.empty()&&born-beforeBorn==died-beforeDied&&tokensBorn-beforeTB==tokensDied-beforeTD&&!strings);
}
int main(){for(unsigned i=0;i<128;++i){unsigned int bits=(i|0x1f80u)<<17;float value;memcpy(&value,&bits,4);float inverse=(float)(1.0/sqrt((double)value));memcpy(&bits,&inverse,4);GFastOneOverSqrtTable[i]=((bits+0x2000u)>>15)&0xffu;}GFastOneOverSqrtTable[64]=255;CGame game;Game=&game;CGameDefinitionManager definitions;Definitions=&definitions;CMainGameComponentInit ci;CMainGameComponent component(game,ci);Component=&component;CMainGameComponent::PMainGameComponent=&component;CPlayerManager players(component,definitions);Players=&players;CPlayer player;ActivePlayer=&player;CNavigatorManager navigation;Navigation=&navigation;WorldInit.Component=&component;WorldInit.PlayerManager=&players;WorldInit.DefinitionManager=&definitions;component.PWorld.reset(new CWorld(WorldInit));ActiveWorld=component.PWorld.get();unsigned long seed=1;ActiveWorld->PThingManager.reset(new CThingManager(component,definitions,*ActiveWorld,players,navigation,&seed,0,0,0));ActiveWorld->PThingSearchTools.reset(new CThingSearchTools(*ActiveWorld->PThingManager,*ActiveWorld->PWorldMap));unsigned cases=0;
 const int guards[]={1,2,4};const int initials[]={4,8,16};const int distances[]={1,4,16};const int exits[]={2,8,32};const int failures[]={0,1,4,5};
 for(int g=0;g<3;++g)for(int urgency=0;urgency<2;++urgency)for(int initial=0;initial<3;++initial)for(int elapsed=0;elapsed<3;++elapsed)for(int partial=0;partial<2;++partial)for(int nearest=0;nearest<2;++nearest)for(int distance=0;distance<3;++distance)for(int exit=0;exit<3;++exit)for(int z=0;z<2;++z)for(int fail=0;fail<4;++fail)for(int vary=0;vary<2;++vary){scenario.guards=guards[g];scenario.player=1;scenario.exit=1+(exit&1);scenario.nearest=nearest;scenario.urgent=urgency;scenario.initial=initials[initial];scenario.elapsed=elapsed*5;scenario.partial=partial*24;scenario.guardDistance=distances[distance];scenario.exitDistance=exits[exit];scenario.exitZ=z*8;scenario.failures=failures[fail];scenario.vary=vary;RunCase();++cases;}
 for(int guard=0;guard<2;++guard)for(int player=0;player<2;++player)for(int exit=0;exit<2;++exit){scenario.guards=guard;scenario.player=player;scenario.exit=exit;scenario.nearest=0;scenario.urgent=0;scenario.initial=8;scenario.elapsed=0;scenario.partial=0;scenario.guardDistance=4;scenario.exitDistance=8;scenario.exitZ=0;scenario.failures=0;scenario.vary=0;RunCase();++cases;}
 assert(cases==31112);puts("00659800_TEST PASS");return 0;}