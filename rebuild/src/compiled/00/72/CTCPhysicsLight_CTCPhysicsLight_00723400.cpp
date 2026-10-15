// CTCPhysicsLight::CTCPhysicsLight(CThing* thing) -- CTCPhysicsBase base, AffectedByGravity cleared
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723400, 29 bytes. Stores vtable 0x01265DFC = CTCPhysicsLight (rtti-vtables.tsv).
// Callees: CTCPhysicsBase::CTCPhysicsBase @ 0x006B0730.
// Layout (PDB CTCPhysicsLight, 0x44; no engine header): CTCPhysicsBase 0x40 (engine/CTCPhysicsBase.h is a flat non-polymorphic
//   struct, so the polymorphic base is modelled locally for the constructor), AffectedByGravity bool:1 at +0x40.
// Shape: the bit cleared (`and byte ptr [esi+40h],0FEh`) before the vtable store.
// Leaf: ??0CTCPhysicsLight
struct CThing;

struct CTCPhysicsBase {
    virtual void v000();
    unsigned char _base[0x3c];              // +0x04 .. +0x40
    CTCPhysicsBase(CThing* thing);          // 0x006B0730
};

struct CTCPhysicsLight : CTCPhysicsBase {
    bool AffectedByGravity : 1;             // +0x40 bit 0
    virtual void v000();
    CTCPhysicsLight(CThing* thing);
};

CTCPhysicsLight::CTCPhysicsLight(CThing* thing)
    : CTCPhysicsBase(thing)
{
    AffectedByGravity = false;
}