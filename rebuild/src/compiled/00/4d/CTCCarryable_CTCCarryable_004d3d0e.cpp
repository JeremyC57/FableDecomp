// CTCCarryable::CTCCarryable(CThing* thing) -- CTCBase(thing), own vtable, not fragile, no carrier, zero attach offset and offset amount
// Leaf: ??0CTCCarryable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3D0E, length 52 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (landed).
// Identity: vtable 0x0123B35C = CTCCarryable (rtti-vtables.tsv). Layout: Ego_r.pdb CTCCarryable (0x24 bytes):
// ForceFragility +0xc, PCarryingTC +0x10, OffsetCoordRelativeToAttachToDummy (C3DVector) +0x14, OffsetAmount +0x20.
// engine/CTCBase.h is layout-only, so the polymorphic base is modelled as CTCBase_Calls.
// Shape: size TU (pass-through push, `and dword ptr [esi+10h],0` for the null carrier); the four floats are
// zeroed with fldz/fstp (the vector through its three-float constructor); scalar stores are init-list
// members scheduled around the vtable store.
#pragma optimize("s", on)

class CThing;
class CTCCarrying;

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CTCBase_Calls {                              // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    CTCBase_Calls(CThing* thing);                   // 0x00686800
};

class CTCCarryable : public CTCBase_Calls { public:               // vtable 0x0123B35C
    bool         ForceFragility;                    // +0x0c
    CTCCarrying* PCarryingTC;                       // +0x10
    C3DVector    OffsetCoordRelativeToAttachToDummy;// +0x14
    float        OffsetAmount;                      // +0x20
    virtual void v00();
    CTCCarryable(CThing* thing);
};

CTCCarryable::CTCCarryable(CThing* thing)
    : CTCBase_Calls(thing)
    , ForceFragility(false)
    , PCarryingTC(0)
    , OffsetCoordRelativeToAttachToDummy(0.0f, 0.0f, 0.0f)
    , OffsetAmount(0.0f)
{
}