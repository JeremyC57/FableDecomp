// CEngineWaterRenderer::RenderIceBumpMap() -- apply the ice bump-map shaders and draw the 14-vertex bump quad twice, first with the X/Y and then the Y/Z channel-select pixel constants, setting the blend states between the passes and clearing the first one after
// Leaf: RenderIceBumpMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6E3E0, length 1195 bytes (+ int3 padding), plain `ret`.
// Signature from Ego_r.pdb: ?RenderIceBumpMap@CEngineWaterRenderer@@AAEXXZ.
// Callees: CShaderRenderManager::ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, the inlined
// SetPixelShaderConstant (PSConstTable +0x3cc, UpdateFlags |= 2; the landed RenderBackgroundPatch 0x00B6E010 model) and
// PrepareForRender(0) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0,
// UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, ResetWorldTransform 0x00988290,
// UpdateCombinedProjectionTransform 0x00988A50), CRenderManagerCore::DrawCurrentBuffersAsIndexedTris @ 0x00A0AC60, and
// four inlined render-core SetState calls (+0x28b8 <- 1, +0x291c/+0x2930 <- [0x01396F6C], +0x28b8 <- 0).
// Layout: retail CEngineWaterRenderer: the three PDB CArray<CVertexShader> members at +0x12c..+0x15c are 12-byte STLport
// vectors in retail (16 in the PDB/engine header), so IceBumpmapVertexShader is at +0x188 (PDB +0x194) and
// IceBumpmapPixelShader at +0x190 (PDB +0x19c); a local view is used, as in the landed CEngineWaterRenderer rows.

class C3DVector { public: float X, Y, Z; };
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
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
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
public:
    void ResetWorldTransform();   // 0x00988290
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
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
    void DrawCurrentBuffersAsIndexedTris(unsigned long minVertexIndex, unsigned long vertexCount,
                                         unsigned long startIndex, unsigned long triangleCount);   // 0x00A0AC60
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x28b8 - 0x2818];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    unsigned char        _pad28cc[0x291c - 0x28cc];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
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

extern unsigned long g_renderStateValue_01396f6c;  // 0x01396F6C

class CEngineWaterRenderer { public:                      // retail layout (three 12-byte CArrays before +0x15c)
    unsigned char _pad000[0x178];
    unsigned char BumpmapVertexShader[8];          // +0x178 (PDB +0x184)
    unsigned char BumpmapPixelShader[8];           // +0x180 (PDB +0x18c)
    unsigned char IceBumpmapVertexShader[8];       // +0x188 (PDB +0x194)
    unsigned char IceBumpmapPixelShader[8];        // +0x190 (PDB +0x19c)
    void RenderIceBumpMap();
};

void CEngineWaterRenderer::RenderIceBumpMap()
{
    g_pShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader*>(IceBumpmapVertexShader), 0);
    g_pShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader*>(IceBumpmapPixelShader));
    g_pShaderRenderManager->SetPixelShaderConstant(5, C4DVector(1.0f, 0.0f, 0.0f, 0.0f));
    g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    g_pShaderRenderManager->PrepareForRender(0);
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, 0xe, 0, 2);
    SetState(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state291c, g_renderStateValue_01396f6c, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2930, g_renderStateValue_01396f6c, g_pRenderManagerCore);
    g_pShaderRenderManager->SetPixelShaderConstant(5, C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(0.0f, 0.0f, 1.0f, 0.0f));
    g_pShaderRenderManager->PrepareForRender(0);
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, 0xe, 0, 2);
    SetState(g_pRenderManagerCore->m_state28b8, 0, g_pRenderManagerCore);
}