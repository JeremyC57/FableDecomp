// CEnginePrimitiveManagerSprites::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass) -- render one sprite through the particle sprite-group manager inside a render-state capture block: vertex constant layout 5, the sprite's tracked render states (two texture stages from 0x01396DD0, z-enable from the sprite's ZBuffer bit, and four fixed states), the global lighting states, then RenderSingleSprite between two world-transform resets
// Leaf: RenderSubPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAD2E0, length 912 bytes (body 0x38c + int3 padding), `ret 8` (virtual).
// PDB: ?RenderSubPrimitive@CEnginePrimitiveManagerSprites@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z.
// Callees: CShaderRenderManager::SetVertexShaderConstantLayout(long) @ 0x0098B5E0 on the static shader manager @ 0x013BC470
//   (current layout +0x8, saved and restored by an inline frame object), CEngineLightingManager::UpdateGlobalLightingStates
//   @ 0x00B46C80 (global @ 0x01436E9C), CShaderRenderManager::ResetWorldTransform @ 0x00988290 (global @ 0x01436E14),
//   CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite @ 0x00BC04F0 (manager table @ 0x01436E84, slot +0x7c),
//   CRenderStateManager::RestoreCaptureBlock @ 0x00A05840.
// Render core 0x01436E18: tracked states +0x2ee4/+0x2f84 (0x01396DD0), +0x2868 (ZBuffer), +0x2854 (0), +0x28b8 (1),
//   +0x28cc (0), +0x2944 (0) through the landed Diffuse-only `SetState(state, value, core)` (core last).
// Sprite: CEngineInternalPrimitiveSprite (landed 0x00BAD670 model) keeps ZBuffer as bit 6 of the +0x84 bitfield byte.
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
    CTrackedRenderState  m_state2868;              // +0x2868
    unsigned char        _pad287c[0x28b8 - 0x287c];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    CTrackedRenderState  m_state28cc;              // +0x28CC
    unsigned char        _pad28e0[0x2944 - 0x28e0];
    CTrackedRenderState  m_state2944;              // +0x2944
    unsigned char        _pad2958[0x2ee4 - 0x2958];
    CTrackedRenderState  m_state2ee4;              // +0x2EE4
    unsigned char        _pad2ef8[0x2f84 - 0x2ef8];
    CTrackedRenderState  m_state2f84;              // +0x2F84
    unsigned char        _pad2f98[0x3a24 - 0x2f98];
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
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

extern unsigned long g_renderStateDefault_01396dd0;    // 0x01396DD0

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
// the constant-valued sets: the capture mask read into a local (retail ORs into the state-mask register here)
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
static __forceinline void SetStateV(unsigned long value, CTrackedRenderState& state, CRenderManagerCoreView* core)
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
struct CShaderLayoutState {                             // the static shader manager object @ 0x013BC470
    unsigned char _pad000[0x8];
    long          CurrentVSConstantLayoutIndex;         // +0x008
    void SetVertexShaderConstantLayout(long layout);    // 0x0098B5E0
};
extern CShaderLayoutState g_ShaderRenderManager;        // 0x013BC470
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

class CShaderRenderManager {
public:
    void ResetWorldTransform();                         // 0x00988290
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
class CEngineLightingManager {
public:
    void UpdateGlobalLightingStates();                  // 0x00B46C80
};
extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C

enum ERenderPassType { RENDER_PASS_NONE };
class CEngineInternalPrimitiveBase { };
struct CEngineInternalPrimitiveSprite : CEngineInternalPrimitiveBase {
    unsigned char _pad00[0x84];
    unsigned char Modulate2X : 1;                       // +0x84
    unsigned char Lit : 1;
    unsigned char Sprite3D : 1;
    unsigned char RotateAroundCentre : 1;
    unsigned char RotateAroundZAxis : 1;
    unsigned char IsParticle : 1;
    unsigned char ZBuffer : 1;
};
class CEngineSubPrimitiveBase {
public:
    void*                         _vptr;            // +0x00
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
};
class CEnginePrimitiveManager { };
class CEnginePrimitiveManagerParticleSpriteGroup : public CEnginePrimitiveManager {
public:
    void RenderSingleSprite(CEngineSubPrimitiveBase* subPrimitive, ERenderPassType pass);           // 0x00BC04F0
};
struct CEnginePrimitiveManagerTable {
    unsigned char            _pad00[0x10];
    CEnginePrimitiveManager* Managers[40];          // +0x10, by primitive type
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84

class CEnginePrimitiveManagerSprites {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass);
};

void CEnginePrimitiveManagerSprites::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass)
{
    CEnginePrimitiveManagerParticleSpriteGroup* group =
        static_cast<CEnginePrimitiveManagerParticleSpriteGroup*>(g_pPrimitiveManagers->Managers[27]);
    CRenderStateCaptureBlock capture;
    CShaderLayoutSaver layout(5);
    SetState(g_pRenderManagerCore->m_state2ee4, g_renderStateDefault_01396dd0, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2f84, g_renderStateDefault_01396dd0, g_pRenderManagerCore);
    SetStateV(static_cast<CEngineInternalPrimitiveSprite*>(subPrimitive.Parent)->ZBuffer,
              g_pRenderManagerCore->m_state2868, g_pRenderManagerCore);
    SetStateConst(g_pRenderManagerCore->m_state2854, 0, g_pRenderManagerCore);
    SetStateConst(g_pRenderManagerCore->m_state28b8, 1, g_pRenderManagerCore);
    SetStateConst(g_pRenderManagerCore->m_state28cc, 0, g_pRenderManagerCore);
    SetStateConst(g_pRenderManagerCore->m_state2944, 0, g_pRenderManagerCore);
    g_pEngineLightingManager->UpdateGlobalLightingStates();
    g_pShaderRenderManager->ResetWorldTransform();
    group->RenderSingleSprite(&subPrimitive, pass);
    g_pShaderRenderManager->ResetWorldTransform();
}