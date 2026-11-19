// C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool transposed) const --
// Grade: DIFFER(551v551) -- lane gap: x87 commutative operand order + frame-slot order of the second inlined copy -- both SSE kernels (GF_SSE_MATRIX_PRESCALE_TRANSPOSED / GF_SSE_MATRIX_PRESCALE) byte-exact; the AlignedPreScale copy homes self/t0..t2 in reverse slot order (retail uses the same order as the first copy), and six x87 products load the other operand first
//   scale each bone matrix by its morph vector (per column when transposed, per row otherwise); retail 0x00A9D750, 551 bytes, `ret 10h`.
//   Both CMatrix4x4 members are expanded in place (__forceinline here; plain inline leaves them called).
// Retail-asm: 0x00A9D7C1 `mov edx,[esp+10h]` + 0x00A9D7C5 `movaps xmm3,[esp+20h]` (GF_SSE_MATRIX_PRESCALE_TRANSPOSED) and 0x00A9D8C5 `mov edx,[esp+10h]` + 0x00A9D8E3 `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE); NOTES sites 1-2
// Leaf: ?ApplyBoneMorphScaling@C3DAnimationManager@@ABEXPBVC3DVector@@PAVCMatrix4x4@@K_N@Z
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
extern bool g_SSEAvailable;                          // 0x013D2880
class C3DAnimationManager {
    void ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool transposed) const;
};

__forceinline void CMatrix4x4::AlignedPreScale(const C3DVector& scale)
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

__forceinline void CMatrix4x4::AlignedPreScaleTransposed(const C3DVector& scale)
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
    E31 *= scale.X; E32 *= scale.Y; E33 *= scale.Z;
}

void C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool transposed) const
{
    if (transposed) {
        for (unsigned long i = 0; i < count; ++i)
            matrices[i].AlignedPreScaleTransposed(morphs[i]);
    } else {
        for (unsigned long i = 0; i < count; ++i)
            matrices[i].AlignedPreScale(morphs[i]);
    }
}
