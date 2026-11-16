// CSurfaceCompositor::CompositeQuad(const CTexture& texture, const CSurfaceCompositor::CVertexType* const vertices, ECompositeBlendType blend) -- draw one textured quad into the composition: set the blend mode, hand the render manager a one-entry texture array, the four 24-byte vertices and the shared two-triangle index list, and count the layer
// Leaf: ?CompositeQuad@CSurfaceCompositor@@QAEXABVCTexture@@QBVCVertexType@1@W4ECompositeBlendType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F39A50, length 79 bytes, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb ?CompositeQuad@CSurfaceCompositor@@QAEXABVCTexture@@QBVCVertexType@1@W4ECompositeBlendType@@@Z;
//   the landed CEngineSurfaceCompositionManager::RenderToTexture 0x00B2B180 passes *layer_texture and the 4-vertex array.
// Callees: GFGetSystemManager 0x009A4EC0, CSurfaceCompositor::SetBlendMode 0x00F38A10 (protected), CRenderManagerCore::DrawAsIndexedTris
//   0x00A0AEA0 (textures, count, vertices, stride, vertex count, indices, triangle count); the quad index list 0x013AC928.
// Shape: `push ecx` = the one-slot texture array local, filled from the argument after the render manager fetch (system +0x60
//   PDisplayManager -> +0x08 PRenderManager, read in place) and before the SetBlendMode call; the seven pushes right to left with
//   `lea ecx,[esp+20h]` for the array, LayerCount (+0x28) bumped as load/inc/store.
// g_QuadIndices is a descriptive name (no PDB, FSE or EgoCore record of the six-entry 16-bit index table: 0,1,2 / 2,1,3 style quad).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CSURFACECOMPOSITOR_SETBLENDMODE
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CBaseClass.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CSurfaceCompositor.h"

extern unsigned short g_QuadIndices[6];            // 0x013AC928 (two triangles over four vertices)

void CSurfaceCompositor::CompositeQuad(const CTexture& texture, const CVertexType* const vertices, ECompositeBlendType blend)
{
    CRenderManager& render_manager = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    const CTexture* textures[1];
    textures[0] = &texture;
    SetBlendMode(blend);
    render_manager.DrawAsIndexedTris(textures, 1, vertices, sizeof(CVertexType), 4, g_QuadIndices, 2);
    ++LayerCount;
}
