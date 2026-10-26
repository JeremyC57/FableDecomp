// CEngineWaterRenderer::RenderBackground(float time) -- bind the water bump map on stages 0/1 and the environment map on stage 2 with their blend states, pick the high-end or low-end background shaders, upload the four water bone matrices and either the bump constant (high end) or the low-end colour pixel constants, then draw the alphaed and the opaque background patch lists
// Leaf: RenderBackground
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B76BC0, length 1965 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?RenderBackground@CEngineWaterRenderer@@AAEXM@Z.
// Callees: CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (inline OpenCaptureBlock on 0x013CB508);
// CShaderRenderManager::ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, SetUserVertexShaderConstant
// @ 0x00989B00, SetBoneTransform @ 0x0098A7F0, BoneMatrices.resize @ 0x00B7EDF0, inline SetPixelShaderConstant and
// PrepareForRender(0x604); CEngineWaterRenderer::RenderBackgroundList @ 0x00B6E260 (landed).
// Globals: two function-local statics (guard word 0x014378A8: bit 1 -> the 0.8/0.8/0/0 constant @ 0x01437898, bit 2 ->
// the 0/0.4/0.8/1 colour @ 0x01437888), the lighting object @ 0x01436E9C (+0x3c ambient CRGBFloatColour*).
// Layout: retail CEngineWaterRenderer (12-byte STLport CArrays where the PDB header has 16): EnvironmentMap +0x1e0,
// WaterBumpMap +0x1f0, BackgroundRenderList +0x208, AlphaedBackgroundRenderList +0x214 (PDB +0x1ec/+0x1fc/+0x218/+0x228);
// HighendWaterEnabled +0x274 and WaterBumpFactor +0x5c0 as in the landed Setup*ShaderConstants rows.
// Shape: the low-end colour is the landed CRGBFloatColour model (RenderBackgroundPatch 0x00B6E010): a CRGBFloatColour
// product local computed before the first pixel constant, passed as C4DVector(colour) (a C4DVector local or a C4DVector
// operator* keeps a 0x44 frame and stores the product late). The +0x28b8 SetState after the first list reads the state's
// capture mask into a local first (retail loads it before the core mask; the plain inline swaps the two loads).

class C3DVector { public: float X, Y, Z; };
class CRGBFloatColour;
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
    inline C4DVector(const CRGBFloatColour& c);
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b) { return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }
inline C4DVector::C4DVector(const CRGBFloatColour& c) : X(c.R), Y(c.G), Z(c.B), W(c.A) { }
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

struct CPreTransposedBoneMatrix {
    float M[12];
    CPreTransposedBoneMatrix() { }
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
    void resize(unsigned int n, const T& value);        // 0x00B7EDF0 for CPreTransposedBoneMatrix
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
    void SetBoneTransform(unsigned long index, const CMatrix3x4* matrix);                        // 0x0098A7F0

    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        *(PSConstTable.First + index) = value;
        UpdateFlags |= 2;
    }

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
    unsigned char _pad1e9[0x1e3];
    CArray<C4DVector> PSConstTable;                     // +0x3cc
    unsigned long UpdateFlags;                          // +0x3d8
    CArray<CPreTransposedBoneMatrix> BoneMatrices;      // +0x3dc

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

class CTexture { public:                                  // engine/CTexture.h: device texture at +0
    void* PD3DTexture;                             // +0x00
    unsigned long ByteLengthAndSource;             // +0x04
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
    __forceinline void SetTexture(long stage, const CTexture* texture)
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
    unsigned char        _pad2818[0x28b8 - 0x2818];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    CTrackedRenderState  m_state28cc;              // +0x28CC
    unsigned char        _pad28e0[0x2ee4 - 0x28e0];
    CTrackedRenderState  m_state2ee4;              // +0x2EE4
    CTrackedRenderState  m_state2ef8;              // +0x2EF8
    CTrackedRenderState  m_state2f0c;              // +0x2F0C
    unsigned char        _pad2f20[0x2f84 - 0x2f20];
    CTrackedRenderState  m_state2f84;              // +0x2F84
    CTrackedRenderState  m_state2f98;              // +0x2F98
    CTrackedRenderState  m_state2fac;              // +0x2FAC
    unsigned char        _pad2fc0[0x3cb8 - 0x2fc0];
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

static __forceinline void SetStateStateMaskFirst(CTrackedRenderState& state, unsigned long value, CRenderManagerCore* core)
{
    if (state.m_value != value) {
        unsigned long stateMask = state.m_captureMask;
        if ((core->m_captureMask & stateMask) == 0) {
            state.m_captureMask = stateMask | core->m_captureMask;
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

extern unsigned long g_renderStateValue_01396dc8;  // 0x01396DC8
extern unsigned long g_renderStateValue_01396dcc;  // 0x01396DCC
extern unsigned long g_renderStateValue_01396dd0;  // 0x01396DD0


struct CEngineLightingView {                       // @ 0x01436E9C
    unsigned char _pad0[0x3c];
    const CRGBFloatColour* AmbientColour;                // +0x3C
};
extern CEngineLightingView* g_pEngineLighting;     // 0x01436E9C

class CEngineWaterBackgroundSubPatch;

class CEngineWaterRenderer { public:                      // retail layout (12-byte CArrays)
    unsigned char _pad000[0x104];
    unsigned char BackgroundVertexShader[8];       // +0x104
    unsigned char BackgroundPixelShader[8];        // +0x10C
    unsigned char _pad114[0x11c - 0x114];
    unsigned char WaterBackgroundLowEndVertexShader[8];   // +0x11C
    unsigned char WaterLowEndPixelShader[8];       // +0x124
    unsigned char _pad12c[0x1e0 - 0x12c];
    CTexture* EnvironmentMap;                      // +0x1E0 (PDB +0x1ec)
    unsigned char _pad1e4[0x1f0 - 0x1e4];
    CTexture* WaterBumpMap;                        // +0x1F0 (PDB +0x1fc)
    unsigned char _pad1f4[0x208 - 0x1f4];
    CArray<CEngineWaterBackgroundSubPatch*> BackgroundRenderList;          // +0x208 (PDB +0x218)
    CArray<CEngineWaterBackgroundSubPatch*> AlphaedBackgroundRenderList;   // +0x214 (PDB +0x228)
    unsigned char _pad220[0x274 - 0x220];
    bool HighendWaterEnabled;                      // +0x274
    unsigned char _pad275[0x2dc - 0x275];
    float BumpScale;                               // +0x2DC
    unsigned char _pad2e0[0x414 - 0x2e0];
    CMatrix3x4 BoneMatrix1;                        // +0x414
    CMatrix3x4 BoneMatrix3;                        // +0x444
    CMatrix3x4 BoneMatrix2;                        // +0x474
    unsigned char _pad4a4[0x504 - 0x4a4];
    CMatrix3x4 BoneMatrix0;                        // +0x504
    unsigned char _pad534[0x5c0 - 0x534];
    float WaterBumpFactor;                         // +0x5C0
    void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time);   // 0x00B6E260
    void RenderBackground(float time);
};

void CEngineWaterRenderer::RenderBackground(float time)
{
    g_pRenderStateManager->OpenCaptureBlock();
    g_pRenderManagerCore->SetTexture(0, WaterBumpMap);
    SetState(g_pRenderManagerCore->m_state2ee4, g_renderStateValue_01396dc8, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2f84, g_renderStateValue_01396dc8, g_pRenderManagerCore);
    g_pRenderManagerCore->SetTexture(1, WaterBumpMap);
    SetState(g_pRenderManagerCore->m_state2ef8, g_renderStateValue_01396dc8, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2f98, g_renderStateValue_01396dc8, g_pRenderManagerCore);
    g_pRenderManagerCore->SetTexture(2, EnvironmentMap);
    SetState(g_pRenderManagerCore->m_state2f0c, g_renderStateValue_01396dcc, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2fac, g_renderStateValue_01396dd0, g_pRenderManagerCore);
    if (HighendWaterEnabled) {
        g_pShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader*>(BackgroundVertexShader), 0);
        g_pShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader*>(BackgroundPixelShader));
    } else {
        g_pShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader*>(WaterBackgroundLowEndVertexShader), 0);
        g_pShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader*>(WaterLowEndPixelShader));
    }
    g_pShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    g_pShaderRenderManager->SetBoneTransform(0, &BoneMatrix0);
    g_pShaderRenderManager->SetBoneTransform(1, &BoneMatrix1);
    g_pShaderRenderManager->SetBoneTransform(2, &BoneMatrix2);
    g_pShaderRenderManager->SetBoneTransform(3, &BoneMatrix3);
    if (HighendWaterEnabled) {
        float bump = WaterBumpFactor * BumpScale;
        g_pShaderRenderManager->SetUserVertexShaderConstant(3, bump, 0.0f, bump * -0.5f, 1.0f);
    } else {
        static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
        CRGBFloatColour colour = lowEndColour * *g_pEngineLighting->AmbientColour;
        g_pShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
        g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(colour));
    }
    g_pShaderRenderManager->PrepareForRender(0x604);
    RenderBackgroundList(AlphaedBackgroundRenderList, time);
    SetStateStateMaskFirst(g_pRenderManagerCore->m_state28b8, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state28cc, 0, g_pRenderManagerCore);
    RenderBackgroundList(BackgroundRenderList, time);
    g_pRenderStateManager->RestoreCaptureBlock();
}