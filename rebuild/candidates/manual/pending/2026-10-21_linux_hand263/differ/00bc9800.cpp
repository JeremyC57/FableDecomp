// Grade: DIFFER(430v428) -- only the UseZBuffer conversion: retail `xor ebx,ebx ; cmp byte ptr [ebp+78h],bl ; setne bl`, ours `mov dl,[ebp+78h] ; test dl,dl ; setne bl` (tried ?:, != 0, != false, TRUE/FALSE, bool-param helper, char member)
// CEnginePrimitiveManagerLines::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) -- draw one line primitive inside a render-state capture block: two tracked states set, z-buffering from the line's UseZBuffer flag, RenderLine, restore; ERendered 2 (0 when the manager is disabled)
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC9800, length 428 bytes, `ret 0Ch` (virtual).
// Callees: CEnginePrimitiveManagerLines::RenderLine(CEngineInternalPrimitiveLine*) @ 0x00BC8A60, CRenderStateManager::
//   RestoreCaptureBlock @ 0x00A05840 (global @ 0x013CB508).
// Globals: the render-state manager is reached both as 0x013CB508 (capture block open/close, the landed RenderColorFilteredQuad
//   0x00B25C00 model) and as the render core 0x01436E18 (tracked states +0x2890 from the default @ 0x01396FB8, +0x2868 = 1,
//   +0x2854 = UseZBuffer; the model of the landed EnableFog 0x00B46890).
// Layout: Ego_r.pdb CEnginePrimitiveManager (SceneList +4, Enable +8); CEngineInternalPrimitiveLine UseZBuffer +0x78.
struct CEngineSubPrimitiveChain;
struct CEngineInternalPrimitiveBase { unsigned char _base[0x48]; };
struct C3DVector { float X, Y, Z; };
struct CEngineInternalPrimitiveLine : CEngineInternalPrimitiveBase {
    C3DVector     Start;                           // +0x48
    C3DVector     End;                             // +0x54
    unsigned long Colour;                          // +0x60
    float         Width;                           // +0x64
    unsigned long BankIndex;                       // +0x68
    unsigned long Frame;                           // +0x6c
    long          BlendMode;                       // +0x70
    long          BlendOp;                         // +0x74
    bool          UseZBuffer;                      // +0x78
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
    CTrackedRenderState  m_state2868;              // +0x2868
    unsigned char        _pad287c[0x2890 - 0x287c];
    CTrackedRenderState  m_state2890;              // +0x2890
    unsigned char        _pad28a4[0x3a24 - 0x28a4];
    CTrackedRenderState  m_bookmarkDummyState;     // +0x3A24

    void RestoreCaptureBlock();                    // 0x00A05840
    void CreateCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = &m_bookmarkDummyState;
        m_captureMask <<= 1;
    }
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508
extern CRenderStateManager* g_pRenderManagerCore;    // 0x01436E18
extern unsigned long g_renderStateDefault_01396fb8;  // 0x01396FB8

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

struct CEnginePrimitiveManager {
    virtual void v00();                            // +0x00 vptr
    void*  SceneList;                              // +0x04
    bool   Enable;                                 // +0x08
};

struct CEnginePrimitiveManagerLines : CEnginePrimitiveManager {
    virtual long RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer, CEngineSubPrimitiveChain& chain);
    void RenderLine(CEngineInternalPrimitiveLine* line);   // 0x00BC8A60
};

long CEnginePrimitiveManagerLines::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer, CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return 0;
    g_pRenderStateManager->CreateCaptureBlock();
    CEngineInternalPrimitiveLine& line = static_cast<CEngineInternalPrimitiveLine&>(primitive);
    SetState(g_pRenderManagerCore->m_state2890, g_renderStateDefault_01396fb8, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2868, 1, g_pRenderManagerCore);
    SetState(g_pRenderManagerCore->m_state2854, line.UseZBuffer ? 1 : 0, g_pRenderManagerCore);
    RenderLine(&line);
    g_pRenderStateManager->RestoreCaptureBlock();
    return 2;
}
