// NCreatureMode::CModeStrafe::LockStrafingCombinationAnimsToMainAnim() -- on a live parent with an animation component, mark the strafing combination anims of types 0x23, 0x24, 0x25 and 0x1f as owned by the main anim
// Leaf: ?LockStrafingCombinationAnimsToMainAnim@CModeStrafe@NCreatureMode@@AAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E97B0, 149 bytes, thiscall, plain `ret` (no arguments).
// Identity (probable): Ego_r.pdb's private no-argument CModeStrafe members are SetUpCombinationAnims,
//   LockStrafingCombinationAnimsToMainAnim and UpdateWeaponState; this body does nothing but set CAnimSet::OwnedByMainAnim
//   (bit 4 of +0x51: Ego_r +0x59 bit field Looping, Finished, Forwards, HadUpdate, OwnedByMainAnim, moved down 8 by the two
//   12-byte retail vectors) on four combination anims -- "lock to main anim". Its only caller is the landed
//   CModeStrafe::OnDeactivate 0x008E9B60 (tail jmp at 0x008E9B6B), which declares it under the descriptive
//   ResetStrafeAnims_008e97b0 (label correction).
// Model: the landed sibling CModeStrafe::ClearStrafingCombinationAnims 0x008E9710 (same guard, same callees).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x5a = CTCAnimationComplex: word 2 bit 26),
//   CTCAnimationComplex::GetAnimation @ 0x0070B460, the CAnimateThing combination-anim lookup by type @ 0x0070C4E0.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAnimSet { public:                           // retail layout (12-byte vectors)
    unsigned char _pad00[0x2c];
    float         CurrentAlpha;             // +0x2c
    float         LastAlpha;                // +0x30
    float         LastUpdateChange;         // +0x34
    float         ChangeAlphaBy;            // +0x38
    unsigned char _pad3c[0x15];
    unsigned char Looping : 1;              // +0x51 bit 0
    unsigned char Finished : 1;             // +0x51 bit 1
    unsigned char Forwards : 1;             // +0x51 bit 2
    unsigned char HadUpdate : 1;            // +0x51 bit 3
    unsigned char OwnedByMainAnim : 1;      // +0x51 bit 4
    void FadeOut()
    {
        if (!Finished) {
            Finished = 1;
            ChangeAlphaBy = CurrentAlpha;
        }
    }
};

class CAnimateThing { public:
    CAnimSet* FindCombinationAnim(long type);   // 0x0070C4E0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();          // 0x0070B460
};

namespace NCreatureMode {

class CModeStrafe {
public:
    virtual void v00();
    CThing* ParentThing;                    // +0x04
private:
    void LockStrafingCombinationAnimsToMainAnim();
};

void CModeStrafe::LockStrafingCombinationAnimsToMainAnim()
{
    CTCAnimationComplex* animation;
    if (ParentThing && !ParentThing->IsDead() && ParentThing->GetTC(TCI_ANIMATION, &animation)) {
        CAnimateThing& animator = animation->GetAnimation();
        CAnimSet* set = animator.FindCombinationAnim(0x23);
        if (set)
            set->OwnedByMainAnim = true;
        set = animator.FindCombinationAnim(0x24);
        if (set)
            set->OwnedByMainAnim = true;
        set = animator.FindCombinationAnim(0x25);
        if (set)
            set->OwnedByMainAnim = true;
        set = animator.FindCombinationAnim(0x1f);
        if (set)
            set->OwnedByMainAnim = true;
    }
}

}