// CEngineSkyRenderer::Render(const CEngineComponentRenderParams& params) -- render the sky inside a render-state capture block: clear state +0x2944 and set +0x2a0c; the sun-flare layer (0x400000) only renders the flare; otherwise swap the engine camera to a sky camera (origin, near 100, far 10000, its own z-buffer range), set +0x2868, draw the sky, restore the camera and issue the (empty in retail) debug sphere at the sun's far-plane position
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B662F0, length 752 bytes, `ret 4`.
// Callees: RenderSunFlare 0x00B64550, CEngineCameraDesc copy constructor 0x00B4AF50 (out of line), CEngineCamera::SetCameraState
//   (const CEngineCameraDesc*, bool, bool) 0x00B30B50 on the camera @ 0x01436EA0, RenderSky(float, const C3DVector*, bool) 0x00B66190,
//   the renderer's debug sphere 0x00B8D040 (an empty `ret 0x10` in retail) on 0x01436E5C, RestoreCaptureBlock 0x00A05840.
// Layout: engine/CEngineSkyRenderer.h; CEngineCameraDesc / CEngineCamera / CEngineLightingManager members from the PDB
//   (struct_layouts_egor.tsv): CameraDesc at camera +0x14, SunVector at lighting manager +0x18.
#include "engine/CEngineSkyRenderer.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CMatrix3x4 { float M[12]; };
struct C2DBoxF { float X0, Y0, X1, Y1; };
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
struct CEngineCamera {
    unsigned char     _pad00[0x14];
    CEngineCameraDesc CameraDesc;                        // +0x14
    void SetCameraState(const CEngineCameraDesc* desc, bool a, bool b);   // 0x00B30B50
};
extern CEngineCamera* g_pEngineCamera;                   // 0x01436EA0
struct CEngineLightingManager {
    unsigned char _pad00[0x18];
    C3DVector     SunVector;                             // +0x18
};
extern CEngineLightingManager* g_pEngineLightingManager; // 0x01436E9C
struct CEngineRendererView {
    void DrawDebugSphere(unsigned long flags, const C3DVector& pos, float radius, unsigned long colour);   // 0x00B8D040
};
extern CEngineRendererView* g_pRenderer_01436e5c;        // 0x01436E5C
extern float g_skyZBufferStart_0139a704;                 // 0x0139A704

struct CEngineInterpolationInfoSet { float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate; };
struct CEngineComponentParams {
    float                       FrameInterpolation;      // +0x00
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent;   // +0x04..
};
struct CEngineComponentRenderParams {
    CEngineComponentParams* Params;                      // +0x00
    long                    RenderLayer;                 // +0x04
    unsigned long           RenderLayerFlags;            // +0x08
};
struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
    __forceinline void Set(unsigned long value);
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18 / 0x013CB508
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2868 - 0x2818];
    CTrackedRenderState  m_state2868;              // +0x2868
    unsigned char        _pad287c[0x2944 - 0x287c];
    CTrackedRenderState  m_state2944;              // +0x2944
    unsigned char        _pad2958[0x2a0c - 0x2958];
    CTrackedRenderState  m_state2a0c;              // +0x2A0C
    unsigned char        _pad2a20[0x3a24 - 0x2a20];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24

    __forceinline void ChangeState(CTrackedRenderState& state, unsigned long value)
    {
        if ((m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= m_captureMask;
            m_captured[m_capturedCount].m_pState = &state;
            m_captured[m_capturedCount].m_priorValue = state.m_value;
            ++m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            m_dirty[m_dirtyCount] = &state;
            ++m_dirtyCount;
        }
    }
    __forceinline void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
    void DrawCurrentBuffersIndexed(unsigned long baseVertex, unsigned long vertexCount,
                                   unsigned long startIndex, unsigned long primitiveCount);   // 0x00A0AD40
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}


static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
{
    if (state.m_value != value)
        core->ChangeState(state, value);
}
struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    void RenderSunFlare();                                                       // 0x00B64550
    void RenderSky(float interpolate, const C3DVector* cameraPos, bool reflection);   // 0x00B66190
    void Render(const CEngineComponentRenderParams& params);
};

void CEngineSkyRenderer_Methods::Render(const CEngineComponentRenderParams& params)
{
    g_pRenderStateManager->OpenCaptureBlock();
    SetState(g_pRenderManagerCore->m_state2944, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2a0c, 1, g_pRenderManagerCore);
    if (params.RenderLayer == 0x400000) {
        RenderSunFlare();
    } else {
        float interpolate = params.Params->BulletTimeCurrent.WFInterpolate;
        CEngineCameraDesc savedDesc(g_pEngineCamera->CameraDesc);
        CEngineCameraDesc skyDesc(savedDesc);
        skyDesc.NearZPlane = 100.0f;
        skyDesc.FarZPlane = 10000.0f;
        skyDesc.ZBufferStart = g_skyZBufferStart_0139a704;
        skyDesc.ZBufferEnd = 1.0f;
        skyDesc.WorldPosition.X = 0.0f;
        skyDesc.WorldPosition.Y = 0.0f;
        skyDesc.WorldPosition.Z = 0.0f;
        g_pEngineCamera->SetCameraState(&skyDesc, false, true);
        SetState(g_pRenderManagerCore->m_state2868, 1, g_pRenderManagerCore);
        RenderSky(interpolate, &g_pEngineCamera->CameraDesc.WorldPosition, false);
        g_pEngineCamera->SetCameraState(&savedDesc, false, true);
        C3DVector sun = g_pEngineLightingManager->SunVector;
        float distance = g_pEngineCamera->CameraDesc.FarZPlane * 0.099f;
        C3DVector pos = sun * distance + g_pEngineCamera->CameraDesc.WorldPosition;
        g_pRenderer_01436e5c->DrawDebugSphere(0x1000000, pos, distance * 0.15f, 0x20);
    }
    g_pRenderStateManager->RestoreCaptureBlock();
}
