// 00b6e890 CEngineWaterRenderer::RenderBumpMap
// Leaf: ?RenderBumpMap@CEngineWaterRenderer@@AAEXW4EWaterArrayIndexAliases@@@Z
// Retail: 0x00B6E890,1371 bytes; private thiscall.
// Flags: none
// Evidence: both original PDBs plus donor02D53F10; original constant_array_index name.
// Settings.BumpmapGenerationShift contains pairs of C2DVector, not four-component offsets.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_CREATEDYNAMICTEXTURES
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#include "engine/EWaterArrayIndexAliases.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CRenderManager.h"
namespace NEngineInternal {
    extern CShaderRenderManager* GPShaderRenderManager;
    extern CRenderManager* GPRenderManager;
}
using namespace NEngineInternal;

void CEngineWaterRenderer::RenderBumpMap(EWaterArrayIndexAliases constant_array_index)
{
    GPShaderRenderManager->SetUserVertexShaderConstant(0,
        Settings.BumpmapGenerationShift[constant_array_index][0].X,
        Settings.BumpmapGenerationShift[constant_array_index][0].Y,
        Settings.BumpmapGenerationShift[constant_array_index][1].X,
        Settings.BumpmapGenerationShift[constant_array_index][1].Y);
    GPShaderRenderManager->SetPixelShaderConstant(5, CRGBFloatColour(1.0f,0.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(6, CRGBFloatColour(1.0f,0.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(7, CRGBFloatColour(0.0f,0.0f,1.0f,1.0f));
    GPShaderRenderManager->PrepareForRender(0);
    GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,14,0,2);
    GPRenderManager->SetAlphaBlendEnable(true);
    GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    GPShaderRenderManager->SetPixelShaderConstant(5, CRGBFloatColour(0.0f,1.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(6, CRGBFloatColour(0.0f,1.0f,0.0f,0.0f));
    GPShaderRenderManager->SetPixelShaderConstant(7, CRGBFloatColour(0.0f,0.0f,1.0f,1.0f));
    GPShaderRenderManager->PrepareForRender(0);
    GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,14,0,2);
    GPRenderManager->SetAlphaBlendEnable(false);
}