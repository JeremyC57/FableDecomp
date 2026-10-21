// Grade: DIFFER(411v415) -- register allocation of the two SetState blocks: retail keeps the default value in esi and uses a scoped `push ebp ; ... ; pop ebp` temp for the second state value (core copied to ecx), ours puts the value in edx and needs no ebp; tried free core-last SetState for both, manager-method for one/both
// CEnginePrimitiveManagerParticleGroup::SetupPixelShader(unsigned long reg, const CPixelShader& maskedShader, const CPixelShader& plainShader) -- when the group writes the mask (+0x22) upload the screen-to-texture constant, set two tracked states from the default @ 0x01396DC8, bind the engine's mask texture on stage 1 and apply the masked shader; otherwise zero the constant and apply the plain shader
// Leaf: SetupPixelShader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBCC70, length 416 bytes (body 0x19f), `ret 0Ch`.
// PDB: ?SetupPixelShader@CEnginePrimitiveManagerParticleGroup@@QAEXKABVCPixelShader@@0@Z.
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, float, float, float, float) @ 0x00989B00 and
//   ApplyPixelShader(const CPixelShader&) @ 0x00988140 (global @ 0x01436E14); IDirect3DDevice9::SetTexture (slot 65, +0x104).
// Globals: the render core @ 0x01436E18 (tracked states +0x2ef8/+0x2f98 -- the EnableFog 0x00B46890 model; PD3DDevice +0x3cf0,
//   the stage count +0x3cb8, the current texture of stage 1 +0x3d04), the engine object @ 0x01436E98 whose texture holder at
//   +0x2aac is read through a null-tested `this ? texture : 0` accessor (retail `add ecx,2AACh ; je`).
// Layout: Ego_r.pdb CEnginePrimitiveManagerParticleGroup (40 bytes): the flag byte at +0x22 (PDB WriteZBuffer).
// Shape: SetState is the `__forceinline` free function with the core LAST (VC7.1 copies inline parameters right to left);
//   the stage texture set is an inline core method; the stage-count clamp is `if (count < stage + 1) count = stage + 1`.
struct CPixelShader;
struct IDirect3DBaseTexture9;

struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40(); virtual void __stdcall v44(); virtual void __stdcall v48(); virtual void __stdcall v4c();
    virtual void __stdcall v50(); virtual void __stdcall v54(); virtual void __stdcall v58(); virtual void __stdcall v5c();
    virtual void __stdcall v60(); virtual void __stdcall v64(); virtual void __stdcall v68(); virtual void __stdcall v6c();
    virtual void __stdcall v70(); virtual void __stdcall v74(); virtual void __stdcall v78(); virtual void __stdcall v7c();
    virtual void __stdcall v80(); virtual void __stdcall v84(); virtual void __stdcall v88(); virtual void __stdcall v8c();
    virtual void __stdcall v90(); virtual void __stdcall v94(); virtual void __stdcall v98(); virtual void __stdcall v9c();
    virtual void __stdcall va0(); virtual void __stdcall va4(); virtual void __stdcall va8(); virtual void __stdcall vac();
    virtual void __stdcall vb0(); virtual void __stdcall vb4(); virtual void __stdcall vb8(); virtual void __stdcall vbc();
    virtual void __stdcall vc0(); virtual void __stdcall vc4(); virtual void __stdcall vc8(); virtual void __stdcall vcc();
    virtual void __stdcall vd0(); virtual void __stdcall vd4(); virtual void __stdcall vd8(); virtual void __stdcall vdc();
    virtual void __stdcall ve0(); virtual void __stdcall ve4(); virtual void __stdcall ve8(); virtual void __stdcall vec();
    virtual void __stdcall vf0(); virtual void __stdcall vf4(); virtual void __stdcall vf8(); virtual void __stdcall vfc();
    virtual void __stdcall v100();
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture);   // slot 65 (+0x104)
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
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18
    unsigned char          _pad0000[0x8];
    SCapturedRenderState   m_captured[0x400];      // +0x0008
    CTrackedRenderState*   m_dirty[0x200];         // +0x2008
    long                   m_capturedCount;        // +0x2808
    long                   m_dirtyCount;           // +0x280C
    long                   m_unknown2810;          // +0x2810
    unsigned long          m_captureMask;          // +0x2814
    unsigned char          _pad2818[0x2ef8 - 0x2818];
    CTrackedRenderState    m_state2ef8;            // +0x2EF8
    unsigned char          _pad2f0c[0x2f98 - 0x2f0c];
    CTrackedRenderState    m_state2f98;            // +0x2F98
    unsigned char          _pad2fac[0x3cb8 - 0x2fac];
    long                   m_textureStageCount;    // +0x3CB8
    unsigned char          _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*      PD3DDevice;             // +0x3CF0
    unsigned char          _pad3cf4[0x3d00 - 0x3cf4];
    IDirect3DBaseTexture9* m_currentTexture[8];    // +0x3D00

    __forceinline void SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture)
    {
        if (m_currentTexture[stage] != texture) {
            m_currentTexture[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (m_textureStageCount < (long)(stage + 1))
            m_textureStageCount = stage + 1;
    }
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
extern unsigned long g_renderStateDefault_01396dc8;    // 0x01396DC8

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

struct CTextureHolder {
    IDirect3DBaseTexture9* m_pTexture;
    IDirect3DBaseTexture9* GetTexture() { return this ? m_pTexture : 0; }
};
struct CEngineData_01436e98 {
    unsigned char  _pad[0x2aac];
    CTextureHolder MaskTexture;                    // +0x2AAC
};
extern CEngineData_01436e98* g_pEngineData_01436e98;   // 0x01436E98

struct CShaderRenderManager {
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void ApplyPixelShader(const CPixelShader& shader);   // 0x00988140
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CEnginePrimitiveManagerParticleGroup {
    void*         _vptr;                           // +0x00
    void*         SceneList;                       // +0x04
    bool          Enable;                          // +0x08
    unsigned char _pad09[3];                       // +0x09
    bool          PointLightsSet;                  // +0x0c
    signed char   PointLightCount;                 // +0x0d
    float         CurrentWorldPos[3];              // +0x10 (PDB +0x0e, 4-aligned)
    float         CurrentBSRadius;                 // +0x1c
    bool          ClipEffectToWeatherMask;         // +0x20
    bool          ReadZBuffer;                     // +0x21
    bool          WriteZBuffer;                    // +0x22
    void SetupPixelShader(unsigned long reg, const CPixelShader& maskedShader, const CPixelShader& plainShader);
};

void CEnginePrimitiveManagerParticleGroup::SetupPixelShader(unsigned long reg, const CPixelShader& maskedShader, const CPixelShader& plainShader)
{
    if (WriteZBuffer) {
        g_pShaderRenderManager->SetUserVertexShaderConstant(reg, 1.0f / 32.0f, 1.0f / 32.0f, 320.0f, 240.0f);
        SetState(g_pRenderManagerCore->m_state2ef8, g_renderStateDefault_01396dc8, g_pRenderManagerCore);
        SetState(g_pRenderManagerCore->m_state2f98, g_renderStateDefault_01396dc8, g_pRenderManagerCore);
        g_pRenderManagerCore->SetTexture(1, g_pEngineData_01436e98->MaskTexture.GetTexture());
        g_pShaderRenderManager->ApplyPixelShader(maskedShader);
    } else {
        g_pShaderRenderManager->SetUserVertexShaderConstant(reg, 0.0f, 0.0f, 0.0f, 0.0f);
        g_pShaderRenderManager->ApplyPixelShader(plainShader);
    }
}
