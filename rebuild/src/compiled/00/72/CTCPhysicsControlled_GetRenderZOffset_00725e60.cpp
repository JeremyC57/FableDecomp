// CTCPhysicsControlled::GetRenderZOffset(float t) -- the render Z offset interpolated between last frame's and this frame's value: (RenderZOffset - LastRenderZOffset) * t + LastRenderZOffset
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725E60, 25 bytes, CTCPhysicsControlled vtable 0x0126616C slot 79 (catalogued as vfunc_79).
// Layout: CTCPhysicsStandard (engine/CTCPhysicsStandard.h, 0x88) -> CTCPhysicsControlled (PDB 0xac, no engine header):
//   ImpulseVelocity +0x88, ForcedMoveToPos +0x94, {ForcedMoveToPosSet:1, ForcedMoveToPosUsePhysics:1} +0xa0,
//   RenderZOffset +0xa4, LastRenderZOffset +0xa8 (PDB names). No callees.
// Shape: `fld [a4] ; fsub [a8] ; fmul t ; fadd [a8]` -- the plain lerp written left to right.
// Leaf: GetRenderZOffset
#include "engine/CTCPhysicsStandard.h"

struct CTCPhysicsControlled : CTCPhysicsStandard {
    float ImpulseVelocity_X;                // +0x88
    float ImpulseVelocity_Y;                // +0x8c
    float ImpulseVelocity_Z;                // +0x90
    float ForcedMoveToPos_X;                // +0x94
    float ForcedMoveToPos_Y;                // +0x98
    float ForcedMoveToPos_Z;                // +0x9c
    unsigned char ForcedMoveToPosSet : 1;   // +0xa0 bit 0
    unsigned char ForcedMoveToPosUsePhysics : 1; // +0xa0 bit 1
    float RenderZOffset;                    // +0xa4
    float LastRenderZOffset;                // +0xa8
    float GetRenderZOffset(float t) const;
};

float CTCPhysicsControlled::GetRenderZOffset(float t) const
{
    return (RenderZOffset - LastRenderZOffset) * t + LastRenderZOffset;
}