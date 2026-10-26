// CTCAmbientBird::CTCAmbientBird(CThing& owner) -- CTCBase(owner), zero initial height, this class's vtable, not flying
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5EFB, length 33 bytes.
// Leaf: ??0CTCAmbientBird
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800. Vtable 0x0123DC0C = CTCAmbientBird (rtti-vtables.tsv). Ego_r.pdb
// CTCAmbientBird (0x14 bytes): bool Flying +0x0c, float InitialHeight +0x10.
// Shape: the size-optimised factory module: `#pragma optimize("s", on)` (pass-through `push dword ptr [esp+8]`); the float
// zero is `fldz ; fstp` ahead of the vtable store, the byte after it (the same shape as 0x0044C47B).
#pragma optimize("s", on)
class CThing;

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
};

struct CTCAmbientBird : CTCBase {              // vtable 0x0123DC0C
    bool  Flying;                              // +0x0c
    float InitialHeight;                       // +0x10
    virtual void v00();
    CTCAmbientBird(CThing& owner);
};

CTCAmbientBird::CTCAmbientBird(CThing& owner)
    : CTCBase(owner), Flying(false), InitialHeight(0.0f)
{
}