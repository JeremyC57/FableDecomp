// CAIStateGroup_CreatureRangedCombat2::Update()
// Leaf: ?Update@CAIStateGroup_CreatureRangedCombat2@@MAEXXZ
// Flags: default
#define FABLE_RETAIL_CREATURE_RANGED_COMBAT2_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_RESETAIMING
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#define FABLE_RETAIL_RANGED_COMBAT_FIXTURE_PASS27
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_SENSED_PASS23
#define FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_CTOR_PASS23
#define FABLE_RETAIL_CCOMBATMANAGER_CTOR
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define private public
#define protected public
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/GFSqr.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/NCreatureAI.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCombatManager.h"
#undef private
#undef protected

inline bool CAIStateGroup_CreatureRangedCombat2::HasRangedWeaponOut() const
{
    return CTCCarrying::IsThingCarryingRangedWeapon(GetCreature());
}

void CAIStateGroup_CreatureRangedCombat2::Update()
{
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return;
    }

    const CTCPerceiveThing* ptc_perceive;
    GetCreature().PeekTC(&ptc_perceive);
    const float FIRING_DISTANCE = ptc_perceive->GetExtendedSightRadius();
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;

    {
        CTCCreatureModeManager* ptc_cmm;
        if (GetCreature().GetTC(&ptc_cmm)) {
            ptc_cmm->RemoveMode(NCreatureMode::MODE_STRAFE);
            AddedStrafe = false;
        }
    }
    if (!HasRangedWeaponOut()) {
        GetCreature().GetCombatManager().CreatureUnsheathWeapon(GetCreature(), UNSHEATHE_RANGED_WEAPON);
        return;
    }

    const CTCCreatureNavigation* ptc_creature_nav;
    GetCreature().PeekTC(&ptc_creature_nav);
    bool is_target_reachable = ptc_creature_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(), PTargetThing->GetPos(), 2.0f);
    if (is_target_reachable) {
        do {
            if (!(IS_THING_ALIVE(PTargetThing) && NCreatureAI::IsBestTargetForAttack(GetCreature(), PTargetThing.Get()) && IsAbleToFire())) {
                Terminate();
                return;
            }
            if ((PTargetThing->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude2D() < GFSqr(FIRING_DISTANCE * 0.95f)
                && ptc_creature_nav->IsPosNavLineOfSightTo(PTargetThing->GetPos())) {
                ret = AI_ACTION_FINISHED;
                break;
            }
            bool nav;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PTargetThing, 2.0f, nav, AI_MOVEMENT_RUNNING,
                                                true, false, true, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return;
        }
    }

    {
        CTCCreatureModeManager* ptc_cmm;
        if (GetCreature().GetTC(&ptc_cmm)) {
            ptc_cmm->AddMode(NCreatureMode::MODE_STRAFE);
            AddedStrafe = true;
        }
    }

    const CTCCarrying* ptc_carrying;
    if (GetCreature().PeekTC(&ptc_carrying)) {
        CThing* pcarried = ptc_carrying->GetThingInPrimarySlot();
        CTCWeapon* ptc_weapon;
        if (IS_THING_ALIVE(pcarried) && pcarried->GetTC(&ptc_weapon) && ptc_weapon->IsProjectileWeapon())
            ptc_weapon->ResetAiming();
    }

    while (IS_THING_ALIVE(PTargetThing) && NCreatureAI::IsBestTargetForAttack(GetCreature(), PTargetThing.Get()) && IsAbleToFire()) {
        float sqr_sep_2d = (GetCreature().GetPos() - PTargetThing->GetPos()).GetSquaredMagnitude2D();
        if (is_target_reachable && sqr_sep_2d > GFSqr(FIRING_DISTANCE))
            return;
        if (!ptc_creature_nav->IsPosNavLineOfSightTo(PTargetThing->GetPos()))
            return;
        ret = AIAction_PumpCombatActions();
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return;
        }
    }
    Terminate();
}
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

#include <stdlib.h>
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = reinterpret_cast<CBaseObjectPointer*>(item); }


static void RequireAt(bool check, int line)
{
    if (!check) {
        printf("check failed on line %d\n", line);
        fflush(0);
        abort();
    }
}
#define Require(check) RequireAt((check), __LINE__)

void CIVCountedPointeeBase::Delete() const { abort(); }
CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
CCombatManager::CCombatManager(CWorld& world) : World(world) {}
CTCCreatureNavigation::CTCCreatureNavigation(CThing& thing) : CTCBase(thing) {}
CTCCreatureNavigation::~CTCCreatureNavigation() {}
CTCPerceiveThing::CTCPerceiveThing(CThing& thing) : CTCBase(thing) {}
CTCPerceiveThing::~CTCPerceiveThing() {}
CTCCreatureModeManager::CTCCreatureModeManager(CThing& thing) : CTCBase(thing) {}
CTCCreatureModeManager::~CTCCreatureModeManager() {}
CTCCarrying::CTCCarrying(CThing& thing) : CTCBase(thing) {}
CTCWeapon::CTCWeapon(CThing& thing) : CTCBase(thing) {}

class TestNavigation : public CTCCreatureNavigation {
public:
    TestNavigation(CThing& thing) : CTCCreatureNavigation(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_NAVIGATION; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestPerception : public CTCPerceiveThing {
public:
    TestPerception(CThing& thing) : CTCPerceiveThing(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_PERCEIVE_THING; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestModes : public CTCCreatureModeManager {
public:
    TestModes(CThing& thing) : CTCCreatureModeManager(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_ENTITY_MODE_MANAGER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestCarrying : public CTCCarrying {
public:
    TestCarrying(CThing& thing) : CTCCarrying(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_CARRYING; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestWeapon : public CTCWeapon {
public:
    TestWeapon(CThing& thing) : CTCWeapon(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_WEAPON; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

static CThingAICreature* archer;
static CThingAICreature* victim;
static CThing* carried;
static TestNavigation* navigation;
static TestPerception* perception;
static TestModes* modes;
static TestCarrying* carrying;
static TestWeapon* weapon;
static CCombatManager* combat;
static C3DVector archer_pos(0.0f, 0.0f, 8.0f);
static C3DVector victim_pos(0.0f, 3.0f, 91.0f);
static bool ranged_out, reachable, line_of_sight, best_target, can_fire, projectile;
static bool advance_after_move, move_target_after_aim, kill_target_after_pump;
static EAIActionReturn move_results[2], pump_result;
static int removed, added, unsheathed, moved, pumped, aims, terminated, route_checks, firing_checks;

float CTCPerceiveThing::GetExtendedSightRadius() const { Require(this == perception); return 10.0f; }
bool CTCCarrying::IsThingCarryingRangedWeapon(const CThing& thing) { Require(&thing == archer); return ranged_out; }
CCombatManager& CThingCreatureBase::GetCombatManager() { Require(this == archer); return *combat; }
void CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase& thing, EUnsheatheWeaponType type)
{
    Require(this == combat && &thing == archer && type == UNSHEATHE_RANGED_WEAPON);
    ++unsheathed;
}
const C3DVector& CThing::GetPos() const
{
    Require(this == archer || this == victim);
    return this == archer ? archer_pos : victim_pos;
}
bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const
{
    Require(this == navigation && &from == &archer_pos && &to == &victim_pos && proximity == 2.0f);
    ++route_checks;
    return reachable;
}
bool CTCCreatureNavigation::IsPosNavLineOfSightTo(const C3DVector& pos) const
{
    Require(this == navigation && &pos == &victim_pos);
    return line_of_sight;
}
bool __fastcall NCreatureAI::IsBestTargetForAttack(const CThingAICreature& creature, const CThingCreatureBase* target)
{
    Require(&creature == archer && target == victim);
    return best_target;
}
bool CAIStateGroup_CreatureRangedCombat2::IsAbleToFire() const { ++firing_checks; return can_fire; }
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { Require(this == modes && mode == NCreatureMode::MODE_STRAFE); ++removed; }
bool CTCCreatureModeManager::AddMode(unsigned long mode) { Require(this == modes && mode == NCreatureMode::MODE_STRAFE); ++added; return true; }
CThing* CTCCarrying::GetThingInPrimarySlot() const { Require(this == carrying); return carried; }
bool CTCWeapon::IsProjectileWeapon() const { Require(this == weapon); return projectile; }
void CTCWeapon::ResetAiming()
{
    Require(this == weapon);
    ++aims;
    if (move_target_after_aim)
        victim_pos.Y = 20.0f;
}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat,
    CIntelligentPointer<const CThing> thing, float proximity, bool& nav, EMovementType move, bool a, bool b, bool c, bool d, float speed)
{
    Require(repeat == ACTION_REPEAT_CALLBACK && thing.Get() == victim && proximity == 2.0f && move == AI_MOVEMENT_RUNNING);
    Require(a && !b && c && d && speed == 0.0f && moved < 2);
    nav = true;
    if (advance_after_move)
        victim_pos.Y = 3.0f;
    return move_results[moved++];
}
EAIActionReturn CAIStateGroupBase::AIAction_PumpCombatActions()
{
    ++pumped;
    if (kill_target_after_pump)
        victim->Dead = true;
    return pump_result;
}
void CAIStateGroupBase::Terminate() { ++terminated; }
static void Put(CThing& thing, ETCInterfaceType key, CTCBase* tc)
{
    thing.UsingComponents[key] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, tc));
}
static void Reset(CAIStateGroup_CreatureRangedCombat2& state)
{
    state.PTargetThing.SetPItem(victim);
    state.AddedStrafe = false;
    victim->Dead = carried->Dead = false;
    victim_pos.Y = 3.0f;
    archer->UsingComponents[TCI_ENTITY_MODE_MANAGER] = true;
    archer->UsingComponents[TCI_CARRYING] = true;
    ranged_out = reachable = line_of_sight = best_target = can_fire = projectile = true;
    advance_after_move = move_target_after_aim = kill_target_after_pump = false;
    move_results[0] = move_results[1] = AI_ACTION_FINISHED;
    pump_result = AI_ACTION_TERMINATE_PROCESS;
    removed = added = unsheathed = moved = pumped = aims = terminated = route_checks = firing_checks = 0;
}
static void Check(CAIStateGroup_CreatureRangedCombat2& state, int r, int a, int u, int m, int p, int w, int t, int routes)
{
    state.Update();
    Require(removed == r && added == a && unsheathed == u && moved == m && pumped == p && aims == w && terminated == t);
    Require(route_checks == routes && state.AddedStrafe == (a != 0));
}
int main()
{
    CThingAICreature actor, target;
    CThing item;
    TestNavigation nav(actor);
    TestModes mode(actor);
    TestPerception perceive(actor);
    TestCarrying carry(actor);
    TestWeapon projectile_component(item);
    static unsigned __int64 game_storage[4], world_storage[4], manager_storage[4], player_storage[4];
    CWorld& world = *reinterpret_cast<CWorld*>(world_storage);
    CCombatManager combat_manager(world);
    CAIBrain brain(*reinterpret_cast<CMainGameComponent*>(game_storage), world,
        *reinterpret_cast<CThingManager*>(manager_storage), *reinterpret_cast<CPlayerManager*>(player_storage),
        actor, CDefPointer<const CBrainDef>());
    CAIStateGroup_CreatureRangedCombat2 state;
    state.PBrain = &brain;
    archer = &actor; victim = &target; carried = &item;
    navigation = &nav; modes = &mode; perception = &perceive; carrying = &carry;
    weapon = &projectile_component; combat = &combat_manager;
    memset(actor.UsingComponents._M_w, 0, sizeof(actor.UsingComponents._M_w));
    memset(item.UsingComponents._M_w, 0, sizeof(item.UsingComponents._M_w));
    Put(actor, TCI_NAVIGATION, &nav);
    Put(actor, TCI_ENTITY_MODE_MANAGER, &mode);
    Put(actor, TCI_PERCEIVE_THING, &perceive);
    Put(actor, TCI_CARRYING, &carry);
    Put(item, TCI_WEAPON, &projectile_component);

    Reset(state); state.PTargetThing.SetPItem(0); Check(state, 0, 0, 0, 0, 0, 0, 1, 0);
    Reset(state); target.Dead = true; Check(state, 0, 0, 0, 0, 0, 0, 1, 0);
    Reset(state); ranged_out = false; Check(state, 1, 0, 1, 0, 0, 0, 0, 0);
    Reset(state); best_target = false; Check(state, 1, 0, 0, 0, 0, 0, 1, 1); Require(firing_checks == 0);
    Reset(state); can_fire = false; Check(state, 1, 0, 0, 0, 0, 0, 1, 1);
    Reset(state); Check(state, 1, 1, 0, 0, 1, 1, 1, 1); Require(firing_checks == 2);
    Reset(state); victim_pos.Y = 20.0f; move_results[0] = AI_ACTION_IN_PROGRESS; advance_after_move = true;
    Check(state, 1, 1, 0, 1, 1, 1, 1, 1);
    Reset(state); victim_pos.Y = 20.0f; move_results[0] = AI_ACTION_TERMINATE_PROCESS; Check(state, 1, 0, 0, 1, 0, 0, 1, 1);
    Reset(state); move_target_after_aim = true; Check(state, 1, 1, 0, 0, 0, 1, 0, 1);
    Reset(state); reachable = false; victim_pos.Y = 20.0f; Check(state, 1, 1, 0, 0, 1, 1, 1, 1);
    Reset(state); reachable = false; line_of_sight = false; Check(state, 1, 1, 0, 0, 0, 1, 0, 1);
    Reset(state); carried->Dead = true; Check(state, 1, 1, 0, 0, 1, 0, 1, 1);
    Reset(state); projectile = false; pump_result = AI_ACTION_IN_PROGRESS; kill_target_after_pump = true;
    Check(state, 1, 1, 0, 0, 1, 0, 1, 1);
    Reset(state); actor.UsingComponents[TCI_ENTITY_MODE_MANAGER] = actor.UsingComponents[TCI_CARRYING] = false;
    Check(state, 0, 0, 0, 0, 1, 0, 1, 1);
    puts("0091ec30_TEST PASS");
}

void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { fprintf(stderr, "unexpected CAIStateGroupBase::Initialise\n"); abort(); }
void CAIStateGroupBase::OnActivate() { fprintf(stderr, "unexpected CAIStateGroupBase::OnActivate\n"); abort(); }
bool CAIStateGroupBase::InitCreaturePosition() { fprintf(stderr, "unexpected CAIStateGroupBase::InitCreaturePosition\n"); abort(); }
bool CAIStateGroupBase::AllowPriorityOverlap() { fprintf(stderr, "unexpected CAIStateGroupBase::AllowPriorityOverlap\n"); abort(); }
bool CAIStateGroupBase::IsStillValid() const { fprintf(stderr, "unexpected CAIStateGroupBase::IsStillValid\n"); abort(); }
const CCharString* CAIStateGroupBase::GetDebugText() const { fprintf(stderr, "unexpected CAIStateGroupBase::GetDebugText\n"); abort(); }
bool CAIStateGroupBase::IsTimingCritical() const { fprintf(stderr, "unexpected CAIStateGroupBase::IsTimingCritical\n"); abort(); }
EAIStateGroupType CAIStateGroupBase::GetStateGroupType() const { fprintf(stderr, "unexpected CAIStateGroupBase::GetStateGroupType\n"); abort(); }
void CAIStateGroupBase::Cleanup() { fprintf(stderr, "unexpected CAIStateGroupBase::Cleanup\n"); abort(); }
void CAIStateGroupBase::OnReplace() { fprintf(stderr, "unexpected CAIStateGroupBase::OnReplace\n"); abort(); }
bool CAIStateGroupBase::IsRunnable() { fprintf(stderr, "unexpected CAIStateGroupBase::IsRunnable\n"); abort(); }
void CAIStateGroupBase::InitStateGroup(const CWorld& world, CThingAICreature& creature) { fprintf(stderr, "unexpected CAIStateGroupBase::InitStateGroup\n"); abort(); }
void CAIStateGroup_CreatureRangedCombat2::InitStateGroup(const CWorld&, CThingAICreature&) { fprintf(stderr, "unexpected CAIStateGroup_CreatureRangedCombat2::InitStateGroup\n"); abort(); }
bool CAIStateGroup_CreatureRangedCombat2::IsRunnable() { fprintf(stderr, "unexpected CAIStateGroup_CreatureRangedCombat2::IsRunnable\n"); abort(); }
void CAIStateGroup_CreatureRangedCombat2::OnActivate() { fprintf(stderr, "unexpected CAIStateGroup_CreatureRangedCombat2::OnActivate\n"); abort(); }
void CAIStateGroup_CreatureRangedCombat2::Cleanup() { fprintf(stderr, "unexpected CAIStateGroup_CreatureRangedCombat2::Cleanup\n"); abort(); }
EAIStateGroupType CAIStateGroup_CreatureRangedCombat2::GetStateGroupType() const { fprintf(stderr, "unexpected CAIStateGroup_CreatureRangedCombat2::GetStateGroupType\n"); abort(); }
void CThingGameObject::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingGameObject::OnHit\n"); abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingCreatureBase::OnHit\n"); abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingCreatureBase::DrawPreparePrimitivesForRendering\n"); abort(); }
ETCInterfaceType CTCPerceiveThing::GetInterfaceType() const { fprintf(stderr, "unexpected CTCPerceiveThing::GetInterfaceType\n"); abort(); }
CCharString CTCPerceiveThing::GetClassTypeName() const { fprintf(stderr, "unexpected CTCPerceiveThing::GetClassTypeName\n"); abort(); }
void CTCPerceiveThing::OnCreate() { fprintf(stderr, "unexpected CTCPerceiveThing::OnCreate\n"); abort(); }
void CTCPerceiveThing::FrameUpdate() { fprintf(stderr, "unexpected CTCPerceiveThing::FrameUpdate\n"); abort(); }
void CTCPerceiveThing::BulletTimeFrameUpdate() { fprintf(stderr, "unexpected CTCPerceiveThing::BulletTimeFrameUpdate\n"); abort(); }
bool CTCPerceiveThing::HasFrameUpdate() const { fprintf(stderr, "unexpected CTCPerceiveThing::HasFrameUpdate\n"); abort(); }
void CTCPerceiveThing::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { fprintf(stderr, "unexpected CTCPerceiveThing::DrawPreparePrimitivesForRendering\n"); abort(); }
void CTCPerceiveThing::OnKill() { fprintf(stderr, "unexpected CTCPerceiveThing::OnKill\n"); abort(); }
void CTCPerceiveThing::OnLevelUnload() { fprintf(stderr, "unexpected CTCPerceiveThing::OnLevelUnload\n"); abort(); }
void CTCPerceiveThing::OnSetInLimbo(bool) { fprintf(stderr, "unexpected CTCPerceiveThing::OnSetInLimbo\n"); abort(); }