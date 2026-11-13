// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& mesh_primitive, const C3DTextureStages& texture_stages) -- draw every static block that is not flagged DegenerateTriangles through the primitive's index buffer, first binding the block's diffuse texture (change flag 1) and bump texture (change flag 2, when the bump stage is in use)
// Leaf: ?RenderMeshPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9E5B0, 402 bytes (extent 0x00B9E5B0..0x00B9E741 `ret`), private static __fastcall (mesh_primitive ecx,
//   texture_stages edx).
// Identity: Ego_r.pdb / FableWin.pdb ?RenderMeshPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@
//   ABVC3DTextureStages@@@Z (FableWin 02e9fba0); caller: the inline RenderMeshPrimitiveBlocks expanded in RenderMeshPrimitive 0x00BA5260.
// Callees: AttachTextureToStage (inline, CRenderManagerCore), the six-argument CShaderRenderManager::DrawBuffersIndexed (inline;
//   SetIndices, RealiseRenderState 0x00A058C0, DrawIndexedPrimitive). FableWin accessors CBaseBlock::GetDegenerateTriangles /
//   GetChangeFlags / GetStartIndex / GetPrimitiveCount / GetPrimitiveType (opt-in FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW)
//   and C3DPrimitive2::GetVertexCount (existing FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS): the draw arguments go straight into the call
//   (named locals or the plain VertexCount field read give other register choices: DIFFER 403v402 / 402v402).
// Same retail TU as RenderMeshPrimitive 0x00BA5260 (whose row source carries this body).
#include "fable_compiler.h"
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_RENDERMESHPRIMITIVE
#include "engine/EPrimitiveType.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CTexture.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14

void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& mesh_primitive,
                                                                                              const C3DTextureStages& texture_stages)
{
    unsigned long no_blocks = mesh_primitive.GetStaticBlockCount();
    for (unsigned long c_block = 0; c_block < no_blocks; ++c_block) {
        N3DPrimitive::CStaticBlock& block_info = *mesh_primitive.GetStaticBlock(c_block);
        if (!block_info.GetDegenerateTriangles()) {
            if (block_info.GetChangeFlags() & 1)
                NEngineInternal::GPRenderManager->AttachTextureToStage(texture_stages.DiffuseTextureStage, block_info.DiffuseTexture);
            if ((block_info.GetChangeFlags() & 2) && texture_stages.BumpTextureStage != -1)
                NEngineInternal::GPRenderManager->AttachTextureToStage(texture_stages.BumpTextureStage, block_info.BumpTexture);
            NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, mesh_primitive.GetVertexCount(), *mesh_primitive.IndexBuffer,
                block_info.GetStartIndex(), block_info.GetPrimitiveCount(), block_info.GetPrimitiveType());
        }
    }
}
