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
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
class CAnimationEntry;
#endif
#include "stlport/fable_map.h"
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "engine/CIntelligentPointer.h"
#include "engine/CSoundPair.h"
class CAnimationEntry;
#endif
namespace NCreatureAction { class CActionEventBase; }
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
class CGameDefinitionManager;class CWorld;
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_GETTHINGMANAGER
class CThingManager;
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
class CWorld;
#endif
// Original Ego_r TPI0x221c9; native factories and calls prove vtable slots40-42.
// Complete native sizea8 after the three retail container reductions.
class CCreatureActionBase : public CBaseObject {
#ifdef FABLE_RETAIL_EXPRESSION_FRAME
public:bool HasPerformedAction()const;
#endif
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
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
    // Opt-in (wf11, row 0x009810B0): Ego_r.pdb ??0CCreatureActionBase@@QAE@AAVCThingCreatureBase@@MMABVCAnimationEntry@@ABVCSoundPair@@JJ@Z
    // (retail 0x00693B30, out of line) and the protected static __fastcall
    // ?GetRandomAnimFromName@CCreatureActionBase@@KIABVCAnimationEntry@@ABVCThingCreatureBase@@ABVCCharString@@@Z (0x006924B0, landed);
    // both used by the same-TU CCreatureAction_SendMessage constructor 0x00982210. Declarations only.
    CCreatureActionBase(CThingCreatureBase& creature, float speed, float blendTime, const class CAnimationEntry& animation,
        const class CSoundPair& sound, long a, long b);                 // 0x00693B30
protected:
    static const class CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature, const CCharString& name);   // 0x006924B0
public:
#endif
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
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_UPDATEANIM
    // Opt-in (w22 agent2007, row 0x00665860 CThingCreatureBase::UpdateAnimation): Ego_r.pdb / FableWin.pdb
    // ?UpdateActionAnimation@CCreatureActionBase@@QAEXXZ (public, out of line in retail at 0x00694890). Declaration only.
public:
    void UpdateActionAnimation();                                        // 0x00694890
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR_B
    // Opt-in (chunk 2026, row 0x0092DA20: the inline CCreatureAction_GetInBed ctor, out-of-line COMDAT 0x0092E300): Ego_r.pdb
    // ??0CCreatureActionBase@@QAE@AAVCThingCreatureBase@@MMABVCAnimationEntry@@ABVCSoundPair@@JJ@Z (donor 0x0051B170), out of line in
    // retail at 0x00693B30 (landed). Declaration only: no layout change.
public:
    CCreatureActionBase(CThingCreatureBase& creature, float speed, float blend, const class CAnimationEntry& anim,
                        const class CSoundPair& sound, long a, long b);   // 0x00693B30
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
    // Chunk 764 (row 0x00859660 CCombatActionBase::ResetMeleeCombatAbilityEffects): Ego_r.pdb protected accessors, out of line in
    // retail: ?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ 0x00692910 and
    // ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ 0x00692940.
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const;       // 0x00692910
    CWorld& GetWorld();                                                  // 0x00692940
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
    // Retail opt-in (wf9, row 0x00867B40 CCombatAction_GenericStrikeResponseKnockdown::FinishAction): Ego_r.pdb protected const accessor
    // ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ (Ego_r 0x005192B0, right after GetWorld 0x005192A0), out of line in retail at
    // 0x00692950 (`add ecx,8 ; call GetPItem ; mov ecx,eax ; jmp CThing::PeekWorld 0x004C7AB0`). Declaration only.
protected:
    const CWorld& PeekWorld() const;                                     // 0x00692950
#endif
#if defined(FABLE_RETAIL_CCREATUREACTIONBASE_HASPERFORMEDACTION) && !defined(FABLE_RETAIL_EXPRESSION_FRAME)
    // Retail opt-in (chunk Q17, row 0x008BF500): Ego_r.pdb ?HasPerformedAction@CCreatureActionBase@@QBE_NXZ (public const), out of line in
    // retail at 0x006925A0 (`mov al,[ecx+62h] ; ret`). Declaration only (the EXPRESSION_FRAME opt-in declares the same member).
public:
    bool HasPerformedAction() const;                                     // 0x006925A0
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_GETFRAMESRUNNING
public:
    // Nested opt-in (chunk Q25, row 0x00847520): Ego_r.pdb / FableWin.pdb ?GetFramesRunning@CCreatureActionBase@@QBEJXZ, out of line
    // in retail at 0x006925C0 (`mov eax,[ecx+1Ch] ; ret`: FramesRunning). Declaration only.
    long GetFramesRunning() const;                                       // 0x006925C0
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
    // Retail opt-in (chunk Q17, rows 0x00966030 / 0x00965640 OnReplace): Ego_r.pdb / FableWin.pdb ?GetActionID@CCreatureActionBase@@QBEJXZ
    // (public), out of line in retail at 0x00692620. Declaration only.
public:
    long GetActionID() const;                                            // 0x00692620
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_GETTHINGMANAGER
    // agentNF16 (row 0x00692980): Ego_r.pdb / FableWin.pdb protected accessor ?GetThingManager@CCreatureActionBase@@IAEAAVCThingManager@@XZ,
    // out of line in retail at 0x00692980 (`add ecx,8 ; call GetPItem 0x00A01B50 ; mov eax,[0x013B8A1C] ; ret`).
protected:
    CThingManager& GetThingManager();                                    // 0x00692980
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_SETANIMATION
    // Retail opt-in (wf7, row 0x00847DF0 CCreatureAction_UseLamp::Initialise): Ego_r.pdb ?SetAnimation@CCreatureActionBase@@IAEXABVCAnimationEntry@@MM@Z
    // (protected), out of line in retail at 0x00693030 (landed). Declaration only.
protected:
    void SetAnimation(const class CAnimationEntry& anim, float a, float b);   // 0x00693030
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_GETTOTALTIMEWORLDFRAMES
    // Retail opt-in (wf7, row 0x00847DF0): Ego_r.pdb ?GetTotalTimeWorldFrames@CCreatureActionBase@@QBEJXZ (public const), out of line in
    // retail at 0x006925B0 (`mov eax,[ecx+10h] ; ret`: TotalTime). Declaration only.
public:
    long GetTotalTimeWorldFrames() const;                                // 0x006925B0
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
    // agentNF19 (row 0x0072D360): Ego_r.pdb 0x00525E80 / FableWin.pdb 0x01DD0230
    // ?SetPItemRemoved@CCreatureActionBase@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z (public; stores the by-value pointer into
    // PItemRemoved +0x2C). Needs FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS (CIntelligentPointer). Declaration only.
public:
    void SetPItemRemoved(CIntelligentPointer<CThing> item);           // 0x0072D360
#endif
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
protected:
 CIntelligentPointer<CThingCreatureBase> PCreature;
 long TotalTime;
 long ActionTime;
 long AnimDelayTime;
 long FramesRunning;
 long InterruptionGroupID;
 long InterruptionPriority;
 long ActionID;
 CIntelligentPointer<CThing> PItemRemoved;
 CSoundPair Sounds;
 unsigned long PlayingSoundID;
 float RunningTimeSeconds;
 float LastRunningTimeSeconds;
 long StartFrame;
 long PauseFrame;
 long PauseTimer;
 C3DVector AnimMovementStep;
 bool AddedActionAnim;
 bool Finished;
 bool PerformedAction;
 bool Initialised;
 long NextMovementPosWorldFrame;
 C3DVector NextMovementPos;
 CCountedPointer<CAnimationEntry> PAnimEntry;
 CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;
private:
 std::vector<CCountedPointer<NCreatureAction::CActionEventBase>,std::allocator<CCountedPointer<NCreatureAction::CActionEventBase> > > ActionEvents;
 std::map<long,CCharString,std::less<long>,std::allocator<std::pair<long const ,CCharString> > > CutSceneEvents;
 std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > DependentThings;
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_GETDEPENDENTTHINGS
 // Retail opt-in (chunk Q17, row 0x008BF500 CCreatureAction_GenericFireProjectileWeapon::FinishAction): FableWin.pdb
 // ?GetDependentThings@CCreatureActionBase@@IAEAAV?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@XZ
 // (protected, 0x02A023E0; no Ego_r.pdb or Fable.exe copy: inline, the row reads DependentThings +0x9C / +0xA0 in place).
protected:
 std::vector<CIntelligentPointer<CThing> >& GetDependentThings() { return DependentThings; }
#endif
#elif defined(FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING)
public:
    // Ego_r221ae public non-const reference accessor; FableWin0281d460.
    CCountedPointer<CAnimationEntry>& GetPPreceedingAnimationEntry(){return PPreceedingPAnimEntry;}
protected:
    unsigned char _pad_0x08[0x74];
    CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;
    unsigned char _pad_0x84[0x24];
#elif defined(FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE)
    // chunk 751 (row 0x00854720 CCreatureAction_SpiritReturnToHero::PerformAction): Ego_r.pdb PCreature
    // (CIntelligentPointer<CThingCreatureBase>) at +0x08; the rest stays opaque. Needs engine/CIntelligentPointer.h first.
protected: CIntelligentPointer<CThingCreatureBase> PCreature; unsigned char _pad_0x10[0x98];
#elif defined(FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION)
    // Retail opt-in (chunk 796, row 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate): Ego_r.pdb InterruptionGroupID +0x20,
    // InterruptionPriority +0x24 and Finished +0x61 (no STLport member before them, so the Ego_r offsets hold), read in retail as
    // `cmp [eax+20h],64h` / `cmp [eax+24h],96h` / `mov cl,[eax+61h]`; the accessors are Ego_r.pdb ?GetInterruptionGroupID@CCreatureActionBase@@QBEJXZ,
    // ?GetInterruptionPriority@CCreatureActionBase@@QBEJXZ and ?IsFinished@CCreatureActionBase@@QBE_NXZ (inline in retail). The rest stays opaque.
public:
    long GetInterruptionGroupID() const { return InterruptionGroupID; }
    long GetInterruptionPriority() const { return InterruptionPriority; }
    bool IsFinished() const { return Finished; }
protected:
    unsigned char _pad_0x08[0x18];
    long InterruptionGroupID;                                            // +0x20
    long InterruptionPriority;                                           // +0x24
    unsigned char _pad_0x28[0x39];
    bool Finished;                                                       // +0x61
    unsigned char _pad_0x62[0x46];
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
#ifdef FABLE_RETAIL_CCREATUREACTIONBASE_STATREGISTER
    // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): Ego_r.pdb / FableWin.pdb ?StatRegisterClass@CCreatureActionBase@@SIXXZ, retail 0x006F5C10 (registers every
    // action prototype in the Registry map 0x013BA8C8 through insert_unique 0x006F5400 after RegisterActionEventTypes 0x006932B0;
    // the catalog's global "StatRegisterClass" label lacks the class). Declaration only: no layout change.
    static void __fastcall StatRegisterClass();                      // 0x006F5C10
#endif
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
