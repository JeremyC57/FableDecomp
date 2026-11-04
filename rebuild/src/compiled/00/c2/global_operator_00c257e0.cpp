// operator+(const C3DVector&, const C3DVector&) -- the component-wise sum of two 3D vectors, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C257E0, length 33 bytes. No callees.
// Leaf: ??H@YI
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// The manifest label "CQuadric::operator+=" is a BSim port (0.78) and is rejected (the real CQuadric is eleven
// doubles; the landed caller 0x00C62460 uses this body as C3DVector addition). Shape: `__fastcall` with the hidden
// result pointer in ecx (moved to eax), the left operand in edx, the right operand on the stack (`ret 4`).
// C3DVector has a user constructor, so it returns through the hidden pointer; the constructor's arguments are
// evaluated right to left (Z sum, Y sum, X sum) and stored X, Y, Z as the three `fstp` pop the stack.
#include "engine/C3DVector.h"

C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}