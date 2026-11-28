#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_AUTOPLACE_TEST_ACCESS
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CCOMBATMANAGER_CTOR
// 0096E700 CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition
// Leaf: ?IsEnemyCreatureInCombatPosition@CAIStateGroup_MeleeCombatNearby@@ABE_NXZ
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#define FABLE_RETAIL_MELEECOMBAT_PREFERRED_RADIUS_PASS30
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/CAIStateGroupBase.h"
// GetCreature is supplied by the fixture. The retail replay separately exercises the real brain reference.
#include "engine/CAIStateGroup_MeleeCombatNearby.h"

// Inferred: Widening each proximity result before the epsilon addition reproduces
// both x87 comparisons. The debug build does not identify the original cast spelling.
bool CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition() const
{
    if (GetCreature().IsInMeleeAsAttacker()) {
        const C3DVector pos = GetCombatManager().GetPreferredPos(GetCreature());
        const C3DVector to_preferred_pos = pos - GetCreature().GetPos();
        if (to_preferred_pos.GetSquaredMagnitude2D() <
            GFSqr(static_cast<double>(GetProximityToBeConsideredNearToPreferredPos()) + DELTA_NOTIONAL_ZERO))
            return true;
        const C3DVector& enemy_pos = PEnemy->GetPos();
        const C3DVector to_enemy_pos = enemy_pos - GetCreature().GetPos();
        if (!GetCombatManager().IsCreatureOnOuterRing(GetCreature()) &&
            to_enemy_pos.GetSquaredMagnitude2D() <
            GFSqr(static_cast<double>(GetProximityToBeConsideredNearToEnemyCreature()) + DELTA_NOTIONAL_ZERO))
            return true;
    }
    return false;
}

#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCamera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// The fixture constructs the real classes. Unused virtual slots fail on a call.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseObject::CBaseObject() {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) { abort(); }
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
CTCPhysicsBase::CTCPhysicsBase(CThing& thing) : CTCBase(thing) {}
CTCPhysicsBase::~CTCPhysicsBase() {}
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
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { abort(); }
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
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { abort(); }
void CTCPhysicsBase::UpdateInterpolation() { abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { abort(); }
void CTCPhysicsBase::UpdateOldPos() { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
CBaseIntelligentPointer::CBaseIntelligentPointer() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CIDrawWorld::~CIDrawWorld() {}
CWorld::~CWorld() {}
void CWorld::PostLoadInit() { abort(); }
void CWorld::LoadWorldFromFile(CAFile&,long) { abort(); }
CThingManager& CWorld::GetThingManager() { abort(); }
const CThingManager& CWorld::PeekThingManager() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo&) const { abort(); }
void CWorld::PostInit(CDisplayEngine&) { abort(); }
C3DVector CWorld::GetGameLODCentre() const { abort(); }
void CWorld::Init() { abort(); }
void CWorld::Uninitialise() { abort(); }
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&) { abort(); }
CWorldMap& CWorld::GetWorldMap() { abort(); }
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }

CCombatManager::CCombatManager(CWorld& world) : World(world) {}
CAIStateGroupBase::CAIStateGroupBase() {}
CAIStateGroup_MeleeCombatNearbyBase::CAIStateGroup_MeleeCombatNearbyBase() {}
CAIStateGroup_MeleeCombatNearbyBase::~CAIStateGroup_MeleeCombatNearbyBase() {}
CAIStateGroup_MeleeCombatNearby::CAIStateGroup_MeleeCombatNearby() {}
CAIStateGroup_MeleeCombatNearby::~CAIStateGroup_MeleeCombatNearby() {}
void CAIStateGroupBase::Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&) { abort(); }
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
void CAIStateGroupBase::InitStateGroup(const CWorld&,CThingAICreature&) { abort(); }
bool CAIStateGroup_MeleeCombatNearbyBase::IsTimingCritical() const { abort(); }
bool CAIStateGroup_MeleeCombatNearbyBase::IsRunnable() { abort(); }
bool CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(bool) const { abort(); }
void CAIStateGroup_MeleeCombatNearbyBase::OnReadyToAttack() { abort(); }
void CAIStateGroup_MeleeCombatNearby::Cleanup() { abort(); }
EAIStateGroupType CAIStateGroup_MeleeCombatNearby::GetStateGroupType() const { abort(); }
const float DELTA_NOTIONAL_ZERO = 0.0001f;

class TestWorld : public CWorld {
public:
    void SetManager(CCombatManager& manager) { PThingCombatManager_ptr = &manager; }
};
class FixturePhysics : public CTCPhysicsBase {
public:
    FixturePhysics(CThing& thing) : CTCPhysicsBase(thing) {}
    CCharString GetClassTypeName() const { abort(); }
};
class CAutoPlaceFixture {
public:
    static void Set(CThing& creature, CTCPhysicsBase& physics, float radius) {
        creature.PhysicsTC = &physics;
        physics.Radius = radius;
    }
};
class FixtureGroup : public CAIStateGroup_MeleeCombatNearby {
public:
    void Run() { Update(); }
};
static bool attacker, outer, latest;
static C3DVector preferred, current, enemyLoc;
static CThingAICreature* actor;
static CThingCreatureBase* enemy;
static TestWorld* testWorld;
static CCombatManager* combat;
static char trace[32];
static unsigned count;
static void event(char value) {
    if (count >= sizeof(trace)-1) abort();
    trace[count++] = value; trace[count] = 0;
}
void CAIStateGroup_MeleeCombatNearby::Update() { latest=IsEnemyCreatureInCombatPosition(); }
CThingAICreature& CAIStateGroupBase::GetCreature() const { return *actor; }
CWorld& CAIStateGroupBase::GetWorld() const { event('W'); return *testWorld; }
bool CThingCreatureBase::IsInMeleeAsAttacker() const {
    if (this != actor) abort();
    event('A'); return attacker;
}
C3DVector CCombatManager::GetPreferredPos(const CThingCreatureBase& creature) const {
    if (this != combat || &creature != actor) abort();
    event('P'); return preferred;
}
const C3DVector& CThing::GetPos() const {
    if (this == actor) { event('C'); return current; }
    if (this != enemy) abort();
    event('E'); return enemyLoc;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { event('H'); return enemy; }
bool CCombatManager::IsCreatureOnOuterRing(const CThingCreatureBase& creature) const {
    if (this != combat || &creature != actor) abort();
    event('O'); return outer;
}

int main() {
    TestWorld world;
    CCombatManager manager(world);
    CThingAICreature creature;
    CThingCreatureBase target;
    FixturePhysics physics(creature);
    FixtureGroup group;
    actor=&creature; enemy=&target; testWorld=&world; combat=&manager;
    world.SetManager(manager);
    const float radii[]={0.0f,0.75f,3.0f};
    const float separation[]={0.0f,0.25f,0.75f,1.5f,2.25f,4.0f,8.0f};
    unsigned scenarios=0;
    for (unsigned a=0;a<2;++a) for (unsigned o=0;o<2;++o)
    for (unsigned r=0;r<3;++r) for (unsigned u=0;u<7;++u)
    for (unsigned v=0;v<7;++v) for (unsigned axes=0;axes<3;++axes) {
        const double nearPreferred=2.0*radii[r]/1.2+0.3+DELTA_NOTIONAL_ZERO;
        const double nearEnemy=2.0*radii[r]/1.2+1.0+DELTA_NOTIONAL_ZERO;
        current=C3DVector(4,-8,16);
        preferred=C3DVector(current.X+(axes!=1 ? separation[u] : 0),current.Y-(axes!=0 ? separation[u] : 0),-128);
        enemyLoc=C3DVector(current.X-(axes!=1 ? separation[v] : 0),current.Y+(axes!=0 ? separation[v] : 0),256);
        const bool closePreferred=separation[u]*separation[u]*(axes==2?2:1)<nearPreferred*nearPreferred;
        const bool closeEnemy=separation[v]*separation[v]*(axes==2?2:1)<nearEnemy*nearEnemy;
        const bool expected=a && (closePreferred || (!o && closeEnemy));
        const char* expectedTrace= !a ? "A" : closePreferred ? "AWPC" : "AWPCHECWO";
        attacker=a!=0; outer=o!=0; count=0; trace[0]=0;
        CAutoPlaceFixture::Set(creature,physics,radii[r]);
        group.Run();
        if (latest!=expected || strcmp(trace,expectedTrace)) {
            fprintf(stderr,"case %u expected %d %s got %d %s\n",scenarios,expected,expectedTrace,latest,trace);
            return 1;
        }
        ++scenarios;
    }
    if (scenarios!=1764) return 1;
    puts("0096e700_TEST PASS");
    return 0;
}
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() { if (PStringData) abort(); }
