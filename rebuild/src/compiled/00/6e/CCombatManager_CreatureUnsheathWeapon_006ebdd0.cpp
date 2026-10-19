// CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase*,long) -- choose immediate or animated unsheathing after action eligibility checks.
// Retail0x006EBDD0,268bytes. Native temporary is UnsheatheWeapons0x118; base dtor0062E900;
// ctor008B85D0, PerformAction008B7BA0, ShouldPlay008B7CF0, incapacitated0085F0C0, knockdown0085F010,
// IsFree00662210, SetCurrentAction006644F0, component lower_bound0040F020. Donor layouts aliased;
// native combat UnsheathingWeaponType+6c vs donor78 and native action tail114 vs donor12c.
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CTCCombat CTCCombat_PDB
#include "engine/CTCCombat.h"
#undef CTCCombat
#define CCreatureAction_UnsheatheWeapons CCreatureAction_UnsheatheWeapons_PDB
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#undef CCreatureAction_UnsheatheWeapons
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#include "fable_string.h"
struct CTCCombat {unsigned char _nativeBeforeWeaponType[0x6c];long UnsheathingWeaponType;};
struct CInterfaceMap {
    struct Entry {long type;CTCCombat* component;};
    Entry* first;Entry* last;
    Entry* LowerBound(const long*); //0040F020
};
struct CThingCreatureBase;
struct CThing;
struct C3DVector { float x, y, z; };
struct CTCWeaponTrail;
struct CCreatureActionBase {
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
struct CSoundPair { long first; long last; CSoundPair() : first(0), last(0) { } };
struct CCombatActionBase : CCreatureActionBase {
    virtual ~CCombatActionBase();
    virtual void PerformAction();
    virtual void FinishAction();
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

struct CThingCreatureBase {
    unsigned char _nativeBeforeUsingComponents[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    bool IsFreeToPerformAction(const CCreatureActionBase&) const; //00662210 accepts native CCreatureActionBase prefix
    bool SetCurrentAction(const CCreatureActionBase&); //006644F0
};
struct CCreatureAction_UnsheatheWeapons : CCombatActionBase {
    long WeaponType;
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase&,long); //008B85D0
    virtual void PerformAction(); //008B7BA0
    virtual void FinishAction(); //008B69D0
    virtual CCreatureActionBase* Clone()const; //008B8AC0
    virtual void Initialise(); //008B7ED0
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_UnsheatheWeapons)==0x118);
extern bool FABLE_FASTCALL ShouldPlayUnsheatheWeaponAction(CThingCreatureBase*,long,bool*,bool*); //008B7CF0
extern bool FABLE_FASTCALL IsIncapacitated(CThingCreatureBase*); //0085F0C0
extern bool FABLE_FASTCALL IsKnockedDown(CThingCreatureBase*); //0085F010
struct CCombatManager {void CreatureUnsheathWeapon(CThingCreatureBase*,long);};
void CCombatManager::CreatureUnsheathWeapon(CThingCreatureBase* creature,long weapon)
{
    bool animate;
    bool secondary=false;
    if(!ShouldPlayUnsheatheWeaponAction(creature,weapon,&animate,&secondary)) return;
    if(IsIncapacitated(creature)) return;
    if(((signed char)creature->UsingComponents[4] & 0x80) != 0) return;
    if(IsKnockedDown(creature)) return;
    if(!(creature->UsingComponents[1]&0x10000)) return;
    CInterfaceMap* map=&creature->Components;
    long type=0x30;
    CInterfaceMap::Entry* found=map->LowerBound(&type);
    if(found==map->last || found->type>0x30) found=map->last;
    CTCCombat* combat=found->component;
    CCreatureAction_UnsheatheWeapons action(*creature,weapon);
    if(creature->IsFreeToPerformAction(action)) {
        if(!animate) {combat->UnsheathingWeaponType=weapon;action.PerformAction();}
        else if(!creature->SetCurrentAction(action)) {
            if(!combat->UnsheathingWeaponType) {combat->UnsheathingWeaponType=weapon;action.PerformAction();}
            else if(combat->UnsheathingWeaponType!=weapon) combat->UnsheathingWeaponType=weapon;
        }
    }
}