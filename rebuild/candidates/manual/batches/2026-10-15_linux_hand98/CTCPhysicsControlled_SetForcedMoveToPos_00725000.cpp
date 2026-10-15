// CTCPhysicsControlled::SetForcedMoveToPos(const C3DVector& pos, bool usePhysics) -- copy the target position, store the use-physics bit and set ForcedMoveToPosSet
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725000, 57 bytes, CTCPhysicsControlled vtable 0x0126616C slot 34 (catalogued as vfunc_34). No callees.
// Layout: CTCPhysicsStandard (engine/CTCPhysicsStandard.h, 0x88) -> CTCPhysicsControlled (PDB 0xac, no engine header):
//   ImpulseVelocity +0x88, ForcedMoveToPos +0x94, {ForcedMoveToPosSet:1, ForcedMoveToPosUsePhysics:1} +0xa0,
//   RenderZOffset +0xa4, LastRenderZOffset +0xa8 (PDB names; C3DVector modelled locally with its inline +=).
// Shape: dword copy of the vector through esi/edx, then the bit insert (shl/xor/and/xor) and the |1 in one byte store.
// Leaf: SetForcedMoveToPos
#include "engine/CTCPhysicsStandard.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity;              // +0x88
    C3DVector ForcedMoveToPos;              // +0x94
    unsigned char ForcedMoveToPosSet : 1;   // +0xa0 bit 0
    unsigned char ForcedMoveToPosUsePhysics : 1; // +0xa0 bit 1
    float RenderZOffset;                    // +0xa4
    float LastRenderZOffset;                // +0xa8
    void AddImpulseVelocity(const C3DVector& v);
    void ClearMovement();
    void SetForcedMoveToPos(const C3DVector& pos, bool usePhysics);
};

void CTCPhysicsControlled::SetForcedMoveToPos(const C3DVector& pos, bool usePhysics)
{
    ForcedMoveToPos = pos;
    ForcedMoveToPosUsePhysics = usePhysics;
    ForcedMoveToPosSet = true;
}
