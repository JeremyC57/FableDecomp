// CTCPhysicsControlled::AddImpulseVelocity(const C3DVector& v) -- ImpulseVelocity += v
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725250, 51 bytes, CTCPhysicsControlled vtable 0x0126616C slot 81 (catalogued as vfunc_81). No callees.
// Layout: CTCPhysicsStandard (engine/CTCPhysicsStandard.h, 0x88) -> CTCPhysicsControlled (PDB 0xac, no engine header):
//   ImpulseVelocity +0x88, ForcedMoveToPos +0x94, {ForcedMoveToPosSet:1, ForcedMoveToPosUsePhysics:1} +0xa0,
//   RenderZOffset +0xa4, LastRenderZOffset +0xa8 (PDB names; C3DVector modelled locally with its inline +=).
// Shape: the inline C3DVector::operator+= (X member-first, Y/Z argument-first is the compiler's own operand choice).
// Leaf: AddImpulseVelocity
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/CTCPhysicsStandard.h"

#include "engine/C3DVector.h"

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

void CTCPhysicsControlled::AddImpulseVelocity(const C3DVector& v)
{
    ImpulseVelocity += v;
}