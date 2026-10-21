// CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass) -- render one weapon trail inside a render-state capture block: pixel shaders off, the engine component at 0x01436E90 prepares its state, two tracked render states set (0 and the global default), the trail drawn, pixel shaders restored to their previous state and the capture block closed
// Leaf: RenderSubPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCD490, length 368 bytes (with int3 padding; body 0x16a), `ret 8` (virtual).
// PDB: ?RenderSubPrimitive@CEnginePrimitiveManagerWeaponTrails@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z.
// Callees: CShaderRenderManager::DisablePixelShaders @ 0x00988190 / EnablePixelShaders @ 0x00988110 (global @ 0x01436E14,
//   PixelShadersEnabled byte +0x391), the virtual slot 0 of the object at +0x20 of the global @ 0x01436E90, RenderWeaponTrail
//   (const CEngineInternalPrimitiveWeaponTrail&) @ 0x00BCC800 (the sub-primitive's Parent +0x0c), CRenderStateManager::
//   RestoreCaptureBlock @ 0x00A05840 (global @ 0x013CB508).
// Globals: the render-state manager is reached both as 0x013CB508 (capture block open/close) and as the render core
//   0x01436E18 (tracked states +0x2854 and +0x2890, the model of the landed EnableFog 0x00B46890); the second state's value
//   is the global @ 0x01396FB8.
// Tracked state: `state.Set(v)` tests the value on the state and forwards to an inline manager ChangeState(state, v)
//   (core in eax, state in ecx, the manager mask in edx as retail); a pure state-side or manager-side setter swaps registers.
// Shape: opening the capture block is inline (push the block marker +0x3a24 on the capture stack, shift the capture mask);
//   each state change is the inline tracked-state Set; the restore arms tail-duplicate the RestoreCaptureBlock call.
class CEngineSubPrimitiveBase;

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
struct CRenderStateManager {
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
    unsigned char        _pad28a4[0x3a24 - 0x28a4];
    unsigned char        m_captureBlockMarker[4];  // +0x3a24
    void RestoreCaptureBlock();                    // 0x00A05840
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
        m_captured[m_capturedCount].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        ++m_capturedCount;
        m_captureMask <<= 1;
    }
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508
extern CRenderStateManager* g_pRenderManagerCore;    // 0x01436E18
extern unsigned long g_renderStateDefault_01396fb8;  // 0x01396FB8

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

struct CShaderRenderManager {
    unsigned char _pad[0x391];
    bool          PixelShadersEnabled;             // +0x391
    void DisablePixelShaders();                    // 0x00988190
    void EnablePixelShaders();                     // 0x00988110
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CRenderStatePreparer {
    virtual void Apply();                          // slot 0
};
struct CEngineComponent_01436e90 {
    unsigned char        _pad[0x20];
    CRenderStatePreparer RenderState;              // +0x20
};
extern CEngineComponent_01436e90* g_pEngineComponent_01436e90;   // 0x01436E90

class CEngineInternalPrimitiveWeaponTrail;
class CEngineSubPrimitiveBase {
public:
    void*                                _vptr;    // +0x00
    unsigned long                        RenderFlags;   // +0x04
    unsigned char                        Alpha;    // +0x08
    CEngineInternalPrimitiveWeaponTrail* Parent;   // +0x0c
};

class CEnginePrimitiveManagerWeaponTrails {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long pass);
    void RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail& trail);   // 0x00BCC800
};

void CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long pass)
{
    g_pRenderStateManager->OpenCaptureBlock();
    bool pixelShaders = g_pShaderRenderManager->PixelShadersEnabled;
    if (pixelShaders)
        g_pShaderRenderManager->DisablePixelShaders();
    g_pEngineComponent_01436e90->RenderState.Apply();
    g_pRenderManagerCore->m_state2854.Set(0);
    g_pRenderManagerCore->m_state2890.Set(g_renderStateDefault_01396fb8);
    RenderWeaponTrail(*subPrimitive.Parent);
    if (pixelShaders != g_pShaderRenderManager->PixelShadersEnabled) {
        if (pixelShaders)
            g_pShaderRenderManager->EnablePixelShaders();
        else
            g_pShaderRenderManager->DisablePixelShaders();
    }
    g_pRenderStateManager->RestoreCaptureBlock();
}
