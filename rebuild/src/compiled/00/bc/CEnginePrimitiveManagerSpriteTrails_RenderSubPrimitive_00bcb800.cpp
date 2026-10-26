// CEnginePrimitiveManagerSpriteTrails::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass) -- draw one sprite trail: inside a render-state capture block with vertex shaders on and pixel shaders off, apply the trail vertex shader and the global state block, set the fixed tracked states, then when the trail's graphic frame is available bind its texture to stage 0, set the texture-stage states, reset the world transform and render the trail
// Leaf: RenderSubPrimitive
// Layout: local models -- render core / capture block / shader-enable saver as the matched ShowGlowTexture 0x00B61690; the
//   counted CGraphicFrame and its frame-data release as the landed CGraphicFrame::operator= 0x0041C0E0; the D3D9 device's
//   SetTexture (slot 65) through the core's stage-0 texture cache (+0x3D00, device +0x3CF0, active texture count +0x3CB8).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct IDirect3DBaseTexture9;
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
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture) = 0;   // slot 65 (+0x104)
};
struct IDirect3DBaseTexture9;
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
    unsigned char        _pad2818[0x2854 - 0x2818];
    CTrackedRenderState  m_state2854;              // +0x2854
    unsigned char        _pad2868[0x2890 - 0x2868];
    CTrackedRenderState  m_state2890;              // +0x2890
    unsigned char        _pad28a4[0x2944 - 0x28a4];
    CTrackedRenderState  m_state2944;              // +0x2944
    unsigned char        _pad2958[0x2ee4 - 0x2958];
    CTrackedRenderState  m_state2ee4;              // +0x2EE4
    unsigned char        _pad2ef8[0x2f84 - 0x2ef8];
    CTrackedRenderState  m_state2f84;              // +0x2F84
    unsigned char        _pad2f98[0x3a24 - 0x2f98];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24
    unsigned char        _pad3a28[0x3cb8 - 0x3a28];
    long                 m_activeTextureCount;     // +0x3CB8
    unsigned char        _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*    m_pDevice;                // +0x3CF0
    unsigned char        _pad3cf4[0x3d00 - 0x3cf4];
    IDirect3DBaseTexture9* m_currentTexture[8];    // +0x3D00
    __forceinline void SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture);

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
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
__forceinline void CRenderManagerCoreView::SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture)
{
    if (g_pRenderManagerCore->m_currentTexture[stage] != texture) {
        g_pRenderManagerCore->m_currentTexture[stage] = texture;
        m_pDevice->SetTexture(stage, texture);
    }
}

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

extern unsigned long g_renderStateDefault_01396fb8;    // 0x01396FB8
extern unsigned long g_renderStateDefault_01396dd0;    // 0x01396DD0

static __forceinline void SetState(unsigned long value, CTrackedRenderState& state, CRenderManagerCoreView* core)
{
    if (state.m_value != value)
        core->ChangeState(state, value);
}
static __forceinline void SetStateConst(CTrackedRenderState& state, unsigned long value, CRenderManagerCoreView* core)
{
    if (state.m_value != value) {
        unsigned long mask = core->m_captureMask;
        if ((state.m_captureMask & mask) == 0) {
            state.m_captureMask |= mask;
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
struct CShaderEnableState {                             // the static shader manager object @ 0x013BC470
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;                 // +0x390
    bool          PixelShadersEnabled;                  // +0x391
    unsigned char _pad392[2];
    long          _pad394;
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


struct CRenderStateCaptureBlock {
    __forceinline CRenderStateCaptureBlock() { g_pRenderStateManager->OpenCaptureBlock(); }
    __forceinline ~CRenderStateCaptureBlock() { g_pRenderStateManager->RestoreCaptureBlock(); }
};
struct CShaderEnableSaver {
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
class CVertexShader;
class CShaderRenderManager { public:
    void ApplyVertexShader(CVertexShader* shader, bool force);   // 0x00988020
    void ResetWorldTransform();                    // 0x00988290
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CStateBlockOwnerBase {                      // 0x20 bytes
    virtual void v00();
    unsigned char _pad04[0x1c];
};
struct CEngineStateBlock {
    virtual void Apply();                          // slot 0
};
struct CEngineStateBlockManager : CStateBlockOwnerBase, CEngineStateBlock {
};
extern CEngineStateBlockManager* g_pEngineStateBlock_01436e90;   // 0x01436E90

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
class CTexture { public:
    IDirect3DBaseTexture9* m_pD3DTexture;          // +0x00
};
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
class CGraphicFrameData { public:
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            --data->ReferenceCount;
            if (data->ReferenceCount == 0) {
                data->~CGraphicFrameData();
                operator delete(data);
            }
            FrameData = 0;
        }
    }
};
class CGraphicInfo { public:                              // 0x2c bytes
    unsigned char _pad[0x2c];
    bool GetFrame(CGraphicFrame* frame, unsigned long frameIndex, unsigned long a, unsigned long b);   // 0x009FF8D0
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<CGraphicInfo> m_records;         // +0x1e0
    __forceinline CGraphicInfo* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
    __forceinline bool GetFrame(CGraphicFrame* frame, unsigned long bankIndex, unsigned long frameIndex)
    {
        return Get(bankIndex)->GetFrame(frame, frameIndex, 0, 0);
    }
};
struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;                  // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

class CEngineInternalPrimitiveSpriteTrail { public:
    unsigned char _pad00[0x4c];
    unsigned long BankIndex;                       // +0x4c
    unsigned long FrameIndex;                      // +0x50
};
class CEngineSubPrimitiveBase { public:
    unsigned char _pad00[0xc];
    CEngineInternalPrimitiveSpriteTrail* m_pPrimitive;   // +0x0c
};
class CEnginePrimitiveManagerSpriteTrails { public:
    void*         SceneList;                       // +0x04
    bool          Enable;                          // +0x08
    struct { void* _vptr; void* Data; } VertexShader;   // +0x0c CVertexShader
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long pass);
    void RenderSpriteTrail(CEngineInternalPrimitiveSpriteTrail* trail, bool textured);   // 0x00BCA950
};

void CEnginePrimitiveManagerSpriteTrails::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long pass)
{
    CEngineInternalPrimitiveSpriteTrail* trail = subPrimitive.m_pPrimitive;
    CRenderStateCaptureBlock capture;
    CShaderEnableSaver shaders(true, false);
    g_pShaderRenderManager->ApplyVertexShader((CVertexShader*)&VertexShader, false);
    g_pEngineStateBlock_01436e90->Apply();
    SetStateConst(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    SetStateConst(g_pRenderManagerCore->m_state2890, g_renderStateDefault_01396fb8, g_pRenderManagerCore);
    CGraphicFrame frame;
    if (g_pEngine_01436e98->m_pGraphicBank->GetFrame(&frame, trail->BankIndex, trail->FrameIndex)) {
        CTexture* texture = frame.FrameData->PTexture.m_pObject;
        CRenderManagerCoreView* core = g_pRenderManagerCore;
        core->SetTexture(0, texture ? texture->m_pD3DTexture : 0);
        if (core->m_activeTextureCount < 1)
            core->m_activeTextureCount = 1;
        SetStateConst(g_pRenderManagerCore->m_state2ee4, g_renderStateDefault_01396dd0, g_pRenderManagerCore);
        SetStateConst(g_pRenderManagerCore->m_state2f84, g_renderStateDefault_01396dd0, g_pRenderManagerCore);
        g_pShaderRenderManager->ResetWorldTransform();
        SetStateConst(g_pRenderManagerCore->m_state2944, 0, g_pRenderManagerCore);
        RenderSpriteTrail(trail, true);
    }
}