// CTCCupboard::StartAnimation(long anim) -- start the cupboard's open/close animation: mark it animating and play `anim` as the main anim (update mode 1, priority 100)
// Leaf: ?StartAnimation@CTCCupboard@@QAEXJ@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model assigned into an UNINITIALISED local (the miss arm reads the slot, `mov ecx,[esp+34h]`): both arms load the component straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749BF0, length 138 bytes (`ret 4`).
// Callees: CAnimInit::Set (seven arguments) @ 0x0070C050 (landed as "CInitRumble::Set"; its stores are the CAnimInit fields:
// Type +4, TypeFlags +0x24, Anim +8, UpdateMode +0, Looping +5, Flags +0x20, Priority +0x28 and the 1.0 speeds/alphas),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x5A = CTCAnimationComplex,
// result unchecked), CTCAnimationComplex::GetAnimation @ 0x0070B460, CAnimateThing::SetMainAnim(const CAnimInit&, long, long)
// @ 0x0070D580.
// Layout: Ego_r.pdb CTCCupboard: Animating +0xC. CAnimInit (0x2C) with CAnimFrameInfo Frame at +0x18 (inline zeroing ctor).
// Shape: the animator is a named local (fetched before the SetMainAnim pushes); the frame-info ctor's zero stores land
// between the Set pushes.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CAnimFrameInfo { public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
};
#include "engine/EAnimUpdateMode.h"
class CAnimInit { public:                              // 0x2c bytes
    long  UpdateMode;                           // +0x00
    unsigned char Type;                         // +0x04
    bool  Looping;                              // +0x05
    long  Anim;                                 // +0x08
    float Speed;                                // +0x0c
    float Alpha;                                // +0x10
    float LastAlpha;                            // +0x14
    CAnimFrameInfo Frame;                       // +0x18
    unsigned long Flags;                        // +0x20
    unsigned long TypeFlags;                    // +0x24
    long  Priority;                             // +0x28
    CAnimInit() {}
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050 Ego_r/FableWin ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z (agentNF16: was Set)
};
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
class CTCCupboard : public CTCBase { public:
    bool Animating;                             // +0x0c
    void StartAnimation(long anim);
};

void CTCCupboard::StartAnimation(long anim)
{
    Animating = true;
    CAnimInit init;
    init.Init((unsigned char)(6), 0, anim, (EAnimUpdateMode)(1), false, 0, 100);
    CTCAnimationComplex* animation;
    if (ParentThing->HasTC(TCI_ANIMATION))
        animation = ((CTCAnimationComplex*)&ParentThing->GetTC(TCI_ANIMATION));
    CAnimateThing& animator = animation->GetAnimation();
    animator.SetMainAnim(init, 1, 0);
}