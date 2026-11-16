// CEngineShadowRenderer::PostLayerRender -- finish shadow pass on layer2, optionally draw its colour texture.
// Retail00b4d2c0,162bytes; shared shadow layout334technique,24ccapture,318texture.
// Calls9bf220SetRenderTargetAsBackScreen,b2fc50RefreshD3DTransforms,a05840RestoreCaptureBlock,9dc6e0Draw2DTexture.
// NF45 (round 7): the stand-in globals are the PDB data symbols (Ego_r.pdb / FableWin.pdb):
//   g2D 0x01436E18 -> ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A (engine/CRenderManager.h; the box overload
//     CRenderManager2D::Draw2DTexture 0x009DC6E0, repo opt-in DRAW2DTEXTUREBOX), gCamera 0x01436EA0 -> NEngineInternal::GPCamera
//     (CEngineCamera::RefreshD3DTransforms 0x00B2FC50, repo opt-in), gDisplay 0x01436E1C -> NEngineInternal::GPDisplayManager
//     (CDisplayManager::SetRenderTargetAsBackScreen 0x009BF220, repo opt-in BACKSCREEN), gState 0x013CB508 ->
//     ShadowBufferRenderStateCaptureBlock.Restore() (CRenderStateCaptureBlock::StateManager; `Open = false` before the
//     RestoreCaptureBlock 0x00A05840 call, repo INLINE + RESTORE_OPEN_FIRST; overlay opt-in types the +0x24C member).
//   The local CRGBColour / C2DBoxF / BoxMethods stand-ins are engine/CRGBColour.h / engine/C2DBoxF.h. The non-MSVC fallback branch
//   (local class models) is dropped.
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_RESTORE_OPEN_FIRST
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWBUFFERRENDERSTATECAPTUREBLOCK
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CTexture.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineComponentRenderParams.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }            // 0x01436E18
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }          // 0x01436E1C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                    // 0x01436EA0
extern bool gShowShadowBuffer; //01437800
struct ShadowMethods:CEngineShadowRenderer {void PostLayerRender(const CEngineComponentRenderParams&);};
void ShadowMethods::PostLayerRender(const CEngineComponentRenderParams& params) {
 if(*reinterpret_cast<long*>(RenderingTechnique) && params.RenderLayer==2){
  NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();NEngineInternal::GPCamera->RefreshD3DTransforms();ShadowBufferRenderStateCaptureBlock.Restore();
 }
 if(gShowShadowBuffer){
  NEngineInternal::GPRenderManager->Draw2DTexture(C2DBoxF(1000,1000,20000,20000),CRGBColour(255,255,255,255),*reinterpret_cast<CTexture*>(ShadowBufferColourTexture),0);
 }
}
