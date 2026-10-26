// CEngineWeatherRenderer::RenderMist(const CEngineComponentRenderParams&) -- bind the mist volume texture on stages 0-3, the mist shaders, index and vertex buffers, upload the ten mist vertex-shader constants, set one render state, flush shader state and draw the mist quad
// Leaf: RenderMist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B54310, length 1234 bytes (+ int3 padding), `ret 4` (params unused).
// Signature from Ego_r.pdb: ?RenderMist@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z.
// Callees: CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (inline OpenCaptureBlock on 0x013CB508),
// CShaderRenderManager::ApplyPixelShader @ 0x00988140, ApplyVertexShader @ 0x00988020, SetUserVertexShaderConstant
// @ 0x00989B00, the inlined PrepareForRender(1) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20,
// UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals
// 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50), CRenderManagerCore::RealiseRenderState @ 0x00A058C0,
// DrawCurrentBuffersAsIndexedTris @ 0x00A0AC60; D3D device slots SetTexture (+0x104), SetStreamSource (+0x190),
// SetIndices (+0x1a0).
// Globals: render-state manager 0x013CB508, shader manager 0x01436E14, render core 0x01436E18, mist parameter object
// 0x01436E9C (+0xd0 position, +0xdc scale), engine camera 0x01436EA0 (CMatrix3x4 at +0xe4), state value 0x01396FB8.
// Layout: engine/CEngineWeatherRenderer.h for `this` (MistVertexBuffer +0x88, MistIndexBuffer +0xbc, MistVertexShader
// +0x100, MistPixelShader +0x108, MistVolumeTexture +0x110, Settings +0x114). Settings field names past the landed
// RenderRainBox view are descriptive (PDB CEngineWeatherSettings offsets +0x74..+0xb4, +0x4c8, +0x4e4).
// Shape: the core inlines SetTexture (attached-texture cache +0x3d00, active stage count +0x3cb8, the landed
// AttachTextureToStage 0x009A0CF0 body), SetIndices (if/else arms so the device loads before the null test) and
// SetStreamSource (the landed CWaterPatchMesh::Render model). The mist-parameter global is read into a named local:
// with the global spelled at each use VC loads it first and swaps the fld/fmul of `height * scale`.
#include "engine/CEngineWeatherRenderer.h"

class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; };
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

class CShaderConstantRange { public:
    void* _vptr;
    long Offset;
    unsigned long Count;
};

class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(const CVertexShader* shader, long flags);   // 0x00988020
    void ApplyPixelShader(const CPixelShader* shader);                 // 0x00988140
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00

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
    unsigned char _pad008[0x1e0];
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];
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
struct IDirect3DDevice9 {
    virtual void __stdcall Slot0() = 0;
    virtual void __stdcall Slot1() = 0;
    virtual void __stdcall Slot2() = 0;
    virtual void __stdcall Slot3() = 0;
    virtual void __stdcall Slot4() = 0;
    virtual void __stdcall Slot5() = 0;
    virtual void __stdcall Slot6() = 0;
    virtual void __stdcall Slot7() = 0;
    virtual void __stdcall Slot8() = 0;
    virtual void __stdcall Slot9() = 0;
    virtual void __stdcall Slot10() = 0;
    virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0;
    virtual void __stdcall Slot13() = 0;
    virtual void __stdcall Slot14() = 0;
    virtual void __stdcall Slot15() = 0;
    virtual void __stdcall Slot16() = 0;
    virtual void __stdcall Slot17() = 0;
    virtual void __stdcall Slot18() = 0;
    virtual void __stdcall Slot19() = 0;
    virtual void __stdcall Slot20() = 0;
    virtual void __stdcall Slot21() = 0;
    virtual void __stdcall Slot22() = 0;
    virtual void __stdcall Slot23() = 0;
    virtual void __stdcall Slot24() = 0;
    virtual void __stdcall Slot25() = 0;
    virtual void __stdcall Slot26() = 0;
    virtual void __stdcall Slot27() = 0;
    virtual void __stdcall Slot28() = 0;
    virtual void __stdcall Slot29() = 0;
    virtual void __stdcall Slot30() = 0;
    virtual void __stdcall Slot31() = 0;
    virtual void __stdcall Slot32() = 0;
    virtual void __stdcall Slot33() = 0;
    virtual void __stdcall Slot34() = 0;
    virtual void __stdcall Slot35() = 0;
    virtual void __stdcall Slot36() = 0;
    virtual void __stdcall Slot37() = 0;
    virtual void __stdcall Slot38() = 0;
    virtual void __stdcall Slot39() = 0;
    virtual void __stdcall Slot40() = 0;
    virtual void __stdcall Slot41() = 0;
    virtual void __stdcall Slot42() = 0;
    virtual void __stdcall Slot43() = 0;
    virtual void __stdcall Slot44() = 0;
    virtual void __stdcall Slot45() = 0;
    virtual void __stdcall Slot46() = 0;
    virtual void __stdcall Slot47() = 0;
    virtual void __stdcall Slot48() = 0;
    virtual void __stdcall Slot49() = 0;
    virtual void __stdcall Slot50() = 0;
    virtual void __stdcall Slot51() = 0;
    virtual void __stdcall Slot52() = 0;
    virtual void __stdcall Slot53() = 0;
    virtual void __stdcall Slot54() = 0;
    virtual void __stdcall Slot55() = 0;
    virtual void __stdcall Slot56() = 0;
    virtual void __stdcall Slot57() = 0;
    virtual void __stdcall Slot58() = 0;
    virtual void __stdcall Slot59() = 0;
    virtual void __stdcall Slot60() = 0;
    virtual void __stdcall Slot61() = 0;
    virtual void __stdcall Slot62() = 0;
    virtual void __stdcall Slot63() = 0;
    virtual void __stdcall Slot64() = 0;
    virtual long __stdcall SetTexture(unsigned long stage, void* texture) = 0;
    virtual void __stdcall Slot66() = 0;
    virtual void __stdcall Slot67() = 0;
    virtual void __stdcall Slot68() = 0;
    virtual void __stdcall Slot69() = 0;
    virtual void __stdcall Slot70() = 0;
    virtual void __stdcall Slot71() = 0;
    virtual void __stdcall Slot72() = 0;
    virtual void __stdcall Slot73() = 0;
    virtual void __stdcall Slot74() = 0;
    virtual void __stdcall Slot75() = 0;
    virtual void __stdcall Slot76() = 0;
    virtual void __stdcall Slot77() = 0;
    virtual void __stdcall Slot78() = 0;
    virtual void __stdcall Slot79() = 0;
    virtual void __stdcall Slot80() = 0;
    virtual void __stdcall Slot81() = 0;
    virtual void __stdcall Slot82() = 0;
    virtual void __stdcall Slot83() = 0;
    virtual void __stdcall Slot84() = 0;
    virtual void __stdcall Slot85() = 0;
    virtual void __stdcall Slot86() = 0;
    virtual void __stdcall Slot87() = 0;
    virtual void __stdcall Slot88() = 0;
    virtual void __stdcall Slot89() = 0;
    virtual void __stdcall Slot90() = 0;
    virtual void __stdcall Slot91() = 0;
    virtual void __stdcall Slot92() = 0;
    virtual void __stdcall Slot93() = 0;
    virtual void __stdcall Slot94() = 0;
    virtual void __stdcall Slot95() = 0;
    virtual void __stdcall Slot96() = 0;
    virtual void __stdcall Slot97() = 0;
    virtual void __stdcall Slot98() = 0;
    virtual void __stdcall Slot99() = 0;
    virtual long __stdcall SetStreamSource(unsigned int stream, void* data, unsigned int offset, unsigned int stride) = 0;
    virtual void __stdcall Slot101() = 0;
    virtual void __stdcall Slot102() = 0;
    virtual void __stdcall Slot103() = 0;
    virtual long __stdcall SetIndices(void* indexData) = 0;
};

struct CBaseTextureView {                          // CBaseTexture: device texture at +4
    void* _vptr;
    void* PD3DTexture;                             // +0x04
};
struct CIndexBufferView {                          // CIndexBuffer: device index buffer at +4
    void* _vptr;
    void* PIndexBuffer;                            // +0x04
};
struct CVertexBufferView {                         // CVertexBufferWin32: device buffer +4, stride +0x10
    void* _vptr;
    void* PVertexBuffer;                           // +0x04
    unsigned char _pad8[8];
    unsigned char Stride;                          // +0x10
};

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
class CRenderManagerCore {                         // the render manager core @ 0x01436E18
public:
    void RealiseRenderState();                     // 0x00A058C0
    void DrawCurrentBuffersAsIndexedTris(unsigned long minVertexIndex, unsigned long vertexCount,
                                         unsigned long startIndex, unsigned long triangleCount);   // 0x00A0AC60
    __forceinline void SetTexture(long stage, const CBaseTextureView* texture)
    {
        void* deviceTexture = texture ? texture->PD3DTexture : 0;
        if (m_attachedTextures[stage] != deviceTexture) {
            m_attachedTextures[stage] = deviceTexture;
            m_pDevice->SetTexture(stage, deviceTexture);
        }
        if (m_activeTextureStageCount < stage + 1)
            m_activeTextureStageCount = stage + 1;
    }
    __forceinline void SetIndices(const CIndexBufferView* indices)
    {
        m_pCurrentIndices = indices;
        if (indices)
            m_pDevice->SetIndices(indices->PIndexBuffer);
        else
            m_pDevice->SetIndices(0);
    }
    __forceinline void SetStreamSource(const CVertexBufferView* vertices)
    {
        if (vertices)
            m_pDevice->SetStreamSource(0, vertices->PVertexBuffer, 0, vertices->Stride);
        else
            m_pDevice->SetStreamSource(0, 0, 0, 0);
    }

    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2890 - 0x2818];
    CTrackedRenderState  m_state2890;              // +0x2890
    unsigned char        _pad28a4[0x3cb8 - 0x28a4];
    long                 m_activeTextureStageCount;   // +0x3CB8
    unsigned char        _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*    m_pDevice;                // +0x3CF0
    const CIndexBufferView* m_pCurrentIndices;     // +0x3CF4
    unsigned char        _pad3cf8[8];
    void*                m_attachedTextures[8];    // +0x3D00
};
extern CRenderManagerCore* g_pRenderManagerCore;   // 0x01436E18

static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManagerCore* core)
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

struct SCaptureEntry { void* Block; unsigned long State; };
class CRenderStateManager { public:                       // @ 0x013CB508
    void RestoreCaptureBlock();                    // 0x00A05840
    unsigned char _pad000[8];
    SCaptureEntry CaptureStack[0x500];             // +0x008
    unsigned long CaptureStackCount;               // +0x2808
    unsigned char _pad280c[8];
    unsigned long CaptureMask;                     // +0x2814
    unsigned char _pad2818[0x3a24 - 0x2818];
    unsigned char CaptureBlock[4];                 // +0x3a24
    void OpenCaptureBlock()
    {
        CaptureStack[CaptureStackCount++].Block = CaptureBlock;
        CaptureMask <<= 1;
    }
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

struct CEngineCameraView {                         // @ 0x01436EA0
    unsigned char _pad0[0xe4];
    CMatrix3x4 ViewTransform;                      // +0xE4
};
extern CEngineCameraView* g_pEngineCamera;         // 0x01436EA0

struct CMistParamsView {                           // @ 0x01436E9C
    unsigned char _pad0[0xd0];
    C3DVector Position;                            // +0xD0
    float Scale;                                   // +0xDC
};
extern CMistParamsView* g_pMistParams;             // 0x01436E9C

class CEngineComponentRenderParams;

struct CEngineWeatherSettingsView {                // CEngineWeatherSettings at this+0x114
    unsigned char _pad00[0x74];
    float MistHeight;                              // +0x74  (this+0x188)
    C3DVector MistColour;                          // +0x78  (this+0x18c)
    C3DVector MistScale;                           // +0x84  (this+0x198)
    C3DVector MistParam1;                          // +0x90  (this+0x1a4)
    C3DVector MistParam2;                          // +0x9c  (this+0x1b0)
    C3DVector MistParam3;                          // +0xa8  (this+0x1bc)
    unsigned char _padb4[0x4c8 - 0xb4];
    float FogScale;                                // +0x4c8 (this+0x5dc)
    unsigned char _pad4cc[0x4e4 - 0x4cc];
    C3DVector FogColour;                           // +0x4e4 (this+0x5f8)
};

struct CEngineWeatherRendererMethods : CEngineWeatherRenderer {
    CEngineWeatherSettingsView& GetSettings() { return *reinterpret_cast<CEngineWeatherSettingsView*>(Settings); }
    void RenderMist(const CEngineComponentRenderParams& params);
};

extern unsigned long g_renderStateValue_01396fb8;  // 0x01396FB8

void CEngineWeatherRendererMethods::RenderMist(const CEngineComponentRenderParams& params)
{
    g_pRenderStateManager->OpenCaptureBlock();
    if (!MistVolumeTexture) {
        g_pRenderStateManager->RestoreCaptureBlock();
        return;
    }
    g_pRenderManagerCore->SetTexture(0, reinterpret_cast<const CBaseTextureView*>(MistVolumeTexture));
    g_pRenderManagerCore->SetTexture(1, reinterpret_cast<const CBaseTextureView*>(MistVolumeTexture));
    g_pRenderManagerCore->SetTexture(2, reinterpret_cast<const CBaseTextureView*>(MistVolumeTexture));
    g_pRenderManagerCore->SetTexture(3, reinterpret_cast<const CBaseTextureView*>(MistVolumeTexture));
    g_pShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader*>(MistPixelShader));
    g_pShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader*>(MistVertexShader), 0);
    g_pRenderManagerCore->SetIndices(reinterpret_cast<const CIndexBufferView*>(MistIndexBuffer));
    g_pRenderManagerCore->SetStreamSource(reinterpret_cast<const CVertexBufferView*>(MistVertexBuffer));
    const CMistParamsView* mist = g_pMistParams;
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, mist->Position.X, mist->Position.Y,
                                                        mist->Position.Z,
                                                        GetSettings().MistHeight * mist->Scale);
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, GetSettings().MistScale.X, GetSettings().MistScale.Y,
                                                        GetSettings().MistScale.Z, 1.0f);
    const CMatrix3x4& view = g_pEngineCamera->ViewTransform;
    g_pShaderRenderManager->SetUserVertexShaderConstant(2, view.XAxis.X, view.YAxis.X, view.ZAxis.X, view.Position.X);
    g_pShaderRenderManager->SetUserVertexShaderConstant(3, view.XAxis.Y, view.YAxis.Y, view.ZAxis.Y, view.Position.Y);
    g_pShaderRenderManager->SetUserVertexShaderConstant(4, view.XAxis.Z, view.YAxis.Z, view.ZAxis.Z, view.Position.Z);
    float fogScale = GetSettings().FogScale;
    g_pShaderRenderManager->SetUserVertexShaderConstant(5, fogScale * GetSettings().FogColour.X,
                                                        fogScale * GetSettings().FogColour.Y,
                                                        fogScale * GetSettings().FogColour.Z, 0.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(6, GetSettings().MistColour.X, GetSettings().MistColour.Y,
                                                        GetSettings().MistColour.Z, 0.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(7, GetSettings().MistParam1.X, GetSettings().MistParam1.Y,
                                                        GetSettings().MistParam1.Z, 1.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(8, GetSettings().MistParam2.X, GetSettings().MistParam2.Y,
                                                        GetSettings().MistParam2.Z, 1.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(9, GetSettings().MistParam3.X, GetSettings().MistParam3.Y,
                                                        GetSettings().MistParam3.Z, 1.0f);
    SetState(g_pRenderManagerCore->m_state2890, g_renderStateValue_01396fb8, g_pRenderManagerCore);
    g_pShaderRenderManager->PrepareForRender(1);
    g_pRenderManagerCore->RealiseRenderState();
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, 4, 0, 2);
    g_pRenderStateManager->RestoreCaptureBlock();
}