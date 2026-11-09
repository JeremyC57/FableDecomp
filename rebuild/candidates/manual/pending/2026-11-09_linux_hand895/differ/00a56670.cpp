// GFGetHermiteSplinePosition(C3DVector& result, const C3DVector& v1, const C3DVector& t1, const C3DVector& v2, const C3DVector& t2, float s) -- cubic Hermite interpolation: result = (2s^3-3s^2+1)*v1 + (3s^2-2s^3)*v2 + (s^3-2s^2+s)*t1 + (s^3-s^2)*t2
// Grade: DIFFER(295v295) -- 4 lines: the final result copy: retail issues `mov eax,[esp+14h] ; mov edx,[esp+18h]` after the last `fadd [esp+34h]` and `mov [ecx+4],edx` after the Z `fstp` (0x00A56779..0x00A5678B); ours issues the loads one slot earlier. chunk 895 (8 probes): shared engine/C3DVector.h (same), a named result temporary, grouping the sum (same); one more PAREN tuple anywhere earlier (h1, h3, s3, an inline (s3 - s2)) moves the loads EARLIER still (6 lines), so retail's region has one tuple FEWER before this point -- look for a spelling with one less no-code tuple (a named local folded away) rather than more parentheses.
// Leaf: ?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56670, 295 bytes, `__fastcall` (result in ecx, v1 in edx, the other
//   three references and s on the stack, `ret 10h`).
// Identity: Ego_r.pdb / FableWin.pdb ?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z; the pair is donor-ordered
//   GFDXGetHermiteSplinePosition (0x00A565C0 in retail: forwards to the D3DXVec3Hermite import 0x00BFEFDA with the same
//   argument order) then GFGetHermiteSplinePosition (this row, the plain-C++ twin). Only caller:
//   CCardinalSpline::GetSplinePosition 0x00A567A0. Argument order follows D3DXVec3Hermite (V1, T1, V2, T2, s).
// Constants: 0x0122DED4 = 3.0f, 0x0122DED8 = 1.0f.
// Shape: the four basis weights as named floats (s^2 / s^3 kept on the x87 stack), then one expression of inline
//   C3DVector operator* / operator+ temporaries; VC evaluates the right operand of each `+` first, hence the
//   t2, t1, v2, v1 product order.
#include "engine/C3DVector.h"

void __fastcall GFGetHermiteSplinePosition(C3DVector& result, const C3DVector& v1, const C3DVector& t1,
                                           const C3DVector& v2, const C3DVector& t2, float s)
{
    float s2 = s * s;
    float s3 = s * s2;
    float two_s3 = s3 + s3;
    float three_s2 = s2 * 3.0f;
    float h1 = two_s3 - three_s2 + 1.0f;
    float h2 = three_s2 - two_s3;
    float h3 = s3 - (s2 + s2) + s;
    float h4 = s3 - s2;
    result = v1 * h1 + v2 * h2 + t1 * h3 + t2 * h4;
}
