// CMatrix4x4::AlignedPreScale(const C3DVector& scale) -- multiply row r of the 3x3 part by scale[r], the fourth column kept; SSE kernel when
// Grade: DIFFER(243v243) -- lane gap: x87 commutative operand order only -- SSE arm byte-exact with GF_SSE_MATRIX_PRESCALE; the fallback fld/fmul pairs of E13, E22, E23, E31 come out swapped (retail swaps only E12); 125 + 729 row-form combinations (unrolled / column loops / row pointers) and 120 prologue permutations reach at most 8 of 9 products
//   the processor has SSE, x87 otherwise. Retail 0x00A9D480, 243 bytes, `ret 4`. Name: FableWin.pdb ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
//   (484 debug bytes, the larger Aligned PreScale member; FableWin order ApplyBoneMorphScaling, AlignedPreScale, AlignedPreScaleTransposed).
// kept); SSE when the processor has it, x87 otherwise
// Retail-asm: 0x00A9D4CD `mov edx,[esp]` + 0x00A9D4D0 `movaps xmm3,[esp+10h]` + 0x00A9D4EA `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE; NOTES site 2)
// Leaf: ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
extern bool g_SSEAvailable;                          // 0x013D2880

void CMatrix4x4::AlignedPreScale(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector vect(scale, 0.0f);
        C3DVector right(E14, E24, E34);
        float* dest = &E11;
        GF_SSE_MATRIX_PRESCALE(dest, vect);
        E14 = right.X; E24 = right.Y; E34 = right.Z;
        return;
    }
    CMatrix4x4& m = *this;
    m(0, 0) *= scale.X;
    m(0, 1) *= scale.X;
    m(0, 2) *= scale.X;
    m(1, 0) *= scale.Y;
    m(1, 1) *= scale.Y;
    m(1, 2) *= scale.Y;
    m(2, 0) *= scale.Z;
    m(2, 1) *= scale.Z;
    m(2, 2) *= scale.Z;
}
