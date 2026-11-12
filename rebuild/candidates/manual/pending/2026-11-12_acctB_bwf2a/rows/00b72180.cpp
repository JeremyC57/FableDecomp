// CEngineWaterRenderer::CreateDynamicTextures(bool render_water_bump, bool render_sea_bump, bool render_ice_bump) -- fetch frame 0 of the
// five bump-map source graphics named by the engine environment def (water, sea, ice, water 2, sea 2; every GetFrame is evaluated, an
// unloaded frame starts loading) and return false when any is missing. Otherwise take the four scratch textures (environment map,
// water / sea / ice bump maps) and, inside a render-state capture block and a CScopedShadersEnable(true, true): switch z-buffer, alpha
// test, specular, alpha blend and fog off, select the foreground texture vertex / index buffers, render the environment map into its
// texture and mip it on the GPU (reflection generation and high-end water only), set wrap UV modes on stages 0 / 1, apply the bump-map
// shaders and render each requested bump map into every mip level of at least 32 x 32 (water: RenderBumpMap(0) from water frames 1 / 2;
// sea: RenderBumpMap(1) from the sea frames; ice: RenderIceBumpMap from the ice frame); then deselect the buffers, restore the back
// screen, detach both render-target surfaces, refresh the camera's D3D transforms and return true.
// Leaf: ?CreateDynamicTextures@CEngineWaterRenderer@@AAE_N_N00@Z
// Retail: Fable.exe @ 0x00B72180, 5587 bytes, thiscall, `ret 0Ch`, aligned frame (and esp,-8 ; sub esp,0A4h), no EH frame; only caller
//   CEngineWaterRenderer::Render 0x00B78584. Identity: Ego_r.pdb 0x00A1EE70 / FableWin.pdb ?CreateDynamicTextures@CEngineWaterRenderer@@AAE_N_N00@Z.
// Shape: the old shader-enable bytes at [esp+1Ch] / [esp+1Dh] (sharing the first expansion's new-CFrameData word) are the two bool members of
//   FableWin.pdb's CScopedShadersEnable (??0 / ??1CScopedShadersEnable@@, Ego_r layout OldVertexEnable / OldPixelEnable), whose destructor runs
//   after the render target / surface and before the capture block; the per-level loops are `while` loops with a `break` (a `for` gives the
//   code after them the loop body's static frequency and VC7.1 then expands ??_GCGraphicFrameData in the five success-path ~CGraphicFrame).
//   SetRenderTargetToSurface is the TU's inline member (retail's unreferenced out-of-line copy 0x00B7AD20, landed) expanded at its four sites.
// Inline-budget: FABLE_FORCEINLINE Lionhead header helpers as in the matched sky rows 0x00B64FA0 / 0x00B65A20 (PeekGraphicInfoFrame, GetFrame,
//   TouchTexture, TouchResource, ~CGraphicFrame), plus ~CScopedShadersEnable (FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE) and
//   SetRenderTargetToSurface (plain `inline` leaves both called); ??_GCGraphicFrameData stays a call at all ten sites as in retail.
// Callees: ??_GCGraphicFrameData 0x0041BDF0, CFrameData::BeginLoading 0x009FDE80, CFrameData(const CGraphicInfoFrame*) 0x009FE380,
//   CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, operator new 0x00BFEA1A, CGPUScratchBufferTextureManager::StartTextureUse
//   0x00F36EE0, CShaderRenderManager Enable/DisableVertexShaders 0x00987FE0 / 0x009880E0, Enable/DisablePixelShaders 0x00988110 / 0x00988190,
//   ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140, CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, CDisplayManager::SetRenderTarget
//   0x009BF160 / SetRenderTargetAsBackScreen 0x009BF220, GFAXToVX 0x009E1DA0 / GFAYToVY 0x009E1DD0, CSurface::Uninitialise 0x009F2E20 /
//   operator= 0x009F2E70 / GetDimensions 0x009F2F90, CTexture::GetDimensions 0x009F9DB0 / GetNoLevels 0x009F9DF0 / GetSurfaceLevel 0x009F9E00 /
//   GenerateMipmapsWithGPU 0x009FAF70, CRenderStateManager::RestoreCaptureBlock 0x00A05840, CRenderManagerCore::SetVWindow 0x00A0B560,
//   CRenderTarget::DetachTarget 0x00A10E10 / DetachZBuffer 0x00A10E20 / AttachTarget 0x00A10E60, CEngineCamera::RefreshD3DTransforms 0x00B2FC50,
//   RenderIceBumpMap 0x00B6E3E0, RenderBumpMap 0x00B6E890, RenderEnvironmentMap 0x00B70A90; IDirect3DDevice9 SetTexture / SetStreamSource /
//   SetIndices through the CRenderManagerCore inline helpers.
// Globals: GPResourceManager 0x01436E98 (GraphicBanks[0] +0x954, GPUScratchBufferTextureManager +0x3190), GPRenderManager 0x01436E18,
//   GPShaderRenderManager 0x01436E14, GPDisplayManager 0x01436E1C, GPCamera 0x01436EA0, g_pEngineEnvironmentDef 0x01436E24 (descriptive),
//   CShaderRenderManager::SingletonInstance 0x013BC470, the capture-block state manager 0x013CB508, D3DTextureUVModeTable 0x01396DC8.
#include "fable_compiler.h"
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_CREATEDYNAMICTEXTURES
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
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
#define FABLE_RETAIL_CTEXTURE_GETWIDTH
#define FABLE_RETAIL_CTEXTURE_WATERDYNAMIC
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR
#define FABLE_RETAIL_CSURFACE_ASSIGN
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#include "fable_string.h"
#include "engine/Direct3D9Types.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CRenderTarget.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnvironmentDef.h"
#include "engine/C2DBoxF.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }                // 0x01436E1C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24 (descriptive, chunk 727)
float __fastcall GFAXToVX(float x);                                                   // 0x009E1DA0
float __fastcall GFAYToVY(float y);                                                   // 0x009E1DD0

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

FABLE_FORCEINLINE void CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface)
{
    target.AttachTarget(surface);
    NEngineInternal::GPDisplayManager->SetRenderTarget(target);
    NEngineInternal::GPRenderManager->SetVWindow(C2DBoxF(0.0f, 0.0f, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())));
}

bool CEngineWaterRenderer::CreateDynamicTextures(bool render_water_bump, bool render_sea_bump, bool render_ice_bump)
{
    const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0);
    CGraphicFrame water_frame;
    CGraphicFrame water_frame2;
    CGraphicFrame sea_frame;
    CGraphicFrame sea_frame2;
    CGraphicFrame ice_frame;
    bool ok = bank->PeekGraphicInfoFrame(g_pEngineEnvironmentDef->WaterBumpMap, 0)->GetFrame(water_frame, 0, 0);
    ok &= bank->PeekGraphicInfoFrame(g_pEngineEnvironmentDef->SeaBumpMap, 0)->GetFrame(sea_frame, 0, 0);
    ok &= bank->PeekGraphicInfoFrame(g_pEngineEnvironmentDef->IceBumpMap, 0)->GetFrame(ice_frame, 0, 0);
    ok &= bank->PeekGraphicInfoFrame(g_pEngineEnvironmentDef->WaterBumpMap2, 0)->GetFrame(water_frame2, 0, 0);
    ok &= bank->PeekGraphicInfoFrame(g_pEngineEnvironmentDef->SeaBumpMap2, 0)->GetFrame(sea_frame2, 0, 0);
    if (!ok)
        return false;

    CGPUScratchBufferTextureManager& scratch = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    EnvironmentMap = scratch.StartTextureUse(EnvironmentMapHandle);
    WaterBumpMap = scratch.StartTextureUse(WaterBumpMapHandle);
    SeaBumpMap = scratch.StartTextureUse(SeaBumpMapHandle);
    IceBumpMap = scratch.StartTextureUse(IceBumpMapHandle);
    {
        CRenderStateScopedCaptureBlock capture;
        CScopedShadersEnable shaders(true, true);
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
        NEngineInternal::GPRenderManager->SetSpecularEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetFogEnable(false);
        NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &ForegroundTextureVertexBuffers);
        NEngineInternal::GPRenderManager->SelectIndexBuffer(&ForegroundTextureIndexBuffers, 0);
        {
            CRenderTarget render_target;
            CSurface surface;
            if (Settings.EnableReflectionGeneration && HighendWaterEnabled) {
                surface = EnvironmentMap->GetSurfaceLevel(0);
                SetRenderTargetToSurface(render_target, surface);
                RenderEnvironmentMap();
                render_target.DetachTarget();
                EnvironmentMap->GenerateMipmapsWithGPU();
            }
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(BumpmapVertexShader, false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(BumpmapPixelShader);
            if (render_water_bump) {
                NEngineInternal::GPRenderManager->AttachTextureToStage(0, water_frame.GetTexture());
                NEngineInternal::GPRenderManager->AttachTextureToStage(1, water_frame2.GetTexture());
                long level = 0;
                while (level < WaterBumpMap->GetNoLevels()) {
                    if (WaterBumpMap->GetWidth(level) < 32 || WaterBumpMap->GetHeight(level) < 32)
                        break;
                    surface = WaterBumpMap->GetSurfaceLevel(level);
                    SetRenderTargetToSurface(render_target, surface);
                    RenderBumpMap((EWaterArrayIndexAliases)0);
                    render_target.DetachTarget();
                    ++level;
                }
            }
            if (render_sea_bump) {
                NEngineInternal::GPRenderManager->AttachTextureToStage(0, sea_frame.GetTexture());
                NEngineInternal::GPRenderManager->AttachTextureToStage(1, sea_frame2.GetTexture());
                long level = 0;
                while (level < SeaBumpMap->GetNoLevels()) {
                    if (SeaBumpMap->GetWidth(level) < 32 || SeaBumpMap->GetHeight(level) < 32)
                        break;
                    surface = SeaBumpMap->GetSurfaceLevel(level);
                    SetRenderTargetToSurface(render_target, surface);
                    RenderBumpMap((EWaterArrayIndexAliases)1);
                    render_target.DetachTarget();
                    ++level;
                }
            }
            if (render_ice_bump) {
                NEngineInternal::GPRenderManager->AttachTextureToStage(0, ice_frame.GetTexture());
                long level = 0;
                while (level < IceBumpMap->GetNoLevels()) {
                    if (IceBumpMap->GetWidth(level) < 32 || IceBumpMap->GetHeight(level) < 32)
                        break;
                    surface = IceBumpMap->GetSurfaceLevel(level);
                    SetRenderTargetToSurface(render_target, surface);
                    RenderIceBumpMap();
                    render_target.DetachTarget();
                    ++level;
                }
            }
            NEngineInternal::GPRenderManager->SelectVertexBuffer(0, 0);
            NEngineInternal::GPRenderManager->SelectIndexBuffer(0, 0);
            NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
            render_target.DetachTarget();
            render_target.DetachZBuffer();
            NEngineInternal::GPCamera->RefreshD3DTransforms();
            surface.Uninitialise();
        }
    }
    return true;
}
