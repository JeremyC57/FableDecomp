// CEngine::SetDefaultRenderStates() -- initialize engine render states
// Grade: DIFFER(4344v4344) -- Same-size three instructions: test operand reversal and OR destination/store at00B2543B..41
// Leaf: ?SetDefaultRenderStates@CEngine@@AAEXXZ
// Retail: 0x00B24850, 4344 bytes, thiscall.
// Identity: both original PDB name/type sets and corresponding donor body.
// Current result:4344/4344 bytes,1080 instructions; three instructions in two hunks differ.
// Residue at native00B2543B..441: commutative bookmark mask operand and destination choice.
// Original namespace globals GPDisplayManager and GPLightingManager are restored;
// evidence and corrected inherited spellings are recorded in NAMING_EVIDENCE.md/NAME_FIXES.tsv.
// The source keeps one shared stage index as a reconstruction choice; original donor has separate n locals.
// Full body and actual inline setter bookkeeping pass both compiler fixtures; not a byte match.
#define FABLE_RETAIL_CRENDERSTATEMANAGER_DEFAULT_TRANSFORM
#define FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN
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
extern CDisplayManager* GPDisplayManager; // Original PDB typed data and donor04AC7FFC.
extern CEngineLightingManager* GPLightingManager; // Original PDB typed data and donor04AC8080.
extern CRenderManager*       GPRenderManager;          // 0x01436E18
extern CShaderRenderManager* GPShaderRenderManager;    // 0x01436E14
}
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
    NEngineInternal::GPLightingManager->SetupDefaultFogStates();
    GPShaderRenderManager->SetVertexShaderConstantLayout(3);
    GPShaderRenderManager->SetTextureTransformCount(0);
    GPRenderManager->EnableXBoxShaderConstants(true);
    NEngineInternal::GPDisplayManager->GetVertexShaderVersion();
    GPShaderRenderManager->SetUVCompressionVertexShaderConstant(1.0f / 2048.0f, 1.0f / 2048.0f, -8.0f, -8.0f);
    if (EnableWireFrame)
        GPRenderManager->SetPolyFillMode(POLY_FILL_WIRE);
}
