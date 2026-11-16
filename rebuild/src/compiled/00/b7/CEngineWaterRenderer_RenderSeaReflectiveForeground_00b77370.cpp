// CEngineWaterRenderer::RenderSeaReflectiveForeground(float time) -- open a scoped render-state capture block; fetch the environment's water-surf and water-edge alpha frames from graphic bank 0 (skip the pass when either is not loaded); bind the sea bump map, the environment map and the surf texture with their address modes; set the bump user constant, apply the reflective sea foreground shaders and the foreground index buffer, upload the reflective sea foreground constants and flush them; then render every patch of the reflective sea foreground list
// Grade: RELOCATION_MATCH (wfA4 successor-6, 2026-11-16). FITTED: the helper shape -- the edge-alpha map read is its own statement inside `if (<surf GetFrame>)`, a successful edge GetFrame jumps (`goto frames_loaded;`) over ONE shared `return false;` to the binding code. Measured: a separate `if (!edge GetFrame) return false;` (old differ / nested-return forms) splits c2's const-0 piece in the edge GetFrame's failure paths (the two `push 0 ; push 0 ; call BeginLoading` tails cross-jump, -13 bytes; retail 0x00B776EF `xor ebp,ebp` + `push ebp` x2); a shared exit placed AFTER the success body (nested ifs, `if (!A) return false; if (B) {...}`) inverts the ??_GCGraphicFrameData call/expand order; an inline edge read (`if (!A || !B) return false;`) evaluates the GP chain first at 0x00B774DD; bool-flag forms keep the split. Precedent for a goto to a shared tail: landed 0x0075D9A0 / 0x00847C90.
// Leaf: ?RenderSeaReflectiveForeground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B77370, length 2734 bytes, `ret 4` (time unused).
// Inferred: 19 dead `index < GraphicList.size()` asserts in the in-row PeekGraphic (FableWin PeekGraphic 170i /Od v 25i without asserts: asserts evidenced, count FITTED -- they move c2's caller-size counter so ??_GCGraphicFrameData is called for the surf frame's final release at 0x00B77AE7 and expanded for the edge frame and the failure-path release; window n=19 only, measured at k=4 with 70 and 78 helper asserts: 18 -> 2804 bytes, 20 -> 2688) + 70 dead `SeaBumpMap != 0` asserts at the helper top (FableWin helper 269i /Od v ours 107i without asserts: debug asserts evidenced; count FITTED -- c2's operand sort of the six SetTextureUVModeU/V bookmark tests 0x00B77826..0x00B77A54 keys on records allocated before them; window 70..86 at k=4..6).
// Fidelity: four unused CGraphicFrame locals (unused_frame0..3) right after the capture block (device c, the 0x00B751F0 closer): re-key the test/or operand order of the UV sites; k=0..3 never match (10..24 hunks), k=4..6 match with 70..86 helper asserts; k=4 is the smallest.
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
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
#include "engine/GFLimit.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    FABLE_RETAIL_ASSERT(index < GraphicList.size());
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}


FABLE_FORCEINLINE bool CEngineWaterRenderer::SetSeaReflectiveForegroundTexturesAndUVModes()
{
    CGraphicFrame surf;
    CGraphicFrame edge_alpha;
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    FABLE_RETAIL_ASSERT(SeaBumpMap != 0);
    long surf_map = g_pEngineEnvironmentDef->WaterSurfMap;
    if (NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(surf_map, 0)->GetFrame(surf, 0, 0)) {
        long edge_map = g_pEngineEnvironmentDef->WaterEdgeAlphaMap;
        if (NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(edge_map, 0)->GetFrame(edge_alpha, 0, 0))
            goto frames_loaded;
    }
    return false;
frames_loaded:
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
    {
        fresnel = Settings.FresnelFactor * 0.25f;
        GFLimit(fresnel, 0.0f, 1.0f);
    }
    else
        fresnel = 0.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
}

void CEngineWaterRenderer::RenderSeaReflectiveForeground(float time)
{
    CRenderStateScopedCaptureBlock capture;
    CGraphicFrame unused_frame0;
    CGraphicFrame unused_frame1;
    CGraphicFrame unused_frame2;
    CGraphicFrame unused_frame3;
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