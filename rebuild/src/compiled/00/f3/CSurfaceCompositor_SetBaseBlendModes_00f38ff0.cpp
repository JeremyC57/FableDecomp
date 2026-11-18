// 00f38ff0 CSurfaceCompositor::SetBaseBlendModes
// Leaf: ?SetBaseBlendModes@CSurfaceCompositor@@IAEXXZ
// Flags: none
// Retail: 0x00F38FF0, 1842 bytes, thiscall.
#define FABLE_RETAIL_CSURFACECOMPOSITOR_BASE_BLEND_PASS24
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CSurfaceCompositor.h"

void CSurfaceCompositor::SetBaseBlendModes()
{
    CRenderManager& renderer = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    renderer.SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    renderer.SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    renderer.SetZBufferEnable(false);
    renderer.SetPolyCullMode(POLY_CULL_NONE);
    renderer.SetSpecularEnable(true);
    renderer.SetPolyFillMode(POLY_FILL_SOLID);
    renderer.SetPolyShadeMode(POLY_SHADE_GOURAUD);
    renderer.SetAlphaBlendEnable(false);
    renderer.SetAlphaTestEnable(false);
    renderer.SetColourWriteEnable(true, true, true, true);
    renderer.SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    renderer.SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);
    renderer.SetTextureOperation(0, TEXTURE_OP_MODULATE);
    renderer.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    renderer.SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
    renderer.SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    renderer.SetTextureOperation(1, TEXTURE_OP_DISABLE);
    renderer.SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
}