// operator*(const C3DVector& v, float s) -- a 3D vector scaled by a float, returned by value
// Leaf: ??D@YI
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C258A0, 33 bytes (`ret 4`). The manifest's "GetNewCameraPosFromOcclusionParams"
//   is propagated: the body is the vector-times-scalar operator of the 0x00C25xxx C3DVector helpers (next to the
//   landed operator- 0x00C257B0, operator+ 0x00C257E0, CrossProduct 0x00C25860); eight callers in 0x00C46xxx.
// Shape: `__fastcall` free operator (as the landed operator+/-): the hidden result pointer in ecx (moved to eax), the
//   vector in edx, the scalar on the stack. C3DVector has a user constructor, so it returns through the hidden pointer;
//   the constructor's arguments are evaluated right to left (Z, Y, X products, each `fld s ; fmul [v.c]`, i.e.
//   `v.c * s` with the right operand loaded first) and stored X, Y, Z as the three `fstp` pop the stack.
#include "engine/C3DVector.h"

C3DVector __fastcall operator*(const C3DVector& v, float s)
{
    return C3DVector(v.X * s, v.Y * s, v.Z * s);
}