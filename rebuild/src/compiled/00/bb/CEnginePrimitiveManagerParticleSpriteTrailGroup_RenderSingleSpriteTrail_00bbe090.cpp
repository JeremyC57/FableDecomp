// Leaf: ?RenderSingleSpriteTrail@CEnginePrimitiveManagerParticleSpriteTrailGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: default
// Inline-budget: default flags. The two blend helpers and point-light selector expand with FABLE_FORCEINLINE. PROBES.md records the plain-inline and per-row flag ablations.
#define FABLE_RETAIL_SPRITETRAILGROUP_RENDER_SINGLE_PASS28
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITETRAILGROUP_CLASS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFO_FRAMEFACTORS
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_FLOATTL
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/EEnginePrimitiveType.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineResourceManager.h"
#include "engine/EGraphicBankNo.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CArray.h"
#include "engine/CGraphicFrame.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEngineInternalPrimitiveSpriteTrail.h"
#include "engine/CEnginePrimitiveManagerSpriteTrails.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"
#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"
namespace NEngineInternal {
    extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;
    extern CEngineLightingManager* GPLightingManager;
    extern CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer;
    extern CRenderManager* GPRenderManager;
    extern CShaderRenderManager* GPShaderRenderManager;
    extern CEngineResourceManager* GPResourceManager;
}
FABLE_FORCEINLINE void CEnginePrimitiveManagerParticleSpriteTrailGroup::ApplySpriteTrailPointLightVertexShader(unsigned long lights)
{
    if (lights > VertexShadersSpriteTrailPointLights.size() - 1)
        lights = VertexShadersSpriteTrailPointLights.size() - 1;
    ApplyVertexShader(VertexShadersSpriteTrailPointLights[lights]);
}
inline void CEnginePrimitiveManagerParticleSpriteTrailGroup::ApplyDisplacementVertexShader()
{
    ApplyVertexShader(VertexShaderDisplacement);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REVSUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
FABLE_FORCEINLINE bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}

void CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail(CEngineSubPrimitiveBase& subprim, ERenderPassType render_pass)
{
    CEngineInternalPrimitiveSpriteTrail* primitive =
        static_cast<CEngineInternalPrimitiveSpriteTrail*>(subprim.GetParentBase());
    const CGraphicInfo* graphic_info = NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
        ->PeekGraphic(primitive->GraphicIndex);
    CGraphicFrame graphic;
    if (!graphic_info->GetFrame(graphic, primitive->GraphicFrame, 0, 0))
        return;
    CRenderStateScopedCaptureBlock state_capture;
    CScopedVertexShaderConstantLayout layout(2);
    CEnginePrimitiveManagerParticleGroup* particle_group_prim_manager = static_cast<CEnginePrimitiveManagerParticleGroup*>(
        NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_PARTICLE_GROUP));
    bool colour_factor_is_alpha;
    if (render_pass == RENDER_PASS_DISPLACEMENT_BUFFER && NEngineInternal::GPScreenEffectDisplacementRenderer->PeekRenderEnabled()) {
        colour_factor_is_alpha = false;
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        ApplyDisplacementVertexShader();
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderDisplacement);
    } else if (render_pass == RENDER_PASS_DISPLACEMENT_STENCIL) {
        colour_factor_is_alpha = false;
        ApplyDisplacementVertexShader();
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(primitive->BlendOp);
        colour_factor_is_alpha = SetupAlphaBlendingStatesFromBlendMode(primitive->BlendMode);
        long point_light_count = 0;
        if (primitive->Flags & 0x80)
            point_light_count = particle_group_prim_manager->SetUpPointLights();
        else
            NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(100);
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
        ApplySpriteTrailPointLightVertexShader(point_light_count);
        particle_group_prim_manager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, graphic.GetTexture());
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    const float tex_offset_u = graphic.GetFloatTLX();
    const float tex_offset_v = graphic.GetFloatTLY();
    const float tex_mult_u = graphic_info->GetFrameWidthFactor();
    const float tex_mult_v = graphic_info->GetFrameHeightFactor();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, tex_offset_u, tex_offset_v, tex_mult_u, tex_mult_v);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
    PTrailManager->RenderSpriteTrail(*primitive, colour_factor_is_alpha);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
}