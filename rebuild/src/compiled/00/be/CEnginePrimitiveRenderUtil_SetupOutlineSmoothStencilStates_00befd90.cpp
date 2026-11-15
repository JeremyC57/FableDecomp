// 00befd90 CEnginePrimitiveRenderUtil::SetupOutlineSmoothStencilStates
// Leaf: ?SetupOutlineSmoothStencilStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M@Z
// Retail: 0x00BEFD90,1100 bytes,public thiscall.
// Flags: none
// Evidence: both PDBs, original module922, donor02EEE7C0 and native state offsets.
// Native release omits debug overrides and stipple state; width conversion includes0.01f.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_PASS22_SMOOTHSTENCIL
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderManager.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
namespace NEngineInternal {
    extern CRenderManager* GPRenderManager;
    extern CShaderRenderManager* GPShaderRenderManager;
    extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow;
}
using namespace NEngineInternal;

void CEnginePrimitiveRenderUtil::SetupOutlineSmoothStencilStates(const CEngineMeshEffectOutline& effect, float mesh_scale)
{
    float width_world_space=effect.WidthWorldSpace*GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    float width_screen_space=effect.WidthScreenSpace*GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    width_screen_space+=GPScreenEffectOutlineGlow->PeekOuterStencilExtraNormalExtrusion();
    width_world_space*=1.0f/mesh_scale;
    width_screen_space*=1.0f/mesh_scale;
    GPShaderRenderManager->SetUserVertexShaderConstant(0,width_world_space*0.01f,width_screen_space*0.01f,0,0);
    GPShaderRenderManager->SetUserVertexShaderConstant(1,CRGBFloatColour(1,1,1,1));
    GPRenderManager->SetZBufferWriteEnable(false);
    GPRenderManager->SetColourWriteEnable(false);
    GPRenderManager->SetAlphaTestEnable(false);
    GPRenderManager->SetFogEnable(false);
    GPRenderManager->SetStencilEnable(true);
    GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
    GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
    unsigned long stencil_mask=1 << (effect.StencilBit+2);
    GPRenderManager->SetStencilWriteMask(stencil_mask);
    GPRenderManager->SetStencilRef(stencil_mask);
}