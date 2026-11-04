// CMatrix4x4::AlignedPreScale(const C3DVector& scale) -- multiply row r of the 3x3 part by scale[r], the fourth column kept; SSE kernel when
// Grade: DIFFER(243v243) -- lane gap: x87 commutative operand order only -- SSE arm byte-exact with GF_SSE_MATRIX_PRESCALE; the fallback fld/fmul pairs of E13, E22, E23, E31 come out swapped (retail swaps only E12); 125 + 729 row-form combinations (unrolled / column loops / row pointers) and 120 prologue permutations reach at most 8 of 9 products
//   the processor has SSE, x87 otherwise. Retail 0x00A9D480, 243 bytes, `ret 4`. Name: FableWin.pdb ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
//   (484 debug bytes, the larger Aligned PreScale member; FableWin order ApplyBoneMorphScaling, AlignedPreScale, AlignedPreScaleTransposed).
// kept); SSE when the processor has it, x87 otherwise
// Retail-asm: 0x00A9D4CD `mov edx,[esp]` + 0x00A9D4D0 `movaps xmm3,[esp+10h]` + 0x00A9D4EA `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE; NOTES site 2)
// Leaf: ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
extern bool g_SSEAvailable;                          // 0x013D2880

void CMatrix4x4::AlignedPreScale(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector s;
        s.X = scale.X; s.Y = scale.Y; s.Z = scale.Z;
        s.W = 0.0f;
        CMatrix4x4* self = this;
        float t0 = E14, t1 = E24, t2 = E34;
        GF_SSE_MATRIX_PRESCALE(self, s);
        E14 = t0; E24 = t1; E34 = t2;
        return;
    }
    E11 *= scale.X; E12 *= scale.X; E13 *= scale.X;
    E21 *= scale.Y; E22 *= scale.Y; E23 *= scale.Y;
    E31 *= scale.Z; E32 *= scale.Z; E33 *= scale.Z;
}
