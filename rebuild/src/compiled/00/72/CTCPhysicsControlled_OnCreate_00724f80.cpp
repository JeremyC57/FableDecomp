// CTCPhysicsControlled::OnCreate() -- CTCPhysicsStandard::OnCreate, then zero ImpulseVelocity, set Moving and clear ForcedMoveToPosSet
// Retry 10: RELOCATION_MATCH -- ImpulseVelocity zeroed through an inline C3DVector::Set(x, y, z) that stores Z, Y, X (stores through the member-function `this` keep the flag-byte loads after them; direct member stores let the lane hoist `mov cl,[esi+3Ch]`).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724F80, 53 bytes, CTCPhysicsControlled vtable 0x0126616C slot 4.
// Callees: CTCPhysicsStandard::OnCreate @ 0x00724010 (manifest label, slot 4 of the base).
// Layout: CTCPhysicsBase (engine/CTCPhysicsBase.h, 0x40; the PDB bool:1 group Moving/OldMoving at +0x3c, which the header
//   carries as the opaque OldMoving[4] bytes, is reached through a bit-field view) -> CTCPhysicsStandard (PDB 0x88) ->
//   CTCPhysicsControlled (PDB 0xac): ImpulseVelocity +0x88, ForcedMoveToPos +0x94, {ForcedMoveToPosSet, ForcedMoveToPosUsePhysics} +0xa0.
// Shape: the chained `X = Y = Z = 0` stores Z, Y, X from one xor register; the two flag updates are plain byte |= / &= (the
//   bit-field forms fold to `or byte ptr [m],1`; the byte forms give retail's load/op/store pairs).
// Leaf: OnCreate
#include "engine/CTCPhysicsBase.h"

class C3DVector { public:
    float X, Y, Z;
    void Set(float x, float y, float z) { Z = z; Y = y; X = x; }
};

struct SPhysicsBaseFlags {                  // CTCPhysicsBase +0x3c bool:1 group
    bool Moving : 1;                        // bit 0
    bool OldMoving : 1;                     // bit 1
};

struct CTCPhysicsStandard : CTCPhysicsBase {
    unsigned char _standard[0x48];          // +0x40 PPhysicsMeshInfo, OffsetVector3D, RHSet, OldRHSet, flags
    void OnCreate();                        // 0x00724010
    SPhysicsBaseFlags& MovingFlags() { return *reinterpret_cast<SPhysicsBaseFlags*>(OldMoving); }
};

struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity;              // +0x88
    C3DVector ForcedMoveToPos;              // +0x94
    unsigned char ForcedFlags;              // +0xa0 bit 0 ForcedMoveToPosSet, bit 1 ForcedMoveToPosUsePhysics
    float RenderZOffset;                    // +0xa4
    float LastRenderZOffset;                // +0xa8
    void OnCreate();
};

void CTCPhysicsControlled::OnCreate()
{
    CTCPhysicsStandard::OnCreate();
    ImpulseVelocity.Set(0.0f, 0.0f, 0.0f);
    OldMoving[0] |= 1;
    ForcedFlags &= ~1;
}