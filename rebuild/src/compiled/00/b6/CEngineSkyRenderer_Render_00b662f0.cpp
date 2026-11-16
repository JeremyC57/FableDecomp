// CEngineSkyRenderer::Render(const CEngineComponentRenderParams& params) -- render the sky inside a render-state capture block: clear state +0x2944 and set +0x2a0c; the sun-flare layer (0x400000) only renders the flare; otherwise swap the engine camera to a sky camera (origin, near 100, far 10000, its own z-buffer range), set +0x2868, draw the sky, restore the camera and issue the (empty in retail) debug sphere at the sun's far-plane position
// Leaf: Render
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B662F0, length 752 bytes, `ret 4`.
// Callees: RenderSunFlare 0x00B64550, CEngineCameraDesc copy constructor 0x00B4AF50 (out of line), CEngineCamera::SetCameraState
//   (const CEngineCameraDesc*, bool, bool) 0x00B30B50 on the camera @ 0x01436EA0, RenderSky(float, const C3DVector*, bool) 0x00B66190,
//   the renderer's debug sphere 0x00B8D040 (an empty `ret 0x10` in retail) on 0x01436E5C, RestoreCaptureBlock 0x00A05840.
// Layout: engine/CEngineSkyRenderer.h; CEngineCameraDesc / CEngineCamera / CEngineLightingManager members from the PDB
//   (struct_layouts_egor.tsv): CameraDesc at camera +0x14, SunVector at lighting manager +0x18.
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at
//   offset 0): the shared inline setters FogEnable(false) +0x2944, DitherEnable(true) +0x2A0C and ZBufferEnable(true) +0x2868; the
//   capture open / RestoreCaptureBlock pair on 0x013CB508 is a CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h).
//   0x01436E5C is NEngineInternal::GPOcclusionManager (?GPOcclusionManager@NEngineInternal@@3PAVCEngineOcclusionManager@@A,
//   engine/CEngineOcclusionManager.h) and the 0x00B8D040 stub is ?AddFlareTest@CEngineOcclusionManager@@QAEXKABVC3DVector@@MJ@Z
//   (overlay declaration opt-in; Ego_r donor delta, see the header). Was a local CRenderManagerCoreView / CRenderStateManager /
//   CTrackedRenderState model with g_pRenderManagerCore / g_pRenderStateManager, and g_pRenderer_01436e5c->DrawDebugSphere on a
//   local CEngineRendererView.
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ADDFLARETEST
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/C3DVector.h"
#include "engine/CEngineOcclusionManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }   // 0x01436E5C
#include "engine/CEngineSkyRenderer.h"

#include "engine/C3DVector.h"
class CMatrix3x4 { public: float M[12]; };
class C2DBoxF { public: float X0, Y0, X1, Y1; };
struct CEngineCameraDesc {                               // 0x6c bytes (PDB)
    C2DBoxF    Window;                                   // +0x00
    CMatrix3x4 RotationMatrix;                           // +0x10
    C3DVector  WorldPosition;                            // +0x40
    float      HorizontalFOV;                            // +0x4c
    float      VerticalFOV;                              // +0x50
    bool       Use2DFOV;                                 // +0x54
    float      NearZPlane;                               // +0x58
    float      FarZPlane;                                // +0x5c
    float      ZBufferStart;                             // +0x60
    float      ZBufferEnd;                               // +0x64
    bool       StateValid;                               // +0x68
    CEngineCameraDesc(const CEngineCameraDesc& other);   // 0x00B4AF50
};
class CEngineCamera { public:
    unsigned char     _pad00[0x14];
    CEngineCameraDesc CameraDesc;                        // +0x14
    void SetCameraState(const CEngineCameraDesc* desc, bool a, bool b);   // 0x00B30B50
};
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                   // 0x01436EA0
class CEngineLightingManager { public:
    unsigned char _pad00[0x18];
    C3DVector     SunVector;                             // +0x18
};
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A
extern float g_skyZBufferStart_0139a704;                 // 0x0139A704

struct CEngineInterpolationInfoSet { float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate; };
class CEngineComponentParams { public:
    float                       FrameInterpolation;      // +0x00
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent;   // +0x04..
};
class CEngineComponentRenderParams { public:
    CEngineComponentParams* Params;                      // +0x00
    long                    RenderLayer;                 // +0x04
    unsigned long           RenderLayerFlags;            // +0x08
};

struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    void RenderSunFlare();                                                       // 0x00B64550
    void RenderSky(float interpolate, const C3DVector* cameraPos, bool reflection);   // 0x00B66190
    void Render(const CEngineComponentRenderParams& params);
};

void CEngineSkyRenderer_Methods::Render(const CEngineComponentRenderParams& params)
{
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetDitherEnable(true);
    if (params.RenderLayer == 0x400000) {
        RenderSunFlare();
    } else {
        float interpolate = params.Params->BulletTimeCurrent.WFInterpolate;
        CEngineCameraDesc savedDesc(NEngineInternal::GPCamera->CameraDesc);
        CEngineCameraDesc skyDesc(savedDesc);
        skyDesc.NearZPlane = 100.0f;
        skyDesc.FarZPlane = 10000.0f;
        skyDesc.ZBufferStart = g_skyZBufferStart_0139a704;
        skyDesc.ZBufferEnd = 1.0f;
        skyDesc.WorldPosition.X = 0.0f;
        skyDesc.WorldPosition.Y = 0.0f;
        skyDesc.WorldPosition.Z = 0.0f;
        NEngineInternal::GPCamera->SetCameraState(&skyDesc, false, true);
        NEngineInternal::GPRenderManager->SetZBufferEnable(true);
        RenderSky(interpolate, &NEngineInternal::GPCamera->CameraDesc.WorldPosition, false);
        NEngineInternal::GPCamera->SetCameraState(&savedDesc, false, true);
        C3DVector sun = NEngineInternal::GPLightingManager->SunVector;
        float distance = NEngineInternal::GPCamera->CameraDesc.FarZPlane * 0.099f;
        C3DVector pos = sun * distance + NEngineInternal::GPCamera->CameraDesc.WorldPosition;
        NEngineInternal::GPOcclusionManager->AddFlareTest(0x1000000, pos, distance * 0.15f, 0x20);
    }
}