// CTCBed::StartGetInBedAnimation(long animation, long sleepAnimation) -- for an animated bed: remember the sleep anim, mark the bed as being got into and play the get-in animation on the thing; the bed is no longer empty
// Leaf: StartGetInBedAnimation
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007469C0, length 169 bytes (+ int3 padding), `ret 8`
// (?StartGetInBedAnimation@CTCBed@@QAEXJJ@Z). Called from 0x00848EF3.
// Callees: CAnimInit::Set @ 0x0070C050, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface
// for CTCAnimationComplex, type 0x5A = bit 26 of CThing+0x28), CTCAnimationComplex::GetAnimation @ 0x0070B460,
// CAnimateThing::SetMainAnim(const CAnimInit&, long, long) @ 0x0070D580.
// Layout = Ego_r.pdb CTCBed (24 bytes): Empty +0x0C, GettingIn +0x0D, GettingOut +0x0E, Animating +0x0F, BedSleepAnim +0x10,
// PDef +0x14; CBedDef Animating +0x38.
// Shape: an inline accessor returning an unset local filled by HasTC + a one-ternary GetTC, so the component lands in ecx: the lookup key and the never-initialised out pointer share the dead
// `sleepAnimation` slot, so the absent path reads it back from there (as retail does).
#include "fable_compiler.h"
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBlendPair {                             // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
class CBedDef { public:
    unsigned char _pad00[0x28];
    long GetInBedAnimName;                      // +0x28 CDefString
    long SleepInBedAnimName;                    // +0x2c
    long GetUpFromBedAnimName;                  // +0x30
    long GetUpFromBedFastAnimName;              // +0x34
    bool Animating;                             // +0x38
};
class CTCBed {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Empty;                              // +0x0c
    bool    GettingIn;                          // +0x0d
    bool    GettingOut;                         // +0x0e
    bool    Animating;                          // +0x0f
    long    BedSleepAnim;                       // +0x10
    const CBedDef* PDef;                        // +0x14 CDefPointer<CBedDef const> object word
    FABLE_FORCEINLINE CTCAnimationComplex* GetAnimationComplex() const
    {
        CTCAnimationComplex* complex;
        if (ParentThing->HasTC(TCI_ANIMATION))
            complex = ((CTCAnimationComplex*)&ParentThing->GetTC(TCI_ANIMATION));
        return complex;
    }
    void StartGetInBedAnimation(long animation, long sleepAnimation);
    void StartGetOutOfBedAnimation(long animation);
};

void CTCBed::StartGetInBedAnimation(long animation, long sleepAnimation)
{
    if (PDef->Animating) {
        BedSleepAnim = sleepAnimation;
        GettingIn = true;
        CAnimInit init(6, 0, animation, 1, false, 0, 100);
        CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
        animate.SetMainAnim(init, 1, 0);
        Animating = true;
    }
    Empty = false;
}