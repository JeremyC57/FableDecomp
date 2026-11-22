// CTCSummonSpell::IsValidTeleportPos — check map availability and collisions at the destination.
// Leaf: ?IsValidTeleportPos@CTCSummonSpell@@AAE_NABVC3DVector@@ABVCWorldMap@@PBVCThing@@@Z
// Flags: default
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
#define FABLE_RETAIL_CWORLDMAP_CTOR
#define FABLE_RETAIL_CWORLDMAP_PLAYERPOSITION_PASS27
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CTCSUMMONSPELL_TEST_CTOR_PASS27
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define protected public
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsCollideResponse.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CWorldMap.h"
#define private public
#include "engine/CTCSummonSpell.h"
#undef private
#undef protected

template<> void CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<CThing*>, CThingFilter_None>(
    const C2DBoxI&, const CThingFilter_None&, std::list<CThing*>&) const;

bool CTCSummonSpell::IsValidTeleportPos(const C3DVector& pos, const CWorldMap& worldmap, const CThing* minion)
{
    const CTCPhysicsBase* ptc_creature_physics;
    if (minion->PeekTC(&ptc_creature_physics)) {
        long map_no = worldmap.GetMapNumberAt(pos);
        if (worldmap.IsMapLoaded(map_no) && worldmap.IsPosPassable(pos.ToC2DCoordI())) {
            bool collided = false;
            float radius = ptc_creature_physics->GetRadius() * 2;
            if (radius < 1.0f)
                radius = 1.0f;
            C2DBoxI search_area(pos.ToC2DCoordI(), (long)radius);
            std::list<CThing*> things;
            PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(search_area, CThingFilter_None(), things);
            std::list<CThing*>::iterator it, endit;
            for (it = things.begin(), endit = things.end(); it != endit; ++it) {
                CThing* something = *it;
                if (something == minion)
                    continue;

                const CTCPhysicsBase* ptc_physics_thing;
                if (something->PeekTC(&ptc_physics_thing)) {
                    CPhysicsCollideResponse response;
                    if (ptc_creature_physics->HasCollidedWithThingFromTo(*ptc_physics_thing, minion->GetPos(),
                                                                       pos + C3DVector(0, 0, 0.1f), response)) {
                        collided = true;
                        break;
                    }
                }
            }
            return !collided;
        }
    }
    return false;
}


// The fixture supplies the engine boundaries used by this method.
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#include "engine/CDrawMapInfo.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include <stdlib.h>
#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
void CTCBase::OnSerialise(CPersistContext&, bool) {}
void CTCBase::OnRemove() {}
void CTCBase::ConstructFromParams(const CTCParamsBase&) {}
void CTCBase::OnCreate() {}
void CTCBase::InitialActivate() {}
void CTCBase::OnInitialActivate() {}
void CTCBase::RenderUpdate(float) {}
bool CTCBase::HasFrameUpdate() const { return false; }
void CTCBase::FrameUpdate() {}
void CTCBase::BulletTimeFrameUpdate() {}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) {}
void CTCBase::Copy(const CTCBase&) {}
void CTCBase::OnDie() {}
void CTCBase::OnKill() {}
void CTCBase::OnLevelUnload() {}
void CTCBase::OnSetInLimbo(bool) {}
bool CTCBase::Validate(CCharString&) const { return true; }
bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return false; }
bool CTCBase::IsSerialisableForHeroState() const { return false; }
bool CTCBase::IsComponentSerialisable() const { return false; }
bool CTCBase::IsDriver() const { return false; }
bool CTCBase::IsCopyable() const { return false; }
void CTCBase::GlobalUpdate(bool) {}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) {}

template<> boost::scoped_ptr<NThingMap::CThingMapManager>::~scoped_ptr() {}
CIDrawWorldMap::~CIDrawWorldMap() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CWorldMap::CWorldMap(const CMainGameComponent& component, CWorld& world, const C2DBoxI&, unsigned long)
    : World(world), Component(component), Granularity(1), GranularityShift(0) {}
CWorldMap::~CWorldMap() {}
CTCPhysicsBase::CTCPhysicsBase(CThing& thing) : CTCBase(thing), Radius(1.0f) {}
CTCPhysicsBase::~CTCPhysicsBase() {}
CTCSummonSpell::CTCSummonSpell(CThing& thing) : CTCBase(thing) {}
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { return TCI_PHYSICS; }
class TestPhysics : public CTCPhysicsBase {
public:
    TestPhysics(CThing& thing) : CTCPhysicsBase(thing) {}
    CCharString GetClassTypeName() const { return CCharString(); }
};

class TestSummonSpell : public CTCSummonSpell {
public:
    TestSummonSpell(CThing& owner) : CTCSummonSpell(owner) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_SUMMON_SPELL; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

static bool loaded, passable, collide;
static int get_map_calls, loaded_calls, passable_calls, searches, collisions;
static CThing* candidates[4];
static int candidate_count;
static C2DBoxI searched_area;
static CTCPhysicsBase* expected_physics;
static bool seen_args;
static C3DVector minion_pos;
static CThingSearchTools* search_tools;

const CThingSearchTools& CTCBase::PeekThingSearchTools() const { return *search_tools; }
CThingSearchTools::CThingSearchTools(CThingManager& things, CWorldMap& world)
    : WorldMap(world), PThingMapManager(0), ThingManager(things) {}
CThingSearchTools::~CThingSearchTools() {}
template<> void CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<CThing*>, CThingFilter_None>(
    const C2DBoxI& area, const CThingFilter_None&, std::list<CThing*>& things) const
{
    searched_area = area;
    ++searches;
    for (int i = 0; i < candidate_count; ++i)
        things.push_back(candidates[i]);
}

long CWorldMap::GetMapNumberAt(const C3DVector& pos) const
{
    ++get_map_calls;
    return pos.X == 10.75f && pos.Y == -2.5f ? 8 : -1;
}
bool CWorldMap::IsMapLoaded(long n) const
{
    ++loaded_calls;
    return loaded && n == 8;
}
bool CWorldMap::IsPosPassable(const C2DCoordI& pos) const
{
    ++passable_calls;
    return passable && pos.X == 10 && pos.Y == -2;
}
const C3DVector& CThing::GetPos() const { return minion_pos; }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& start,
    const C3DVector& end, CPhysicsCollideResponse& response) const
{
    ++collisions;
    seen_args = &other == expected_physics && &start == &minion_pos
        && end.X == 10.75f && end.Y == -2.5f && end.Z > 5.09f && end.Z < 5.11f
        && response.Friction == 1.0f;
    return collide;
}
static void AddPhysics(CThing& thing, CTCPhysicsBase& physics)
{
    const ETCInterfaceType type = TCI_PHYSICS;
    thing.UsingComponents[type] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(type, &physics));
}

int main()
{
    // These managers are opaque handles held by the boundary fixtures.
    static unsigned __int64 world_storage[16], game_storage[16], manager_storage[16];
    CWorld& world = *reinterpret_cast<CWorld*>(world_storage);
    const CMainGameComponent& game = *reinterpret_cast<const CMainGameComponent*>(game_storage);
    CThingManager& manager = *reinterpret_cast<CThingManager*>(manager_storage);
    CWorldMap worldmap(game, world, C2DBoxI(), 1);
    CThingSearchTools tools(manager, worldmap);
    search_tools = &tools;
    CThing minion, other, no_collision_component;
    memset(minion.UsingComponents._M_w, 0, sizeof(minion.UsingComponents._M_w));
    memset(other.UsingComponents._M_w, 0, sizeof(other.UsingComponents._M_w));
    memset(no_collision_component.UsingComponents._M_w, 0, sizeof(no_collision_component.UsingComponents._M_w));
    TestPhysics physics(minion), other_physics(other);
    expected_physics = &other_physics;
    TestSummonSpell test(minion);
    const C3DVector target(10.75f, -2.5f, 5.0f);
    minion_pos = C3DVector(1.0f, 2.0f, 3.0f);

    if (test.IsValidTeleportPos(target, worldmap, &minion) || get_map_calls != 0)
        return 1;
    AddPhysics(minion, physics);
    if (test.IsValidTeleportPos(target, worldmap, &minion) || get_map_calls != 1
        || loaded_calls != 1 || passable_calls || searches)
        return 2;
    loaded = true;
    if (test.IsValidTeleportPos(target, worldmap, &minion) || passable_calls != 1 || searches)
        return 3;
    passable = true;
    physics.Radius = 0.25f;
    if (!test.IsValidTeleportPos(target, worldmap, &minion) || searches != 1
        || searched_area.TLX != 9 || searched_area.TLY != -3
        || searched_area.BRX != 11 || searched_area.BRY != -1)
        return 4;

    candidates[0] = &minion;
    candidates[1] = &no_collision_component;
    candidates[2] = &other;
    candidate_count = 3;
    AddPhysics(other, other_physics);
    physics.Radius = 2.0f;
    if (!test.IsValidTeleportPos(target, worldmap, &minion) || collisions != 1 || !seen_args
        || searched_area.TLX != 6 || searched_area.TLY != -6
        || searched_area.BRX != 14 || searched_area.BRY != 2)
        return 5;
    collide = true;
    candidates[3] = &other;
    candidate_count = 4;
    if (test.IsValidTeleportPos(target, worldmap, &minion) || collisions != 2 || !seen_args)
        return 6;
    puts("0079a890_TEST PASS");
    return 0;
}

// Unused vtable entries fail if the leaf calls them.
void CWorldMap::DeleteAllLevels() { abort(); }
void CWorldMap::DeleteAllRegions() { abort(); }
void CWorldMap::LoadFromFile(CAFile&, long) { abort(); }
void CWorldMap::SaveToFile(CAFile&) { abort(); }
void CWorldMap::RemoveLevel(long) { abort(); }
void CWorldMap::LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&) { abort(); }
void CWorldMap::PostLoadTopology(long) { abort(); }
void CWorldMap::ForceLoad(long) { abort(); }
void CWorldMap::ForceUnload(long) { abort(); }
bool CWorldMap::IsMapUsed(long) const { abort(); }
long CWorldMap::GetGranularity() const { abort(); }
const C2DBoxI& CWorldMap::GetDimensions() const { abort(); }
CMap& CWorldMap::GetMapAt(const C2DCoordI&) { abort(); }
const CMap& CWorldMap::PeekMapAt(const C2DCoordI&) const { abort(); }
long CWorldMap::GetMapNumberAt(const C2DCoordI&) const { abort(); }
CMap& CWorldMap::GetMap(long) { abort(); }
const CMap& CWorldMap::PeekMap(long) const { abort(); }
void CWorldMap::GetAllMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::PostRegionLoad(long) { abort(); }
C2DCoordI CWorldMap::GetMapPos(long) const { abort(); }
bool CWorldMap::IsMapChangeable(long) const { abort(); }
bool CWorldMap::IsPosChangeable(const C2DCoordI&) const { abort(); }
std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> > CWorldMap::GetLoadableSections() const { abort(); }
void CWorldMap::Update() { abort(); }
void CWorldMap::HeightAreaChanged(const C2DBoxI*) { abort(); }
long CWorldMap::DrawGetFrame() const { abort(); }
long CWorldMap::DrawGetNoMaps() const { abort(); }
CDrawMapInfo CWorldMap::DrawGetMapInfo(long) const { abort(); }
C2DBoxI CWorldMap::DrawGetDimensions() const { abort(); }
long CWorldMap::DrawGetGranularity() const { abort(); }
bool CWorldMap::DrawIsPosInMap(const C3DVector&) const { abort(); }
float CWorldMap::DrawGetSizeZAt(const C3DVector&, float) const { abort(); }
C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const { abort(); }
long CWorldMap::DrawGetMyPlayerMapNumber() const { abort(); }
bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
void CWorldMap::LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>) { abort(); }
long CWorldMap::AllocateNewMap() { abort(); }
C2DVector CTCPhysicsBase::GetOffsetVector() const { abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { abort(); }
void CTCPhysicsBase::ResetMovementState() { abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { abort(); }
void CTCPhysicsBase::EnableGravity(bool) { abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { abort(); }
bool CTCPhysicsBase::IsImmoveable() const { abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector() const { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const { abort(); }
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const { abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &) { abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { abort(); }
void CTCPhysicsBase::ResetInterpolation() { abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &) { abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float) { abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { abort(); }
void CTCPhysicsBase::FrameUpdate() { abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { abort(); }
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &) { abort(); }
void CTCPhysicsBase::OnCreate() { abort(); }
void CTCPhysicsBase::InitialActivate() { abort(); }
void CTCPhysicsBase::Move() { abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { abort(); }
bool CTCPhysicsBase::IsCopyable() const { abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { abort(); }
void CTCPhysicsBase::UpdateInterpolation() { abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { abort(); }
void CTCPhysicsBase::UpdateOldPos() { abort(); }