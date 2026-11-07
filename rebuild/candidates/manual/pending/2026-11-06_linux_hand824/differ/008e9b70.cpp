// NCreatureMode::CModeStrafe::AnimUpdate(CAnimateThing& animate) -- keep the strafe animations current: restart the main strafe anim (type 0x1b, priority 0x5a) when the strafing-anim name changed or the main set is empty / finished / of another type; with no usable projectile weapon (or an ability mutex lock) drop the aim / build-up / pull-back combination anims and keep the 0x23 upper-body anim on the ability's strafe anim or PAnimStrafeUpperBody; otherwise drive the aim (0x24, best aiming frame), build-up (0x25, RumbleParam alpha) and pull-back (0x1f, PullBackAmount, the weapon's animating object in manual update) combination anims; clear ForceAnimNoBlendIn
// Grade: DIFFER(1699v1705) -- 61 lines (chunk 824 retry, 11 more attempts, none better): register rotation at the SetMainAnim / PeekTC / CAnimInit + AddCombinationAnim sites (retail loads AnimIndex into edx before `push 5Ah`, keeps the creature in eax for the carrying test and `test cl,40h`; ours reload / ecx / `test al,40h`, 6 bytes shorter) and PullBackAmount (retail spills it with fst [esp+58h] for the later push, VC7.1 here reloads the member)
// Next idea: a same-TU body of GetStrafingAnimationName 0x008E8FF0 (not landed; it returns a CCharString by value) and the landed GetBestFrameInAimingAnim 0x008E9460 above AnimUpdate under auto_inline(off) -- retail compiled both earlier in this TU; the GetAnimDistanceAndIndexFromAnimName stand-in alone is inert, as are reference/pointer creature, declaration order, template PeekTC and a named amount (esi/edi flip)
// Leaf: ?AnimUpdate@CModeStrafe@NCreatureMode@@UAEXAAVCAnimateThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9B70, 1705 bytes, CModeStrafe vtable 0x01268588 slot 2 (virtual, `ret 4`), frame 0x44, no EH frame.
// Identity (proven): Ego_r.pdb ?AnimUpdate@CModeStrafe@NCreatureMode@@UAEXAAVCAnimateThing@@@Z (slot 2 of the CModeBase vtable; the
//   oracle's "vfunc_2"); the members it reads are Ego_r's CModeStrafe at retail offsets (StrafeAnimName +0x20 ... PAnimStrafeUpperBody +0x68).
// Callees (PDB spellings): GetStrafingAnimationName 0x008E8FF0 / GetAnimDistanceAndIndexFromAnimName 0x008E9080 (both __fastcall),
//   CThingCreatureBase::GetAnimFromName 0x00662A00, CAnimationEntry::GetDelayTime 0x005DC1F0 / GetTimeToBlendIn 0x005DC230 /
//   GetAnimateThingFlags 0x005DC390, NAnimationEntryBodge::CAnimEntryLite::GetTimeToBlendIn 0x0073A690, CAnimInit ctor 0x005B33F4 /
//   Init 0x0070C050, CAnimateThing::SetMainAnim 0x0070D580 / AddCombinationAnim 0x0070D780 / RemoveCombinationAnimsOfType 0x0070D8C0 /
//   RemoveCombinationAnim 0x0070D8E0 / GetFirstCombinationAnimFromType 0x0070C4E0, CTCSpecialAbilities::GetAbilityMutexLock 0x0072A860 /
//   GetCurrentAbilityStrafeAnim 0x0072B870, CTCWeapon::IsProjectileWeaponLoaded 0x005DA980, CModeStrafe::GetBestFrameInAimingAnim
//   0x008E9460, CAnimSet::SetAlpha 0x0070BFC0 / SetInternalAlpha 0x0070C280, CTCAnimatingObject::SetCurrentAnimAsManualUpdate 0x007F8D00 /
//   SetFractionComplete 0x007F8D60, CCharString ctor / dtor / operator=.
// Inline split: retail expands the CAnimInit constructor at the aim site only and calls the COMDAT 0x005B33F4 at the others; the
//   do { ... } while (0) block around that site's CAnimInit + AddCombinationAnim reproduces it (s11), as do the split interrupted /
//   not-loaded remove branches for the pull-back block order (s14).
// Header overlays: engine/CThingCreatureBase.h FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION, engine/CTCWeapon.h FABLE_RETAIL_CTCWEAPON_LOADING,
//   engine/CCreatureActionBase.h FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION (all opt-in).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCCreatureModeManager.h"

extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C

class CTCSpecialAbilities : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SPECIAL_ABILITIES; }
    bool GetAbilityMutexLock() const;                    // 0x0072A860
    CCharString GetCurrentAbilityStrafeAnim() const;     // 0x0072B870
};

class CTCAnimatingObject : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ANIMATING_OBJECT; }
    void SetCurrentAnimAsManualUpdate();                 // 0x007F8D00
    void SetFractionComplete(float fraction);            // 0x007F8D60
};

class CAnimationEntry {
public:
    long BankIndex;                                      // +0x00
    unsigned char _rest[0xc];
    long GetDelayTime() const;                           // 0x005DC1F0
    long GetTimeToBlendIn(const CThing& thing) const;    // 0x005DC230
    unsigned long GetAnimateThingFlags() const;          // 0x005DC390
};

namespace NAnimationEntryBodge {
class CAnimEntryLite {
public:
    long BankIndex;                                      // +0x00
    long DelayTime;                                      // +0x04
    long TransitionTime;                                 // +0x08
    long AnimFlags;                                      // +0x0c
    long GetTimeToBlendIn(const CThing& thing) const;    // 0x0073A690
};
}

enum EAnimUpdateMode { };

class CAnimFrameInfo {
public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }
    CAnimFrameInfo(float frame) : Current(frame), Last(frame) { }
    CAnimFrameInfo(float current, float last) : Current(current), Last(last) { }
};

class CAnimInit {
public:
    EAnimUpdateMode UpdateMode;                          // +0x00
    unsigned char   Type;                                // +0x04
    bool            Looping;                             // +0x05
    long            Anim;                                // +0x08
    float           Speed;                               // +0x0c
    float           Alpha;                               // +0x10
    float           LastAlpha;                           // +0x14
    CAnimFrameInfo  Frame;                               // +0x18
    unsigned long   Flags;                               // +0x20
    unsigned long   TypeFlags;                           // +0x24
    long            Priority;                            // +0x28
    CAnimInit(unsigned char type, unsigned long flags, long anim, EAnimUpdateMode mode, bool looping, unsigned long typeFlags, long priority)
    {
        Init(type, flags, anim, mode, looping, typeFlags, priority);
    }
    void Init(unsigned char type, unsigned long flags, long anim, EAnimUpdateMode mode, bool looping, unsigned long typeFlags, long priority);   // 0x0070C050
};

class CAnimSet {
public:
    EAnimUpdateMode UpdateMode;                          // +0x00
    std::vector<short> Anims;                            // +0x04
    unsigned char   InternalAlphas[0xc];                 // +0x10
    float           CurrentPercentageComplete;           // +0x1c
    float           LastPercentageComplete;              // +0x20
    float           Speed;                               // +0x24
    unsigned long   FrameInternalAlphasUpdated;          // +0x28
    float           CurrentAlpha;                        // +0x2c
    float           LastAlpha;                           // +0x30
    float           LastUpdateChange;                    // +0x34
    float           ChangeAlphaBy;                       // +0x38
    void*           PAnimator;                           // +0x3c
    unsigned long   Flags;                               // +0x40
    unsigned long   TypeFlags;                           // +0x44
    long            AnimDelay;                           // +0x48
    long            Priority;                            // +0x4c
    unsigned char   Type;                                // +0x50
    bool            Looping:1;                           // +0x51
    bool            Finished:1;
    bool            Forwards:1;
    bool            HadUpdate:1;
    bool            OwnedByMainAnim:1;
    void SetAlpha(float alpha);                          // 0x0070BFC0
    long GetAnimIndex() const { return Anims[0]; }
    bool IsFinished() const { return Finished; }
    float GetPercentageComplete() const { return CurrentPercentageComplete; }
    void SetFrame(const CAnimFrameInfo& frame) { CurrentPercentageComplete = frame.Current; LastPercentageComplete = frame.Last; }
    void SetAsUpdated(bool updated) { HadUpdate = updated; }
    void SetChangeAlphaBy(float change) { ChangeAlphaBy = change; }
    void SetInternalAlpha(long index, float alpha, bool immediate);   // 0x0070C280
};

class CAnimateThing {
public:
    CAnimSet MainAnimSet;                                // +0x00 (CCompleteAnimation CurrentAnim)
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay);                         // 0x0070D580
    void AddCombinationAnim(const CAnimInit& init, long blendIn, long delay, bool a, bool b);  // 0x0070D780
    void RemoveCombinationAnimsOfType(long type);                                              // 0x0070D8C0
    void RemoveCombinationAnim(const CAnimSet* anim);                                          // 0x0070D8E0
    CAnimSet* GetFirstCombinationAnimFromType(long type);                                      // 0x0070C4E0
};

enum EMovementType { };                                  // global in Ego_r (?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z)
namespace NCreatureMode {


CCharString __fastcall GetStrafingAnimationName(const CThing& thing, EMovementType type);                                 // 0x008E8FF0
bool __fastcall GetAnimDistanceAndIndexFromAnimName(const CThingCreatureBase& creature, const CCharString& name, long& index, float& distance);   // 0x008E9080

class CModeStrafe : public CTCCreatureModeManager::CModeBase {
public:
    virtual void AnimUpdate(CAnimateThing& animate);
private:
    bool GetBestFrameInAimingAnim(float& frame) const;   // 0x008E9460
    float         AnimMovementDistance;                  // +0x1c
    CCharString   StrafeAnimName;                        // +0x20
    long          AnimIndex;                             // +0x24
    bool          LastUsable;                            // +0x28
    unsigned char Filter[0xc];                           // +0x2c CAnimFilterOfGroup
    long          ActiveWeapon;                          // +0x38
    long          AimingNumFrames;                       // +0x3c
    long          AimingCenterFrame;                     // +0x40
    float         RumbleParam;                           // +0x44
    float         PullBackAmount;                        // +0x48
    bool          ForceAnimNoBlendIn;                    // +0x4c
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim;        // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileBuildUp;    // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectilePullBack;   // +0x60
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimStrafeUpperBody;      // +0x68
};

void CModeStrafe::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(&ParentThing);
    CCharString name = GetStrafingAnimationName(*creature, (EMovementType)0);
    if (name != StrafeAnimName || animate.MainAnimSet.Anims.empty() || animate.MainAnimSet.Finished || animate.MainAnimSet.Type != 0x1b) {
        const CThingCreatureBase& strafer = static_cast<const CThingCreatureBase&>(ParentThing);
        StrafeAnimName = name;
        GetAnimDistanceAndIndexFromAnimName(strafer, name, AnimIndex, AnimMovementDistance);
        const CAnimationEntry& entry = creature->GetAnimFromName(name, 0);
        animate.SetMainAnim(CAnimInit(0x1b, 0, AnimIndex, (EAnimUpdateMode)1, true, entry.GetAnimateThingFlags(), 0x5a),
            ForceAnimNoBlendIn ? 0 : entry.GetTimeToBlendIn(ParentThing), ForceAnimNoBlendIn ? 0 : entry.GetDelayTime());
    }
    CThing* primary = 0;
    const CTCWeapon* weapon = 0;
    bool projectile = false;
    const CTCCarrying* carrying;
    if (creature->PeekTC(TCI_CARRYING, &carrying)) {
        primary = carrying->GetThingInPrimarySlot();
        if (primary && primary->PeekTC(TCI_WEAPON, &weapon) && weapon->IsProjectileWeapon())
            projectile = true;
    }
    bool locked = false;
    const CTCSpecialAbilities* abilities;
    if (creature->PeekTC(TCI_SPECIAL_ABILITIES, &abilities) && abilities->GetAbilityMutexLock())
        locked = true;
    if (locked || !projectile) {
        animate.RemoveCombinationAnimsOfType(0x24);
        animate.RemoveCombinationAnimsOfType(0x25);
        animate.RemoveCombinationAnimsOfType(0x1f);
        CAnimSet* upperBody = animate.GetFirstCombinationAnimFromType(0x23);
        if (locked && abilities->GetCurrentAbilityStrafeAnim() != "") {
            CCharString abilityAnim = abilities->GetCurrentAbilityStrafeAnim();
            const CAnimationEntry& entry = creature->GetAnimFromName(abilityAnim, 0);
            if (!upperBody || upperBody->GetAnimIndex() != entry.BankIndex) {
                animate.RemoveCombinationAnimsOfType(0x23);
                CAnimInit init(0x23, 0, entry.BankIndex, (EAnimUpdateMode)1, true, entry.GetAnimateThingFlags(), 0x5f);
                animate.AddCombinationAnim(init, ForceAnimNoBlendIn ? 0 : entry.GetTimeToBlendIn(*creature), 0, false, false);
            }
        } else if (const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimStrafeUpperBody.Data) {
            if (!upperBody || upperBody->GetAnimIndex() != lite->BankIndex) {
                animate.RemoveCombinationAnimsOfType(0x23);
                CAnimInit init(0x23, 0, lite->BankIndex, (EAnimUpdateMode)1, true, lite->AnimFlags, 0x5f);
                animate.AddCombinationAnim(init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(*creature), 0, false, false);
            }
        } else
            animate.RemoveCombinationAnimsOfType(0x23);
    } else {
        animate.RemoveCombinationAnimsOfType(0x23);
        if (PAnimProjectileAim.Data) {
            float frame;
            if (!GetBestFrameInAimingAnim(frame))
                frame = -1.0f;
            CAnimSet* aim = animate.GetFirstCombinationAnimFromType(0x24);
            if (!aim) {
                do {
                const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectileAim.Data;
                CAnimInit init(0x24, 0, lite->BankIndex, (EAnimUpdateMode)0, true, lite->AnimFlags, 0x5f);
                animate.AddCombinationAnim(init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(ParentThing), 0, false, false);
                } while (0);
                aim = animate.GetFirstCombinationAnimFromType(0x24);
                if (aim)
                    aim->SetFrame(CAnimFrameInfo(frame > -DELTA_NOTIONAL_ZERO ? frame : 0.5f));
            }
            if (aim) {
                float last = aim->GetPercentageComplete();
                float current = last;
                if (frame > -DELTA_NOTIONAL_ZERO)
                    current = frame;
                aim->SetFrame(CAnimFrameInfo(current, last));
                aim->SetAsUpdated(true);
            }
        }
        if (PAnimProjectileBuildUp.Data) {
            CAnimSet* buildUp = animate.GetFirstCombinationAnimFromType(0x25);
            if (!buildUp || buildUp->IsFinished()) {
                if (buildUp)
                    animate.RemoveCombinationAnim(buildUp);
                const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectileBuildUp.Data;
                CAnimInit init(0x25, 0, lite->BankIndex, (EAnimUpdateMode)1, true, lite->AnimFlags, 0x69);
                animate.AddCombinationAnim(init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(ParentThing), 0, false, false);
                buildUp = animate.GetFirstCombinationAnimFromType(0x25);
                if (buildUp) {
                    buildUp->SetAlpha(RumbleParam);
                    buildUp->SetChangeAlphaBy(0.0f);
                }
            }
            buildUp->SetAlpha(RumbleParam);
            buildUp->SetInternalAlpha(0, RumbleParam, false);
        }
        if (PAnimProjectilePullBack.Data && primary && !primary->IsDead() && weapon && weapon->IsAnalogueLoading()) {
            CAnimSet* pullBack = animate.GetFirstCombinationAnimFromType(0x1f);
            const CCreatureActionBase* action = creature->PeekPCurrentAction();
            if (action && !action->IsFinished() && action->GetInterruptionGroupID() == 100 && action->GetInterruptionPriority() > 150) {
                if (pullBack)
                    animate.RemoveCombinationAnim(pullBack);
            } else if (!weapon->IsProjectileWeaponLoaded()) {
                if (pullBack)
                    animate.RemoveCombinationAnim(pullBack);
            } else {
                CTCAnimatingObject* animating;
                if (!primary->GetTC(TCI_ANIMATING_OBJECT, &animating))
                    animating = 0;
                if (!pullBack) {
                    const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectilePullBack.Data;
                    CAnimInit init(0x1f, 0, lite->BankIndex, (EAnimUpdateMode)0, false, lite->AnimFlags, 0x6e);
                    animate.AddCombinationAnim(init, 0, 0, false, false);
                    pullBack = animate.GetFirstCombinationAnimFromType(0x1f);
                    if (pullBack)
                        pullBack->SetFrame(CAnimFrameInfo(PullBackAmount, PullBackAmount));
                    if (animating)
                        animating->SetCurrentAnimAsManualUpdate();
                }
                pullBack->SetFrame(CAnimFrameInfo(PullBackAmount, pullBack->GetPercentageComplete()));
                pullBack->SetAsUpdated(true);
                if (animating)
                    animating->SetFractionComplete(PullBackAmount);
            }
        }
    }
    ForceAnimNoBlendIn = false;
}

}
