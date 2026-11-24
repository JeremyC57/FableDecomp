// CAIStateGroup_NymphAttack::Update()
// Leaf: ?Update@CAIStateGroup_NymphAttack@@UAEXXZ
// Flags: default
#define FABLE_RETAIL_NYMPH_ATTACK_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define private public
#define protected public
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_NymphAttack.h"
#include "engine/CAIBrain.h"
#include "engine/CTCNymph.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/CCreatureAction_NymphSummonInto.h"
#include "engine/CCreatureAction_NymphSummonOutOf.h"
#include "engine/CCreatureAction_SummonCreatures.h"
#include "engine/CCreatureAction_NymphIdleAfterShot.h"
#include "engine/CCreatureAction_NymphShot.h"
#undef private
#undef protected

void CAIStateGroup_NymphAttack::Update()
{
    CTCNymph* pTCNymph;
    GetCreature().GetTC(&pTCNymph);
    CIntelligentPointer<CThingCreatureBase> pTarget(pTCNymph->GetTarget());
    if (IS_THING_ALIVE(pTarget)) {
        if (pTCNymph->GetShouldSummon() && IsTargetInLineOfSight(pTarget.Get())) {
            if (AIAction_CombatPerformActionAndWait(CCreatureAction_NymphSummonInto(GetCreature(), *pTarget), false) == AI_ACTION_FINISHED) {
                if (IS_THING_ALIVE(pTarget) && !pTCNymph->HasBeenHit()
                    && AIAction_CombatPerformActionAndWait(CCreatureAction_SummonCreatures(GetCreature(), *pTarget, false), false) == AI_ACTION_FINISHED) {
                    if (!pTCNymph->HasBeenHit())
                        AIAction_CombatPerformActionAndWait(CCreatureAction_NymphSummonOutOf(GetCreature()), false);
                }
            }
        } else {
            long loopCounter = 0;
            const long shotLoops = pTCNymph->GetNymphDef().ShotsBeforeIdle;
            const long idleFrames = (long)(CMainGameComponent::GetConstantFPS() * pTCNymph->GetNymphDef().IdleTimeBetweenShots);
            long lastShotFrame = 0;
            pTCNymph->SetIsReadyToSummon(true);
            while (IS_THING_ALIVE(pTarget) && pTCNymph->IsDecentAttackRange(pTarget->GetPos())) {
                if (CWorld::GetFrame() < lastShotFrame + idleFrames || (shotLoops > 0 && ++loopCounter > shotLoops)) {
                    loopCounter = 0;
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_NymphIdleAfterShot(GetCreature(), *pTarget), false) != AI_ACTION_FINISHED)
                        break;
                } else {
                    if (!IsTargetInLineOfSight(pTarget.Get()))
                        break;
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_NymphShot(GetCreature(), *pTarget), false) != AI_ACTION_FINISHED)
                        break;
                    lastShotFrame = CWorld::GetFrame();
                }
                if (pTCNymph->HasBeenHit())
                    break;
            }
        }
        pTCNymph->ClearReadyToAttackFlags();
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

void CIVCountedPointeeBase::Delete() const { delete this; }
CResource::CResource() {}
CResource::~CResource() {}
CDefClassBase::CDefClassBase() {}
CSubDefClassBase::CSubDefClassBase() {}
CAIStateGroupBase::CAIStateGroupBase() {}
CAIBrain::CAIBrain(const CMainGameComponent&, CWorld&, CThingManager&, CPlayerManager&,
    CThingAICreature& creature, CDefPointer<const CBrainDef>) : Creature(creature) {}
CTCNymph::CTCNymph(CThing& thing) : CTCBase(thing), CanAttack(true), ReadyToAttack(true), ReadyToSummon(false), BeenHit(false) {}

class TestNymph : public CTCNymph {
public:
    TestNymph(CThing& thing) : CTCNymph(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_NYMPH; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

class TestNymphDef : public CNymphDef {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
};

enum TestAction {
    SUMMON_INTO,
    SUMMON_CREATURES,
    SUMMON_OUT_OF,
    IDLE_AFTER_SHOT,
    SHOOT
};
static CAnimationEntry anim;
static int built_action;
static const CCombatActionBase* built_ptr;
static CThingCreatureBase* wanted_creature;
static CThingCreatureBase* wanted_target;
static bool should_summon, line_of_sight, in_range;
static unsigned los_calls, range_calls, action_calls, term_calls, clear_calls;
static unsigned length;
static int expected_actions[8];
static EAIActionReturn ret[8];
static int hit_after, die_after;
static CTCNymph* nymph;
static long frame = 100;
static C3DVector target_pos(3.0f, 2.0f, 1.0f);

CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) {}
CCreatureActionBase::~CCreatureActionBase() {}
CCombatActionBase::CCombatActionBase(CThingCreatureBase& creature, long, const CSoundPair& sound)
    : CCreatureActionBase(creature, 0.0f, 0.0f, anim, sound, 0, 0) {}
CCombatActionBase::~CCombatActionBase() {}
CCreatureActionBase* CCombatActionBase::Clone() const { abort(); }

static void RegisterAction(int kind, const CCombatActionBase& action, const CThingCreatureBase& creature, const CThingCreatureBase* target)
{
    if (&creature != wanted_creature || (target && target != wanted_target))
        abort();
    built_action = kind;
    built_ptr = &action;
}

CCreatureAction_NymphSummonInto::CCreatureAction_NymphSummonInto(CThingCreatureBase& creature, CThingCreatureBase& target)
    : CCombatActionBase(creature, 0, CSoundPair()) { RegisterAction(SUMMON_INTO, *this, creature, &target); }
CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures(
    CThingCreatureBase& creature, CThingCreatureBase& target, bool spawn_on_target)
    : CCombatActionBase(creature, 0, CSoundPair()), SpawnOnTarget(spawn_on_target)
{
    if (spawn_on_target)
        abort();
    RegisterAction(SUMMON_CREATURES, *this, creature, &target);
}
CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 0, CSoundPair()) { RegisterAction(SUMMON_OUT_OF, *this, creature, 0); }
CCreatureAction_NymphIdleAfterShot::CCreatureAction_NymphIdleAfterShot(CThingCreatureBase& creature, CThingCreatureBase& target)
    : CCombatActionBase(creature, 0, CSoundPair()) { RegisterAction(IDLE_AFTER_SHOT, *this, creature, &target); }
CCreatureAction_NymphShot::CCreatureAction_NymphShot(CThingCreatureBase& creature, CThingCreatureBase& target)
    : CCombatActionBase(creature, 0, CSoundPair()) { RegisterAction(SHOOT, *this, creature, &target); }

EAIActionReturn CAIStateGroupBase::AIAction_CombatPerformActionAndWait(const CCombatActionBase& action, bool wait)
{
    if (&action != built_ptr || wait || action_calls >= length || built_action != expected_actions[action_calls])
        abort();
    ++action_calls;
    if ((int)action_calls == hit_after)
        nymph->BeenHit = true;
    if ((int)action_calls == die_after)
        wanted_target->Dead = true;
    return ret[action_calls - 1];
}
void CAIStateGroupBase::Terminate() { ++term_calls; }
bool CAIStateGroup_NymphAttack::IsRunnable() { abort(); }
bool CAIStateGroup_NymphAttack::IsTargetInLineOfSight(CThingCreatureBase* target)
{
    ++los_calls;
    return line_of_sight && target == wanted_target;
}
bool CTCNymph::GetShouldSummon() { return should_summon; }
bool CTCNymph::IsDecentAttackRange(const C3DVector& pos) const
{
    ++range_calls;
    return in_range && &pos == &target_pos;
}
void CTCNymph::ClearReadyToAttackFlags()
{
    ++clear_calls;
    ReadyToAttack = CanAttack = false;
    PTarget.SetPItem(0);
}
const C3DVector& CThing::GetPos() const { return target_pos; }
long FABLE_FASTCALL CWorld::GetFrame() { return frame; }
long CMainGameComponent::ConstantFPS = 10;

static void Reset(TestNymphDef& def)
{
    should_summon = true;
    line_of_sight = true;
    in_range = true;
    nymph->BeenHit = false;
    nymph->ReadyToSummon = false;
    wanted_target->Dead = false;
    nymph->PTarget.SetPItem(wanted_target);
    def.ShotsBeforeIdle = 0;
    def.IdleTimeBetweenShots = 0.0f;
    frame = 100;
    los_calls = range_calls = action_calls = term_calls = clear_calls = 0;
    length = 0;
    hit_after = die_after = -1;
    for (int i = 0; i < 8; ++i) ret[i] = AI_ACTION_FINISHED;
}

static bool Check(unsigned actions, unsigned clear)
{
    return action_calls == actions && clear_calls == clear && term_calls == 1
        && (!clear || nymph->PTarget.Get() == 0);
}

int main()
{
    CThingAICreature creature, target;
    wanted_creature = &creature;
    wanted_target = &target;
    TestNymph component(creature);
    nymph = &component;
    TestNymphDef* def = new TestNymphDef;
    component.PDef.Object = def;
    memset(creature.UsingComponents._M_w, 0, sizeof(creature.UsingComponents._M_w));
    const ETCInterfaceType key = TCI_NYMPH;
    creature.UsingComponents[key] = true;
    creature.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, &component));
    // These collaborators are retained by the engine constructor signature.
    // The fixture initializes the creature reference used by the leaf.
    static unsigned __int64 game_storage[4], world_storage[4], manager_storage[4], player_storage[4];
    CAIBrain brain(*reinterpret_cast<CMainGameComponent*>(game_storage), *reinterpret_cast<CWorld*>(world_storage),
        *reinterpret_cast<CThingManager*>(manager_storage), *reinterpret_cast<CPlayerManager*>(player_storage),
        creature, CDefPointer<const CBrainDef>());
    CAIStateGroup_NymphAttack state;
    state.PBrain = &brain;

    Reset(*def);
    component.PTarget.SetPItem(0);
    state.Update();
    if (!Check(0, 0) || los_calls)
        return 1;
    Reset(*def);
    target.Dead = true;
    state.Update();
    if (!Check(0, 0) || los_calls)
        return 2;

    Reset(*def);
    length = 3;
    expected_actions[0] = SUMMON_INTO;
    expected_actions[1] = SUMMON_CREATURES;
    expected_actions[2] = SUMMON_OUT_OF;
    state.Update();
    if (!Check(3, 1) || los_calls != 1 || range_calls)
        return 3;

    Reset(*def);
    length = 1;
    ret[0] = AI_ACTION_FAILED;
    state.Update();
    if (!Check(1, 1))
        return 4;

    Reset(*def);
    length = 1;
    hit_after = 1;
    state.Update();
    if (!Check(1, 1))
        return 5;

    Reset(*def);
    length = 2;
    hit_after = 2;
    state.Update();
    if (!Check(2, 1))
        return 6;

    Reset(*def);
    length = 1;
    die_after = 1;
    state.Update();
    if (!Check(1, 1))
        return 7;

    Reset(*def);
    line_of_sight = false;
    in_range = false;
    state.Update();
    if (!Check(0, 1) || !component.ReadyToSummon || los_calls != 1 || range_calls != 1)
        return 8;

    Reset(*def);
    should_summon = false;
    def->IdleTimeBetweenShots = 1.0f;
    frame = 0;
    length = 1;
    expected_actions[0] = IDLE_AFTER_SHOT;
    ret[0] = AI_ACTION_FAILED;
    state.Update();
    if (!Check(1, 1) || los_calls)
        return 9;

    Reset(*def);
    should_summon = false;
    length = 1;
    expected_actions[0] = SHOOT;
    ret[0] = AI_ACTION_FAILED;
    state.Update();
    if (!Check(1, 1) || los_calls != 1)
        return 10;

    Reset(*def);
    should_summon = false;
    length = 1;
    hit_after = 1;
    state.Update();
    if (!Check(1, 1))
        return 11;

    Reset(*def);
    should_summon = false;
    def->ShotsBeforeIdle = 1;
    length = 2;
    expected_actions[0] = SHOOT;
    expected_actions[1] = IDLE_AFTER_SHOT;
    ret[1] = AI_ACTION_FAILED;
    state.Update();
    if (!Check(2, 1) || range_calls != 2)
        return 12;

    Reset(*def);
    should_summon = false;
    line_of_sight = false;
    state.Update();
    if (!Check(0, 1) || los_calls != 1)
        return 13;

    puts("0091c6a0_TEST PASS");
    return 0;
}

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
void CCombatActionBase::PerformAction() { abort(); }