// Grade: DIFFER(1547v1547) -- one commutative pair: the +0x2a0c SetState block emits test eax,esi / or esi,eax where retail has test esi,eax / or eax,esi (the 0x3164/0x30c4 blocks already match); mask-local helper, Set member, k-sweep and 118 per-site variants inert
// CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur() -- fade the four texture-to-back-buffer quad vertices to the current parameters' Fade (grey level, opaque alpha), then draw the second blur texture over the back buffer with the texture-diffuse pixel shader inside a render-state capture block, vertex constant layout 2 and the shaders forced on, all restored afterwards
// Leaf: ApplyRadialBlur
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5E0F0, length 1552 bytes (body 0x60b + int3 padding), plain `ret`.
// Twin of the landed ShowBlurTexture 0x00B5E700 (same RAII savers, same inlined PrepareForRender(0) and SetState helper):
//   the fade is __ftol2 @ 0x00BFEA70 of Fade * 255.0f (0x01230014) clamped to 0..255 and spread to 0xFFgggggg; shaders
//   ScreenCopyVertexShader +0x20 / TextureDiffusePixelShader +0x48; states +0x28b8 <- 1, +0x28cc/+0x2868/+0x2a0c <- 0,
//   +0x291c/+0x2930 <- [0x01396F6C], +0x2890 <- [0x01396FB8], +0x3164/+0x30c4 <- [0x01396E5C]; DrawAsIndexedTris of
//   BlurTexture256_2 with TextureToBackBufferVerts (+0x110, 0x18-byte CTVertexColTex1, Diffuse +0xc) and the quad
//   indices @ 0x0139A544.
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h.
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

struct C4DVector { float X, Y, Z, W; };
struct CTexture;

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
    unsigned char        _pad287c[0x2890 - 0x287c];
    CTrackedRenderState  m_state2890;              // +0x2890
    unsigned char        _pad28a4[0x28b8 - 0x28a4];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    CTrackedRenderState  m_state28cc;              // +0x28CC
    unsigned char        _pad28e0[0x291c - 0x28e0];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
    unsigned char        _pad2944[0x2a0c - 0x2944];
    CTrackedRenderState  m_state2a0c;              // +0x2A0C
    unsigned char        _pad2a20[0x30c4 - 0x2a20];
    CTrackedRenderState  m_state30c4;              // +0x30C4
    unsigned char        _pad30d8[0x3164 - 0x30d8];
    CTrackedRenderState  m_state3164;              // +0x3164
    unsigned char        _pad3178[0x3a24 - 0x3178];
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
    void DrawAsIndexedTris(CTexture** textures, unsigned long textureCount, const void* vertices, long stride,
                           long vertexCount, const void* indices, long triangleCount);   // 0x00A0AEA0
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

extern unsigned long g_renderStateDefault_01396fb8;    // 0x01396FB8
extern unsigned long g_renderStateDefault_01396f6c;    // 0x01396F6C
extern unsigned long g_renderStateDefault_01396e5c;    // 0x01396E5C
extern const unsigned short g_QuadIndices_0139a544[6]; // 0x0139A544

static __forceinline void SetState(unsigned long value, CTrackedRenderState& state, CRenderManagerCoreView* core)
{
    if (state.m_value != value)
        core->ChangeState(state, value);
}
struct CShaderEnableState {                             // the static shader manager object @ 0x013BC470
    unsigned char _pad000[0x8];
    long          CurrentVSConstantLayoutIndex;         // +0x008
    unsigned char _pad00c[0x390 - 0xc];
    bool          VertexShadersEnabled;                 // +0x390
    bool          PixelShadersEnabled;                  // +0x391
    unsigned char _pad392[2];
    long          _pad394;
    void SetVertexShaderConstantLayout(long layout);    // 0x0098B5E0
    void EnableVertexShaders();                         // 0x00987FE0
    void DisableVertexShaders();                        // 0x009880E0
    void EnablePixelShaders();                          // 0x00988110
    void DisablePixelShaders();                         // 0x00988190
    __forceinline void SetVertexShadersEnabled(bool enable)
    {
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    __forceinline void SetPixelShadersEnabled(bool enable)
    {
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};
extern CShaderEnableState g_ShaderRenderManager;        // 0x013BC470

template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
struct CShaderConstantRange {
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};
struct CVertexShaderConstantLayout {
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};
struct CVertexShader;
struct CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(CVertexShader* shader, bool force);                  // 0x00988020
    void ApplyPixelShader(CPixelShader* shader);                                // 0x00988140

    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];                       // +0x1e9
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void ResetWorldTransform();                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
struct CShaderEnableSaver {                             // saves and forces the shader enables, restores them on exit
    bool m_vertexShaders;
    bool m_pixelShaders;
    __forceinline CShaderEnableSaver(bool vertexShaders, bool pixelShaders)
    {
        m_vertexShaders = g_ShaderRenderManager.VertexShadersEnabled;
        g_ShaderRenderManager.SetVertexShadersEnabled(vertexShaders);
        m_pixelShaders = g_ShaderRenderManager.PixelShadersEnabled;
        g_ShaderRenderManager.SetPixelShadersEnabled(pixelShaders);
    }
    __forceinline ~CShaderEnableSaver()
    {
        g_ShaderRenderManager.SetVertexShadersEnabled(m_vertexShaders);
        g_ShaderRenderManager.SetPixelShadersEnabled(m_pixelShaders);
    }
};
struct CShaderLayoutSaver {                             // selects a vertex constant layout, restores the previous one on exit
    long m_layout;
    __forceinline CShaderLayoutSaver(long layout)
    {
        m_layout = g_ShaderRenderManager.CurrentVSConstantLayoutIndex;
        g_ShaderRenderManager.SetVertexShaderConstantLayout(layout);
    }
    __forceinline ~CShaderLayoutSaver() { g_ShaderRenderManager.SetVertexShaderConstantLayout(m_layout); }
};
struct CRenderStateCaptureBlock {
    __forceinline CRenderStateCaptureBlock() { g_pRenderStateManager->OpenCaptureBlock(); }
    __forceinline ~CRenderStateCaptureBlock() { g_pRenderStateManager->RestoreCaptureBlock(); }
};

struct CTVertexColTex1 {                         // 0x18 bytes
    float X, Y, Z;
    unsigned long Diffuse;
    float U, V;
};

struct CEngineScreenEffectRadialBlurRenderer_Methods : CEngineScreenEffectRadialBlurRenderer {
    void ApplyRadialBlur();
};

void CEngineScreenEffectRadialBlurRenderer_Methods::ApplyRadialBlur()
{
    long fade = (long)(CurrentParameters_Fade * 255.0f);
    if (fade < 0)
        fade = 0;
    else if (fade > 255)
        fade = 255;
    for (long i = 0; i < 4; ++i)
        ((CTVertexColTex1*)TextureToBackBufferVerts)[i].Diffuse = fade * 0x10101 + 0xff000000;
    CRenderStateCaptureBlock capture;
    CShaderLayoutSaver layout(2);
    CShaderEnableSaver shaders(true, true);
    g_pShaderRenderManager->ApplyVertexShader((CVertexShader*)ScreenCopyVertexShader, false);
    g_pShaderRenderManager->ApplyPixelShader((CPixelShader*)TextureDiffusePixelShader);
    SetState(1, g_pRenderManagerCore->m_state28b8, g_pRenderManagerCore);
    SetState(0, g_pRenderManagerCore->m_state28cc, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396f6c, g_pRenderManagerCore->m_state291c, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396f6c, g_pRenderManagerCore->m_state2930, g_pRenderManagerCore);
    SetState(0, g_pRenderManagerCore->m_state2868, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396fb8, g_pRenderManagerCore->m_state2890, g_pRenderManagerCore);
    SetState(0, g_pRenderManagerCore->m_state2a0c, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396e5c, g_pRenderManagerCore->m_state3164, g_pRenderManagerCore);
    SetState(g_renderStateDefault_01396e5c, g_pRenderManagerCore->m_state30c4, g_pRenderManagerCore);
    g_pShaderRenderManager->PrepareForRender(0);
    CTexture* texture = BlurTexture256_2;
    g_pRenderManagerCore->DrawAsIndexedTris(&texture, 1, TextureToBackBufferVerts, 0x18, 4, g_QuadIndices_0139a544, 2);
}
