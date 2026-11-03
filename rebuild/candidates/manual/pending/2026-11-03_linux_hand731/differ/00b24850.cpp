// CEngine::SetDefaultRenderStates() -- put the render manager into the engine's default state: every texture stage
// Grade: DIFFER(4344v4344) on the final chunk 731 overlay, 6 diff lines (normalized), 12 logged attempts / ~25 grades. Residue: the one inlined SetRenderState bookmark block of SetTextureTransformMode(0, ...) inside the second stage loop (0x00B25426..): retail `test esi,eax ; or eax,esi ; mov [edx+8],eax`, built `test eax,esi ; or esi,eax ; mov [edx+8],esi` -- the documented VC7.1 lane gap (mask operand order follows temp numbering; source operand order is inert; /G6 /G7 /GB /Oa /Ow, loop forms, aliases and a CStateInfo& local in the setter all leave it).
// modulates texture by diffuse with linear filtering, clamped UVs and no texture transform; alpha test always, Gouraud,
// solid fill, CCW culling, z test / write on (less-equal), fog off, one / zero blending, dither and clipping on, lighting
// off; then the lighting manager's fog defaults, the shader manager's constant layout, texture transform count and UV
// compression constants, and wire-frame fill when the engine asks for it.
// Leaf: ?SetDefaultRenderStates@CEngine@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00B24850, 4344 bytes, plain `ret` (private, non-virtual: Ego_r.pdb / FableWin.pdb
//   ?SetDefaultRenderStates@CEngine@@AAEXXZ).
// Callees: CEngineLightingManager::SetupDefaultFogStates 0x00B47630 (on g_pEngineLightingManager 0x01436E9C),
//   CShaderRenderManager::SetVertexShaderConstantLayout 0x0098B5E0 / SetTextureTransformCount 0x00988630 (stores +0x38C
//   TextureTransformCount; Ego_r 0x00AE4C90 right before ResetLight, as retail 0x00988640) / SetUVCompressionVertexShaderConstant
//   0x00988730 (empty `ret 0x10` PC stub), all on NEngineInternal::GPShaderRenderManager 0x01436E14;
//   CRenderManagerCore::EnableXBoxShaderConstants 0x00A0AA20 (empty `ret 4` stub) on NEngineInternal::GPRenderManager 0x01436E18;
//   CDisplayManager::GetVertexShaderVersion 0x009BEDE0 on g_pDisplayManager_01436e1c (result unused).
// Everything else is the inline CRenderStateManager setter family (engine/CRenderStateManager.h FABLE_RETAIL_CRENDERSTATEMANAGER):
//   two 8-stage loops (`stage * 0x14` strength-reduced into ebp) and the single states, each through its D3D table.
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/Direct3D9Types.h"
#include "engine/CEngine.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CDisplayManager.h"
namespace NEngineInternal {
extern CRenderManager*       GPRenderManager;          // 0x01436E18
extern CShaderRenderManager* GPShaderRenderManager;    // 0x01436E14
}
extern CDisplayManager*        g_pDisplayManager_01436e1c;  // 0x01436E1C
extern CEngineLightingManager* g_pEngineLightingManager;    // 0x01436E9C
using namespace NEngineInternal;

void CEngine::SetDefaultRenderStates()
{
    long stage;
    for (stage = 0; stage < 8; ++stage) {
        GPRenderManager->SetTextureAlphaOperation(stage, TEXTURE_OP_DISABLE);
        GPRenderManager->SetTextureArgument1(stage, TEXTURE_ARG_TEXTURE);
        GPRenderManager->SetTextureArgument2(stage, TEXTURE_ARG_DIFFUSE);
        GPRenderManager->SetTextureAlphaArgument1(stage, TEXTURE_ARG_TEXTURE);
        GPRenderManager->SetTextureAlphaArgument2(stage, TEXTURE_ARG_DIFFUSE);
    }
    GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_ALWAYS);
    GPRenderManager->SetPolyShadeMode(POLY_SHADE_GOURAUD);
    GPRenderManager->SetPolyFillMode(POLY_FILL_SOLID);
    GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    GPRenderManager->SetZBufferWriteEnable(true);
    GPRenderManager->SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    GPRenderManager->SetZBufferEnable(true);
    GPRenderManager->SetFogEnable(false);
    GPRenderManager->SetFogColour(0xffffff);
    GPRenderManager->SetFogStart(0.0f);
    GPRenderManager->SetFogEnd(1.0f);
    GPRenderManager->SetAlphaRef(0x80);
    GPRenderManager->SetSpecularEnable(false);
    GPRenderManager->SetDitherEnable(true);
    for (stage = 0; stage < 8; ++stage) {
        GPRenderManager->SetTextureMaxAnisotropy(stage, AnisotropicFilteringLevel);
        GPRenderManager->SetTextureMipmapLODBias(stage, 0.0f);
        GPRenderManager->SetTextureMipMapMode(stage, TEXTURE_MIPMAP_LINEAR);
        GPRenderManager->SetTextureFilterModeMin(stage, TEXTURE_FILTER_LINEAR);
        GPRenderManager->SetTextureFilterModeMag(stage, TEXTURE_FILTER_LINEAR);
        GPRenderManager->SetTextureUVModeU(stage, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureUVModeV(stage, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureUVModeW(stage, TEXTURE_UV_CLAMP);
        GPRenderManager->SetTextureTransformMode(0, TEXTURE_TRANSFORM_DIABLED);
    }
    GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);
    GPRenderManager->SetTextureOperation(1, TEXTURE_OP_DISABLE);
    GPRenderManager->SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    GPRenderManager->SetTextureOperation(0, TEXTURE_OP_MODULATE);
    GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    GPRenderManager->SetTextureEnable(EnableTextures);
    GPRenderManager->SetAlphaBlendEnable(false);
    GPRenderManager->SetClippingEnable(true);
    GPRenderManager->SetLightingEnable(false);
    g_pEngineLightingManager->SetupDefaultFogStates();
    GPShaderRenderManager->SetVertexShaderConstantLayout(3);
    GPShaderRenderManager->SetTextureTransformCount(0);
    GPRenderManager->EnableXBoxShaderConstants(true);
    g_pDisplayManager_01436e1c->GetVertexShaderVersion();
    GPShaderRenderManager->SetUVCompressionVertexShaderConstant(1.0f / 2048.0f, 1.0f / 2048.0f, -8.0f, -8.0f);
    if (EnableWireFrame)
        GPRenderManager->SetPolyFillMode(POLY_FILL_WIRE);
}
