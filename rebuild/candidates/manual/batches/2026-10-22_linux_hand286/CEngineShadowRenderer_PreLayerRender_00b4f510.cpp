// CEngineShadowRenderer::PreLayerRender(const CEngineComponentRenderParams& params) -- before the shadow-buffer layer (2), when a shadow technique is active: open the shadow capture block, set the depth-bias states from the shadow renderer's z offsets, bind the shadow render target and clear it to white (colour only for technique 1, which also clears +0x2840; colour+depth otherwise), set the window to the shadow texture's size, set states +0x28cc=1 / +0x28e0=0x80, an identity view and the shadow projection
// Leaf: PreLayerRender
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4F510, length 976 bytes, `ret 4` (virtual).
// Callees: CDisplayManager::SetRenderTarget 0x009BF160 / ClearRenderTarget(const CRGBColour*, ulong) 0x009BE420 on 0x01436E1C,
//   CTexture::GetDimensions(long) 0x009F9DB0 (C2DExtentsI by value), CRenderManagerCore::SetAWindow(const C2DBoxF*) 0x00A0AA80,
//   CShaderRenderManager::SetViewTransform 0x00988350 / SetProjectionTransform 0x00988540 on 0x01436E14.
// The z offsets are read through the shadow renderer global 0x01436E60, not `this`.
// Layout: engine/CEngineShadowRenderer.h (ShadowBufferRenderStateCaptureBlock +0x24c, RenderTarget +0x2c4,
//   ShadowBufferTexture +0x2e8, RenderingTechnique +0x334); the projection matrix sits in SceneData at +0x4c.
#include "engine/CEngineShadowRenderer.h"

struct C2DExtentsI { unsigned long Width; unsigned long Height; };
struct C2DBoxF { float X0, Y0, X1, Y1; };
struct CMatrix3x4 { float M[12]; };
struct CMatrix4x4 { float M[16]; };
struct CRGBColour {
    unsigned char B, G, R, A;
};
struct CRenderTarget;
struct CTextureView {
    C2DExtentsI GetDimensions(long level) const;        // 0x009F9DB0
};
struct CDisplayManager {
    void SetRenderTarget(CRenderTarget* target);        // 0x009BF160
    void ClearRenderTarget(const CRGBColour* colour, unsigned long flags);   // 0x009BE420
};
extern CDisplayManager* g_pDisplayManager;              // 0x01436E1C
class CShaderRenderManager {
public:
    void SetViewTransform(const CMatrix3x4& view);      // 0x00988350
    void SetProjectionTransform(const CMatrix4x4& projection);   // 0x00988540
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
extern CEngineShadowRenderer* g_pShadowRenderer;        // 0x01436E60
struct CEngineComponentRenderParams {
    void*         Params;                               // +0x00
    long          RenderLayer;                          // +0x04
    unsigned long RenderLayerFlags;                     // +0x08
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
    unsigned char        _pad2818[0x2840 - 0x2818];
    CTrackedRenderState  m_state2840;              // +0x2840
    unsigned char        _pad2854[0x28cc - 0x2854];
    CTrackedRenderState  m_state28cc;              // +0x28CC
    CTrackedRenderState  m_state28e0;              // +0x28E0
    unsigned char        _pad28f4[0x2b60 - 0x28f4];
    CTrackedRenderState  m_state2b60;              // +0x2B60
    CTrackedRenderState  m_state2b74;              // +0x2B74
    unsigned char        _pad2b88[0x3a24 - 0x2b88];
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
    void SetAWindow(const C2DBoxF* window);        // 0x00A0AA80
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
    if (state.m_value != value) {
        if ((core->m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= core->m_captureMask;
            core->m_captured[core->m_capturedCount].m_pState = &state;
            core->m_captured[core->m_capturedCount].m_priorValue = state.m_value;
            ++core->m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            core->m_dirty[core->m_dirtyCount] = &state;
            ++core->m_dirtyCount;
        }
    }
}

static __forceinline unsigned long FloatBits(float value) { return *(unsigned long*)&value; }

static __forceinline void SetWindowSize(C2DBoxF& window, unsigned long width, unsigned long height)
{
    window.X0 = 0.0f;
    window.Y0 = 0.0f;
    window.X1 = (float)width;
    window.Y1 = (float)height;
}
struct CEngineShadowRenderer_Methods : CEngineShadowRenderer {
    void PreLayerRender(const CEngineComponentRenderParams& params);
};

void CEngineShadowRenderer_Methods::PreLayerRender(const CEngineComponentRenderParams& params)
{
    if (*(long*)RenderingTechnique != 0 && params.RenderLayer == 2) {
        *(bool*)ShadowBufferRenderStateCaptureBlock = true;
        g_pRenderStateManager->OpenCaptureBlock();
        SetState(g_pRenderManagerCore->m_state2b60, FloatBits(g_pShadowRenderer->ShadowProjectionZOffset), g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_state2b74, FloatBits(g_pShadowRenderer->ShadowProjectionZSlopeOffset), g_pRenderManagerCore);
        g_pDisplayManager->SetRenderTarget((CRenderTarget*)RenderTarget);
        if (*(long*)RenderingTechnique == 1) {
            CRGBColour white;
            white.R = 0xff;
            white.G = 0xff;
            white.B = 0xff;
            white.A = 0xff;
            g_pDisplayManager->ClearRenderTarget(&white, 1);
            SetState(g_pRenderManagerCore->m_state2840, 0, g_pRenderManagerCore);
        } else {
            CRGBColour white;
            white.R = 0xff;
            white.G = 0xff;
            white.B = 0xff;
            white.A = 0xff;
            g_pDisplayManager->ClearRenderTarget(&white, 5);
        }
        C2DBoxF window;
        SetWindowSize(window, ((CTextureView*)ShadowBufferTexture)->GetDimensions(0).Width,
                      ((CTextureView*)ShadowBufferTexture)->GetDimensions(0).Height);
        g_pRenderManagerCore->SetAWindow(&window);
        SetState(g_pRenderManagerCore->m_state28cc, 1, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_state28e0, 0x80, g_pRenderManagerCore);
        CMatrix3x4 identity;
        identity.M[11] = 0.0f;
        identity.M[10] = 0.0f;
        identity.M[9] = 0.0f;
        identity.M[7] = 0.0f;
        identity.M[6] = 0.0f;
        identity.M[5] = 0.0f;
        identity.M[3] = 0.0f;
        identity.M[2] = 0.0f;
        identity.M[1] = 0.0f;
        identity.M[8] = 1.0f;
        identity.M[4] = 1.0f;
        identity.M[0] = 1.0f;
        g_pShaderRenderManager->SetViewTransform(identity);
        g_pShaderRenderManager->SetProjectionTransform(*(const CMatrix4x4*)(SceneData + 0x40));
    }
}
