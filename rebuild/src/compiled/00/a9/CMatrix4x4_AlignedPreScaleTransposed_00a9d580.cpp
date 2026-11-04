// CMatrix4x4::AlignedPreScaleTransposed(const C3DVector& scale) -- multiply column c of the 3x3 part by scale[c] (rows 0..2 times (X, Y, Z, 0) with the fourth column E14/E24/E34 saved around the product); the SSE kernel when the processor has SSE, x87 otherwise
// Leaf: ?AlignedPreScaleTransposed@CMatrix4x4@@QAEXABVC3DVector@@@Z
// Flags: none
// Retail-asm: 0x00A9D5C9 `mov edx,[esp]` + 0x00A9D5CC `movaps xmm3,[esp+10h]` + 0x00A9D5DC `mulps xmm0,xmm3` (the GF_SSE_MATRIX_PRESCALE_TRANSPOSED block of this function; C++/intrinsic table in chunk 748 NOTES, site 1)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9D580, 221 bytes, `ret 4`, `and esp,-16` frame (the aligned scale local at [esp+10h]).
// Name: FableWin.pdb ?AlignedPreScaleTransposed@CMatrix4x4@@QAEXABVC3DVector@@@Z (466 debug bytes, the smaller of the two
//   Aligned*PreScale members; FableWin order ApplyBoneMorphScaling, AlignedPreScale, AlignedPreScaleTransposed = retail's
//   0x00A9D750 neighbours 0x00A9D480 / 0x00A9D580; this body scales columns, the transposed form). The manifest's
//   "CMatrix3x4::PostScale" is a propagated label (NAME_FIXES.tsv).
// SSE: the isolated kernel GF_SSE_MATRIX_PRESCALE_TRANSPOSED (engine/CMatrix4x4AlignedSSE.h, grade retail_asm): retail homes
//   `this` in a frame word and re-reads it into edx inside the block (`mov [esp],ecx ; ... ; mov edx,[esp]`), the fourth
//   column goes through dword frame slots. Intrinsic spellings (chunk 748 NOTES table, 12 graded) never re-read the pointer.
// x87: VC7.1 picks the fld operand of each `E *= s` from the whole function's load-node order; the third row written as a
//   column loop gives retail's operand order in every product (125 row-form combinations searched; UUL family).
// Global: g_SSEAvailable 0x013D2880.
#define FABLE_RETAIL_CMATRIX4X4
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
extern bool g_SSEAvailable;                          // 0x013D2880

void CMatrix4x4::AlignedPreScaleTransposed(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector s;
        s.X = scale.X; s.Y = scale.Y; s.Z = scale.Z;
        s.W = 0.0f;
        CMatrix4x4* self = this;
        float t0 = E14, t1 = E24, t2 = E34;
        GF_SSE_MATRIX_PRESCALE_TRANSPOSED(self, s);
        E14 = t0; E24 = t1; E34 = t2;
        return;
    }
    E11 *= scale.X; E12 *= scale.Y; E13 *= scale.Z;
    E21 *= scale.X; E22 *= scale.Y; E23 *= scale.Z;
    for (int c = 0; c < 3; ++c) Data[2][c] *= (&scale.X)[c];
}