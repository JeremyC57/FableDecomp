// 0057df85 CTCHeroStats::KickHeroToMostRecentSafeRegion standalone original-object fixture
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
enum EEnginePrimitiveStatIndex{};enum EPlayerMode{};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};enum EPlayerSpawnStatus{};enum ESaveType{};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWORLDMAP_CTOR
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#include "engine/ETCInterfaceType.h"
#define private public
#define protected public
// 0057df85 CTCHeroStats::KickHeroToMostRecentSafeRegion
// Leaf: ?KickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ
// Flags: /GS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_KICK_PASS23
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_KICKREGION_PASS23
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MOVEHEROTOREGION
#define FABLE_RETAIL_CWORLD_KICKREGION_PASS23
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/C3DVector.h"
#include "engine/CTCHeroStats.h"
#include "engine/CGameEvent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CPlayerGui.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasDefName.h"
#include "engine/CThingFilter_HasScriptData.h"
#include "engine/ThingAlive.h"
void CTCHeroStats::KickHeroToMostRecentSafeRegion()
{
    MirroredCriminalStatus best_data;
    long best = 0;
    CCharString current_region(PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetName());
    CThingMarker* kick_marker;
    if (PeekThingSearchTools().GetPFirstLeafThing(
            GFFilter_Bind2(CThingFilter_HasDefName("REGION_KICK_TO_POINT"), CThingFilter_HasScriptData(current_region)),
            &kick_marker)) {
        if(IS_THING_ALIVE(&GetParentThing())) {
        GetWorld().SetToDisplayMiniMapOnRegionLoad(true);
        CGameEvent game_event(0x2c);
        game_event.AddFloat(kick_marker->GetPos().X);
        game_event.AddFloat(kick_marker->GetPos().Y);
        game_event.AddFloat(kick_marker->GetPos().Z);
        game_event.AddFloat(kick_marker->GetPhysicsTC()->GetFacingAngleXY());
        game_event.AddUBYTE(false);
        game_event.AddUBYTE(false);
        game_event.AddUBYTE(false);
        game_event.SetPlayer(GetParentThing().GetPlayerNumber());
        long region_number = PeekWorldMap().GetRegionNumberMapIsIn(kick_marker->GetCurrentMapNumber());
        if (region_number > 0) {
            CPlayerGui* gui = &CPlayerGui::Get();
            gui->SetMiniMapRegion(PeekWorldMap().GetRegion(region_number));
        }
        GetWorld().HandleMoveHeroToRegionGameEvent(game_event);
        }
        return;
    }
    for (std::map<CCharString, MirroredCriminalStatus>::iterator i = OutstandingCrimes.begin(); i != OutstandingCrimes.end(); ++i) {
        long fine;
        long left;
        GetOutstandingFineAndFramesLeft(i->first, fine, left);
        long exited;
        if (fine == 0 && i->first != current_region && (exited = i->second.LastTimeExited) > best) {
            best_data = i->second;
            best = exited;
        }
    }
    if(best!=0 && IS_THING_ALIVE(&GetParentThing())) {
    GetWorld().SetToDisplayMiniMapOnRegionLoad(true);
    CGameEvent game_event(0x2c);
    game_event.AddFloat(best_data.EntrancePosX);
    game_event.AddFloat(best_data.EntrancePosY);
    game_event.AddFloat(best_data.EntrancePosZ);
    game_event.AddFloat(best_data.EntranceAngle);
    game_event.AddUBYTE(false);
    game_event.AddUBYTE(false);
    game_event.AddUBYTE(false);
    game_event.SetPlayer(GetParentThing().GetPlayerNumber());
    C3DVector entrance_coord(best_data.EntrancePosX, best_data.EntrancePosY, best_data.EntrancePosZ);
    long region_number = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(entrance_coord));
    if (region_number > 0) {
        CPlayerGui* gui = &CPlayerGui::Get();
        gui->SetMiniMapRegion(PeekWorldMap().GetRegion(region_number));
    }
    GetWorld().HandleMoveHeroToRegionGameEvent(game_event);
    }
}

#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CGame.h"
#include "engine/CThingManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CNavigatorManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDrawMapInfo.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCamera.h"
#undef private
#undef protected

namespace std {
template<> _Vector_base<CMapInfo,allocator<CMapInfo> >::~_Vector_base(){assert(!_M_start);}template<> vector<CMapInfo>::~vector(){assert(!_M_start&&!_M_finish);}
template<> _Vector_base<CRegion,allocator<CRegion> >::~_Vector_base(){assert(!_M_start);}template<> vector<CRegion>::~vector(){assert(!_M_start&&!_M_finish);}
template<> _Vector_base<CWorldMap::CPlayerPositionInfo,allocator<CWorldMap::CPlayerPositionInfo> >::~_Vector_base(){assert(!_M_start);}template<> vector<CWorldMap::CPlayerPositionInfo>::~vector(){assert(!_M_start&&!_M_finish);}
template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);}template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);}
}
namespace boost {template<> scoped_ptr<NThingMap::CThingMapManager>::~scoped_ptr(){assert(!ptr);}}
static CWorldMap* FixtureMapForManager;
CThingSearchTools::CThingSearchTools(CThingManager&m,CWorldMap&w):WorldMap(w),PThingMapManager(0),ThingManager(m){}CThingSearchTools::~CThingSearchTools(){assert(AreaMaps.empty());}
CClassFactory::CClassFactory(unsigned char){}CClassFactory::~CClassFactory(){}
CDefinitionManager::~CDefinitionManager(){}CResourceList::~CResourceList(){}
CFailedAllocationHandler::~CFailedAllocationHandler(){}CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}
CThingManager::CThingManager(const CMainGameComponent&c,const CGameDefinitionManager&d,CWorld&w,CPlayerManager&p,CNavigatorManager&n,unsigned long*seed,unsigned long,unsigned long,unsigned long):CClassFactory(0),Component(c),DefinitionManager(d),World(w),WorldMap(*FixtureMapForManager),WorldSeed(seed),PlayerManager(p),NavigationManager(n){}CThingManager::~CThingManager(){}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}

CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CTCPhysicsBase::CTCPhysicsBase(CThing&t):CTCBase(t){}CTCPhysicsBase::~CTCPhysicsBase(){}
float CTCPhysicsBase::GetFacingAngleXY()const{abort();}
long CWorldMap::GetMapNumberAt(const C2DCoordI&)const{abort();}
long CWorldMap::DrawGetNoMaps()const{abort();}
CTCBase::CTCBase(CThing&t):ParentThing(&t),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}CTCBase::~CTCBase(){}
CTCHeroStats::CTCHeroStats(CThing&t):CTCBase(t){}void CTCHeroStats::OnSerialise(CPersistContext&,bool){abort();}
CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus(){for(int n=0;n<10;++n)Crimes[n]=0;LastTimeSeenByGuard=LastTimeExited=0;ExitedUsingTeleporter=false;EntrancePosX=EntrancePosY=EntrancePosZ=EntranceAngle=0;}
CWorld::~CWorld(){}CIDrawWorld::~CIDrawWorld(){}CIDrawWorldMap::~CIDrawWorldMap(){}
CWorldMap::CWorldMap(const CMainGameComponent&c,CWorld&w,const C2DBoxI&a,unsigned long):World(w),Component(c),Granularity(1),GranularityShift(0),Area(a){}CWorldMap::~CWorldMap(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
void CIVCountedPointeeBase::Delete()const{abort();}
template<> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::~CTBaseSingletonNoDefaultAlloc(){}
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass=CCountedPointer<CPlayerGui>();
namespace std {
// Controlled external rebalancer accepts only the fixture's root-first, at most three-node balanced insertion shapes.
template<class Dummy>void __cdecl _Rb_global<Dummy>::_Rebalance(_Rb_tree_node_base*n,_Rb_tree_node_base*&root){assert(n&&root&&(!n->_M_parent||n==root||n->_M_parent==root));n->_M_color=true;root->_M_color=false;}
}
void CBaseObject::Validate() const{abort();}
void CClass::Save(CCharString&){abort();}
void CThingGameObject::OnHit(CHitParameters&){abort();}
void CTCBase::OnSerialise(CPersistContext&, bool){abort();}
void CTCBase::OnRemove(){abort();}
void CTCBase::ConstructFromParams(const CTCParamsBase&){abort();}
void CTCBase::OnCreate(){abort();}
void CTCBase::InitialActivate(){abort();}
void CTCBase::OnInitialActivate(){abort();}
void CTCBase::RenderUpdate(float){abort();}
bool CTCBase::HasFrameUpdate() const{abort();}
void CTCBase::FrameUpdate(){abort();}
void CTCBase::BulletTimeFrameUpdate(){abort();}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&){abort();}
void CTCBase::Copy(const CTCBase&){abort();}
void CTCBase::OnDie(){abort();}
void CTCBase::OnKill(){abort();}
void CTCBase::OnLevelUnload(){abort();}
void CTCBase::OnSetInLimbo(bool){abort();}
bool CTCBase::Validate(CCharString&) const{abort();}
bool CTCBase::IsFreeForDeletion() const{abort();}
bool CTCBase::IsSerialiseCreatable() const{abort();}
bool CTCBase::IsSerialisableForHeroState() const{abort();}
bool CTCBase::IsComponentSerialisable() const{abort();}
bool CTCBase::IsDriver() const{abort();}
bool CTCBase::IsCopyable() const{abort();}
void CTCBase::GlobalUpdate(bool){abort();}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&){abort();}
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const{abort();}
C2DVector CTCPhysicsBase::GetOffsetVector() const{abort();}
C3DVector CTCPhysicsBase::GetOffsetVector3D() const{abort();}
void CTCPhysicsBase::SetPosition(const C3DVector &){abort();}
void CTCPhysicsBase::ResetPosition(const C3DVector &){abort();}
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &){abort();}
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &){abort();}
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool){abort();}
void CTCPhysicsBase::ResetMovementState(){abort();}
void CTCPhysicsBase::EnablePhysics(bool){abort();}
bool CTCPhysicsBase::IsPhysicsEnabled() const{abort();}
void CTCPhysicsBase::EnableGravity(bool){abort();}
bool CTCPhysicsBase::IsGravityEnabled() const{abort();}
void CTCPhysicsBase::EnableNavigator(bool){abort();}
bool CTCPhysicsBase::IsNavigationEnabled() const{abort();}
bool CTCPhysicsBase::HasNavigationLayers() const{abort();}
void CTCPhysicsBase::SetInteractionFlags(long){abort();}
void CTCPhysicsBase::ReturnToDefaultInteractionFlags(){abort();}
void CTCPhysicsBase::OnDroppedFromUpdateList(){abort();}
void CTCPhysicsBase::OnAddedToUpdateList(){abort();}
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const{abort();}
bool CTCPhysicsBase::IsAffectedByGravity() const{abort();}
bool CTCPhysicsBase::IsAffectedByAir() const{abort();}
bool CTCPhysicsBase::IsAffectedByWind() const{abort();}
bool CTCPhysicsBase::IsAngledWithGround() const{abort();}
bool CTCPhysicsBase::IsImmoveable() const{abort();}
bool CTCPhysicsBase::IsOnAnySurface() const{abort();}
bool CTCPhysicsBase::IsOnAnyStationarySurface() const{abort();}
bool CTCPhysicsBase::IsOnMeshSurface() const{abort();}
bool CTCPhysicsBase::IsOnGroundSurface() const{abort();}
bool CTCPhysicsBase::CanCollideWithThings() const{abort();}
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const{abort();}
bool CTCPhysicsBase::CanCollideWithLandscape() const{abort();}
bool CTCPhysicsBase::CanInteractWithThings() const{abort();}
bool CTCPhysicsBase::IsExcludingCreatures() const{abort();}
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const{abort();}
float CTCPhysicsBase::GetFacingAngleYZ() const{abort();}
float CTCPhysicsBase::GetFacingAngleXZ() const{abort();}
void CTCPhysicsBase::SetFacingAngleXY(float){abort();}
void CTCPhysicsBase::SetFacingAngleYZ(float){abort();}
void CTCPhysicsBase::SetFacingAngleXZ(float){abort();}
C3DVector CTCPhysicsBase::GetFacingVector() const{abort();}
C3DVector CTCPhysicsBase::GetFacingVector(float) const{abort();}
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const{abort();}
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const{abort();}
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const{abort();}
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &){abort();}
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &){abort();}
C3DVector CTCPhysicsBase::GetImpulse() const{abort();}
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &){abort();}
void CTCPhysicsBase::SetRHSetAsFlushOnGround(){abort();}
float CTCPhysicsBase::GetRenderZOffset(float) const{abort();}
void CTCPhysicsBase::ResetInterpolation(){abort();}
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &){abort();}
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float){abort();}
bool CTCPhysicsBase::IsPushableByHero() const{abort();}
void CTCPhysicsBase::SetAsPushableByHero(bool){abort();}
void CTCPhysicsBase::FrameUpdate(){abort();}
bool CTCPhysicsBase::HasFrameUpdate() const{abort();}
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long){abort();}
void CTCPhysicsBase::SetScaleFromOwner(){abort();}
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &){abort();}
void CTCPhysicsBase::OnCreate(){abort();}
void CTCPhysicsBase::InitialActivate(){abort();}
void CTCPhysicsBase::Move(){abort();}
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const{abort();}
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool){abort();}
bool CTCPhysicsBase::IsComponentSerialisable() const{abort();}
bool CTCPhysicsBase::IsSerialisableForHeroState() const{abort();}
bool CTCPhysicsBase::IsCopyable() const{abort();}
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const{abort();}
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const{abort();}
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const{abort();}
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const{abort();}
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const{abort();}
void CTCPhysicsBase::UpdateInterpolation(){abort();}
void CTCPhysicsBase::UpdateOldRHSet(){abort();}
void CTCPhysicsBase::UpdateOldPos(){abort();}
void CWorld::PostLoadInit(){abort();}
void CWorld::LoadWorldFromFile(CAFile& file, long version){abort();}
CThingManager& CWorld::GetThingManager(){abort();}
const CThingManager& CWorld::PeekThingManager() const{abort();}
CCamera CWorld::GetCurrentRenderCamera() const{abort();}
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const{abort();}
void CWorld::PostInit(CDisplayEngine& displayEngine){abort();}
C3DVector CWorld::GetGameLODCentre() const{abort();}
void CWorld::Init(){abort();}
void CWorld::Uninitialise(){abort();}
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info){abort();}
CWorldMap& CWorld::GetWorldMap(){abort();}
const CWorldMap& CWorld::PeekWorldMap() const{abort();}
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const{abort();}
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const{abort();}
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const{abort();}
void CWorldMap::DeleteAllLevels(){abort();}
void CWorldMap::DeleteAllRegions(){abort();}
void CWorldMap::LoadFromFile(CAFile&, long){abort();}
void CWorldMap::SaveToFile(CAFile&){abort();}
void CWorldMap::RemoveLevel(long){abort();}
void CWorldMap::LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&){abort();}
void CWorldMap::PostLoadTopology(long){abort();}
void CWorldMap::ForceLoad(long){abort();}
void CWorldMap::ForceUnload(long){abort();}
bool CWorldMap::IsMapLoaded(long) const{abort();}
bool CWorldMap::IsMapUsed(long) const{abort();}
long CWorldMap::GetGranularity() const{abort();}
const C2DBoxI& CWorldMap::GetDimensions() const{abort();}
CMap& CWorldMap::GetMapAt(const C2DCoordI&){abort();}
const CMap& CWorldMap::PeekMapAt(const C2DCoordI&) const{abort();}
CMap& CWorldMap::GetMap(long){abort();}
const CMap& CWorldMap::PeekMap(long) const{abort();}
void CWorldMap::GetAllMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const{abort();}
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const{abort();}
void CWorldMap::PostRegionLoad(long){abort();}
C2DCoordI CWorldMap::GetMapPos(long) const{abort();}
bool CWorldMap::IsMapChangeable(long) const{abort();}
bool CWorldMap::IsPosChangeable(const C2DCoordI&) const{abort();}
std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> > CWorldMap::GetLoadableSections() const{abort();}
void CWorldMap::Update(){abort();}
void CWorldMap::HeightAreaChanged(const C2DBoxI*){abort();}
long CWorldMap::DrawGetFrame() const{abort();}
CDrawMapInfo CWorldMap::DrawGetMapInfo(long) const{abort();}
C2DBoxI CWorldMap::DrawGetDimensions() const{abort();}
long CWorldMap::DrawGetGranularity() const{abort();}
bool CWorldMap::DrawIsPosInMap(const C3DVector&) const{abort();}
float CWorldMap::DrawGetSizeZAt(const C3DVector&, float) const{abort();}
C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const{abort();}
long CWorldMap::DrawGetMyPlayerMapNumber() const{abort();}
bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const{abort();}
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const{abort();}
void CWorldMap::LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>){abort();}
long CWorldMap::AllocateNewMap(){abort();}
void CGameComponent::Init(){abort();}
void CGameComponent::SetQuit(){abort();}
bool CMainGameComponent::Run(CGameComponent**){abort();}
void CMainGameComponent::ChangeTextureColourDepth(long){abort();}
void CMainGameComponent::OnPreDeviceReset(){abort();}
bool CMainGameComponent::OnPostDeviceReset(){abort();}
void CClassFactory::AddNewClassType(const CClassTypeInit&){abort();}
void CClassFactory::OnCreateClass(CClass&){abort();}
void CClassFactory::OnDeleteClass(CClass&){abort();}
void CThingManager::OnCreateClass(CClass&){abort();}
void CThingManager::OnDeleteClass(CClass&){abort();}
void CGameDefinitionManager::InitAndCompile(){abort();}
bool CResourceList::HandleFailedAllocation(unsigned long){abort();}
bool CResource::UnloadResource(){abort();}
unsigned long CResource::GetUnloadableMemory() const{abort();}
bool CResource::IsResourceLoaded() const{abort();}
void CResource::OnResourceLoaded(){abort();}
bool CResource::CanAbortResourceLoad(long) const{abort();}
void CResource::OnResourceLoadAborted(){abort();}
unsigned long CResource::GetDebugMemoryInUse() const{abort();}
void CResource::ResetDebugMemoryInUse(){abort();}
static int StringObjects,DataBlocks;
static void ReleaseData(CCharStringData*p){if(p&&--p->NoRefs==0){delete[]p->Data.PData;delete p;--DataBlocks;}}
CCharString::CCharString():PStringData(0){++StringObjects;}
CCharString::CCharString(const char*text,long length):PStringData(0){++StringObjects;if(length<0)length=(long)strlen(text);if(length){PStringData=new CCharStringData;++DataBlocks;PStringData->NoRefs=1;PStringData->Data.PData=new char[length+1];memcpy(PStringData->Data.PData,text,length);PStringData->Data.PData[length]=0;PStringData->Data.StringLength=length;PStringData->Data.DataLength=length+1;PStringData->Data.UseFastExtend=false;}}
CCharString::CCharString(const CCharString&o):PStringData(o.PStringData){++StringObjects;if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){ReleaseData(PStringData);--StringObjects;}
const CCharString& CCharString::operator=(const CCharString&o){if(o.PStringData)++o.PStringData->NoRefs;ReleaseData(PStringData);PStringData=o.PStringData;return *this;}
static const char* Text(const CCharString&s){return s.PStringData?s.PStringData->Data.PData:"";}
bool CCharString::operator!=(const CCharString&o)const{return strcmp(Text(*this),Text(o))!=0;}
bool CCharString::operator<(const CCharString&o)const{return strcmp(Text(*this),Text(o))<0;}
static CWorld* ExpectedWorld;static CWorldMap* ExpectedMap;static CThingSearchTools* ExpectedTools;static CTCHeroStats* ExpectedStats;static CPlayerGui* ExpectedGui;
static CThingMarker* Markers[3];static CRegion* FixtureRegions[3];static C3DVector MarkerPosition;static float MarkerAngle;static bool Active;
static bool Scan[3],DefMatch[3],ScriptMatch[3];static std::list<CClass*> TypeList;
static long Fines[3],Times[3],TargetRegion;static int ExpectedChoice,ExpectedFallback;
static int ScanCalls[3],DefCalls[3],ScriptCalls[3],FineCalls[3],PosCalls,FacingCalls,MapCalls,RegionCalls,GuiCalls,MoveCalls,LoadedCalls,WorldCalls,TypeCalls,MapRegionCalls;
static C3DVector ExpectedPosition;static float ExpectedAngle;static signed char ExpectedPlayer;
static int MarkerIndex(const CThing&x){for(int n=0;n<3;++n)if(&x==Markers[n])return n;abort();return -1;}
static int RegionIndex(const CCharString&s){for(int n=0;n<3;++n)if(strcmp(Text(s),n==0?"REGION_A":n==1?"REGION_B":"REGION_C")==0)return n;abort();return -1;}
const CWorldMap& CTCBase::PeekWorldMap()const{assert(Active&&this==ExpectedStats);return *ExpectedMap;}
CWorld& CTCBase::GetWorld()const{assert(Active&&this==ExpectedStats);++WorldCalls;return *ExpectedWorld;}
const CThingSearchTools& CTCBase::PeekThingSearchTools()const{assert(Active&&this==ExpectedStats);return *ExpectedTools;}
const std::list<CClass*>& CThingSearchTools::PeekTypeList(long type)const{assert(Active&&this==ExpectedTools&&type==11);++TypeCalls;return TypeList;}
bool CThing::IsIncludedInThingScans()const{assert(Active);int n=MarkerIndex(*this);++ScanCalls[n];return Scan[n];}
bool CThingFilter_HasDefName::operator()(const CThing&x)const{assert(Active&&strcmp(Text(Name),"REGION_KICK_TO_POINT")==0);int n=MarkerIndex(x);assert(ScanCalls[n]==1&&Scan[n]);++DefCalls[n];return DefMatch[n];}
bool CThingFilter_HasScriptData::operator()(const CThing&x)const{assert(Active&&Data.PStringData==FixtureRegions[1]->Name.PStringData);int n=MarkerIndex(x);assert(ScanCalls[n]==1&&DefCalls[n]==1&&Scan[n]&&DefMatch[n]);++ScriptCalls[n];return ScriptMatch[n];}
const C3DVector& CThing::GetPos()const{assert(Active&&ExpectedChoice>=0&&this==Markers[ExpectedChoice]);++PosCalls;return MarkerPosition;}
class FixturePhysics:public CTCPhysicsBase{
public:FixturePhysics(CThing&t):CTCPhysicsBase(t){}
CCharString GetClassTypeName()const{abort();return CCharString();}
float GetFacingAngleXY()const{assert(Active&&ExpectedChoice>=0&&this==Markers[ExpectedChoice]->PhysicsTC);++FacingCalls;return MarkerAngle;}
};
class FixtureStats:public CTCHeroStats{
public:FixtureStats(CThing&t):CTCHeroStats(t){}
ETCInterfaceType GetInterfaceType()const{return TCI_HERO_STATS;}
CCharString GetClassTypeName()const{abort();return CCharString();}
};
long CWorldMap::GetLoadedRegion()const{assert(Active&&this==ExpectedMap);++LoadedCalls;return 1;}
const CRegion& CWorldMap::GetRegion(long n)const{assert(Active&&this==ExpectedMap&&n>=0&&n<3);++RegionCalls;return *FixtureRegions[n];}
long CWorldMap::GetMapNumberAt(const C3DVector&p)const{assert(Active&&this==ExpectedMap&&ExpectedChoice<0&&ExpectedFallback>=0&&p.X==ExpectedPosition.X&&p.Y==ExpectedPosition.Y&&p.Z==ExpectedPosition.Z);++MapCalls;return 123;}
long CWorldMap::GetRegionNumberMapIsIn(long n)const{assert(Active&&this==ExpectedMap);assert(n==(ExpectedChoice>=0?Markers[ExpectedChoice]->CurrentMapNumber:123));++MapRegionCalls;return TargetRegion;}
void CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString&r,long&fine,long&left){assert(Active&&this==ExpectedStats&&ExpectedChoice<0);int n=RegionIndex(r);++FineCalls[n];fine=Fines[n];left=500+n;}
void CPlayerGui::SetMiniMapRegion(const CRegion&r){assert(Active&&this==ExpectedGui&&TargetRegion>0&&&r==FixtureRegions[TargetRegion]&&ExpectedWorld->DisplayMiniMapOnRegionLoad);++GuiCalls;}
void CGameEvent::AddData(long size,const void*p){assert(Active&&Type==0x2c&&Player==-1&&Valid&&!Replacement&&EndPos+size<=32);assert(size==(EndPos<16?4:1));if(size==4){float value;memcpy(&value,p,4);float expected=EndPos==0?ExpectedPosition.X:EndPos==4?ExpectedPosition.Y:EndPos==8?ExpectedPosition.Z:ExpectedAngle;assert(value==expected);}else assert(*(const unsigned char*)p==0);memcpy(Data+EndPos,p,size);EndPos=(unsigned char)(EndPos+size);}
void CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent&e){assert(Active&&this==ExpectedWorld&&DisplayMiniMapOnRegionLoad&&e.Type==0x2c&&e.Player==ExpectedPlayer&&e.Valid&&!e.Replacement&&e.EndPos==19&&GuiCalls==(TargetRegion>0?1:0));++MoveCalls;float values[4];memcpy(values,e.Data,16);assert(values[0]==ExpectedPosition.X&&values[1]==ExpectedPosition.Y&&values[2]==ExpectedPosition.Z&&values[3]==ExpectedAngle);assert(e.Data[16]==0&&e.Data[17]==0&&e.Data[18]==0);}
static void ConfigureStatus(CTCHeroStats::MirroredCriminalStatus&s,int n){for(int k=0;k<10;++k)s.Crimes[k]=n*100+k;s.LastTimeSeenByGuard=100+n;s.LastTimeExited=Times[n];s.ExitedUsingTeleporter=(n&1)!=0;s.EntrancePosX=n+.25f;s.EntrancePosY=n+.5f;s.EntrancePosZ=n+.75f;s.EntranceAngle=n+1.25f;}
int main(){
 unsigned cases=0;
 {
 CGame game;CMainGameComponentInit component_init;CMainGameComponent component(game,component_init);CWorld world;C2DBoxI area;area.Initialise(0,0,100,100);CWorldMap map(component,world,area,1);FixtureMapForManager=&map;CGameDefinitionManager definitions;CPlayerManager players;CNavigatorManager navigator;unsigned long seed_value=0;CThingManager manager(component,definitions,world,players,navigator,&seed_value,0,0,0);CThingSearchTools tools(manager,map);CPlayerGui gui;CRegion r0,r1,r2;CThingMarker hero,m0,m1,m2;FixtureStats stats(hero);FixturePhysics p0(m0),p1(m1),p2(m2);
 Markers[0]=&m0;Markers[1]=&m1;Markers[2]=&m2;m0.PhysicsTC=&p0;m1.PhysicsTC=&p1;m2.PhysicsTC=&p2;m0.CurrentMapNumber=7;m1.CurrentMapNumber=8;m2.CurrentMapNumber=9;FixtureRegions[0]=&r0;FixtureRegions[1]=&r1;FixtureRegions[2]=&r2;
 r0.Name=CCharString("REGION_A");r1.Name=CCharString("REGION_B");r2.Name=CCharString("REGION_C");ExpectedWorld=&world;ExpectedMap=&map;ExpectedTools=&tools;ExpectedStats=&stats;ExpectedGui=&gui;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data=&gui;CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Info=0;
 const long time_values[][3]={{0,0,0},{10,20,30},{30,20,10},{50,100,50},{-1,3,-2},{1,2,1}};const long fine_values[][3]={{0,0,0},{1,0,0},{0,0,1},{1,1,1},{-1,0,0}};
 for(int mapmask=0;mapmask<8;++mapmask)for(int time=0;time<6;++time)for(int fine=0;fine<5;++fine)for(int scenario=0;scenario<8;++scenario)for(int alive=0;alive<2;++alive)for(int region=0;region<3;++region){
  Active=false;TypeList.clear();stats.OutstandingCrimes.clear();for(int n=0;n<3;++n){Times[n]=time_values[time][n];Fines[n]=fine_values[fine][n];Scan[n]=scenario!=1||n!=0;DefMatch[n]=scenario==2?n==1:scenario==3?n==2:scenario>=4;ScriptMatch[n]=scenario==5?n==1:scenario==6?n==2:scenario>=4;ScanCalls[n]=DefCalls[n]=ScriptCalls[n]=FineCalls[n]=0;}
  int insertion[]={1,0,2};for(int k=0;k<3;++k){int n=insertion[k];if(mapmask&(1<<n)){CTCHeroStats::MirroredCriminalStatus st;ConfigureStatus(st,n);stats.OutstandingCrimes[n==0?r0.Name:n==1?r1.Name:r2.Name]=st;}}
  // Search uses zero, one, two and three genuine marker objects and original shared list nodes.
  int list_count=scenario==0?0:scenario==1?1:3;for(int n=0;n<list_count;++n)TypeList.push_back(static_cast<CClass*>(Markers[n]));
  ExpectedChoice=-1;for(int n=0;n<list_count;++n)if(Scan[n]&&DefMatch[n]&&ScriptMatch[n]){ExpectedChoice=n;break;}
  ExpectedFallback=-1;long best=0;if(ExpectedChoice<0)for(int n=0;n<3;++n)if((mapmask&(1<<n))&&Fines[n]==0&&n!=1&&Times[n]>best){ExpectedFallback=n;best=Times[n];}
  MarkerPosition=C3DVector(time+.25f,fine+.5f,scenario+.75f);MarkerAngle=region+.125f;
  if(ExpectedChoice>=0){ExpectedPosition=MarkerPosition;ExpectedAngle=MarkerAngle;}else{int n=ExpectedFallback>=0?ExpectedFallback:0;ExpectedPosition=C3DVector(n+.25f,n+.5f,n+.75f);ExpectedAngle=n+1.25f;}
  TargetRegion=region-1;if(region==2)TargetRegion=2;hero.Dead=!alive;hero.PlayerNumber=(signed char)(time%4);ExpectedPlayer=hero.PlayerNumber;world.DisplayMiniMapOnRegionLoad=false;
  PosCalls=FacingCalls=MapCalls=RegionCalls=GuiCalls=MoveCalls=LoadedCalls=WorldCalls=TypeCalls=MapRegionCalls=0;
  unsigned char before[sizeof(stats)];memcpy(before,&stats,sizeof(stats));int old_strings=StringObjects,old_blocks=DataBlocks;
  Active=true;stats.KickHeroToMostRecentSafeRegion();Active=false;
  bool move=alive&&(ExpectedChoice>=0||ExpectedFallback>=0);assert(MoveCalls==(move?1:0)&&GuiCalls==(move&&TargetRegion>0?1:0)&&world.DisplayMiniMapOnRegionLoad==move);assert(LoadedCalls==1&&TypeCalls==1&&RegionCalls==1+GuiCalls&&MapRegionCalls==(move?1:0)&&WorldCalls==(move?2:0));assert(PosCalls==(move&&ExpectedChoice>=0?3:0)&&FacingCalls==(move&&ExpectedChoice>=0?1:0)&&MapCalls==(move&&ExpectedChoice<0?1:0));
  for(int n=0;n<3;++n){bool visited=n<list_count&&(ExpectedChoice<0||n<=ExpectedChoice);assert(ScanCalls[n]==(visited?1:0));assert(DefCalls[n]==(visited&&Scan[n]?1:0));assert(ScriptCalls[n]==(visited&&Scan[n]&&DefMatch[n]?1:0));assert(FineCalls[n]==(ExpectedChoice<0&&(mapmask&(1<<n))?1:0));}
  assert(memcmp(before,&stats,sizeof(stats))==0&&StringObjects==old_strings&&DataBlocks==old_blocks);++cases;
 }
 TypeList.clear();stats.OutstandingCrimes.clear();CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data=0;assert(cases==11520);
 }
 assert(StringObjects==0&&DataBlocks==0);puts("0057df85_TEST PASS");return 0;
}