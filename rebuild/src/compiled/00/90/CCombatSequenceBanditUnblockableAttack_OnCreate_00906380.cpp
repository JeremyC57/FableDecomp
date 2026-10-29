// CCombatSequence_BanditUnblockableAttack::OnCreate -- enqueue UnblockableMidAttack, LeapBack and CombatIdle actions.
// Native ctor 0x008bfbe0 is CCreatureAction_UnblockableMidAttack (RTTI vtable 0x0127f8a4), followed by LeapBack and CombatIdle.
#define CCombatSequence_BanditUnblockableAttack CCombatSequence_BanditUnblockableAttack_PDB
#include "engine/CCombatSequence_BanditUnblockableAttack.h"
#undef CCombatSequence_BanditUnblockableAttack
#define CCreatureAction_LeapBack CCreatureAction_LeapBack_PDB
#include "engine/CCreatureAction_LeapBack.h"
#undef CCreatureAction_LeapBack
#define CCreatureAction_CombatIdle CCreatureAction_CombatIdle_PDB
#include "engine/CCreatureAction_CombatIdle.h"
#undef CCreatureAction_CombatIdle
#define CCreatureAction_UnblockableMidAttack CCreatureAction_UnblockableMidAttack_PDB
#include "engine/CCreatureAction_UnblockableMidAttack.h"
#undef CCreatureAction_UnblockableMidAttack
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
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
    virtual C3DVector GetNextPos() const;
    virtual bool IsToUseRotationInAnimation() const;
    virtual bool IsToAlignWithGroundNormal() const;
    virtual bool IsToFaceTarget() const;
    virtual bool IsToFaceTargetAtStart() const;
    unsigned char nativeFields[0xb0]; // actual CCreatureActionBase header data after its vptr, native size 0xb4
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
    unsigned char nativeCombatFields[0x60]; // retail extends the native 0xb4 action base to 0x114
    CCombatActionBase(CThingCreatureBase& owner, CThing& target, long actionCode, const CSoundPair& sounds); // 0x00857f30
};
class CCreatureAction_LeapBack : public CCombatActionBase { public:
    virtual ~CCreatureAction_LeapBack();
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsToFaceTarget() const;
    virtual float GetIsToFaceTargetCone() const;
    CCreatureAction_LeapBack(CThingCreatureBase& owner, CThing& target)
        : CCombatActionBase(owner, target, 50, CSoundPair()) { }
};
class CCreatureAction_CombatIdle : public CCombatActionBase { public:
    virtual ~CCreatureAction_CombatIdle();
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsToFaceTargetAtStart() const;
    CCreatureAction_CombatIdle(CThingCreatureBase& owner, CThing& target)
        : CCombatActionBase(owner, target, 50, CSoundPair()) { }
};
class CCreatureAction_UnblockableMidAttack : public CCombatActionBase { public:
    virtual ~CCreatureAction_UnblockableMidAttack();
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockable() const;
    virtual bool IsToFaceMovementDirection() const; // appended native slot 81
    CCreatureAction_UnblockableMidAttack(CThingCreatureBase& owner, CThing& target); // PDB refs, ctor @ 0x008bfbe0
};
struct SCountBlock { long refs; void (__fastcall* destroy)(void*); void* object; };
struct CCountedPointer {
    CCreatureActionBase* object;
    SCountBlock* count;
    CCountedPointer() : object(0), count(0) { }
    CCountedPointer& operator=(CCreatureActionBase* value) { Reset(value); return *this; }
    void Reset(CCreatureActionBase* value); // external CCountedPointer::Reset @ 0x00905fd0
    ~CCountedPointer() {
        if (count) { --count->refs; if (!count->refs) { count->destroy(count->object); delete count; } }
        object = 0; count = 0;
    }
};
class CCombatSequenceBase { public: void OnCreate(); }; // external CCombatSequenceBase::OnCreate @ 0x008b0650
struct CCombatSequence_BanditUnblockableAttack_NativeView : CCombatSequenceBase {
    unsigned char prefix[0x1c];
    CThingCreatureBase* m_owner;
    CThing* m_target;
    void AddAction(const CCountedPointer& action); // external CCombatSequenceBase::AddAction @ 0x008b0b10
};
struct CCombatSequence_BanditUnblockableAttack : CCombatSequence_BanditUnblockableAttack_NativeView {
    void OnCreate();
};
void CCombatSequence_BanditUnblockableAttack::OnCreate() {
    CCountedPointer action;
    action = new CCreatureAction_UnblockableMidAttack(*m_owner, *m_target);
    AddAction(action);
    action = new CCreatureAction_LeapBack(*m_owner, *m_target);
    AddAction(action);
    action = new CCreatureAction_CombatIdle(*m_owner, *m_target);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}