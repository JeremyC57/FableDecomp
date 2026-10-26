// CTCPhysicsControlled::CTCPhysicsControlled(CThing* thing) -- CTCPhysicsStandard base, both forced-move bits cleared, RenderZOffset and LastRenderZOffset zeroed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724F50, 45 bytes. Stores vtable 0x0126616C = CTCPhysicsControlled (rtti-vtables.tsv).
// Callees: CTCPhysicsStandard::CTCPhysicsStandard @ 0x00723FD0 (manifest "CTCActivationTrigger::CTCActivationTrigger", propagated).
// Layout (PDB CTCPhysicsControlled, 0xac; no engine header): ImpulseVelocity +0x88, ForcedMoveToPos +0x94,
//   {ForcedMoveToPosSet:1, ForcedMoveToPosUsePhysics:1} +0xa0, RenderZOffset +0xa4, LastRenderZOffset +0xa8.
// Shape: everything is stored before the vtable as body assignments in source order: the bit pair cleared in one `and 0FCh`,
//   then the two floats from the xor zero register (as initialisers the floats are stored first).
// Leaf: ??0CTCPhysicsControlled
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

struct CTCPhysicsStandard {
    virtual void v000();
    unsigned char _base[0x84];              // +0x04 CTCPhysicsBase + CTCPhysicsStandard members
    CTCPhysicsStandard(CThing* thing);      // 0x00723FD0
};

struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity;              // +0x88
    C3DVector ForcedMoveToPos;              // +0x94
    bool      ForcedMoveToPosSet : 1;       // +0xa0 bit 0
    bool      ForcedMoveToPosUsePhysics : 1;// +0xa0 bit 1
    float     RenderZOffset;                // +0xa4
    float     LastRenderZOffset;            // +0xa8
    virtual void v000();
    CTCPhysicsControlled(CThing* thing);
};

CTCPhysicsControlled::CTCPhysicsControlled(CThing* thing)
    : CTCPhysicsStandard(thing)
{
    ForcedMoveToPosSet = false;
    ForcedMoveToPosUsePhysics = false;
    RenderZOffset = 0.0f;
    LastRenderZOffset = 0.0f;
}