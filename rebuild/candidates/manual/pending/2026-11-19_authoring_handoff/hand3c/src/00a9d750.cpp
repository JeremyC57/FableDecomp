// C3DAnimationManager::ApplyBoneMorphScaling(const C3DVector* bone_morphs, CMatrix4x4* dest, unsigned long no_bones, bool transpose) const --
//   scale each bone matrix by its morph vector: per column when transpose is set, per row otherwise. Retail 0x00A9D750, 551 bytes, `ret 10h`.
// Leaf: ?ApplyBoneMorphScaling@C3DAnimationManager@@ABEXPBVC3DVector@@PAVCMatrix4x4@@K_N@Z
// Flags: none
// Retail-asm: 0x00A9D7C1 `mov edx,[esp+10h]` + 0x00A9D7C5 `movaps xmm3,[esp+20h]` (GF_SSE_MATRIX_PRESCALE_TRANSPOSED) and 0x00A9D8C5 `mov edx,[esp+10h]` + 0x00A9D8E3 `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE); NOTES sites 1-2
// Names: Ego_d.pdb parameters bone_morphs / dest / no_bones / transpose and one `c_bone` per loop scope; Ego_r.pdb / FableWin.pdb
//   ?ApplyBoneMorphScaling@C3DAnimationManager@@ABEXPBVC3DVector@@PAVCMatrix4x4@@K_N@Z (private const, the shared C3DAnimationManager.h
//   declaration under FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS).
// Shape: both CMatrix4x4 members are expanded in place. Their bodies are the shared header text (opt-in
//   FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES, made __forceinline for this TU by FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_FORCEINLINE).
//   Out of line, the same text also gives RELOCATION_MATCH for 0x00A9D480 and 0x00A9D580.
//   Each loop is written `if (false) {} else for (...)`. FableWin 0x03382ED0 opens every for loop with `xor reg,reg ; je` (the VC6
//   for-scope idiom, `#define for if (0) {} else for`), and Ego_d gives each loop its own c_bone scope. With plain `for` the c2 record
//   ids leave one x87 operand pair swapped: E13 of the inline AlignedPreScale at 0x00A9D927.
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
