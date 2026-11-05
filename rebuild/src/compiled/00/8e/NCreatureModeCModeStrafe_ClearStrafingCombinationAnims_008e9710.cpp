// NCreatureMode::CModeStrafe::ClearStrafingCombinationAnims(bool) -- on a live parent with an animation component, finish the combination anims of types 0x1b and 0x19 (Finished set, ChangeAlphaBy = CurrentAlpha)
// Leaf: ClearStrafingCombinationAnims
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9710, 149 bytes (`ret 4`; the bool argument is unused).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x5a = CTCAnimationComplex: word 2 bit 26),
// CTCAnimationComplex::GetAnimation @ 0x0070B460, the CAnimateThing combination-anim lookup by type @ 0x0070C4E0.
// Layout: CAnimSet (retail, 12-byte vectors): CurrentAlpha +0x2c, ChangeAlphaBy +0x38, Finished = bit 1 of +0x51;
// CThing dead bit +0x91 bit 0; CModeBase::ParentThing +4.
// Shape: `if (parent && !dead && GetInterface(..))` (the template's bool is the third test); each found set gets an
// inline `FadeOut()` (test the bit, set it, copy the alpha).
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
    void FadeOut()
    {
        if (!Finished) {
            Finished = 1;
            ChangeAlphaBy = CurrentAlpha;
        }
    }
};

class CAnimateThing { public:
    CAnimSet* GetFirstCombinationAnimFromType(long type);   // 0x0070C4E0 (Ego_r.pdb)
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();          // 0x0070B460
};

namespace NCreatureMode {

struct CModeStrafe {
    virtual void v00();
    CThing* ParentThing;                    // +0x04
    void ClearStrafingCombinationAnims(bool immediately);
};

void CModeStrafe::ClearStrafingCombinationAnims(bool immediately)
{
    CTCAnimationComplex* animation;
    if (ParentThing && !ParentThing->IsDead() && ParentThing->GetTC(TCI_ANIMATION, &animation)) {
        CAnimateThing& animator = animation->GetAnimation();
        CAnimSet* set = animator.GetFirstCombinationAnimFromType(0x1b);
        if (set)
            set->FadeOut();
        set = animator.GetFirstCombinationAnimFromType(0x19);
        if (set)
            set->FadeOut();
    }
}

}