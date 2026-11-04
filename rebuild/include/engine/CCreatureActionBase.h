#ifndef FABLE_ENGINE_CCREATUREACTIONBASE_H
#define FABLE_ENGINE_CCREATUREACTIONBASE_H
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE
// Retail opt-in (Codex pass 11 consolidation 2): the retail CCreatureActionBase model (row 0x00667450 CThingCreatureBase::ReactToGenericStrike); see below.
#include "engine/CBaseObject.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "engine/EActionRegisteredType.h"
class CThing;class CThingCreatureBase;class CTCWeaponTrail;
#include "stlport/fable_map.h"
namespace NCreatureAction { class CActionEventBase; }
// Original Ego_r TPI0x221c9; native factories and calls prove vtable slots40-42.
// Complete native sizea8 after the three retail container reductions.
class CCreatureActionBase : public CBaseObject {
public:
    static CCountedPointer<CCreatureActionBase> __fastcall GetPActionFromEnum(EActionRegisteredType,CThingCreatureBase&);
    // RegisterActionEventTypes 0x006932B0 (Ego_r 0x00519CE0 / FableWin 0x01DB27E0 ?RegisterActionEventTypes@CCreatureActionBase@@SIXXZ).
    static void __fastcall RegisterActionEventTypes();
private:
    // Static members (both PDBs: `dynamic initializer for 'CCreatureActionBase::RegisteredActionEvents'`); retail 0x013BA8BC
    // ?RegisteredActionEvents@CCreatureActionBase@@0V?$map@VCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@...
    // (private static, keyed by GetHook()); DebugEventTrace is the bool 0x006932B0 clears last.
    static std::map<CCharString,CCountedPointer<NCreatureAction::CActionEventBase> > RegisteredActionEvents;
    static bool DebugEventTrace;
public:
public:
    virtual ~CCreatureActionBase();
public:
    virtual void EditUpdate(float);
public:
    virtual void PerformAction() =0;
public:
    virtual void FinishAction();
public:
    virtual bool IsPlayingAnimation() const;
public:
    virtual long GetAnimDelayTime() const;
public:
    virtual long GetAnimationIndex() const;
public:
    virtual long GetAnimationTransitionInTime() const;
public:
    virtual long GetAnimationTransitionOutTime() const;
public:
    virtual unsigned long GetAnimExtraFlags() const;
public:
    virtual CCreatureActionBase * Clone() const =0;
public:
    virtual void Initialise();
public:
    virtual bool IsStrikeEvent(long) const;
public:
    virtual bool IsStrikeEventActive() const;
public:
    virtual bool IsStrikeAction() const;
public:
    virtual bool IsPartOfComboChain() const;
public:
    virtual bool IsStrikeEventFinishedBeingActive() const;
public:
    virtual bool IsVitalStrike() const;
public:
    virtual bool IsKnockdownStrike() const;
public:
    virtual bool IsBlockCounter() const;
public:
    virtual bool IsMeleeFlourish() const;
public:
    virtual bool IsActionWithStrikeEventHelperName() const;
public:
    virtual CCharString GetStrikeEventHelperName() const;
public:
    virtual bool IsOverrideableByActionOfSamePriority() const;
public:
    virtual void OnFrameUpdate(float);
public:
    virtual bool IsQueuable() const;
public:
    virtual bool IsActionReadyForInterruption() const;
public:
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector &) const;
public:
    virtual bool IsBlockable() const;
public:
    virtual bool IsAffectedByHaste() const;
public:
    virtual bool IsAffectedByHeroStats() const;
public:
    virtual bool IsPhysicsPushableByHero() const;
public:
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
public:
    virtual bool IsAlwaysUseAnimationMovement() const;
public:
    virtual bool IsClearingMovementVector() const;
public:
    virtual bool IsToHaveEmptyHands() const;
public:
    virtual bool IsToReplaceItemIfRemoved() const;
public:
    virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const;
public:
    virtual bool IsPreventingQuickAccessItemUsage() const;
public:
    virtual void SetHitter(const CThing &);
public:
    virtual void SetHitDirection(C3DVector);
public:
    virtual void SetHitPosition(C3DVector);
public:
    virtual CTCWeaponTrail * GetWeaponTrailUsedInStrikeAction(long &) const;
public:
    virtual float GetTotalTimeSeconds() const;
public:
    virtual bool IsToEvadeAllHits() const;
public:
    virtual bool IsToCauseRecoil() const;
public:
    virtual bool IsAbleToBeAttacked() const;
public:
    virtual bool IsPlayHitResponse() const;
public:
    virtual bool IsPreventingNormalMovement() const;
public:
    virtual bool IsPreventingLookTurning() const;
public:
    virtual void OnBlowBlocked(bool);
public:
    virtual void OnFollowUpActionFailedToAdd();
public:
    virtual void UpdateActionRunningTime();
public:
    virtual bool IsDecapitatingAttack() const;
public:
    virtual bool PreventSneaking() const;
protected:
    virtual void FrameUpdate();
protected:
    virtual bool IsUsingAnimationMovement() const;
protected:
    virtual bool IsUsingForcedMovement() const;
protected:
    virtual bool IsMovementUsingPhysics() const;
protected:
    virtual void UpdateActionMoveToPos();
protected:
    virtual void InitialiseActionMovement();
protected:
    virtual float GetFrameUpdateTime() const;
protected:
    virtual float GetActionSpeedParameter() const;
protected:
    virtual float GetActionStartRunningTimeSeconds() const;
protected:
    virtual bool OverrideFaceWithDefault() const;
protected:
    virtual bool AllowsCombinationAnims() const;
protected:
    virtual bool CanAnimationTransitionOut() const;
protected:
    virtual void OnAnimationLoaded();
protected:
    virtual bool WaitForMeshToLoad() const;
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
    // chunk 751 (row 0x00854720 CCreatureAction_SpiritReturnToHero::PerformAction): Ego_r.pdb PCreature
    // (CIntelligentPointer<CThingCreatureBase>) at +0x08; the rest stays opaque. Needs engine/CIntelligentPointer.h first.
protected: CIntelligentPointer<CThingCreatureBase> PCreature; unsigned char _pad_0x10[0x98];
#else
protected: unsigned char _pad_0x08[0xa0];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CCreatureActionBase)==0xa8);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CCreatureActionBase = 0xb4.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CCreatureActionBase = 0xa8.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CAnimationEntry;
class CCPPointerInfo;

#pragma pack(push, 1)
class CCreatureActionBase {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    PCreature[0x8];                         // +0x08 CIntelligentPointer<CThingCreatureBase>
    long             TotalTime;                              // +0x10
    long             ActionTime;                             // +0x14
    long             AnimDelayTime;                          // +0x18
    long             FramesRunning;                          // +0x1c
    long             InterruptionGroupID;                    // +0x20
    long             InterruptionPriority;                   // +0x24
    long             ActionID;                               // +0x28
    unsigned char    PItemRemoved[0x8];                      // +0x2c CIntelligentPointer<CThing>
    long             Sounds_FirstSound;                      // +0x34 CSoundPair::FirstSound
    long             Sounds_LastSound;                       // +0x38 CSoundPair::LastSound
    unsigned long    PlayingSoundID;                         // +0x3c
    float            RunningTimeSeconds;                     // +0x40
    float            LastRunningTimeSeconds;                 // +0x44
    long             StartFrame;                             // +0x48
    long             PauseFrame;                             // +0x4c
    long             PauseTimer;                             // +0x50
    float            AnimMovementStep_X;                     // +0x54 C3DVector::X
    float            AnimMovementStep_Y;                     // +0x58 C3DVector::Y
    float            AnimMovementStep_Z;                     // +0x5c C3DVector::Z
    bool             AddedActionAnim;                        // +0x60
    bool             Finished;                               // +0x61
    bool             PerformedAction;                        // +0x62
    bool             Initialised;                            // +0x63
    long             NextMovementPosWorldFrame;              // +0x64
    float            NextMovementPos_X;                      // +0x68 C3DVector::X
    float            NextMovementPos_Y;                      // +0x6c C3DVector::Y
    float            NextMovementPos_Z;                      // +0x70 C3DVector::Z
    CAnimationEntry* PAnimEntry_Data;                        // +0x74 CCountedPointer<CAnimationEntry>::Data
    CCPPointerInfo*  PAnimEntry_Info;                        // +0x78 CCountedPointer<CAnimationEntry>::Info
    CAnimationEntry* PPreceedingPAnimEntry_Data;             // +0x7c CCountedPointer<CAnimationEntry>::Data
    CCPPointerInfo*  PPreceedingPAnimEntry_Info;             // +0x80 CCountedPointer<CAnimationEntry>::Info
    unsigned char    ActionEvents[0xc];                      // +0x84 vector<CCountedPointer<NCreatureAction::CActionEventBase>,std::allocator<CCountedPointer<NCreatureAction::CActionEventBase>_>_>
    unsigned char    CutSceneEvents[0xc];                    // +0x90 map<long,CCharString,std::less<long>,std::allocator<std::pair<long_const_,CCharString>_>_>
    unsigned char    DependentThings[0xc];                   // +0x9c vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CCreatureActionBase) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PCreature) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, TotalTime) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionTime) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimDelayTime) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, FramesRunning) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, InterruptionGroupID) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, InterruptionPriority) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionID) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PItemRemoved) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Sounds_FirstSound) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Sounds_LastSound) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PlayingSoundID) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, RunningTimeSeconds) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, LastRunningTimeSeconds) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, StartFrame) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PauseFrame) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PauseTimer) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimMovementStep_X) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimMovementStep_Y) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AnimMovementStep_Z) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, AddedActionAnim) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Finished) == 0x61);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PerformedAction) == 0x62);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, Initialised) == 0x63);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPosWorldFrame) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPos_X) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPos_Y) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, NextMovementPos_Z) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PAnimEntry_Data) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PAnimEntry_Info) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PPreceedingPAnimEntry_Data) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, PPreceedingPAnimEntry_Info) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, ActionEvents) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, CutSceneEvents) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase, DependentThings) == 0x9c);

#endif // FABLE_RETAIL_CCREATUREACTIONBASE
#endif // FABLE_ENGINE_CCREATUREACTIONBASE_H
