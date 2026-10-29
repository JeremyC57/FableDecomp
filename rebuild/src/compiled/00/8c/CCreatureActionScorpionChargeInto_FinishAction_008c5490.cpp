#define CCreatureAction_ScorpionChargeInto CCreatureAction_ScorpionChargeIntoPdb
#include "engine/CCreatureAction_ScorpionChargeInto.h"
#undef CCreatureAction_ScorpionChargeInto
#define CCreatureAction_ScorpionCharge CCreatureAction_ScorpionChargePdb
#include "engine/CCreatureAction_ScorpionCharge.h"
#undef CCreatureAction_ScorpionCharge
// CCreatureAction_ScorpionChargeInto::FinishAction @008C5490,280 bytes.
// Releases shield reaction exemption, kills obstruction, and transitions to charge if target survives.
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
class CThing;class CThingCreatureBase;class CCreatureActionBase;class CTCWeaponTrail;
class C3DVector { public:float x,y,z;};
class CSoundPair { public:long first,last;CSoundPair():first(0),last(0){}};
struct CBaseObjectPointer {CBaseObject* object;unsigned long refcount;};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer();
 CBaseObjectPointer* PData;
 CBaseIntelligentPointer();
 CBaseObject* GetPItem()const;
 void SetPItem(CBaseObject*);
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer { public:
 virtual ~CIntelligentPointer();
 CIntelligentPointer():CBaseIntelligentPointer(){}
 T* operator->()const{return static_cast<T*>(GetPItem());}
 T& operator*()const{return *static_cast<T*>(GetPItem());}
 operator T*()const{return static_cast<T*>(GetPItem());}
 bool IsValid()const{return GetPItem()&&!(static_cast<T*>(GetPItem())->DeadFlags&1);}
};
// Retail DeadFlags91 vs donor ThingStateA1: actual PDB containers add16 bytes before this flag.
// Creature view adds only the real native SetCurrentAction API; no additional fields touched here.
class CTCPhysicsBase;struct CDefLookup;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry *Start,*Finish,*End;
    Entry* LowerBound(const ETCInterfaceType& key); //40F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing:public CBaseObject { public:
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
    void Kill(bool); //004C9B80
    template<class T> __forceinline bool GetInterface(T** out,long key) {
        if(UsingComponents[key>>5]&(1UL<<(key&31))) {
            ETCInterfaceType type = (ETCInterfaceType)(key);
            CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(type);
            if(it==map->Finish || key<it->first) it=map->Finish;
            *out=(T*)((void*)it->second);return true;
        }
        return false;
    }
};
class CThingCreatureBase:public CThing { public:bool SetCurrentAction(const CCreatureActionBase&);}; //006644F0
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
    unsigned char    DependentThings[0xc];
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
 CCombatActionBase(CThingCreatureBase*,CThing*,long,const CSoundPair&); //00857F30
 CCombatActionBase(CThingCreatureBase*,long,const CSoundPair&); //00858030
};
class CCreatureAction_ScorpionCharge:public CCombatActionBase { public:
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsBlockable() const;
    virtual void FrameUpdate();
    virtual bool IsToFaceMovementDirection() const;
    virtual bool IsToRetainSafeDistance() const;
 float ChargeDistance;C3DVector StartPos;
 CCreatureAction_ScorpionCharge(CThingCreatureBase&,CThing&,float); //native0x8c4ea0
};
class CCreatureAction_ScorpionChargeInto:public CCombatActionBase { public:
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsBlockable() const;
    virtual bool IsToFaceMovementDirection() const;
    virtual bool IsToRetainSafeDistance() const;
 float ChargeDistance;CIntelligentPointer<CThing> PObstruction;
 CCreatureAction_ScorpionChargeInto(CThingCreatureBase&,CThing&,float); //native0x8c4e20
};
class CTCPhysicalShieldSpell { public:void AllowReactionsOnHitFromAttacker(CThing*,bool);}; //0079E580; PDB007A3910, set at48
void CCreatureAction_ScorpionChargeInto::FinishAction() {
 CCombatActionBase::FinishAction();
 if(PTarget.IsValid()) {
  CTCPhysicalShieldSpell* shield;
  if(PTarget->GetInterface(&shield,0x93))shield->AllowReactionsOnHitFromAttacker(PCreature,false);
 }
 if(PObstruction.IsValid())PObstruction->Kill(true);
 if(PTarget.IsValid())PCreature->SetCurrentAction(CCreatureAction_ScorpionCharge(*PCreature,*PTarget,ChargeDistance));
}