// NShape::IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c) -- true when the normalised c is closer in direction to both a and b than a and b are to each other
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- C3DVector gets its memberwise user copy constructor (X, Y, Z), so the third (stack-parameter) copy loads its pointer into eax and stores X first as retail; the implicit POD copy picked ecx
// Leaf: ?IsVectorBetween2OthersInThePlane@NShape@@YI_NABVC3DVector@@00@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075FF00, 221 bytes (+int3 pad to 224), `ret 4`.
// Identity: Ego_r.pdb ?IsVectorBetween2OthersInThePlane@NShape@@YI_NABVC3DVector@@00@Z (a __fastcall free function:
//   a in ecx, b in edx, c on the stack). Callers: 0x00760842 and four absolute references (function-pointer uses).
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (three times).
// Shape: three local copies (C3DVector's memberwise copy ctor, through integer registers) normalised in place, then
//   `dot(a,c) > dot(a,b) && dot(b,c) > dot(a,b)` with dot(a,b) kept on the x87 stack across both compares (the
//   `fstp st(0)` on each exit) and C3DVector::Dot's products evaluated z, y, x.
#include "engine/C3DVector.h"

namespace NShape {
bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c);

bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c)
{
    C3DVector na = a;
    na.AccurateNormalise();
    C3DVector nb = b;
    nb.AccurateNormalise();
    C3DVector nc = c;
    nc.AccurateNormalise();
    float ab = na.Dot(nb);
    return na.Dot(nc) > ab && nb.Dot(nc) > ab;
}
}