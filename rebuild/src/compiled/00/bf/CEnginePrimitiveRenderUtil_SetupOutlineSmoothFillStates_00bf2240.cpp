// CEnginePrimitiveRenderUtil::SetupOutlineSmoothFillStates()
// Leaf: ?SetupOutlineSmoothFillStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@M_NJ@Z
// Flags: (none)
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_PASS22_RENDERUTIL_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_ORIGINAL_COMPLETE_PASS22
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ORIGINAL_FORMALS_PASS22
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEMESHEFFECT_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCILSTATES
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_PIXEL_CONSTANT_AT
#define FABLE_RETAIL_CARRAY_ORIGINAL_AT_VECTOR_INDEX_PASS22
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEnginePrimitiveRenderUtil.h"

namespace NEngineInternal {
    extern CRenderManager* GPRenderManager;
    extern CShaderRenderManager* GPShaderRenderManager;
    extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow;
}

void CEnginePrimitiveRenderUtil::SetupOutlineSmoothFillStates(const CEngineMeshEffectOutline& effect,
        float mesh_scale, bool enable_alpha, long depth_texture_stage)
{
    float width_world_space = effect.WidthWorldSpace * NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    float width_screen_space = effect.WidthScreenSpace * NEngineInternal::GPScreenEffectOutlineGlow->PeekOutlineGlowScaleFactor();
    CRGBColour colour = effect.Colour;
    long alpha_ref = effect.TextureAlphaRef;
    long alpha_bias = effect.TextureAlphaBias;
    width_world_space *= 0.01f / mesh_scale;
    width_screen_space *= 0.01f / mesh_scale;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,
        width_world_space, width_screen_space, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, CRGBFloatColour(colour));

    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetStencilEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);

    if (enable_alpha) {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_MAX);
        float ref = alpha_ref * (1.0f / 255.0f);
        float bias = alpha_bias * (1.0f / 255.0f);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(bias, bias, bias, ref));
    }
    else {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    }
}