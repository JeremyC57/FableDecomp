// CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4*
// bone_matrices) -- for every animated block of the primitive: gather the block's bone groups from the 64-byte matrix array
// into the manager's 48-byte palette, upload them as the bone constant rows of the lights-and-bones vertex-shader layout, then
// bind the primitive's index buffer and draw the block (triangle strip or list).
// Leaf: ?RenderPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerAnimatedMeshesBase@@AAEXAAVC3DPrimitive2@@PBVCMatrix4x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCFB00, length 336 bytes, `ret 8`.
// Callees: CShaderRenderManager::SetVSConstants 0x0098B930 (through the expanded SetBoneTransforms), ~CBaseClass 0x0099A2E0 (the
// GetBoneMatrices() temporary, vftable 0x0129A04C), CRenderStateManager::RealiseRenderState 0x00A058C0, IDirect3DDevice9::SetIndices
// (+0x1A0) and DrawIndexedPrimitive (+0x148) (the expanded six-argument DrawBuffersIndexed).
// Globals: NEngineInternal::GPResourceManager 0x01436E98 (VSConstantLayoutLightsBones +0x2D7C, its BoneMatrices +0xD4),
// NEngineInternal::GPShaderRenderManager 0x01436E14 (RenderManager +0x1D0, D3DDevice +0x1D4).
// Layout: engine/C3DPrimitive2.h (IndexBuffer +0x34, AnimatedBlockCount +0x48, AnimatedBlocks +0x4C, VertexCount +0x58),
// N3DPrimitive::CAnimatedBlock (0x30 bytes: PrimitiveCount +0, StartIndex +4, IsStrip +8, GroupCount +0x17, Groups +0x18); the
// manager's BoneMatrixBuffer +0x330 (Ego_r.pdb name; the class has no shared header).
// Shape: everything goes through the PDB's inline accessors (GetAnimatedBlock / GetGroupCount / GetGroup / GetPrimitiveType ...).
// The range temporary is copied and destroyed by the implicit members (one vftable store, then only ~CBaseClass). The draw's
// vertex count, primitive count and start index are read into locals in that order before the call: it fixes retail's load order
// (`mov eax,[ebx] ; mov edx,[esi+58h] ; mov ecx,[ebx+4]`); passing the accessors directly loads the start index first.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTLIGHTSBONES
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETBONEMATRICES_INLINE
#define FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTLIGHTSBONES
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CBASEBLOCK_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#include "engine/EPrimitiveType.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineVSConstantLayoutLightsBones.h"
#include "engine/CEngineResourceManager.h"
#include "engine/C3DPrimitive2.h"
namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;          // 0x01436E98
extern CShaderRenderManager* GPShaderRenderManager;        // 0x01436E14
}
class CEnginePrimitiveManagerAnimatedMeshesBase {          // no shared header: Ego_r.pdb BoneMatrixBuffer (retail +0x330)
private:
    void RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* bone_matrices);   // 0x00BCFB00
    unsigned char _pad_0x000[0x330];                       // PointLightShaderIndex .. TextureOverlayFadeDistandeFactor
    CPreTransposedBoneMatrix BoneMatrixBuffer[18];         // +0x330
};
void CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* bone_matrices)
{
    unsigned long block_count = primitive.GetAnimatedBlockCount();
    for (unsigned long i = 0; i < block_count; ++i) {
        N3DPrimitive::CAnimatedBlock* block = primitive.GetAnimatedBlock(i);
        unsigned long group_count = block->GetGroupCount();
        for (unsigned long j = 0; j < group_count; ++j)
            BoneMatrixBuffer[j] = *reinterpret_cast<const CPreTransposedBoneMatrix*>(&bone_matrices[block->GetGroup(j)]);
        NEngineInternal::GPShaderRenderManager->SetBoneTransforms(
            NEngineInternal::GPResourceManager->PeekVSConstantLayoutLightsBones().GetBoneMatrices().Offset, group_count, BoneMatrixBuffer);
        unsigned long vertex_count = primitive.GetVertexCount();
        unsigned long primitive_count = block->GetPrimitiveCount();
        unsigned long start_index = block->GetStartIndex();
        NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, vertex_count, *primitive.IndexBuffer, start_index, primitive_count, block->GetPrimitiveType());
    }
}