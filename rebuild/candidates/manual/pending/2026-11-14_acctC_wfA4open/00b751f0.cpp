// CEngineWaterRenderer::RenderIceForeground(float time) -- open a scoped render-state capture block; read the shadow technique; fetch the environment's ice texture frame from graphic bank 0 (skip the pass when it is not loaded); bind the ice bump map, the environment map and the ice texture with their address modes (plus the shadow-function states when shadows are on); upload the ice foreground constants; update the global lighting states, enable specular and select the foreground index buffer; then per ice patch set up its (up to 3) local point lights, apply the technique's ice shaders for that light count and render it; finally disable the point lights
// Grade: DIFFER(2722v2722) -- only the bookmark test/or operand order of the six SetTextureUVMode SetRenderState expansions (0x00B754D5..0x00B7568D: retail `test esi,eax ; or eax,esi`); a whole-body record-count tie (dead-assert sweeps 0..90: best 16..21 fixes sites 3-6 but breaks the specular site 0x00B759EB; position irrelevant).
// Leaf: ?RenderIceForeground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B751F0, length 2722 bytes, `ret 4` (time unused).
#include "fable_compiler.h"
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_C3DBOUNDINGBOX
#include "engine/CEngineWaterRenderer.h"
#include "engine/CWaterPatchMesh.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEnvironmentDef.h"
#include "engine/C4DVector.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }      // 0x01436E9C
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }        // 0x01436E60

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

FABLE_FORCEINLINE bool CEngineWaterRenderer::SetIceForegroundTexturesAndUVModes(bool shadows)
{
    CGraphicFrame ice;
    long ice_texture = g_pEngineEnvironmentDef->IceTexture;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(ice_texture, 0)->GetFrame(ice, 0, 0))
        return false;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, IceBumpMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2, ice.GetTexture());
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_WRAP);
    if (shadows)
        NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(3, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 3));
    return true;
}

FABLE_FORCEINLINE void CEngineWaterRenderer::SetupIceForegroundShaderConstantsAndFog()
{
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, Settings.SoftEdgeSpanFactor, 0.0f, 0.0f, 0.0f);
    float factor = 0.0f;
    if (Settings.EnableBumpMap)
        factor = Settings.BumpMatFactor * 0.5f;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, factor, factor, factor, factor);
    NEngineInternal::GPShaderRenderManager->SetBoneMatrixCount(4);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0, Settings.OtherShaderVars);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1, Settings.ShoreLookupMaskMatrix);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2, Settings.ShoreLookupMaskMatrixForBounds);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3, Settings.ShoreLookupMaskMatrix2);
    float fresnel;
    if (Settings.EnableFresnel)
        fresnel = Clamp(Settings.FresnelFactor * 0.25f, 0.0f, 1.0f);
    else
        fresnel = 0.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x614);
}

void CEngineWaterRenderer::RenderIceForeground(float time)
{
    CRenderStateScopedCaptureBlock capture;
    CEngineShadowRenderer::EShadowRenderingTechnique technique = NEngineInternal::GPShadowRenderer->PeekRenderingTechnique();
    if (SetIceForegroundTexturesAndUVModes(technique != CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_NONE)) {
        SetupIceForegroundShaderConstantsAndFog();
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
        NEngineInternal::GPRenderManager->SetSpecularEnable(true);
        NEngineInternal::GPRenderManager->SelectIndexBuffer(&ForegroundIndexBuffers, 0);
        for (std::pair<C3DBoundingBox, CWaterPatchMesh*>* it = IceForegroundRenderList.begin(); it != IceForegroundRenderList.end(); ++it) {
            C3DBoundingBox area = it->first;
            CWaterPatchMesh* mesh = it->second;
            CEngineLocalLightArray* lights = NEngineInternal::GPLightingManager->GetLightsInArea(area, 3, CEnginePrimitiveThemeChannelDesc());
            NEngineInternal::GPLightingManager->SetupShaderPointLights(*lights);
            switch (technique) {
            case CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_NONE:
                NEngineInternal::GPShaderRenderManager->ApplyPixelShader(IceForegroundPixelShader);
                NEngineInternal::GPShaderRenderManager->ApplyVertexShader(GetIceForegroundVertexShader(lights->LightCount), false);
                break;
            case CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_SHADOW_BUFFER:
                NEngineInternal::GPShaderRenderManager->ApplyPixelShader(IceForegroundShadowedPixelShader);
                NEngineInternal::GPShaderRenderManager->ApplyVertexShader(GetIceForegroundShadowedVertexShader(lights->LightCount), false);
                break;
            case CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER:
                NEngineInternal::GPShaderRenderManager->ApplyPixelShader(IceForegroundColbuffShadowedPixelShader);
                NEngineInternal::GPShaderRenderManager->ApplyVertexShader(GetIceForegroundColbuffShadowedVertexShader(lights->LightCount), false);
                break;
            }
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0x14);
            mesh->Render(WaterForegroundPolyCount);
        }
        NEngineInternal::GPLightingManager->DisablePointLights();
    }
}
