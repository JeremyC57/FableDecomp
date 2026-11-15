// CEngineWaterRenderer::RenderForeground -- original foreground water pass.
// Leaf: ?RenderForeground@CEngineWaterRenderer@@AAEXM@Z
// Retail: 0x00B761D0,2531bytes.
// Flags: none
// Inline-budget: native expands these two original water helpers; plain definitions,
// row /d2inlT10000 /d2inlS10000 and depth255 all retain the328-byte called-helper target.
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUND_BODIES_PASS22
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_SEAMIDGROUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEWATERRENDERER
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineWaterRenderer.h"
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicFrame.h"
namespace NEngineInternal {
 extern CEngineResourceManager* GPResourceManager;
 extern const CEnvironmentDef* GPEnvironmentDef;
 extern CRenderManager* GPRenderManager;
 extern CShaderRenderManager* GPShaderRenderManager;
}
#include "engine/CWaterPatchMesh.h"
// Inline-budget: native expands this genuine lookup; plain/depth255 retain a call,
// while budgets10000 expand the original native-called vector resize as well.
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long index,unsigned long frame) const { return PeekGraphic(index) ? PeekGraphic(index)->PeekInfoFrame(frame) : 0; }
inline const CGraphicDataBank* CEngineResourceManager::PeekTerrainBank() const { return PeekGraphicBank((EGraphicBankNo)0); }
// Inline-budget: same original getter-chain expansion requirement and budget/depth proof above.
FABLE_FORCEINLINE const CGraphicInfoFrame* CEngineResourceManager::GetTerrainTexture(long index) { return PeekTerrainBank()->PeekGraphicInfoFrame(index,0); }

FABLE_FORCEINLINE bool CEngineWaterRenderer::SetForegroundTexturesAndUVModes()
{
    CGraphicFrame graphic_frame;
    if (!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->WaterEdgeAlphaMap)->GetFrame(graphic_frame,0,0))
        return false;
    bool animate_bump_map=true;
    if (animate_bump_map)
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,WaterBumpMap);
    else {
        CGraphicFrame lake_bump_graphic;
        if (!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->WaterBumpMapPC)->GetFrame(lake_bump_graphic,Settings.WaterBumpMapPCFrameIndex,0))
            return false;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,lake_bump_graphic.GetTexture());
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(1,EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2,EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(3,graphic_frame.GetTexture());
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0,TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0,TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1,TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2,TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3,TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3,TEXTURE_UV_WRAP);
    return true;
}
// Inline-budget: plain and depth255 keep the original Fresnel helper called; rowT200
// expands it but also incorrectly expands native called vector::resize.
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupFresnelPixelShaderConstants()
{
    // Native retail uses float-width quarter/zero/one operands; later debug donor uses double literals.
    float fresnel_factor=Settings.EnableFresnel ?
        ((Settings.FresnelFactor/4.0f)<0.0f ? 0.0f :
        ((Settings.FresnelFactor/4.0f)>1.0f ? 1.0f : (Settings.FresnelFactor/4.0f))) : 0.0f;
    C4DVector fresnel(fresnel_factor,fresnel_factor,fresnel_factor,fresnel_factor);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5,fresnel);
}
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupForegroundShaderConstantsAndFog()
{
    if (Settings.EnableSwell)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,Settings.SwellFactor,Settings.ShoreSwellFactor,Settings.RefOffset,0.0f);
    else
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,0.0f,0.0f,0.0f,0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1,Settings.SoftEdgeSpanFactor,0.0f,0.0f,0.0f);
    NEngineInternal::GPShaderRenderManager->SetBoneMatrixCount(4);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0,Settings.OtherShaderVars);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1,Settings.ShoreLookupMaskMatrix);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2,Settings.ShoreLookupMaskMatrixForBounds);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3,Settings.ShoreLookupMaskMatrix2);
    SetupFresnelPixelShaderConstants();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
}
void CEngineWaterRenderer::RenderForeground(const float interpolate)
{
    CRenderStateScopedCaptureBlock block;
    if (SetForegroundTexturesAndUVModes()) {
        SetupForegroundShaderConstantsAndFog();
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ForegroundVertexShader,false);
        NEngineInternal::GPRenderManager->SelectIndexBuffer(&ForegroundIndexBuffers,0);
        for (CArray<CWaterPatchMesh*>::iterator patch_it=ForegroundRenderList.begin();patch_it!=ForegroundRenderList.end();++patch_it) {
            CWaterPatchMesh& water_patch=**patch_it;
            float bump_factor=Settings.BumpMatFactor*WaterBumpFactor;
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(RefractionForegroundPixelShader);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,1.0f,0.0f);
            water_patch.Render(WaterForegroundPolyCount);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,0.0f,1.0f);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(ForegroundPixelShader);
            water_patch.Render(WaterForegroundPolyCount);
        }
    }
}