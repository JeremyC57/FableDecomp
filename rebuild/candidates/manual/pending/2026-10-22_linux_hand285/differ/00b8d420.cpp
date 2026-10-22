// Grade: DIFFER(664v664) -- only the device address: retail keeps shaders in ebp and &shaders->Device in esi (mov ebp,esi ; add esi,1D4h ; mov eax,[esi]) for SetIndices / Draw; ours re-addresses [esi+1D4h] / [esi+1D0h]
// CEngineOcclusionManager::RenderOcclusionBox(const CMatrix3x4& transform) -- draw the unit occlusion box under the given transform inside a render-state capture block (occlusion-box shaders, two render states cleared, shader state flushed, 12 triangles from the unit-box buffers) and allocate an occlusion entry for it; returns the entry handle, or -1 when the entry buffer is full
// Leaf: RenderOcclusionBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8D420, length 662 bytes (`ret 4`).
// Callees: CShaderRenderManager::ApplyVertexShader(CVertexShader*, bool) @ 0x00988020, ApplyPixelShader @ 0x00988140,
//   SetWorldTransform @ 0x009881F0 and the inlined PrepareForRender(1) (SetVSConstants @ 0x0098B930, UpdatePixelShader @ 0x00988A20,
//   UpdatePixelShaderConstants @ 0x00989BF0, UpdateAmbient @ 0x00989760, UpdateFogColour @ 0x009897C0, UpdateLightGlobals @ 0x0098A760,
//   UpdateCombinedProjectionTransform @ 0x00988A50) as the landed RenderRainBox 0x00B54100; CRenderStateManager::RealiseRenderState
//   @ 0x00A058C0 and RestoreCaptureBlock @ 0x00A05840; IDirect3DDevice9 SetStreamSource (+0x190), SetIndices (+0x1A0) and
//   DrawIndexedPrimitive (+0x148).
// Globals: the render-state manager @ 0x013CB508 (capture block), the shader render manager @ 0x01436E14, the render manager core
//   @ 0x01436E18 (tracked states +0x2840 / +0x2854, model of the landed CEngineStateBlockDiffuseOnly::Apply 0x00B58DE0), the
//   occlusion manager @ 0x01436E5C (the entry is allocated through the global, not `this`).
// Layout: retail CEngineOcclusionManager (12-byte vectors) as the landed destructor 0x00B8DA80: UnitBoxVB +0x24068 (PVertexBuffer +4,
//   Stride +0x10), UnitBoxIB +0x2409C (PD3DIndexBuffer +4, PrimitiveType +0xc), OcclusionBoxVShader +0x240B0, OcclusionBoxPixelShader +0x240B8.
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DDevice9 {
    virtual long __stdcall Slot00();
    virtual long __stdcall Slot01();
    virtual long __stdcall Slot02();
    virtual long __stdcall Slot03();
    virtual long __stdcall Slot04();
    virtual long __stdcall Slot05();
    virtual long __stdcall Slot06();
    virtual long __stdcall Slot07();
    virtual long __stdcall Slot08();
    virtual long __stdcall Slot09();
    virtual long __stdcall Slot0a();
    virtual long __stdcall Slot0b();
    virtual long __stdcall Slot0c();
    virtual long __stdcall Slot0d();
    virtual long __stdcall Slot0e();
    virtual long __stdcall Slot0f();
    virtual long __stdcall Slot10();
    virtual long __stdcall Slot11();
    virtual long __stdcall Slot12();
    virtual long __stdcall Slot13();
    virtual long __stdcall Slot14();
    virtual long __stdcall Slot15();
    virtual long __stdcall Slot16();
    virtual long __stdcall Slot17();
    virtual long __stdcall Slot18();
    virtual long __stdcall Slot19();
    virtual long __stdcall Slot1a();
    virtual long __stdcall Slot1b();
    virtual long __stdcall Slot1c();
    virtual long __stdcall Slot1d();
    virtual long __stdcall Slot1e();
    virtual long __stdcall Slot1f();
    virtual long __stdcall Slot20();
    virtual long __stdcall Slot21();
    virtual long __stdcall Slot22();
    virtual long __stdcall Slot23();
    virtual long __stdcall Slot24();
    virtual long __stdcall Slot25();
    virtual long __stdcall Slot26();
    virtual long __stdcall Slot27();
    virtual long __stdcall Slot28();
    virtual long __stdcall Slot29();
    virtual long __stdcall Slot2a();
    virtual long __stdcall Slot2b();
    virtual long __stdcall Slot2c();
    virtual long __stdcall Slot2d();
    virtual long __stdcall Slot2e();
    virtual long __stdcall Slot2f();
    virtual long __stdcall Slot30();
    virtual long __stdcall Slot31();
    virtual long __stdcall Slot32();
    virtual long __stdcall Slot33();
    virtual long __stdcall Slot34();
    virtual long __stdcall Slot35();
    virtual long __stdcall Slot36();
    virtual long __stdcall Slot37();
    virtual long __stdcall Slot38();
    virtual long __stdcall Slot39();
    virtual long __stdcall Slot3a();
    virtual long __stdcall Slot3b();
    virtual long __stdcall Slot3c();
    virtual long __stdcall Slot3d();
    virtual long __stdcall Slot3e();
    virtual long __stdcall Slot3f();
    virtual long __stdcall Slot40();
    virtual long __stdcall Slot41();
    virtual long __stdcall Slot42();
    virtual long __stdcall Slot43();
    virtual long __stdcall Slot44();
    virtual long __stdcall Slot45();
    virtual long __stdcall Slot46();
    virtual long __stdcall Slot47();
    virtual long __stdcall Slot48();
    virtual long __stdcall Slot49();
    virtual long __stdcall Slot4a();
    virtual long __stdcall Slot4b();
    virtual long __stdcall Slot4c();
    virtual long __stdcall Slot4d();
    virtual long __stdcall Slot4e();
    virtual long __stdcall Slot4f();
    virtual long __stdcall Slot50();
    virtual long __stdcall Slot51();
    virtual long __stdcall DrawIndexedPrimitive(long type, long baseVertex, unsigned long minIndex, unsigned long numVertices, unsigned long startIndex, unsigned long primCount);   // +0x148
    virtual long __stdcall Slot53();
    virtual long __stdcall Slot54();
    virtual long __stdcall Slot55();
    virtual long __stdcall Slot56();
    virtual long __stdcall Slot57();
    virtual long __stdcall Slot58();
    virtual long __stdcall Slot59();
    virtual long __stdcall Slot5a();
    virtual long __stdcall Slot5b();
    virtual long __stdcall Slot5c();
    virtual long __stdcall Slot5d();
    virtual long __stdcall Slot5e();
    virtual long __stdcall Slot5f();
    virtual long __stdcall Slot60();
    virtual long __stdcall Slot61();
    virtual long __stdcall Slot62();
    virtual long __stdcall Slot63();
    virtual long __stdcall SetStreamSource(unsigned long stream, IDirect3DVertexBuffer9* buffer, unsigned long offset, unsigned long stride);   // +0x190
    virtual long __stdcall Slot65();
    virtual long __stdcall Slot66();
    virtual long __stdcall Slot67();
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indices);   // +0x1a0
};

struct CMatrix3x4 { float E[12]; };
struct C4DVector { float X, Y, Z, W; };
struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderStateManager {
    void*                __vftable;                // +0x00
    void*                PD3DDevice;               // +0x04
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_allStatesCount;         // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2840 - 0x2818];
    CTrackedRenderState  m_state2840;              // +0x2840
    CTrackedRenderState  m_state2854;              // +0x2854
    unsigned char        _pad2868[0x3a24 - 0x2868];
    CTrackedRenderState  BookmarkDummyState;       // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = m_capturedCount;
        m_captured[size].m_pState = &BookmarkDummyState;
        m_capturedCount++;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
    void RealiseRenderState();                     // 0x00A058C0
};
extern CRenderStateManager* g_pRenderStateManager; // 0x013CB508
extern CRenderStateManager* g_pRenderManagerCore;  // 0x01436E18

static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderStateManager* core)
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

struct CVertexShader { void* _vptr; void* Data; };
struct CPixelShader { void* _vptr; void* Data; };
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

struct CD3DDeviceHandle {                          // the device pointer wrapper at +0x1d4 of the shader render manager
    IDirect3DDevice9* PD3DDevice;
    void SetStreamSource(unsigned long stream, IDirect3DVertexBuffer9* buffer, unsigned long offset, unsigned long stride)
    {
        PD3DDevice->SetStreamSource(stream, buffer, offset, stride);
    }
    void SetIndices(IDirect3DIndexBuffer9* indices) { PD3DDevice->SetIndices(indices); }
    void DrawIndexedPrimitive(long type, long baseVertex, unsigned long minIndex, unsigned long numVertices, unsigned long startIndex, unsigned long primCount)
    {
        PD3DDevice->DrawIndexedPrimitive(type, baseVertex, minIndex, numVertices, startIndex, primCount);
    }
};
class CShaderRenderManager {
public:
    void DrawIndexedPrimitive(long type, long baseVertex, unsigned long minIndex, unsigned long numVertices, unsigned long startIndex, unsigned long primCount)
    {
        RenderStateManager->RealiseRenderState();
        Device.DrawIndexedPrimitive(type, baseVertex, minIndex, numVertices, startIndex, primCount);
    }
    void ApplyVertexShader(CVertexShader* shader, bool force);   // 0x00988020
    void ApplyPixelShader(CPixelShader* shader);   // 0x00988140
    void SetWorldTransform(const CMatrix3x4& matrix);   // 0x009881F0

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
    unsigned char _pad008[0x1c8];                       // +0x008
    CRenderStateManager* RenderStateManager;            // +0x1d0
    CD3DDeviceHandle Device;                            // +0x1d4
    unsigned char _pad1d8[0x10];                        // +0x1d8
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


struct CVertexBufferWin32 {                        // 0x34 bytes
    void*                   _vptr;
    IDirect3DVertexBuffer9* PVertexBuffer;         // +0x04
    unsigned long           Length;                // +0x08
    long                    Type;                  // +0x0c
    unsigned char           Stride;                // +0x10
    unsigned char           _pad11[0x23];
};
struct CIndexBuffer {                              // 0x14 bytes
    void*                  _vptr;
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    long                   PrimitiveType;          // +0x0c
    void*                  PAllocatedBuffer;       // +0x10
};
struct COccluderEntry {
    unsigned long Handle;
    unsigned long Result;
    COccluderEntry(unsigned long handle, unsigned long result) : Handle(handle), Result(result) { }
};

struct CEngineOcclusionManager {
    unsigned char      _pad00000[0x4014];
    COccluderEntry*    Entries;                    // +0x4014
    COccluderEntry*    PrevFrameEntries;           // +0x4018
    unsigned long      EntriesSize;                // +0x401c
    unsigned char      _pad04020[0x24060 - 0x4020];
    CVertexShader      VertexShaderFlareTest;      // +0x24060
    CVertexBufferWin32 UnitBoxVB;                  // +0x24068
    CIndexBuffer       UnitBoxIB;                  // +0x2409c
    CVertexShader      OcclusionBoxVShader;        // +0x240b0
    CPixelShader       OcclusionBoxPixelShader;    // +0x240b8
    long AllocateEntry()
    {
        if (EntriesSize < 0x2000) {
            long handle = EntriesSize;
            Entries[handle] = COccluderEntry(0, 0);
            ++EntriesSize;
            return handle;
        }
        return -1;
    }
    long RenderOcclusionBox(const CMatrix3x4& transform);
};
extern CEngineOcclusionManager* g_pEngineOcclusionManager;   // 0x01436E5C

long CEngineOcclusionManager::RenderOcclusionBox(const CMatrix3x4& transform)
{
    g_pRenderStateManager->CreateCaptureBlock();
    g_pShaderRenderManager->ApplyVertexShader(&OcclusionBoxVShader, false);
    g_pShaderRenderManager->ApplyPixelShader(&OcclusionBoxPixelShader);
    g_pShaderRenderManager->SetWorldTransform(transform);
    SetState(g_pRenderManagerCore->m_state2840, 0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    g_pShaderRenderManager->PrepareForRender(1);
    CShaderRenderManager* shaders = g_pShaderRenderManager;
    shaders->Device.SetStreamSource(0, UnitBoxVB.PVertexBuffer, 0, UnitBoxVB.Stride);
    shaders->Device.SetIndices(UnitBoxIB.PD3DIndexBuffer);
    shaders->DrawIndexedPrimitive(UnitBoxIB.PrimitiveType, 0, 0, 8, 0, 12);
    long handle = g_pEngineOcclusionManager->AllocateEntry();
    g_pRenderStateManager->RestoreCaptureBlock();
    return handle;
}
