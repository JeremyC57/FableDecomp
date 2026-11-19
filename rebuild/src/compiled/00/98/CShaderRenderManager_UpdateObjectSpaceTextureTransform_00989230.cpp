// CShaderRenderManager::UpdateObjectSpaceTextureTransform() -- for every in-use texture-transform range of the current layout upload
//   TextureTransforms[i] * WorldMatrix (at most four rows), then move the dirty bit 0x800 to 0x1000
// Grade: RELOCATION_MATCH
// Leaf: ?UpdateObjectSpaceTextureTransform@CShaderRenderManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00989230, 636 bytes, plain `ret`.
// Locals: the Ego_d.pdb record (debug length 262): this; { c_transform (unsigned long) { range (const CShaderConstantRange&)
//   { transform (CMatrix4x4), rows (unsigned long) } } } -- exactly this text. The product is the inline AlignedSetMultiplyTransposed ->
//   AlignedSetMultiply (Ego_d body, FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD, shared with 0x00ADEFC0 / 0x00AE1320 / 0x00AE29D0 /
//   0x00A9E1E0 / 0x00BD2D90); the upload is the in-TU SetVSConstants body, verbatim the landed 0x00989CB0's in-TU copy of 0x0098B930
//   (the landed 0x0098B930 row itself is a view-struct stand-in).
// Retail-asm: 0x0098929D `mov eax,[esp+2Ch] ; mov edx,[esp+28h] ; mov edi,[esp+24h]` + 0x009892A9 `movaps xmm0,[edx]` .. `addps xmm4,xmm6`
//   (GF_SSE_MATRIX_MULTIPLY of engine/CMatrix4x4AlignedSSE.h via AlignedSetMultiply).
// Inferred: FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230 -- a named reference to rhs inside the shared x87 arm, defined
//   by this row only (advisor decision, owner delegated). VC7.1 orders the four x87 products of each element by a mod-4 count of the
//   function's live symbols; with the Ego_d body and this row's Ego_d text every byte matches except that order, and one added live
//   statement gives retail exactly (a global `if (G & K) G |= M;` at the top, before or after the flags, or a member one before them;
//   a member one after the flag pair gives a third order), i.e. the donor compiles one symbol the evidence does not name. Units (mod 4):
//   live `if (x & K) x |= M;` +1; bare store +2; the two flag statements written as one +3; Ego_d body vs ACCESSORS +1; a named reference
//   INSIDE the inlinee +1 (this opt-in); the same reference in the row 0; std::min(range.Count, 4ul) +1 but +10 bytes; a `count` local +2
//   bytes; 0 (tried, byte-neutral): HRESULT hr in SetVSConstants (3 forms), SetVSConstants placement / inline keyword, count tests,
//   loop cast, post-increment, clamp spellings (<, <=, >, >=, 4 <, 4 >, if, if-else), data-argument spellings, direct AlignedSetMultiply,
//   flags swapped; ruled out by evidence: SetVSMatrixConstantTransposed (retail 0x00988840 transposes into a local), GetD3DDevice (not an
//   accessor), PeekVertexShaderConstantLayout (returns an index). Byte-changing: flags before the loop, rows before transform.
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD_RHSREF_00989230
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#include "engine/CShaderRenderManager.h"
inline void CShaderRenderManager::SetVSConstants(long start,const void* data,unsigned long count)
{
    if(count!=0) D3DDevice->SetVertexShaderConstantF(start,(const float*)data,count);
}
void CShaderRenderManager::UpdateObjectSpaceTextureTransform()
{
    for (unsigned long c_transform = 0; c_transform < (unsigned long)TextureTransformCount; ++c_transform) {
        const CShaderConstantRange& range = CurrentVSConstantLayout->TextureTransform[c_transform];
        if (range.Count) {
            __declspec(align(16)) CMatrix4x4 transform;
            transform.AlignedSetMultiplyTransposed(WorldMatrix, TextureTransforms[c_transform]);
            unsigned long rows = range.Count < 4 ? range.Count : 4;
            SetVSConstants(range.Offset, &transform, rows);
        }
    }
    UpdateFlags |= 0x1000;
    UpdateFlags &= ~0x800;
}