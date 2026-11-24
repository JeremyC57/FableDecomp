#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CMODEBASE_CTOR
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define private public
#define protected public
// CAIStateGroup_SitRoundCampfire::Update()
// Leaf: ?Update@CAIStateGroup_SitRoundCampfire@@UAEXXZ
// Flags: default
#define FABLE_RETAIL_SIT_ROUND_CAMPFIRE_PASS27
#define FABLE_RETAIL_CANIMFILTERBASE_SIZE_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CMainGameComponent.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_SitRoundCampfire.h"
#include "engine/CAIBrain.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ThingAlive.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureAction_SitDown.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/GFRandom.h"
#include "engine/CWorld.h"
#undef private
#undef protected

namespace {
CCharString SIT_ON_FLOOR_ANIM_NAME("ST_SIT_ON_FLOOR");
const unsigned long MIN_TIME_TO_SIT = CMainGameComponent::GetConstantFPS() * 20;
const unsigned long RANGE_TIME_TO_SIT = CMainGameComponent::GetConstantFPS() * 120;
}

void CAIStateGroup_SitRoundCampfire::Update()
{
    if (!IS_THING_ALIVE(PCampfire)) {
        Terminate();
        return;
    }

    CTCSingleUserActionPoints* ptc_action_points;
    PCampfire->GetTC(&ptc_action_points);
    C3DVector pos_to_sit;
    CRightHandedSet rhset;
    if (!ptc_action_points->GetFreeActionPoint(GetCreature(), pos_to_sit, rhset, SeatPosIndex)) {
        Terminate();
        return;
    }
    GotSeatPos = true;

    EAIActionReturn ret = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PCampfire)) {
            Terminate();
            return;
        }
        bool nav_route_calculated;
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, pos_to_sit, nav_route_calculated,
                                 AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);

    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PCampfire)) {
        Terminate();
        return;
    }

    C3DVector required_facing_dir(rhset.GetForward());
    C3DVector pos_to_look_at = GetCreature().PeekPos() + required_facing_dir;
    ret = AI_ACTION_FAILED;
    do {
        if (!IS_THING_ALIVE(PCampfire)) {
            Terminate();
            return;
        }
        ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, pos_to_look_at, 0x11, 0.0027777f);
    } while (ret == AI_ACTION_IN_PROGRESS);

    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PCampfire)) {
        Terminate();
        return;
    }

    const CAnimationEntry& anim_entry = GetCreature().GetRandomAnimFromNameWithFilter(SIT_ON_FLOOR_ANIM_NAME, CAnimFilterAnyGroup());
    CCountedPointer<NCreatureMode::CModeSitFloor> psit_mode(new NCreatureMode::CModeSitFloor(GetCreature(), anim_entry.GroupName));
    Sitting = true;
    if (AIAction_PerformAction(CCreatureAction_SitDown(GetCreature(), anim_entry, psit_mode)) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }

    unsigned long frame_to_stop_sitting = PeekWorld().GetFrame() + MIN_TIME_TO_SIT + GFRandom(RANGE_TIME_TO_SIT, GetPWorldSeed());
    while ((unsigned long)PeekWorld().GetFrame() < frame_to_stop_sitting && AIAction_DoNothing() == AI_ACTION_IN_PROGRESS) {
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
CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    if (length < 0)
        length = strlen(text);
    PStringData->NoRefs = 1;
    PStringData->Data.PData = new char[length + 1];
    PStringData->Data.StringLength = length;
    PStringData->Data.DataLength = length + 1;
    PStringData->Data.UseFastExtend = false;
    memcpy(PStringData->Data.PData, text, length);
    PStringData->Data.PData[length] = 0;
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CCharString::~CCharString()
{
    if (PStringData && --PStringData->NoRefs == 0) {
        delete[] PStringData->Data.PData;
        delete PStringData;
    }
}

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


long CMainGameComponent::ConstantFPS = 1;

CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
void CIVCountedPointeeBase::Delete() const { abort(); }

CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) {}
CCreatureActionBase::~CCreatureActionBase() {}

static CThingAICreature* active_creature;
static CThingObject* active_campfire;
static CTCSingleUserActionPoints* active_points;
static const CWorld* world;
static const C3DVector seat(1.5f, 2.0f, 3.5f);
static const C3DVector facing(3.25f, 0.5f, 3.75f);
static const C3DVector start(2.75f, 4.5f, 6.25f);
static CAnimationEntry animation;
static CCharString group_name;
static unsigned long random_seed = 111;
static int frame;
static int point_requests, moves, looks, sits, waits, terminations, mode_count;
static bool free_point, kill_on_move, kill_on_look;
static EAIActionReturn move_results[2], look_results[2], sit_result, wait_result;

static void Require(bool check)
{
    if (!check)
        abort();
}
static bool HasPosition(const C3DVector& v, const C3DVector& expected)
{
    return v.X == expected.X && v.Y == expected.Y && v.Z == expected.Z;
}

CTCUserActionPointBase::CTCUserActionPointBase(CThing& thing) : CTCBase(thing) {}
CTCSingleUserActionPoints::CTCSingleUserActionPoints(CThing& thing) : CTCUserActionPointBase(thing) {}
class TestActionPoints : public CTCSingleUserActionPoints {
public:
    TestActionPoints(CThing& thing) : CTCSingleUserActionPoints(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_SINGLE_USER_ACTION_POINTS; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
bool CTCUserActionPointBase::GetFreeActionPoint(CThing& creature, C3DVector& pos, CRightHandedSet& rhset, long& index)
{
    Require(this == active_points && &creature == active_creature);
    ++point_requests;
    if (!free_point)
        return false;
    pos = seat;
    rhset.Forward = facing;
    index = 12;
    return true;
}
EAIActionReturn CAIStateGroupBase::AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos,
    bool& nav_route, EMovementType movement, bool a, bool b, const CThing* follow, bool c, float d)
{
    Require(repeat == ACTION_REPEAT_CALLBACK && HasPosition(pos, seat) &&
        movement == AI_MOVEMENT_WALKING && a && b && !follow && c && d == 0.0f && moves < 2);
    nav_route = true;
    if (kill_on_move)
        active_campfire->Dead = true;
    return move_results[moves++];
}
const C3DVector& CThingPhysical::PeekPos() const
{
    Require(this == active_creature);
    return start;
}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float speed)
{
    const C3DVector expected = start + facing;
    Require(repeat == ACTION_REPEAT_CALLBACK && HasPosition(pos, expected) &&
        flags == 0x11 && speed == 0.0027777f && looks < 2);
    if (kill_on_look)
        active_campfire->Dead = true;
    return look_results[looks++];
}
bool CAnimFilterAnyGroup::operator()(const CAnimationEntry&) const { return true; }
CAnimFilterBase* CAnimFilterAnyGroup::Clone() const { abort(); }
const CAnimationEntry& CThingCreatureBase::GetRandomAnimFromNameWithFilter(
    const CCharString& name, const CAnimFilterBase& filter) const
{
    Require(this == active_creature && &name == &SIT_ON_FLOOR_ANIM_NAME
        && strcmp(name.PStringData->Data.PeekData(), "ST_SIT_ON_FLOOR") == 0 && filter(animation));
    return animation;
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline CDefString::operator CCharString() const
{
    return GDefStringTable.GetString(TablePos);
}

CWideString::CWideString() {}
CWideString::~CWideString() {}
CDefStringTable GDefStringTable;
CCharString CDefStringTable::GetString(long pos) const
{
    Require(this == &GDefStringTable && pos == 41);
    return CCharString("Campfire mode group");
}
CTCCreatureModeManager::CModeBase::CModeBase(CThing& thing) : ParentThing(thing) {}
CTCCreatureModeManager::CModeBase::~CModeBase() {}
NCreatureMode::CModeSitBase::CModeSitBase(CThing& thing)
    : CModeBase(thing), IdleCounter(0), PlayingIdle(false) {}
NCreatureMode::CModeSitBase::~CModeSitBase() {}
NCreatureMode::CModeSitFloor::CModeSitFloor(CThing& thing, const CCharString& name)
    : CModeSitBase(thing)
{
    Require(&thing == active_creature && name.PStringData &&
        strcmp(name.PStringData->Data.PeekData(), "Campfire mode group") == 0);
    ++mode_count;
}
NCreatureMode::CModeSitFloor::~CModeSitFloor() { --mode_count; }
unsigned long NCreatureMode::CModeSitFloor::GetType() const { abort(); }

CCreatureAction_SitDown::CCreatureAction_SitDown(CThingCreatureBase& creature,
    const CAnimationEntry& anim, CCountedPointer<NCreatureMode::CModeSitBase> mode)
    : CCreatureActionBase(creature, 0.0f, 0.0f, anim, CSoundPair(), 0, 0), PSitMode(mode)
{
    Require(&creature == active_creature && &anim == &animation && mode.Get() && mode_count == 1);
}
CCreatureAction_SitDown::~CCreatureAction_SitDown() {}
void CCreatureAction_SitDown::PerformAction() { abort(); }
CCreatureActionBase* CCreatureAction_SitDown::Clone() const { abort(); }

EAIActionReturn CAIStateGroupBase::AIAction_PerformAction(const CCreatureActionBase&)
{
    Require(mode_count == 1);
    ++sits;
    return sit_result;
}
const CWorld& CAIStateGroupBase::PeekWorld() const { return *world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() const { return &random_seed; }
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    Require(range == 120 && RANGE_TIME_TO_SIT == 120 && MIN_TIME_TO_SIT == 20 && seed == &random_seed);
    return 2;
}
long __fastcall CWorld::GetFrame() { return frame; }
EAIActionReturn CAIStateGroupBase::AIAction_DoNothing()
{
    ++waits;
    ++frame;
    return wait_result;
}
void CAIStateGroupBase::Terminate() { ++terminations; }

static void Reset(CAIStateGroup_SitRoundCampfire& state)
{
    state.PCampfire.SetPItem(active_campfire);
    state.SeatPosIndex = -1;
    state.GotSeatPos = false;
    state.Sitting = false;
    active_campfire->Dead = false;
    point_requests = moves = looks = sits = waits = terminations = frame = 0;
    free_point = true;
    kill_on_move = kill_on_look = false;
    move_results[0] = look_results[0] = sit_result = AI_ACTION_FINISHED;
    move_results[1] = look_results[1] = AI_ACTION_FINISHED;
    wait_result = AI_ACTION_IN_PROGRESS;
}
static void Check(CAIStateGroup_SitRoundCampfire& state, int points, int move, int look, int sit, int wait)
{
    state.Update();
    Require(point_requests == points && moves == move && looks == look && sits == sit && waits == wait);
    Require(terminations == 1 && mode_count == 0 && state.GotSeatPos == (move != 0) && state.Sitting == (sit != 0));
    Require(state.SeatPosIndex == (move != 0 ? 12 : -1));
}
int main()
{
    animation.GroupName.TablePos = 41;
    static CThingAICreature creature;
    static CThingObject campfire;
    static TestActionPoints points(campfire);
    static CAIStateGroup_SitRoundCampfire state;
    static unsigned char world_memory[sizeof(CWorld)];
    CMainGameComponent* game = reinterpret_cast<CMainGameComponent*>(world_memory);
    CThingManager* manager = reinterpret_cast<CThingManager*>(world_memory);
    CPlayerManager* player = reinterpret_cast<CPlayerManager*>(world_memory);
    CWorld& stub_world = *reinterpret_cast<CWorld*>(world_memory);
    world = &stub_world;
    CAIBrain brain(*game, stub_world, *manager, *player, creature, CDefPointer<const CBrainDef>());
    state.PBrain = &brain;
    active_creature = &creature;
    active_campfire = &campfire;
    active_points = &points;
    memset(&campfire.UsingComponents, 0, sizeof(campfire.UsingComponents));
    campfire.UsingComponents[TCI_SINGLE_USER_ACTION_POINTS] = true;
    campfire.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(TCI_SINGLE_USER_ACTION_POINTS, &points));

    Reset(state); state.PCampfire.SetPItem(0); Check(state, 0, 0, 0, 0, 0);
    Reset(state); campfire.Dead = true; Check(state, 0, 0, 0, 0, 0);
    Reset(state); free_point = false; Check(state, 1, 0, 0, 0, 0);
    Reset(state); move_results[0] = AI_ACTION_FAILED; Check(state, 1, 1, 0, 0, 0);
    Reset(state); kill_on_move = true; move_results[0] = AI_ACTION_IN_PROGRESS; Check(state, 1, 1, 0, 0, 0);
    Reset(state); kill_on_move = true; Check(state, 1, 1, 0, 0, 0);
    Reset(state); look_results[0] = AI_ACTION_FAILED; Check(state, 1, 1, 1, 0, 0);
    Reset(state); kill_on_look = true; look_results[0] = AI_ACTION_IN_PROGRESS; Check(state, 1, 1, 1, 0, 0);
    Reset(state); kill_on_look = true; Check(state, 1, 1, 1, 0, 0);
    Reset(state); sit_result = AI_ACTION_FAILED; Check(state, 1, 1, 1, 1, 0);
    Reset(state); wait_result = AI_ACTION_FAILED; Check(state, 1, 1, 1, 1, 1);
    Reset(state); move_results[0] = look_results[0] = AI_ACTION_IN_PROGRESS; Check(state, 1, 2, 2, 1, 22);
    puts("009392a0_TEST PASS");
}

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
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
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
void CAIStateGroup_SitRoundCampfire::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) { abort(); }
bool CAIStateGroup_SitRoundCampfire::IsRunnable() { abort(); }
void CAIStateGroup_SitRoundCampfire::OnReplace() { abort(); }
void CAIStateGroup_SitRoundCampfire::InitStateGroup(const CWorld&, CThingAICreature&) { abort(); }
bool CAIStateGroup_SitRoundCampfire::AllowPriorityOverlap() { abort(); }