#pragma optimize("s", on)
// CEnginePrimitiveHandle::CEnginePrimitiveHandle(long) -- CBaseClass base (@ 0x0099A2D0), own vtable (0x0122F598), null counted Data pointer; the argument is unused (ret 4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB32E, length 24 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (stores vtable 0x01231710).
// The manifest label (C3DAnimationBlendState::C3DAnimationBlendState) is propagated; the stored
// vtable is CEnginePrimitiveHandle's (rtti-vtables.tsv) and the PDB layout is {vptr,
// CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4}. Size-optimised TU (`and [m],0`).
struct CBaseClass {                        // vtable 0x01231710
    virtual void v00();
    CBaseClass();                          // 0x0099A2D0
};

struct CIVCountedPointeeBase;

struct CIVCountedPointer {                 // embedded member with an inline constructor: keeps the
    CIVCountedPointeeBase* Object;         // vtable store above its null store
    CIVCountedPointer() : Object(0) { }
};

struct CEnginePrimitiveHandle : CBaseClass {   // vtable 0x0122F598
    CIVCountedPointer Data;                // +4
    CEnginePrimitiveHandle(long unused);
};

CEnginePrimitiveHandle::CEnginePrimitiveHandle(long unused)
{
}