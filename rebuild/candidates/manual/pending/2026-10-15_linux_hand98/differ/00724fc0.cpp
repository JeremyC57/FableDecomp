// CTCPhysicsControlled::ClearMovement() -- zero ImpulseVelocity, clear ForcedMoveToPosSet, set ForcedMoveToPosUsePhysics, zero Velocity
// Grade: DIFFER(ours v retail) -- 48v48: same instructions, the lane hoists 'mov dl,[ecx+0A0h] ; and ; or' above the three ImpulseVelocity zero stores (retail keeps source order)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724FC0, 48 bytes, CTCPhysicsControlled vtable 0x0126616C slot 35 (catalogued as vfunc_35). No callees.
// Layout: CTCPhysicsBase (engine/CTCPhysicsBase.h, 0x40) -> CTCPhysicsStandard (PDB 0x88) -> CTCPhysicsControlled (PDB 0xac):
//   ImpulseVelocity +0x88, ForcedMoveToPos +0x94, {ForcedMoveToPosSet:1, ForcedMoveToPosUsePhysics:1} +0xa0,
//   RenderZOffset +0xa4, LastRenderZOffset +0xa8 (PDB names; C3DVector modelled locally with its inline +=).
// Shape: one xor zero register for all six float stores (Z, Y, X order), the bitfield pair updated in one byte read-modify-write.
// Leaf: ClearMovement
#include "engine/CTCPhysicsBase.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

struct CTCPhysicsStandard : CTCPhysicsBase {
    unsigned char _standard[0x48];          // +0x40 PPhysicsMeshInfo, OffsetVector3D, RHSet, OldRHSet, flags
};

struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity;              // +0x88
    C3DVector ForcedMoveToPos;              // +0x94
    bool ForcedMoveToPosSet : 1;     // +0xa0 bit 0
    bool ForcedMoveToPosUsePhysics : 1;   // +0xa0 bit 1
    float RenderZOffset;                    // +0xa4
    float LastRenderZOffset;                // +0xa8
    void AddImpulseVelocity(const C3DVector& v);
    void ClearMovement();
    void SetForcedMoveToPos(const C3DVector& pos, bool usePhysics);
};

void CTCPhysicsControlled::ClearMovement()
{
    ImpulseVelocity.X = ImpulseVelocity.Y = ImpulseVelocity.Z = 0.0f;
    ForcedMoveToPosSet = false;
    ForcedMoveToPosUsePhysics = true;
    Velocity_X = Velocity_Y = Velocity_Z = 0.0f;
}
