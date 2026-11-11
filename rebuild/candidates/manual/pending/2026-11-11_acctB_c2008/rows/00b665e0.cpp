// CEngineSkyRenderer::GetEnvironmentMap(CTexture* texture, const CEngineComponentRenderParams& params) -- render the sky into the top surface of the given texture: inside a render-state capture block, attach the texture's surface as the render target, set the virtual window to its size, clear it to opaque black, switch fog off, swap the engine camera to a 120-degree mirrored camera looking straight down the Z axis from below the sky (z buffer off) and draw the sky from a fixed position, then restore the back screen, the target and the saved camera
// Leaf: ?GetEnvironmentMap@CEngineSkyRenderer@@QAEXPAVCTexture@@ABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B665E0, 1171 bytes (`ret 8` at 0x00B66A70, then int3 padding and the small functions at
//   0x00B66A80..; the 1454-byte reservation ran over them), thiscall.
// Identity (two sources): Ego_r.pdb 0x00A18F50 / FableWin.pdb 0x02D418F0 ?GetEnvironmentMap@CEngineSkyRenderer@@QAEXPAVCTexture@@ABVCEngineComponentRenderParams@@@Z
//   (the only two-argument CEngineSkyRenderer member taking a texture; Ego_r donor order Render -> GetEnvironmentMap -> BuildStarFieldVB
//   = retail Render 0x00B662F0 -> 0x00B665E0); it calls RenderSky 0x00B66190 with `reflection` true.
// Callees: CEngineCameraDesc copy 0x00B4AF50, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, CTexture::GetSurfaceLevel 0x009F9E00,
//   CSurface::operator= 0x009F2E70 / Uninitialise 0x009F2E20 / GetDimensions 0x009F2F90, CRenderTarget::AttachTarget 0x00A10E60 /
//   DetachTarget 0x00A10E10 / DetachZBuffer 0x00A10E20, CDisplayManager::SetRenderTarget 0x009BF160 / SetRenderTargetAsBackScreen 0x009BF220
//   (0x01436E1C), GFAYToVY 0x009E1DD0 / GFAXToVX 0x009E1DA0, CRenderManagerCore::SetVWindow 0x00A0B560, CRenderManager2D::ClearRenderTarget
//   0x009D8CF0 (NEngineInternal::GPRenderManager 0x01436E18, with the inline SetFogEnable / SetZBufferEnable), CEngineCamera::SetCameraState
//   0x00B30B50 / RefreshD3DTransforms 0x00B2FC50 (NEngineInternal::GPCamera 0x01436EA0), RenderSky 0x00B66190, RestoreCaptureBlock 0x00A05840.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_ENVMAP
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_ASSIGN
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#include "engine/Direct3D9Types.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CRenderTarget.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineComponentRenderParams.h"
#include <math.h>

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
namespace NEngineInternal { extern CEngineCamera* GPCamera; }           // 0x01436EA0
extern CDisplayManager* g_pDisplayManager_01436e1c;                     // 0x01436E1C (established descriptive spelling)
extern float g_skyZBufferStart_0139a704;                                // 0x0139A704 (established descriptive spelling)
float __fastcall GFAXToVX(float x);                                     // 0x009E1DA0
float __fastcall GFAYToVY(float y);                                     // 0x009E1DD0

void CEngineSkyRenderer::GetEnvironmentMap(CTexture* texture, const CEngineComponentRenderParams& params)
{
    CRenderStateScopedCaptureBlock capture;
    CEngineCameraDesc old_desc(NEngineInternal::GPCamera->PeekCameraState());
    CRenderTarget render_target;
    CSurface surface;
    surface = texture->GetSurfaceLevel(0);
    render_target.AttachTarget(surface);
    g_pDisplayManager_01436e1c->SetRenderTarget(render_target);
    NEngineInternal::GPRenderManager->SetVWindow(C2DBoxF(0.0f, 0.0f, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())));
    NEngineInternal::GPRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 255), 4);
    NEngineInternal::GPRenderManager->SetFogEnable(false);

    float interpolate = params.Params->FrameInterpolation;
    CEngineCameraDesc desc(old_desc);
    const float fov = 2.0943952f;
    float z = -6500.0f / (float)tan(fov * 0.5f);
    static C3DVector sky_pos(0.0f, 0.0f, -2550.0f);
    desc.HorizontalFOV = fov;
    desc.FarZPlane = (7000.0f - z) * 1.1f;
    desc.NearZPlane = 1.0f;
    desc.ZBufferStart = g_skyZBufferStart_0139a704;
    desc.ZBufferEnd = 1.0f;
    desc.WorldPosition.X = 0.0f;
    desc.WorldPosition.Y = 0.0f;
    desc.WorldPosition.Z = z;
    desc.RotationMatrix.E11 = 1.0f; desc.RotationMatrix.E21 = 0.0f; desc.RotationMatrix.E31 = 0.0f;
    desc.RotationMatrix.E12 = 0.0f; desc.RotationMatrix.E22 = -1.0f; desc.RotationMatrix.E32 = 0.0f;
    desc.RotationMatrix.E13 = 0.0f; desc.RotationMatrix.E23 = 0.0f; desc.RotationMatrix.E33 = 1.0f;
    desc.RotationMatrix.E41 = 0.0f; desc.RotationMatrix.E42 = 0.0f; desc.RotationMatrix.E43 = 0.0f;
    desc.Window.TLX = 0.0f;
    desc.Window.TLY = 0.0f;
    desc.Window.BRX = (float)surface.GetWidth();
    desc.Window.BRY = (float)surface.GetHeight();
    NEngineInternal::GPCamera->SetCameraState(desc, false, true);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    RenderSky(interpolate, sky_pos, true);

    g_pDisplayManager_01436e1c->SetRenderTargetAsBackScreen();
    render_target.DetachTarget();
    render_target.DetachZBuffer();
    NEngineInternal::GPCamera->RefreshD3DTransforms();
    surface.Uninitialise();
    NEngineInternal::GPCamera->SetCameraState(old_desc, false, true);
}
