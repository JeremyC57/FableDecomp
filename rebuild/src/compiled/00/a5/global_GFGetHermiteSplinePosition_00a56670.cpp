// GFGetHermiteSplinePosition(C3DVector& dest, const C3DVector& from_pos, const C3DVector& from_tan, const C3DVector& to_pos, const C3DVector& to_tan, float s) -- cubic Hermite interpolation: dest = (2s^3-3s^2+1)*from_pos + (3s^2-2s^3)*to_pos + (s^3-2s^2+s)*from_tan + (s^3-s^2)*to_tan
// Leaf: ?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56670, 295 bytes, `__fastcall` (dest in ecx, from_pos in edx, the other three references
//   and s on the stack, `ret 10h`).
// Identity: Ego_r.pdb / FableWin.pdb ?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z; the pair is donor-ordered
//   GFDXGetHermiteSplinePosition (0x00A565C0 in retail: forwards to the D3DXVec3Hermite import 0x00BFEFDA with the same argument order)
//   then GFGetHermiteSplinePosition (this row, the plain-C++ twin). Only caller: CCardinalSpline::GetSplinePosition 0x00A567A0.
// Names: Ego_d.pdb (debug length 246): parameters dest, from_pos, from_tan, to_pos, to_tan, s; float locals s2, s3, wpos_from, wpos_to,
//   wtan_from, wtan_to.
// Shape: Ego_d.pdb C13 line table (lines 37..46), one statement per line: 38 s2 (9 bytes), 39 s3 (9), 40 wpos_from (29: two constant
//   products, a subtraction, an add of 1), 41 wpos_to (23), 42 wtan_from (18), 43 wtan_to (9), 45 the weighted sum (131). Retail folds the
//   repeated 2 * s3 and 3 * s2 itself (`fld st(0) ; fadd st(1)` 0x00A56685, `fmul [3.0]` 0x00A5668B); they are not named in the source.
// Constants: 0x0122DED4 = 3.0f, 0x0122DED8 = 1.0f.
#include "engine/C3DVector.h"

void FABLE_FASTCALL GFGetHermiteSplinePosition(C3DVector& dest, const C3DVector& from_pos, const C3DVector& from_tan,
                                           const C3DVector& to_pos, const C3DVector& to_tan, float s)
{
    float s2 = s * s;
    float s3 = s2 * s;
    float wpos_from = 2.0f * s3 - 3.0f * s2 + 1.0f;
    float wpos_to = 3.0f * s2 - 2.0f * s3;
    float wtan_from = s3 - 2.0f * s2 + s;
    float wtan_to = s3 - s2;
    dest = from_pos * wpos_from + to_pos * wpos_to + from_tan * wtan_from + to_tan * wtan_to;
}