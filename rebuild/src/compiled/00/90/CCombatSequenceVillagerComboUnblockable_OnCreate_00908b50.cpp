// CCombatSequence_VillagerComboUnblockable::OnCreate -- enqueue two interruptable mid-attacks and one non-blockable villager knockdown action.
// Retail address 0x00908b50, 296 bytes. Native sequence fields are owner +0x1c and target +0x20. The first two actions use the audited 81-slot InterruptableMidAttack hierarchy and inline CCombatActionBase ctor 0x00857f30 (code 150, zero sound pair); the third is the external RTTI-confirmed VillagerAttackKnockdown ctor 0x0097cf90 (PDB9A2B60 two Creature references, native zero-offset Thing base view, Blockable=false).
// Counted-pointer reset is 0x00905fd0, AddAction 0x008b0b10, and base OnCreate 0x008b0650. The interruptable action has native size 0x118; its +0x114 InterruptTime remains uninitialized by this ctor.
#define CCombatSequence_VillagerComboUnblockable CCombatSequence_VillagerComboUnblockable_PDB
#include "engine/CCombatSequence_VillagerComboUnblockable.h"
#undef CCombatSequence_VillagerComboUnblockable
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCreatureAction_InterruptableMidAttack CCreatureAction_InterruptableMidAttack_PDB
#include "engine/CCreatureAction_InterruptableMidAttack.h"
#undef CCreatureAction_InterruptableMidAttack
#define CCreatureAction_VillagerAttackKnockdown CCreatureAction_VillagerAttackKnockdown_PDB
#include "engine/CCreatureAction_VillagerAttackKnockdown.h"
#undef CCreatureAction_VillagerAttackKnockdown
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "fable_string.h"

class CThingCreatureBase;
class CThing;
class C3DVector { public: float x, y, z; };
class CTCWeaponTrail;
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    virtual void Validate() const;
    virtual void EditUpdate(float elapsed);
    virtual void PerformAction();
    virtual void FinishAction();
    virtual bool IsPlayingAnimation() const;
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual long GetAnimationTransitionOutTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsStrikeEvent(long event) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsPartOfComboChain() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual CCharString GetStrikeEventHelperName() const;
    virtual bool IsOverrideableByActionOfSamePriority() const;
    virtual void OnFrameUpdate(float elapsed);
    virtual bool IsQueuable() const;
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector& vector) const;
    virtual bool IsBlockable() const;
    virtual bool IsAffectedByHaste() const;
    virtual bool IsAffectedByHeroStats() const;
    virtual bool IsPhysicsPushableByHero() const;
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
    virtual bool IsAlwaysUseAnimationMovement() const;
    virtual bool IsClearingMovementVector() const;
    virtual bool IsToHaveEmptyHands() const;
    virtual bool IsToReplaceItemIfRemoved() const;
    virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const;
    virtual bool IsPreventingQuickAccessItemUsage() const;
    virtual void SetHitter(const CThing& thing);
    virtual void SetHitDirection(C3DVector direction);
    virtual void SetHitPosition(C3DVector position);
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& id) const;
    virtual float GetTotalTimeSeconds() const;
    virtual bool IsToEvadeAllHits() const;
    virtual bool IsToCauseRecoil() const;
    virtual bool IsAbleToBeAttacked() const;
    virtual bool IsPlayHitResponse() const;
    virtual bool IsPreventingNormalMovement() const;
    virtual bool IsPreventingLookTurning() const;
    virtual void OnBlowBlocked(bool blocked);
    virtual void OnFollowUpActionFailedToAdd();
    virtual void UpdateActionRunningTime();
    virtual bool IsDecapitatingAttack() const;
    virtual bool PreventSneaking() const;
    virtual void FrameUpdate();
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsUsingForcedMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
    virtual float GetFrameUpdateTime() const;
    virtual float GetActionSpeedParameter() const;
    virtual float GetActionStartRunningTimeSeconds() const;
    virtual bool OverrideFaceWithDefault() const;
    virtual bool AllowsCombinationAnims() const;
    virtual bool CanAnimationTransitionOut() const;
    virtual void OnAnimationLoaded();
    virtual bool WaitForMeshToLoad() const;
    unsigned char nativeFields[0xa4]; // native ActionBase70 slots, size0xa8
};
class CSoundPair { public: long first; long last; CSoundPair() : first(0), last(0) { } };
class CCombatActionBase : public CCreatureActionBase { public:
    virtual ~CCombatActionBase();
    virtual void PerformAction();
    protected: virtual void FinishAction(); public:
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual bool IsStrikeEvent(long event) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual CCharString GetStrikeEventHelperName() const;
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector& vector) const;
    virtual bool IsBlockable() const;
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& id) const;
    virtual float GetTotalTimeSeconds() const;
    virtual void OnBlowBlocked(bool blocked);
    virtual void UpdateActionRunningTime();
    virtual bool IsDecapitatingAttack() const;
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
    virtual bool AllowsCombinationAnims() const;
    virtual void OnAnimationLoaded();
    virtual C3DVector GetNextPos() const;
    virtual bool IsToUseRotationInAnimation() const;
    virtual bool IsToAlignWithGroundNormal() const;
    virtual bool IsToFaceTarget() const;
    virtual bool IsToFaceTargetAtStart() const;
    virtual C3DVector GetActionMovementVector() const;
    virtual bool IsToPerformActionOnStrike() const;
    virtual bool IsToHitProneThings() const;
    virtual float GetIsToFaceTargetCone() const;
    virtual float GetIsToFaceTargetMinDist() const;
    virtual float GetDamageMultiplier() const;
    unsigned char nativeCombatFields[0x6c]; // retail extends the native 0xa8 action base to 0x114
    CCombatActionBase(CThingCreatureBase& owner, CThing& target, long actionCode, const CSoundPair& sounds); // 0x00857F30
};
class CCreatureAction_VillagerAttackKnockdown : public CCombatActionBase { public:
    bool Blockable;                   // native +0x114; PDB donor offset +0x12c
    unsigned char nativePadding[3];
    CCreatureAction_VillagerAttackKnockdown(CThingCreatureBase& owner, CThingCreatureBase& target, bool blockable); // 0x0097CF90
};
class CCreatureAction_InterruptableMidAttack : public CCombatActionBase { public:
    long InterruptTime; // +0x114 native; intentionally untouched by this ctor
    virtual ~CCreatureAction_InterruptableMidAttack();
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsPartOfComboChain() const;
    virtual void FrameUpdate();
    virtual bool IsToFaceTargetAtStart() const;
    CCreatureAction_InterruptableMidAttack(CThingCreatureBase& owner, CThing& target)
        : CCombatActionBase(owner, target, 150, CSoundPair()) { }
};
#ifdef _MSC_VER
typedef char CCombatActionBaseNativeSize[(sizeof(CCombatActionBase) == 0x114) ? 1 : -1];
#endif
#ifdef _MSC_VER
typedef char CCreatureAction_VillagerAttackKnockdownNativeSize[(sizeof(CCreatureAction_VillagerAttackKnockdown) == 0x118) ? 1 : -1];
#endif
#ifdef _MSC_VER
typedef char CCreatureAction_InterruptableMidAttackNativeSize[(sizeof(CCreatureAction_InterruptableMidAttack) == 0x118) ? 1 : -1];
#endif
struct SCountBlock {
    long references;
    void (__fastcall* destroy)(void*);
    void* object;
};
struct CCountedPointer {
    void* object;
    SCountBlock* count;
    CCountedPointer() : object(0), count(0) { }
    CCountedPointer& operator=(void* value) { Reset_00905fd0(value); return *this; }
    void Reset_00905fd0(void* value); // 0x00905fd0
    ~CCountedPointer()
    {
        if (count) {
            --count->references;
            if (count->references == 0) {
                count->destroy(count->object);
                delete count;
            }
        }
    }
};
class CCombatSequenceBase { public:
    void* nativeVptr; // data-only vptr; no sequence vtable is emitted by this caller
    unsigned char nativePrefix[0x18];
    CThingCreatureBase* Creature; // native +0x1c (donor base header has a 4-byte difference)
    CThingCreatureBase* Target;               // native +0x20
    CThingCreatureBase& GetCreature()const{return *Creature;}
    CThingCreatureBase& GetTarget()const{return *Target;}
    void AddAction(const CCountedPointer& action); // 0x008B0B10
    void OnCreate(); // non-virtual direct-call adapter for the real base slot at 0x008B0650
};
struct CCombatSequence_VillagerComboUnblockable : CCombatSequenceBase {
    void OnCreate(); // selected body; native PDB identifies this as a virtual override
};
#include <stddef.h>
void* operator new(size_t); // 0x00BFEA1A
void operator delete(void*) throw(); // 0x00BFE9BC
void CCombatSequence_VillagerComboUnblockable::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_InterruptableMidAttack(*Creature, reinterpret_cast<CThing&>(*Target));
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_InterruptableMidAttack(*Creature, reinterpret_cast<CThing&>(*Target));
    CCombatSequenceBase::AddAction(action);
    action = new CCreatureAction_VillagerAttackKnockdown(GetCreature(),GetTarget(),false);
    CCombatSequenceBase::AddAction(action);
    CCombatSequenceBase::OnCreate();
}