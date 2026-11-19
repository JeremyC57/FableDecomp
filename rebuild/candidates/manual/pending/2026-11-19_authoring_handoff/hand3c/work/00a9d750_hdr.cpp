// C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* morphs, CMatrix4x4* matrices, unsigned long count, bool transposed) const --
//   scale each bone matrix by its morph vector (per column when transposed, per row otherwise); retail 0x00A9D750, 551 bytes, `ret 10h`.
//   Both CMatrix4x4 members are expanded in place (__forceinline here; plain inline leaves them called).
// Retail-asm: 0x00A9D7C1 `mov edx,[esp+10h]` + 0x00A9D7C5 `movaps xmm3,[esp+20h]` (GF_SSE_MATRIX_PRESCALE_TRANSPOSED) and 0x00A9D8C5 `mov edx,[esp+10h]` + 0x00A9D8E3 `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE); NOTES sites 1-2
// Leaf: ?ApplyBoneMorphScaling@C3DAnimationManager@@ABEXPBVC3DVector@@PAVCMatrix4x4@@K_N@Z
// Flags: none
#include "fable_compiler.h"
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_FORCEINLINE
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
#include "engine/C3DAnimationManager.h"

void C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* bone_morphs, CMatrix4x4* dest, unsigned long no_bones, bool transpose) const
{
    if (transpose) {
        if (false) {} else for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            dest[c_bone].AlignedPreScaleTransposed(bone_morphs[c_bone]);
    } else {
        if (false) {} else for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
            dest[c_bone].AlignedPreScale(bone_morphs[c_bone]);
    }
}
