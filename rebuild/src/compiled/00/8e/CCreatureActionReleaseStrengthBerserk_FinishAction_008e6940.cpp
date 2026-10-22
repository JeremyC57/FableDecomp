// CCreatureAction_ReleaseStrengthBerserk::FinishAction -- unlock abilities and transfer living dependents to combat block exemptions.
// Retail008E6940,291bytes. Native component167 is CTCSpecialAbilities (type getter72D9F0), not CTCBerserk.
#define CCreatureAction_ReleaseStrengthBerserk CCreatureAction_ReleaseStrengthBerserkPdb
#include "engine/CCreatureAction_ReleaseStrengthBerserk.h"
#undef CCreatureAction_ReleaseStrengthBerserk
#define CTCCombat CTCCombatPdb
#include "engine/CTCCombat.h"
#undef CTCCombat
// Native combat action projection: base70 genuine slots; combat adds11 =81 slots.
// BaseA8 and Combat114; PDB baseB4/Combat12C. See actual headers and raw vtable audit.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBasePdb
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CThing CThingPdb
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/_quarantine/CThing.h"
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#undef CThing
#include "engine/CBaseObject.h"
#include "fable_string.h"
struct CThing;struct CThingCreatureBase;struct CCreatureActionBase;struct CTCWeaponTrail;
struct C3DVector {float x,y,z;};
struct CSoundPair {long first,last;CSoundPair():first(0),last(0){}};
struct CBaseObjectPointer {CBaseObject* object;unsigned long refcount;};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer();
 CBaseObjectPointer* PData;
 CBaseIntelligentPointer();
 CBaseObject* GetPItem()const;
 void SetPItem(CBaseObject*);
};
template<class T>struct CIntelligentPointer:CBaseIntelligentPointer {
 virtual ~CIntelligentPointer();
 CIntelligentPointer():CBaseIntelligentPointer(){}
 explicit CIntelligentPointer(T* p):CBaseIntelligentPointer(){SetPItem(p);}
 T* operator->()const{return static_cast<T*>(GetPItem());}
 T& operator*()const{return *static_cast<T*>(GetPItem());}
 operator T*()const{return static_cast<T*>(GetPItem());}
 bool IsValid()const{return GetPItem()&&!(static_cast<T*>(GetPItem())->DeadFlags&1);}
};
// Retail DeadFlags91 vs donor ThingStateA1: actual PDB containers add16 bytes before this flag.
// Creature view adds only the real native SetCurrentAction API; no additional fields touched here.
struct CTCPhysicsBase;struct CDefLookup;
struct CInterfaceMap {
    struct Entry { long type; void* component; };
    Entry *Start,*Finish,*End;
    Entry* LowerBound(const long*); //40F020
};
struct CThing:CBaseObject {
    unsigned char BaseState[0x18];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char UpdateComponentsList[8];
    void* SerialisationSectionName;
    void* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    void* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefLookup* PBaseDef;
    unsigned char LaterState[0x1d];
    unsigned char DeadFlags;
    const C3DVector& GetPosition()const; //004C73D0
    void Kill(bool); //004C9B80
    template<class T> __forceinline bool GetInterface(T** out,long key) {
        if(UsingComponents[key>>5]&(1UL<<(key&31))) {
            long type=key;
            CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(&type);
            if(it==map->Finish || key<it->type) it=map->Finish;
            *out=(T*)it->component;return true;
        }
        return false;
    }
};
struct CThingCreatureBase:CThing {bool SetCurrentAction(const CCreatureActionBase&);}; //006644F0
struct IntelligentThingVector {CIntelligentPointer<CThing> *Start,*Finish,*End;void PushBack(const CIntelligentPointer<CThing>&);CIntelligentPointer<CThing>* Erase(CIntelligentPointer<CThing>*,CIntelligentPointer<CThing>*);}; //004AD520,004AC0F0
struct __declspec(novtable) CCreatureActionBase {
    virtual ~CCreatureActionBase();
    virtual void Validate() const;
    virtual void EditUpdate(float elapsed);
    virtual void PerformAction() = 0;
    virtual void FinishAction();
    virtual bool IsPlayingAnimation() const;
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual long GetAnimationTransitionOutTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual CCreatureActionBase* Clone() const = 0;
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
    unsigned char    _pad_0x04[0x4];
    CIntelligentPointer<CThingCreatureBase> PCreature;
    long             TotalTime;
    long             ActionTime;
    long             AnimDelayTime;
    long             FramesRunning;
    long             InterruptionGroupID;
    long             InterruptionPriority;
    long             ActionID;
    unsigned char    PItemRemoved[0x8];
    long             Sounds_FirstSound;
    long             Sounds_LastSound;
    unsigned long    PlayingSoundID;
    float            RunningTimeSeconds;
    float            LastRunningTimeSeconds;
    long             StartFrame;
    long             PauseFrame;
    long             PauseTimer;
    float            AnimMovementStep_X;
    float            AnimMovementStep_Y;
    float            AnimMovementStep_Z;
    bool             AddedActionAnim;
    bool             Finished;
    bool             PerformedAction;
    bool             Initialised;
    long             NextMovementPosWorldFrame;
    float            NextMovementPos_X;
    float            NextMovementPos_Y;
    float            NextMovementPos_Z;
    CAnimationEntry* PAnimEntry_Data;
    CCPPointerInfo*  PAnimEntry_Info;
    CAnimationEntry* PPreceedingPAnimEntry_Data;
    CCPPointerInfo*  PPreceedingPAnimEntry_Info;
    unsigned char    ActionEvents[0xc];
    unsigned char    CutSceneEvents[0xc];
    IntelligentThingVector DependentThings;
    CCreatureActionBase(const CCreatureActionBase& other); // native copy ctor @0x0062EBC0
    CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long first, long second); // native @0x00693C70
};
struct __declspec(novtable) CCombatActionBase:CCreatureActionBase {
    virtual ~CCombatActionBase();
    virtual void PerformAction();
    virtual void FinishAction();
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
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
    virtual void FrameUpdate();
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
    virtual float GetFrameUpdateTime() const;
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
 CIntelligentPointer<CThing> PTarget;
 CIntelligentPointer<const CThing> PCameraPoint;
 CDefPointeeBase* PMeleeAbilityDef;
 void* StrikeEvents;
 long LastStrikeEventEndTime;float StrikeEventInterruptTime,StrikeEventResetComboTime;
 long MovementEventInterruptFrame;float StrikeSpeedVariationStartTime,StrikeSpeedVariationEndTime;
 bool HasStrikeEventInterrupt,HasStrikeEventResetCombo,HasMovementEventInterrupt,StrikeEventInterruptActive,MovementEventInterruptActive,PreferredFinalPosSet,HasPerformAction,SentMissedEvent,HasResetMeleeCombatAbilityEffects;
 long LastAnimStage;bool HitThing,HitCreature;long FrameUpdatePauseFrames;
 struct AnimationVector {CAnimationEntry** First;CAnimationEntry** Last;CAnimationEntry** End;} CombatAnimations;
 long ActiveStrikeEventIndex;CIntelligentPointer<CThing> LastWeaponUsedInStrike;C3DVector PreferredMovementVector;
 void SetAnimationName(const CCharString&); //008574D0
 CCombatActionBase(CThingCreatureBase*,CThing*,long,const CSoundPair&); //00857F30
 CCombatActionBase(CThingCreatureBase*,long,const CSoundPair&); //00858030
};
// Native first fields agree with PDB CTCSpecialAbilities; CurrentAbility is EHeroAbility, not float.
struct CTCSpecialAbilities {void* VPtr;CThing* PThing;long State;bool AbilityMutexLock,UsingBed;long CurrentAbility;};
// CTCCombat donor AlwaysBlockThings+5C is native+4C; earlier native combat state and vector layouts differ.
struct CTCCombat {void* VPtr;CThing* PThing;long State;unsigned char CombatState[0x40];IntelligentThingVector AlwaysBlockThings;};
struct CCreatureAction_ReleaseStrengthBerserk:CCombatActionBase {
    virtual CCreatureActionBase* Clone()const;
    virtual void Initialise();
    virtual void PerformAction();
    virtual void FinishAction();
    virtual void FrameUpdate();
    virtual bool IsToCauseRecoil()const;
    virtual bool IsAbleToBeAttacked()const;
 CCreatureAction_ReleaseStrengthBerserk(CThingCreatureBase*); //008E68E0 base858030(actor,145,sounds0)
};
void CCreatureAction_ReleaseStrengthBerserk::FinishAction(){
 CTCSpecialAbilities* abilities;
 if(PCreature->GetInterface(&abilities,167)){abilities->AbilityMutexLock=false;abilities->CurrentAbility=0;}
 CTCCombat* combat;
 if(PCreature->GetInterface(&combat,132)){
  IntelligentThingVector& dependents=DependentThings;
  for(CIntelligentPointer<CThing>* it=dependents.Start;it!=dependents.Finish;++it){
   CThing* thing=*it;
   if(thing&&!(thing->DeadFlags&1)){CIntelligentPointer<CThing> held(thing);combat->AlwaysBlockThings.PushBack(held);}
  }
  dependents.Erase(dependents.Start,dependents.Finish);
 }
 CCombatActionBase::FinishAction();
}