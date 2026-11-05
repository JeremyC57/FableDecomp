// C3DCoordI::operator-=(const C3DVector& v) -- subtract a float vector from the integer coordinate (X and Y; retail ADDS the Z component), truncating each result back to long
// Leaf: ??ZC3DCoordI@@QAEABV0@ABVC3DVector@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13C20, 57 bytes, `ret 4`. Name: Ego_r/FableWin PDB
//   ??ZC3DCoordI@@QAEABV0@ABVC3DVector@@@Z (const C3DCoordI& result), right after operator+= (landed 0x00A13BE0) as in
//   the PDB pair ??Y / ??Z. Callee: __ftol2 @ 0x00BFEA70 (x3).
// Shape: as operator+= with `fsub` for X and Y; the Z line is `fadd` in retail (a source slip: Z = Z + v.Z). Retail also
//   holds three bytes no compiler emits between the last store and `pop edi` (0F 5F C1 = `maxps xmm0,xmm1`, 0x00A13C4F).
// Retail-asm: 0x00A13C4F holds `maxps xmm0, xmm1` (0F 5F C1) after the Z store of an otherwise plain x87 body; no C++ route
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/C3DCoordIStrayMaxps.h (byte-matching path + FABLE_PORTABLE twin);
//   the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family MAXPS_STRAY.
//   emits an SSE instruction on registers the function never loads (an _mm_max_ps intrinsic needs __m128 operands it would
//   load). The sequence is unique in Fable.exe, so it may be a post-build patch rather than source: see NOTES.md.
#include "engine/C3DVector.h"

#include "engine/C3DCoordIStrayMaxps.h"
class C3DCoordI {
public:
    long X, Y, Z;
    const C3DCoordI& operator-=(const C3DVector& v);
};

const C3DCoordI& C3DCoordI::operator-=(const C3DVector& v)
{
    X = (long)((float)X - v.X);
    Y = (long)((float)Y - v.Y);
    Z = (long)((float)Z + v.Z);
    GF_STRAY_MAXPS_XMM0_XMM1();                     // the stray retail `maxps` (dead SSE registers: no effect to model)
    return *this;
}