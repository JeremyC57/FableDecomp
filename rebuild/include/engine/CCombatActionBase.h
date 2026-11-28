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
#ifdef FABLE_RETAIL_INTERRUPTABLE_AUTOTURN_PASS30
protected:
 CDefPointer<const CMeleeCombatAbilityDef> GetPMeleeAbility() const { return PMeleeAbilityDef; }
public:
 CCountedPointer<CAnimationEntry>& GetPAnimationEntry() { return PAnimEntry; }
#else
 CDefPointer<CMeleeCombatAbilityDef const > GetPMeleeAbility() const;
#endif
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
FABLE_STATIC_ASSERT_AT(133, sizeof(CCombatActionBase)==276);
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
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
    // Retail opt-in (NF39, row 0x008C4E20 the CCreatureAction_ScorpionChargeInto constructor): Ego_r.pdb
    // ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@AAVCThing@@JABVCSoundPair@@@Z (creature, target, priority, sound), out of line in
    // retail at 0x00857F30 (the first branch above declares the same constructor). Declaration only; no layout change.
public: CCombatActionBase(CThingCreatureBase& creature, CThing& target, long priority, const CSoundPair& sound);   // 0x00857F30
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
    // NF148 opt-in (2026-11-20, q11785, row 0x00956960 CCreatureAction_ThrowShotObject::Clone): Ego_r.pdb 0x00545FD0 + FableWin.pdb
    // 0x01D919A0 ??0CCombatActionBase@@QAE@ABV0@@Z, out of line in retail at 0x0062F3B0 (the derived classes' implicit copy constructors
    // call it, then store their own vftable). The first branch above declares it too. Declaration only; no layout change.
public: CCombatActionBase(const CCombatActionBase& other);               // 0x0062F3B0
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_PRIORITYCTOR
    // NF257 opt-in (2026-11-22, q12722, row 0x008B83C0 CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures()): Ego_r.pdb 0x00838AC0
    // + FableWin.pdb 0x02823960 ??0CCombatActionBase@@IAE@J@Z (protected), out of line in retail at 0x00857E40 (the third branch's PRIORITYCTOR
    // declares the same constructor). Declaration only; no layout change.
protected: CCombatActionBase(long priority);                             // 0x00857E40
#endif
public: virtual ~CCombatActionBase();
    virtual void PerformAction();
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SCREAMER_VTABLE_PASS27
    // 0096DF50: native temporary's vtable retains these base overrides. Both donor
    // PDBs give the actual access and signatures. These are declarations only.
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual bool IsStrikeEvent(long) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual CCharString GetStrikeEventHelperName() const;
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector&) const;
    virtual float GetTotalTimeSeconds() const;
    virtual void OnBlowBlocked(bool);
    virtual void UpdateActionRunningTime();
    virtual float GetFrameUpdateTime() const;
    virtual bool AllowsCombinationAnims() const;
protected:
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual bool IsBlockable() const;
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long&) const;
    virtual bool IsDecapitatingAttack() const;
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
public:
#endif
#ifdef FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
    // Original virtual overrides, in the native 81-entry action vtable.
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual bool IsStrikeEvent(long) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual CCharString GetStrikeEventHelperName() const;
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector&) const;
    virtual float GetTotalTimeSeconds() const;
    virtual void OnBlowBlocked(bool);
    virtual void UpdateActionRunningTime();
    virtual float GetFrameUpdateTime() const;
    virtual bool AllowsCombinationAnims() const;
protected:
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual bool IsBlockable() const;
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long&) const;
    virtual bool IsDecapitatingAttack() const;
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
#endif
protected: virtual CCreatureActionBase* Clone() const;
public: class CStrikeEvent;
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
    // NF75 opt-in (round 7, land1117fu q10916, rows 0x0085BAD0 / 0x008E6390): Ego_r.pdb 0x0083A360 / FableWin.pdb 0x0282A4B0
    // ?FrameUpdate@CCombatActionBase@@UAEXXZ (public virtual, over the protected CCreatureActionBase::FrameUpdate, slot 56), out of line in
    // retail at 0x00859D80; derived FrameUpdate overrides call it directly (tail `jmp`). Declaration only; no layout change.
public: virtual void FrameUpdate();                                      // 0x00859D80
#endif
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
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
    // NF175 opt-in (q11895 / q11896, rows 0x00956720 / 0x00956730): the eleven virtuals CCombatActionBase adds to CCreatureActionBase's
    // seventy, in the MELEE branch (the same Ego_r.pdb class-record names and order as the other branch's _OWN_VIRTUALS), declared after
    // every other virtual of this class so they take slots 70..80 (retail vftable 0x012944D4 of CCreatureAction_ThrowShotObject has 81
    // slots; NF148 NOTES). Declarations only; no layout change.
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
protected:
#endif
#if defined(FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS) && !defined(FABLE_RETAIL_CCOMBATACTIONBASE_SCREAMER_VTABLE_PASS27) && !defined(FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27)
    // NF264 opt-in (2026-11-22, q12750, row 0x0062EA10 and the vtable emitters): FableWin.pdb 0x01D916B0
    // ?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ (public virtual const: CCombatActionBase's override of the protected
    // CCreatureActionBase slot 66) + retail CCombatActionBase vftable 0x01259E44 slot 66 = 0x0062EA10 (body `xor al,al ; ret`), so the
    // tables of classes that do not override it bind slot 66 to 0x0062EA10, not to CCreatureActionBase's 0x00692890. Both class branches;
    // the PASS27 blocks above already declare it. Declaration only; no layout change; the access before and after is protected.
public:
    virtual bool AllowsCombinationAnims() const;                         // 0x0062EA10
protected:
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
    // p25E opt-in (row 0x00859EF0 CCombatActionBase::OnAnimationLoaded): the protected members it uses, with the Ego_r.pdb / FableWin.pdb
    // spellings the first class branch declares (Ego_r ?UpdateFacingDirection / ?InitialiseActionTimes / ?SetUpCombatAnimationNotes /
    // ?SetupCombatStrikeEvents / ?SetupCombatInterrupts@CCombatActionBase@@IAEXXZ, ?OnAnimationLoaded@CCombatActionBase@@MAEXXZ, the
    // override of CCreatureActionBase's virtual). Out of line in retail at 0x00856B20 / 0x00856A20 / 0x00859EC0 (expanded by that row: its
    // same-TU definition) / 0x00859A70 / 0x00858A20. Declarations only; no layout change.
    void UpdateFacingDirection();                                        // 0x00856B20
    void InitialiseActionTimes();                                        // 0x00856A20
    void SetUpCombatAnimationNotes();                                    // 0x00859EC0
    void SetupCombatStrikeEvents();                                      // 0x00859A70
    void SetupCombatInterrupts();                                        // 0x00858A20
    virtual void OnAnimationLoaded();                                    // 0x00859EF0
#endif
#ifdef FABLE_RETAIL_COMBAT_NEXT_POS_PASS27
protected:
    const CMeshDataBank* PeekMeshBank() const;
    float GetTimeThroughCurrentAnimation() const;
    float GetLastTimeThroughCurrentAnimation() const;
#endif
#ifdef FABLE_RETAIL_DAMAGE_ACTION_PASS29
    void PerformPossibleDamageAction(float);
    void ProcessPossibleCreatureStrikeEvent(float,const std::pair<C3DVector,C3DVector>&,const std::pair<C3DVector,C3DVector>&,CThingGameObject&);
    void ProcessPossibleProneThingsStrikeEvent(float,const std::pair<C3DVector,C3DVector>&,const std::pair<C3DVector,C3DVector>&,CThingGameObject&);
    void ProcessPossibleShotStrikeEvent(float,const std::pair<C3DVector,C3DVector>&,const std::pair<C3DVector,C3DVector>&,CThingGameObject&);
    void ProcessPossibleObjectStrikeEvent(float,const std::pair<C3DVector,C3DVector>&,const std::pair<C3DVector,C3DVector>&,CThingGameObject&);
#endif
    void ResetMeleeCombatAbilityEffects();                               // 0x00859660
    CIntelligentPointer<CThing> PTarget;                                 // +0xa8
#ifdef FABLE_RETAIL_ACTION_EVENT_SHOT_PASS30
public:
    // FableWin 0x02838020 expands the intelligent pointer at native +0xA8.
    const CThing* GetTarget() const { return PTarget.Get(); }
    CThing* GetWeaponUsedInStrikeAction() const;  // 0x00857070
protected:
#endif
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
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
    // Retail opt-in (wfE11, row 0x006F5C10): Ego_r.pdb ??0CCombatActionBase@@IAE@J@Z (protected), out of line in retail at 0x00857E40;
    // the derived action constructors expanded in CCreatureActionBase::StatRegisterClass call it with 50 / 150 / 250. Declaration only.
protected: CCombatActionBase(long priority);                             // 0x00857E40
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
public: CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
protected: static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);   // 0x008561A0
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
    // Retail opt-in (NF39, row 0x008C4E20 the CCreatureAction_ScorpionChargeInto constructor): Ego_r.pdb
    // ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@AAVCThing@@JABVCSoundPair@@@Z (creature, target, priority, sound), out of line in
    // retail at 0x00857F30 (the first branch above declares the same constructor). Declaration only; no layout change.
public: CCombatActionBase(CThingCreatureBase& creature, CThing& target, long priority, const CSoundPair& sound);   // 0x00857F30
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
#ifdef FABLE_RETAIL_JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27
    // Original overrides from the first CCombatActionBase model. They occupy existing slots in the
    // Forward and HoverOutOf tables at retail 0x012940E4 and 0x01293D0C.
public:
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual bool IsStrikeEvent(long event) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual CCharString GetStrikeEventHelperName() const;
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector& position) const;
    virtual float GetTotalTimeSeconds() const;
    virtual void OnBlowBlocked(bool blocked);
    virtual void UpdateActionRunningTime();
    virtual float GetFrameUpdateTime() const;
    virtual bool AllowsCombinationAnims() const;
protected:
    virtual void FinishAction();
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual bool IsBlockable() const;
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& trail) const;
    virtual bool IsDecapitatingAttack() const;
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
    virtual void OnAnimationLoaded();
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
    // NF75 opt-in (round 7, land1117fu q10916, rows 0x0085BAD0 / 0x008E6390): Ego_r.pdb 0x0083A360 / FableWin.pdb 0x0282A4B0
    // ?FrameUpdate@CCombatActionBase@@UAEXXZ (public virtual, over the protected CCreatureActionBase::FrameUpdate, slot 56), out of line in
    // retail at 0x00859D80; derived FrameUpdate overrides call it directly (tail `jmp`). Declaration only; no layout change.
public: virtual void FrameUpdate();                                      // 0x00859D80
#endif
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
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SETTARGET
    // Opt-in (NF52, row 0x00857430): Ego_r.pdb ?SetTarget@CCombatActionBase@@QAEXAAVCThing@@@Z (public, as the first class branch declares
    // it), out of line in retail at 0x0085AA90. Declaration only, no layout change.
public:
    void SetTarget(CThing& target);                                    // 0x0085AA90
#endif
#if defined(FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS) && !defined(FABLE_RETAIL_JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27)
    // NF264 opt-in (2026-11-22, q12750): the declaration the MELEE branch has under the same macro (FableWin.pdb 0x01D916B0
    // ?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ + retail vftable 0x01259E44 slot 66 = 0x0062EA10). Declaration only.
public:
    virtual bool AllowsCombinationAnims() const;                         // 0x0062EA10
#endif
protected: unsigned char _pad_0xa8[0x6c];
};
#endif
FABLE_STATIC_ASSERT_AT(245, sizeof(CCombatActionBase)==0x114);
#endif
#endif
