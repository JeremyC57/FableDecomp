// CEnginePrimitiveManagerText::RenderSubPrimitive -- render text through the current font and alpha mask.
// Inline-budget: native expands the two real mask setup methods; plain bodies, row threshold flags and scoped depth255 all outline one or both.
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerText@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail:0x00bc6db0,2217bytes,thiscall,ret8.
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C2DALPHAMASKINFO_RESET
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERMANAGER2D_VIEWPORTBOXFLOAT
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#include "engine/CRenderManager2D.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEnginePrimitiveManagerText.h"
#include "engine/CEngineSubPrimitiveText.h"
#include "engine/C2DVector.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CStreamingFontBank.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
namespace NEngineInternal {
    extern CRenderManager* GPRenderManager;
    extern CShaderRenderManager* GPShaderRenderManager;
    extern CDisplayManager* GPDisplayManager;
    extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;
}

#include "engine/GFDebugPDynamicCast.h"

inline const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo() {
    CEnginePrimitive2DViewportManager* manager=GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(33),(CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::DisableTextureFiltering(unsigned long stage) {
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stage,TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stage,TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureMipMapMode(stage,TEXTURE_MIPMAP_DISABLE);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::Set2DAlphaMaskTextureMode(unsigned long stage,bool mask_outside,bool filter) {
    if(!filter)DisableTextureFiltering(stage);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(stage,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(stage,TEXTURE_UV_CLAMP);
}

void CEnginePrimitiveManagerText::RenderSubPrimitive(CEngineSubPrimitiveBase& subprim_base, ERenderPassType render_pass)
{
    CEngineSubPrimitiveText& subprim = static_cast<CEngineSubPrimitiveText&>(subprim_base);
    bool vs_enable = !CStreamingFontBank::GetUseFixedFunction();
    CScopedVertexShaderConstantLayout layout(1);
    CRenderStateScopedCaptureBlock state;
    CScopedShadersEnable shaders_enable(vs_enable, true);
    if (vs_enable) {
        C2DExtentsI dim = NEngineInternal::GPDisplayManager->GetRenderTargetDimensions();
        const float one_on_half_screen_width = 1.0f / ((float)dim.X * 0.5f);
        const float one_on_half_screen_height = 1.0f / ((float)dim.Y * 0.5f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1,one_on_half_screen_width,one_on_half_screen_height,-1.0f,1.0f);
    }
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    TEXTURE_FILTER_MODE filter_mode=TEXTURE_FILTER_LINEAR;
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, filter_mode);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, filter_mode);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode((ALPHA_BLEND_MODE)4);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode((ALPHA_BLEND_MODE)5);
    if (subprim.Mask) {
        const C2DAlphaMaskInfo& alpha_mask_info = *GetCurrent2DAlphaMaskInfo();
        Set2DAlphaMaskTextureMode(1,alpha_mask_info.MaskObjectsOutsideAlphaMask,alpha_mask_info.FilterMask);
        if (vs_enable) {
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, alpha_mask_info.AlphaMaskPositionFactors);
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VShaderMask, false);
        } else {
            C4DVector alpha_mask_uvs = alpha_mask_info.AlphaMaskPositionFactors;
            CStreamingFontBank::SetAlphaMaskUVs(C2DVector(alpha_mask_uvs.X, alpha_mask_uvs.Y), C2DVector(alpha_mask_uvs.Z, alpha_mask_uvs.W));
        }
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PShaderMask);
    } else {
        if (vs_enable)
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PShader);
    }
    C3DVector pos;
    const C2DBoxF& window = NEngineInternal::GPRenderManager->GetCurrentAWindow();
    pos.X = window.TLX + subprim.Pos.X;
    pos.Y = window.TLY + subprim.Pos.Y;
    pos.Z = subprim.Pos.Z;
    bool fixed_function=false;
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    subprim.Font->PrepareToRender(fixed_function);
    subprim.Font->Render(subprim.Text, pos, subprim.Scale, subprim.Colour, 0, subprim.Mask);
}