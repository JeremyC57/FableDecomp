// CTCFlashOnHit::CTCFlashOnHit(CThing& thing) -- thing-component constructor: the CTCBase base, then the members zeroed
// Leaf: ??0CTCFlashOnHit@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6292, 38 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x0123E034 (RTTI .?AVCTCFlashOnHit@@); the zeroed members match Ego_r.pdb CTCFlashOnHit; its caller is CTCFlashOnHit::Alloc 0x004D94AB.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family M095, representative 0x004D43B9): `: CTCBase(thing), FadeInTime(0), FadeOutTime(0), LifeTime(0), Timer(0) {}` (the landed CTCGold 0x007CBDA0 model);
//   `optimize("s")`: the thing reference pushed straight from its slot.
// Header debt: engine/CTCBase.h flattens the vptr, so the base is modelled locally (as the landed CTCGold).
#pragma optimize("s", on)
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb)
public:
    CTCBase(CThing& thing);                              // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCFlashOnHit : public CTCBase {
public:
    CTCFlashOnHit(CThing& thing);
    long FadeInTime;                                     // +0x0C
    long FadeOutTime;                                    // +0x10
    long LifeTime;                                       // +0x14
    long Timer;                                          // +0x18
};
CTCFlashOnHit::CTCFlashOnHit(CThing& thing)
    : CTCBase(thing), FadeInTime(0), FadeOutTime(0), LifeTime(0), Timer(0)
{
}