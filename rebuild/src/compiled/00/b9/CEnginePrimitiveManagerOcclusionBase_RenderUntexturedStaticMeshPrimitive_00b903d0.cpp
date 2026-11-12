// CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive(C3DPrimitive2* primitive, const CMatrix3x4& transform)
// -- draw every static block of a mesh primitive with the manager's occlusion vertex shader and no textures: apply the shader, set
// the world transform (fixed-function core or shader path), flush the dirty shader state, then one indexed draw per static block.
// Leaf: ?RenderUntexturedStaticMeshPrimitive@CEnginePrimitiveManagerOcclusionBase@@AAEXPAVC3DPrimitive2@@ABVCMatrix3x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B903D0, length 464 bytes, `ret 8`.
// Callees: CShaderRenderManager::ApplyVertexShader 0x00988020, CRenderManagerCore::SetWorldTransform 0x00A0B140 /
//   CShaderRenderManager::SetWorldTransform 0x009881F0 (by IsFixedFunctionVertexMode), the expanded PrepareForRender(1)
//   (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760,
//   UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50), the expanded
//   seven-argument DrawBuffersIndexed (IDirect3DDevice9 SetStreamSource +0x190, SetIndices +0x1A0, DrawIndexedPrimitive +0x148,
//   CRenderStateManager::RealiseRenderState 0x00A058C0).
// Globals: NEngineInternal::GPShaderRenderManager 0x01436E14, g_pRenderManagerCore 0x01436E18.
// Layout: engine headers (VertexShader +0xC; C3DPrimitive2 VertexBuffer +0x30, IndexBuffer +0x34, StaticBlockCount +0x40,
//   StaticBlocks +0x44, VertexCount +0x58; CShaderRenderManager RenderManager +0x1D0, D3DDevice +0x1D4, FixedFunctionVertexMode
//   +0x395, UpdateFlags +0x3D8).
// Shape: everything goes through the PDB's inline members (IsFixedFunctionVertexMode, PrepareForRender, GetStaticBlockCount /
//   GetVertexCount / GetStaticBlock, GetStartIndex / GetPrimitiveCount / GetPrimitiveType, SelectVertexBuffer and both
//   DrawBuffersIndexed overloads). The block count and the vertex count are read once before the loop; the manager global, the
//   block array and both buffers are re-read every iteration; the draw's by-value index buffer and primitive type are loaded
//   at the inline call (before SetStreamSource) and PT_NULL is resolved after SetIndices, as in the landed 0x00B8EB00.
#include "fable_compiler.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BUFFERDRAW
#define FABLE_RETAIL_CBASEBLOCK_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_RENDERUNTEXTURED
#include "engine/EPrimitiveType.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderManagerCore.h"
#include "engine/C3DPrimitive2.h"
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
namespace NEngineInternal {
extern CShaderRenderManager* GPShaderRenderManager;        // 0x01436E14
}
extern CRenderManagerCore* g_pRenderManagerCore;           // 0x01436E18
void CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive(C3DPrimitive2* primitive, const CMatrix3x4& transform)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    if (NEngineInternal::GPShaderRenderManager->IsFixedFunctionVertexMode())
        g_pRenderManagerCore->SetWorldTransform(transform);
    else
        NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    unsigned long block_count = primitive->GetStaticBlockCount();
    unsigned long vertex_count = primitive->GetVertexCount();
    for (unsigned long i = 0; i < block_count; ++i) {
        N3DPrimitive::CStaticBlock* block = primitive->GetStaticBlock(i);
        NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(*primitive->VertexBuffer, 0, vertex_count, *primitive->IndexBuffer,
                                                                   block->GetStartIndex(), block->GetPrimitiveCount(), block->GetPrimitiveType());
    }
}