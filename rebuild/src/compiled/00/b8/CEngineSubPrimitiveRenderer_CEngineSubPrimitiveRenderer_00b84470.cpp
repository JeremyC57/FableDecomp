// CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer() -- construct the engine's sub-primitive renderer: the class-root and engine-component bases, fifteen empty sorted+enabled render lists, the empty sort buffer, the sort distance range (10000), a cleared static scratch table, each list's render pass (pre-pass, bounding-volume and stable-sort options) and the global frame-pool pointer
// Leaf: ??0CEngineSubPrimitiveRenderer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84470, 270 bytes, thiscall, plain `ret`.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_CTOR
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memset)
#endif
#define FABLE_RETAIL_CENGINE
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngine.h"
class CSimpleMemoryPool;
namespace NEngineInternal {
extern CEngine* GPEngine;                                   // 0x01436EA4
}
extern CSimpleMemoryPool* PFrameMemoryPool_0143bac4;        // 0x0143BAC4 (= &GPEngine->FrameMemoryPool)
extern unsigned long SubPrimitiveRendererScratchTable[0x1000];   // 0x014378C8 (16 KB, cleared here only; descriptive)

CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer()
{
    SetMaxSortDistance(10000.0f);
    memset(SubPrimitiveRendererScratchTable, 0, sizeof(SubPrimitiveRendererScratchTable));
    GetList(SUBPRIM_TYPE_OPAQUE).RenderPass = RENDER_PASS_COMBINED;
    GetList(SUBPRIM_TYPE_ALPHA).RenderPass = RENDER_PASS_COMBINED;
    GetList(SUBPRIM_TYPE_ALPHA).PreRenderPass = RENDER_PASS_Z_WRITE;
    GetList(SUBPRIM_TYPE_GLOW_BUFFER).RenderPass = RENDER_PASS_GLOW_BUFFER;
    GetList(SUBPRIM_TYPE_DISPLACEMENT_BUFFER).RenderPass = RENDER_PASS_DISPLACEMENT_BUFFER;
    GetList(SUBPRIM_TYPE_DISPLACEMENT_STENCIL).RenderPass = RENDER_PASS_DISPLACEMENT_STENCIL;
    GetList(SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH).RenderPass = RENDER_PASS_OUTLINE_SMOOTH_STENCIL;
    GetList(SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH).RenderPass = RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK;
    GetList(SUBPRIM_TYPE_OUTLINE_STENCIL).RenderPass = RENDER_PASS_OUTLINE_STENCIL;
    GetList(SUBPRIM_TYPE_STENCIL_MESHES).RenderPass = RENDER_PASS_COMBINED;
    GetList(SUBPRIM_TYPE_POSTWATER_MESHES).RenderPass = RENDER_PASS_COMBINED;
    GetList(SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL).RenderPass = RENDER_PASS_OUTLINE_SMOOTH_FILL;
    GetList(SUBPRIM_TYPE_OUTLINE_FILL).RenderPass = RENDER_PASS_OUTLINE_FILL;
    GetList(SUBPRIM_TYPE_BOUNDING_VOLUME).BoundingVolumePass = true;
    GetList(SUBPRIM_TYPE_2D).RenderPass = RENDER_PASS_COMBINED;
    GetList(SUBPRIM_TYPE_2D).RequiresStableSort = true;
    GetList(SUBPRIM_TYPE_2D_VIEWPORT).RenderPass = RENDER_PASS_COMBINED;
    GetList(SUBPRIM_TYPE_2D_VIEWPORT).RequiresStableSort = true;
    PFrameMemoryPool_0143bac4 = reinterpret_cast<CSimpleMemoryPool*>(NEngineInternal::GPEngine->FrameMemoryPool);
}