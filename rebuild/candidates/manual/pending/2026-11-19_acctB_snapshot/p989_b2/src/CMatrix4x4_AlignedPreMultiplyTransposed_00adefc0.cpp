// CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4&) -- this = this * matrix through an aligned copy of *this (the out-of-line copy)
// Grade: RELOCATION_MATCH
// Leaf: ?AlignedPreMultiplyTransposed@CMatrix4x4@@QAEXABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADEFC0, 471 bytes, `ret 4`.
// Name: FableWin.pdb / Ego_r.pdb ?AlignedPreMultiplyTransposed@CMatrix4x4@@QAEXABV1@@Z; Ego_d.pdb record (debug length 70, in
//   Engine_Primitive_Manager_Animated_Meshes.obj): param `matrix`, local `m` (CMatrix4x4).
// Retail-asm: 0x00ADF006 `movaps xmm0,[edx]` .. 0x00ADF0E7 `movaps [edi+30h],xmm4` (GF_SSE_MATRIX_MULTIPLY of engine/CMatrix4x4AlignedSSE.h,
//   expanded through the inline AlignedSetMultiplyTransposed -> AlignedSetMultiply).
// Body: the header's inline text (engine/CMatrix4x4.h FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED), out of line here; the
//   multiply is the Ego_d AlignedSetMultiply of FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD (its x87 term order is retail's in this
//   context-free copy, which is what pins that body for every expander).
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_DEFINITION
#include "engine/CMatrix4x4.h"

void CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4& matrix)
{
    __declspec(align(16)) CMatrix4x4 m;
    m = *this;
    AlignedSetMultiplyTransposed(matrix, m);
}
