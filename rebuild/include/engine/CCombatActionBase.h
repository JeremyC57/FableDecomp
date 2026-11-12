#ifndef FABLE_ENGINE_CCOMBATACTIONBASE_H
#define FABLE_ENGINE_CCOMBATACTIONBASE_H
#if defined(FABLE_RETAIL_LEADER_INITIALISE) || defined(FABLE_RETAIL_KRAKEN_ROAR) || defined(FABLE_RETAIL_BRIAR_ROSE) || defined(FABLE_RETAIL_JACK_STAGE1_UPDATE)
#include "engine/CCreatureActionBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
class CCombatManager;class CMeshDataBank;class CMeleeCombatAbilityDef;class CThingGameObject;
class CCombatActionBase:public CCreatureActionBase {
public:class CStrikeEvent;
public:
 CCombatActionBase(const CCombatActionBase & p0);
protected:
 CCombatActionBase(long p0);
public:
 CCombatActionBase(CThingCreatureBase & p0, long p1, const CSoundPair & p2);
 CCombatActionBase(CThingCreatureBase & p0, CThing & p1, long p2, const CSoundPair & p3);
 static CCountedPointer<CCombatActionBase> FABLE_FASTCALL GetPCombatActionFromEnum(EActionRegisteredType p0, CThingCreatureBase & p1, CThingCreatureBase & p2);
 virtual void FrameUpdate();
 virtual void UpdateActionRunningTime();
 virtual float GetFrameUpdateTime() const;
 virtual void PerformAction();
 virtual void Initialise();
 virtual long GetAnimationIndex() const;
 virtual float GetTotalTimeSeconds() const;
 virtual long GetAnimationTransitionInTime() const;
 virtual long GetAnimDelayTime() const;
 virtual bool AllowsCombinationAnims() const;
 virtual bool IsActionReadyForInterruption() const;
 virtual bool IsActionReadyForInterruptionByMovement(const C3DVector & p0) const;
 virtual unsigned long GetAnimExtraFlags() const;
 virtual void OnBlowBlocked(bool p0);
 float GetAnimMovementDistance() const;
 virtual C3DVector GetNextPos() const;
 bool IsThingInStruckThingList(const CThingGameObject & p0) const;
 virtual bool IsStrikeAction() const;
 virtual bool IsStrikeEvent(long p0) const;
 virtual bool IsStrikeEventActive() const;
 virtual bool IsStrikeEventFinishedBeingActive() const;
 virtual bool IsToUseRotationInAnimation() const;
 virtual bool IsToAlignWithGroundNormal() const;
 virtual bool IsToFaceTarget() const;
 virtual bool IsToFaceTargetAtStart() const;
 virtual C3DVector GetActionMovementVector() const;
 virtual bool IsToPerformActionOnStrike() const;
 virtual bool IsToHitProneThings() const;
 virtual float GetIsToFaceTargetCone() const;
 virtual float GetIsToFaceTargetMinDist() const;
 CThing * GetWeaponUsedInStrikeAction() const;
 virtual CCharString GetStrikeEventHelperName() const;
 void SetCombatAnimation(const std::vector<CAnimationEntry const *,std::allocator<CAnimationEntry const *> > & p0);
 void SetCombatAnimation(const CCharString & p0);
 const CThing * GetTarget() const;
 void SetTarget(CThing & p0);
 virtual float GetDamageMultiplier() const;
protected:
 virtual CCreatureActionBase * Clone() const;
 const CMeshDataBank * PeekMeshBank() const;
 void UpdateFacingDirection();
 void InitialiseActionTimes();
 CCombatManager & GetCombatManager();
 const CCombatManager & GetCombatManager() const;
 void PerformPossibleDamageAction(float p0);
 void ProcessPossibleCreatureStrikeEvent(float p0, const std::pair<C3DVector,C3DVector> & p1, const std::pair<C3DVector,C3DVector> & p2, CThingGameObject & p3);
 void ProcessPossibleObjectStrikeEvent(float p0, const std::pair<C3DVector,C3DVector> & p1, const std::pair<C3DVector,C3DVector> & p2, CThingGameObject & p3);
 void ProcessPossibleShotStrikeEvent(float p0, const std::pair<C3DVector,C3DVector> & p1, const std::pair<C3DVector,C3DVector> & p2, CThingGameObject & p3);
 void ProcessPossibleProneThingsStrikeEvent(float p0, const std::pair<C3DVector,C3DVector> & p1, const std::pair<C3DVector,C3DVector> & p2, CThingGameObject & p3);
 void SetForcedMoveToPos(const C3DVector & p0);
 void ProcessStrikeEvents();
 bool IsStrikeActionHitThing() const;
 bool IsStrikeActionHitCreature() const;
 virtual void UpdateActionMoveToPos();
 virtual bool IsVitalStrike() const;
 virtual bool IsKnockdownStrike() const;
 virtual bool IsBlockCounter() const;
 virtual bool IsMeleeFlourish() const;
 virtual bool IsDecapitatingAttack() const;
 virtual bool IsBlockable() const;
 virtual bool IsUsingAnimationMovement() const;
 virtual bool IsMovementUsingPhysics() const;
 virtual void InitialiseActionMovement();
 virtual void FinishAction();
 static CSoundPair FABLE_FASTCALL GetSoundPair(const CThingCreatureBase & p0, const CCharString & p1);
 float GetTimeThroughCurrentAnimation() const;
 float GetLastTimeThroughCurrentAnimation() const;
 void ClearCombatActionVariables();
 void SetUpCombatAnimationNotes();
 void SetupCombatStrikeEvents();
 void SetupCombatInterrupts();
 void ProcessPlayerControl();
 virtual CTCWeaponTrail * GetWeaponTrailUsedInStrikeAction(long & p0) const;
 virtual bool IsActionWithStrikeEventHelperName() const;
 CCombatActionBase::CStrikeEvent * GetPActiveStrikeEvent();
 const CCombatActionBase::CStrikeEvent * GetPActiveStrikeEvent() const;
 long GetLastStrikeEventEndTime() const;
 void SetupCombatAnimation();
 void ResetMeleeCombatAbilityEffects();
 void SetPMeleeAbility(CDefPointer<CMeleeCombatAbilityDef const > p0);
 CDefPointer<CMeleeCombatAbilityDef const > GetPMeleeAbility() const;
 virtual void OnAnimationLoaded();
 CIntelligentPointer<CThing> PTarget;
 CIntelligentPointer<CThing const > PCameraPoint;
 CDefPointer<CMeleeCombatAbilityDef const > PMeleeAbilityDef;
 std::list<CCountedPointer<CCombatActionBase::CStrikeEvent>,std::allocator<CCountedPointer<CCombatActionBase::CStrikeEvent> > > StrikeEvents;
 long LastStrikeEventEndTime;
 float StrikeEventInterruptTime;
 float StrikeEventResetComboTime;
 long MovementEventInterruptFrame;
 float StrikeSpeedVariationStartTime;
 float StrikeSpeedVariationEndTime;
 bool HasStrikeEventInterrupt;
 bool HasStrikeEventResetCombo;
 bool HasMovementEventInterrupt;
 bool StrikeEventInterruptActive;
 bool MovementEventInterruptActive;
 bool PreferredFinalPosSet;
 bool HasPerformAction;
 bool SentMissedEvent;
 bool HasResetMeleeCombatAbilityEffects;
 long LastAnimStage;
 bool HitThing;
 bool HitCreature;
 long FrameUpdatePauseFrames;
 std::vector<CAnimationEntry const *,std::allocator<CAnimationEntry const *> > CombatAnimations;
 long ActiveStrikeEventIndex;
 CIntelligentPointer<CThing> LastWeaponUsedInStrike;
private:
 C3DVector PreferredMovementVector;
public:
 virtual ~CCombatActionBase();
 CCombatActionBase & operator=(const CCombatActionBase & p0);
};
FABLE_STATIC_ASSERT(sizeof(CCombatActionBase)==276);
#else
#include "engine/CCreatureActionBase.h"
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
// Retail opt-in (chunk 796, row 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover): Ego_r.pdb
// ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (out of line, retail 0x00858030; the inline
// CCreatureAction_DragonHoverOutOf(CThingCreatureBase&) ctor calls it with 250 and the "SWORD_SWING" pair) and the protected static
// ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z (__fastcall, hidden result, 0x008561A0).
// Declarations only, in both class branches; no layout change.
class CThingCreatureBase; class CSoundPair; class CCharString;
#endif
// Ego_r0x37dee; native base is0x114 after STLport list/vector reductions.
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
// Chunk 764 (row 0x00859660 ResetMeleeCombatAbilityEffects): the Ego_r.pdb members from +0xa8 with retail STLport sizes (Ego_r PTarget
// +0xb4 = retail +0xa8: CCreatureActionBase is 0xc smaller; StrikeEvents list 4 not 0xc, CombatAnimations vector 0xc not 0x10). Row
// evidence: PTarget +0xa8 (IS_THING_ALIVE / GetPItem), PMeleeAbilityDef +0xb8, HasResetMeleeCombatAbilityEffects +0xe0; size 0x114.
// Needs engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), engine/CDefPointer.h, engine/CCountedPointer.h,
// stlport/fable_containers.h and engine/C3DVector.h first.
class CThing;class CMeleeCombatAbilityDef;class CAnimationEntry;
class CCombatActionBase : public CCreatureActionBase {
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
public: CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
protected: static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);   // 0x008561A0
#endif
public: virtual ~CCombatActionBase();
    virtual void PerformAction();
protected: virtual CCreatureActionBase* Clone() const;
public: class CStrikeEvent;
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
    // Chunk 764 (row 0x0096DBC0): Ego_r.pdb ?Initialise@CCombatActionBase@@UAEXXZ (0x00857520, called directly by the derived
    // Initialise) and ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z (0x008574D0).
public:
    virtual void Initialise();                                           // 0x00857520
    void SetCombatAnimation(const CCharString& name);                    // 0x008574D0
#endif
protected:
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
    virtual void FinishAction();
#endif
    void ResetMeleeCombatAbilityEffects();                               // 0x00859660
    CIntelligentPointer<CThing> PTarget;                                 // +0xa8
    CIntelligentPointer<const CThing> PCameraPoint;                      // +0xb0
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;          // +0xb8
    std::list<CCountedPointer<CStrikeEvent> > StrikeEvents;              // +0xbc
    long LastStrikeEventEndTime;                                         // +0xc0
    float StrikeEventInterruptTime,StrikeEventResetComboTime;            // +0xc4
    long MovementEventInterruptFrame;                                    // +0xcc
    float StrikeSpeedVariationStartTime,StrikeSpeedVariationEndTime;     // +0xd0
    bool HasStrikeEventInterrupt,HasStrikeEventResetCombo,HasMovementEventInterrupt,StrikeEventInterruptActive,
         MovementEventInterruptActive,PreferredFinalPosSet,HasPerformAction,SentMissedEvent,
         HasResetMeleeCombatAbilityEffects;                              // +0xd8 .. +0xe0
    long LastAnimStage;                                                  // +0xe4
    bool HitThing,HitCreature;                                           // +0xe8
    long FrameUpdatePauseFrames;                                         // +0xec
    std::vector<const CAnimationEntry*> CombatAnimations;                // +0xf0
    long ActiveStrikeEventIndex;                                         // +0xfc
    CIntelligentPointer<CThing> LastWeaponUsedInStrike;                  // +0x100
    C3DVector PreferredMovementVector;                                   // +0x108
};
#else
class CCombatActionBase : public CCreatureActionBase {
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
public: CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
protected: static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);   // 0x008561A0
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_STRIKEEVENT
    // Per-TU opt-in (agentFAM01 2026-11-11, row 0x0062D9A0 ~CCountedPointer<CCombatActionBase::CStrikeEvent>): the PDB nested class
    // (Ego_r.pdb / FableWin.pdb ??1?$CCountedPointer@VCStrikeEvent@CCombatActionBase@@@@QAE@XZ), declared as in the other branches.
    // Declaration only; no layout change.
public: class CStrikeEvent;
#endif
public: virtual ~CCombatActionBase();
    virtual void PerformAction();
protected: virtual CCreatureActionBase* Clone() const;
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS
    // Retail opt-in (wf8, row 0x008C61C0 CCreatureAction_TrollWhackGroundBase::WhackGround): the eleven virtuals CCombatActionBase
    // adds to CCreatureActionBase's seventy (Ego_r.pdb class record, the names and order of the PDB branch above), so a derived
    // class's own virtuals land on their retail slots (vtable 0x0128E0EC: slot 81 InitAnimation, slot 82 GetEffectName, called as
    // `call [edx+148h]` at 0x008C6202). Declarations only; no layout change.
public:
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
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
    // Nested opt-in (chunk 854, row 0x00924C70: the inline CCreatureAction_KrakenReturnToWater ctor): Ego_r.pdb
    // ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z (0x008574D0), the declaration the MELEE branch has under _INITIALISE.
public: void SetCombatAnimation(const CCharString& name);              // 0x008574D0
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER
public:
    virtual void Initialise();
    void SetCombatAnimation(const CCharString&);
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_GETPCOMBATACTIONFROMENUM
    // Per-TU opt-in (wf7, row 0x008B0B60 CCombatSequenceGeneric::OnCreateSpecial): the static factory, as the first branch declares it
    // (Ego_r.pdb ?GetPCombatActionFromEnum@CCombatActionBase@@SI?AV?$CCountedPointer@VCCombatActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@1@Z,
    // landed 0x00857430). Declaration only; needs engine/CCountedPointer.h and engine/EActionRegisteredType.h first.
public:
    static CCountedPointer<CCombatActionBase> FABLE_FASTCALL GetPCombatActionFromEnum(EActionRegisteredType type, CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x00857430
#endif
protected: unsigned char _pad_0xa8[0x6c];
};
#endif
FABLE_STATIC_ASSERT(sizeof(CCombatActionBase)==0x114);
#endif
#endif
