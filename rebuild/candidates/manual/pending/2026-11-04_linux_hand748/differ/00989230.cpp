// CShaderRenderManager::UpdateObjectSpaceTextureTransform() -- for every in-use texture-transform range of the current layout upload
// Grade: DIFFER(636v636) -- lane gap: x87 term order of the inlined CMatrix4x4::AlignedSetMultiply fallback only -- the SSE kernel (GF_SSE_MATRIX_MULTIPLY, operands homed in the branch), the loop, frame and upload match; retail sums each column as 3,1,2,0 (column 2: 0,3,1,2) while every single-expression form is canonicalised to one order (96 + 96 + 144 body forms tried)
//   TextureTransforms[i] * WorldMatrix (at most four rows) and move the dirty bit 0x800 to 0x1000; retail 0x00989230, 636 bytes.
// Retail-asm: 0x009892A5 `mov edi,[esp+24h]` + 0x009892A9 `movaps xmm0,[edx]` + `shufps xmm4,xmm4,00h` / `addps xmm4,xmm6` (GF_SSE_MATRIX_MULTIPLY via AlignedSetMultiply; NOTES site 3)
// Leaf: ?UpdateObjectSpaceTextureTransform@CShaderRenderManager@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CMATRIX4X4
#include "engine/CShaderRenderManager.h"
inline void CShaderRenderManager::SetVSConstants(long start,const void* data,unsigned long count)
{
    if(count!=0) D3DDevice->SetVertexShaderConstantF(start,(const float*)data,count);
}
void CShaderRenderManager::UpdateObjectSpaceTextureTransform()
{
    for (unsigned long i = 0; i < (unsigned long)TextureTransformCount; ++i) {
        const CShaderConstantRange& range = CurrentVSConstantLayout->TextureTransform[i];
        if (range.Count) {
            __declspec(align(16)) CMatrix4x4 transform;
            transform.AlignedSetMultiply(TextureTransforms[i], WorldMatrix);
            SetVSConstants(range.Offset, &transform, range.Count < 4 ? range.Count : 4);
        }
    }
    UpdateFlags = (UpdateFlags & ~0x800) | 0x1000;
}
