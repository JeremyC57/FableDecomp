#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_CPP_INLINE
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_FORCED_SUCCESS_SCOPE_PASS22
#define FABLE_RETAIL_PASS22_WATER_ORIGINAL_COMPLETE_OWNER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ORIGINAL_CASTTOBASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22
// 00b6fa40 CEngineWaterRenderer::RenderSeaMidground
// Leaf: ?RenderSeaMidground@CEngineWaterRenderer@@AAEXM@Z
// Flags: /d2inlT128
// Inline-budget: native expands both original helpers; plain, tested row budgets and scoped depth leave both calls.
// Original module965 names and helper boundaries; see NAMING_EVIDENCE.md.
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_TEXTURE_BINDING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CARRAY_ORIGINAL_AT_VECTOR_INDEX_PASS22
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_PIXEL_CONSTANT_AT
#define FABLE_RETAIL_PASS22_STATE_ORIGINAL_CAPTURE_STORE_SEQUENCE
#define FABLE_RETAIL_PASS22_WATER_ORIGINAL_COPY_COLOUR_FORMALS
#define FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS
#define FABLE_RETAIL_CGRAPHICFRAMEDATA_ORIGINAL_TEXTURE_GET_PASS22
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_GETFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
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
#define FABLE_RETAIL_C4DVECTOR_ORIGINAL_COLOUR_CHAIN
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_FLOAT_CTORS
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
#define FABLE_RETAIL_CENGINEWATERSETTINGS_COLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND_BODIES
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GETSETTINGS
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALDIFFUSE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_SEAMIDGROUND
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
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
 extern CEngineLightingManager* GPLightingManager;
 extern CEngineLandscapeRenderer* GPLandscapeRenderer;
 extern CEngineWorldMap* GPEngineWorldMap;
}
// Original terrain lookup composition, also verified in the foreground sibling.
inline const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long index,unsigned long frame) const { return PeekGraphic(index) ? PeekGraphic(index)->PeekInfoFrame(frame) : 0; }
inline const CGraphicDataBank* CEngineResourceManager::PeekTerrainBank() const { return PeekGraphicBank((EGraphicBankNo)0); }
inline const CGraphicInfoFrame* CEngineResourceManager::GetTerrainTexture(long index) { return PeekTerrainBank()->PeekGraphicInfoFrame(index,0); }
FABLE_FORCEINLINE bool CEngineWaterRenderer::SetSeaMidgroundTexturesAndUVModes()
{
    bool animate_bump_map=true;
    if(animate_bump_map)
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, SeaBumpMap);
    else {
        CGraphicFrame sea_bump_graphic;
        if(!NEngineInternal::GPResourceManager->GetTerrainTexture(NEngineInternal::GPEnvironmentDef->SeaBumpMapPC)->GetFrame(sea_bump_graphic,Settings.SeaBumpMapPCFrameIndex,0))
            return false;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0,sea_bump_graphic.GetTexture());
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2, EnvironmentMap);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
    return true;
}
FABLE_FORCEINLINE void CEngineWaterRenderer::SetupSeaMidgroundShaderConstantsAndFog()
{
    const float fade_mult=NEngineInternal::GPLandscapeRenderer->GetSettings().BackgroundFadeTransform.X;
    const float fade_offset=NEngineInternal::GPLandscapeRenderer->GetSettings().BackgroundFadeTransform.W;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,Settings.SeaBumpmapSpan,fade_mult,fade_offset,1.0f);
    if(HighendWaterEnabled) {
        float bump_factor=Settings.BumpMatFactor*WaterBumpFactor;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,-bump_factor/2.0f,1.0f);
        C4DVector colour(Settings.Colour);
        const CIDrawEnvironmentParams& params=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        colour.W=params.ThemeChannels[0].Scalers[2]*Settings.ColourToReflectionFactor;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7,colour);
    } else {
        static C4DVector light_dot(0.8f,0.8f,0.0f,0.0f);
        static CRGBFloatColour colour(0.0f,0.4f,0.8f);
        CRGBFloatColour col=colour*NEngineInternal::GPLightingManager->PeekGlobalDiffuseColour();
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5,light_dot);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6,C4DVector(col));
    }
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x204);
}
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CMatrix3x4.h"
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }
// Original same-module helper recovered from both PDBs and full donor; no local class model.
void CEngineWaterRenderer::RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& patch_list,const float interpolate)
{
    for(CArray<CEngineWaterBackgroundSubPatch*>::iterator patch_it=patch_list.begin();patch_it!=patch_list.end();++patch_it) {
        if(Settings.EnableWaterOcclusion) {
            unsigned long occlusion_id=(*patch_it)->GetOcclusionID();
            if(occlusion_id>0 && !NEngineInternal::GPOcclusionManager->GetPrevFrameResult(occlusion_id)) {
                CMatrix3x4 matt((*patch_it)->PeekWidth(),0.0f,0.0f,
                    0.0f,(*patch_it)->PeekHeight(),0.0f,
                    0.0f,0.0f,(*patch_it)->PeekDepth(),
                    (*patch_it)->PeekWorldX(),(*patch_it)->PeekWorldY(),(*patch_it)->PeekWorldZ());
                occlusion_id=NEngineInternal::GPOcclusionManager->RenderOcclusionBox(matt);
            } else {
                NEngineInternal::GPOcclusionManager->StartOcclusionTest();
                RenderBackgroundPatch(*patch_it);
                occlusion_id=NEngineInternal::GPOcclusionManager->AllocateOccluderHandle();
                NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_id);
            }
            (*patch_it)->SetOcclusionID(occlusion_id);
        } else RenderBackgroundPatch(*patch_it);
    }
}

void CEngineWaterRenderer::RenderSeaMidground(const float interpolate)
{
    CRenderStateScopedCaptureBlock block;
    if(SetSeaMidgroundTexturesAndUVModes()) {
    if (HighendWaterEnabled) {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SeaBackgroundPixelShader);
    } else {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundLowEndVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(WaterLowEndPixelShader);
    }
        SetupSeaMidgroundShaderConstantsAndFog();
        RenderBackgroundList(SeaAlphaedMidgroundRenderList,interpolate);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
        RenderBackgroundList(SeaMidgroundRenderList,interpolate);
    }
}
