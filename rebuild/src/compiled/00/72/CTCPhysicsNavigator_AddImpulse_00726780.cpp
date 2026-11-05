// CTCPhysicsNavigator::AddImpulse(const C3DVector& impulse) -- AdditionalImpulse += impulse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00726780, 51 bytes, CTCPhysicsNavigator vtable 0x01266314 slot 81 (catalogued as vfunc_81;
//   CTCPhysicsControlled 0x00725250 is the same slot on ImpulseVelocity). No callees.
// Layout: engine/CTCPhysicsNavigator.h (packed 1: AdditionalImpulse_X/Y/Z at +0x95/+0x99/+0x9d), viewed as a C3DVector.
// Shape: the inline C3DVector += (X member-first, Y/Z argument-first is the compiler's operand choice).
// Leaf: AddImpulse
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/CTCPhysicsNavigator.h"

#include "engine/C3DVector.h"

struct CTCPhysicsNavigator_Methods : CTCPhysicsNavigator {
    C3DVector& AdditionalImpulseVector() { return *reinterpret_cast<C3DVector*>(&AdditionalImpulse_X); }
    void AddImpulse(const C3DVector& impulse);
};

void CTCPhysicsNavigator_Methods::AddImpulse(const C3DVector& impulse)
{
    AdditionalImpulseVector() += impulse;
}