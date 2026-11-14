// CEngineWaterRenderer::RenderSeaReflectiveForeground(float time) -- open a scoped render-state capture block; fetch the environment's water-surf and water-edge alpha frames from graphic bank 0 (skip the pass when either is not loaded); bind the sea bump map, the environment map and the surf texture with their address modes; set the bump user constant, apply the reflective sea foreground shaders and the foreground index buffer, upload the reflective sea foreground constants and flush them; then render every patch of the reflective sea foreground list
// Grade: DIFFER(2798v2734) -- everything matches to 0x00B77660; retail calls ??_GCGraphicFrameData 0x0041BDF0 for the surf frame's final release (0x00B77AE9) where we expand it (trace: all ~CGraphicFrameData/??_G sites sc=60 f=0.25, a+g 4850 v 5078 at the edge / surf sites: a caller-size flip would need +3922..4150 tuples); the rest (0x48 v 0x44 frame, ebx/[esp+10] homes, test/or order) follows from it. ~CGraphicFrame via the repo nested FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA (FableWin ??1CGraphicFrame 9i = one ReleaseData call; code-identical here).
// Leaf: ?RenderSeaReflectiveForeground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B77370, length 2734 bytes, `ret 4` (time unused).
#include "fable_compiler.h"
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
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#include "engine/CEngineWaterRenderer.h"
#include "engine/CWaterPatchMesh.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEnvironmentDef.h"
#include "engine/C4DVector.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24

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

FABLE_FORCEINLINE bool CEngineWaterRenderer::SetSeaReflectiveForegroundTexturesAndUVModes()
{
    CGraphicFrame surf;
    long surf_map = g_pEngineEnvironmentDef->WaterSurfMap;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(surf_map, 0)->GetFrame(surf, 0, 0))
        return false;
    CGraphicFrame edge_alpha;
    long edge_map = g_pEngineEnvironmentDef->WaterEdgeAlphaMap;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(edge_map, 0)->GetFrame(edge_alpha, 0, 0))
        return false;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, SeaBumpMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2, EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(3, surf.GetTexture());
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_WRAP);
    return true;
}

FABLE_FORCEINLINE void CEngineWaterRenderer::SetupSeaReflectiveForegroundShaderConstantsAndFog()
{
    if (Settings.EnableSwell)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SwellFactor, Settings.ShoreSwellFactor, Settings.RefOffset, 0.0f);
    else
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, Settings.SoftEdgeSpanFactor, Settings.EdgeWakeMultFactor,
        Settings.EdgeWakeAddFactor, Settings.EdgeWakeSpanFactorU);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, Settings.EdgeWakeSpanFactorV, Settings.EdgeWakeDepthClamp,
        -Settings.EdgeWakeOffset, 0.0f);
    float factor = Settings.BumpMatFactor * WaterBumpFactor;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
    NEngineInternal::GPShaderRenderManager->SetBoneMatrixCount(4);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0, Settings.OtherSeaShaderVars);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1, Settings.ShoreLookupMaskMatrix);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2, Settings.ShoreLookupMaskMatrixForBounds);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3, Settings.ShoreLookupMaskMatrix2);
    float fresnel;
    if (Settings.EnableFresnel)
        fresnel = Clamp(Settings.FresnelFactor * 0.25f, 0.0f, 1.0f);
    else
        fresnel = 0.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
}

void CEngineWaterRenderer::RenderSeaReflectiveForeground(float time)
{
    CRenderStateScopedCaptureBlock capture;
    if (SetSeaReflectiveForegroundTexturesAndUVModes()) {
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, Settings.BumpMatFactor * WaterBumpFactor, 0.0f, 1.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaForegroundReflectiveVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SeaForegroundReflectivePixelShader);
        NEngineInternal::GPRenderManager->SelectIndexBuffer(&ForegroundIndexBuffers, 0);
        SetupSeaReflectiveForegroundShaderConstantsAndFog();
        for (CWaterPatchMesh** it = SeaReflectiveForegroundRenderList.begin(); it != SeaReflectiveForegroundRenderList.end(); ++it)
            (*it)->Render(WaterForegroundPolyCount);
    }
}
