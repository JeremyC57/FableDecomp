// C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool transposed) const --
//   scale each bone matrix by its morph vector (per column when transposed, per row otherwise); retail 0x00A9D750, 551 bytes, `ret 10h`.
//   Both CMatrix4x4 members are expanded in place (__forceinline here; plain inline leaves them called).
// Retail-asm: 0x00A9D7C1 `mov edx,[esp+10h]` + 0x00A9D7C5 `movaps xmm3,[esp+20h]` (GF_SSE_MATRIX_PRESCALE_TRANSPOSED) and 0x00A9D8C5 `mov edx,[esp+10h]` + 0x00A9D8E3 `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE); NOTES sites 1-2
// Leaf: ?ApplyBoneMorphScaling@C3DAnimationManager@@ABEXPBVC3DVector@@PAVCMatrix4x4@@K_N@Z
// Flags: none
#include "fable_compiler.h"
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
extern bool g_SSEAvailable;                          // 0x013D2880
class C3DAnimationManager {
    void ApplyBoneMorphScaling(const C3DVector* bone_morphs, CMatrix4x4* dest, unsigned long no_bones, bool transpose) const;
};

FABLE_FORCEINLINE void CMatrix4x4::AlignedPreScale(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector vect(scale, 0.0f);
        float* dest = &E11;
        C3DVector right(E14, E24, E34);
        GF_SSE_MATRIX_PRESCALE(dest, vect);
        E14 = right.X; E24 = right.Y; E34 = right.Z;
        return;
    }
    CMatrix4x4& m = *this;
    m(0, 0) *= scale.X; m(0, 1) *= scale.X; m(0, 2) *= scale.X;
    m(1, 0) *= scale.Y; m(1, 1) *= scale.Y; m(1, 2) *= scale.Y;
    m(2, 0) *= scale.Z; m(2, 1) *= scale.Z; m(2, 2) *= scale.Z;
}

FABLE_FORCEINLINE void CMatrix4x4::AlignedPreScaleTransposed(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector vect(scale, 0.0f);
        float* dest = &E11;
        C3DVector right(E14, E24, E34);
        GF_SSE_MATRIX_PRESCALE_TRANSPOSED(dest, vect);
        E14 = right.X; E24 = right.Y; E34 = right.Z;
        return;
    }
    CMatrix4x4& m = *this;
    m(0, 0) *= scale.X; m(0, 1) *= scale.Y; m(0, 2) *= scale.Z;
    m(1, 0) *= scale.X; m(1, 1) *= scale.Y; m(1, 2) *= scale.Z;
    m(2, 0) *= scale.X; m(2, 1) *= scale.Y; m(2, 2) *= scale.Z;
}

void C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* bone_morphs, CMatrix4x4* dest, unsigned long no_bones, bool transpose) const
{
    if (transpose) {
        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            dest[c_bone].AlignedPreScaleTransposed(bone_morphs[c_bone]);
    } else {
        for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            dest[c_bone].AlignedPreScale(bone_morphs[c_bone]);
    }
}
