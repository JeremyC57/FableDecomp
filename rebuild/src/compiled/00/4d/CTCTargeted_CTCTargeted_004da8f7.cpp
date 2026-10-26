// CTCTargeted::CTCTargeted(CThing* owner) -- targeted-thing component constructor: CTCBase(owner), no targeting flags, targetable with glow, no LOS exemption, no targeting def
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA8F7, length 43 bytes (ret 4). Vtable 0x0123F024 (CTCTargeted).
// Callee: CTCBase::CTCBase(CThing*) @ 0x00686800. Created by Create_CTCTargeted @ 0x004DBF1D.
// Layout: engine/CTCTargeted.h (Ego_r.pdb, 0x18 bytes): TargetingFlags +0x0c, Targetable +0x10, UseTargetGlow +0x11,
// ValidTargetWithoutLOS +0x12, PTargetingDef (CDefPointer<const CTargetingDef>) +0x14. The header is a flattened POD
// (its vptr is a `void*`), so the polymorphic CTCBase chain is modelled locally with the header's member names.
// Shape: a size-optimised TU (`push dword ptr [esp+8]` pass-through of the owner): `#pragma optimize("s",on)`.
class CThing;
class CTargetingDef;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    CTCBase(CThing* owner);                  // 0x00686800
};

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};

class CTCTargeted : public CTCBase { public:               // vtable 0x0123F024
    unsigned long TargetingFlags;            // +0x0c
    bool          Targetable;                // +0x10
    bool          UseTargetGlow;             // +0x11
    bool          ValidTargetWithoutLOS;     // +0x12
    CDefPointer<const CTargetingDef> PTargetingDef;   // +0x14
    virtual void v00();
    CTCTargeted(CThing* owner);
};

CTCTargeted::CTCTargeted(CThing* owner)
    : CTCBase(owner)
    , TargetingFlags(0)
    , Targetable(true)
    , UseTargetGlow(true)
    , ValidTargetWithoutLOS(false)
    , PTargetingDef()
{
}