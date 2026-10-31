// CSurfaceCompositor::StartComposition(bool flag) -- begin an off-screen composition: open a render-state capture block, retarget rendering at the compositor's surface (beginning the frame if it was not already rendering), set default states, save the render window and set it to the surface, set four tracked states and the base blend modes
// Leaf: StartComposition
// Grade: DIFFER(794v796) -- the inline CreateCaptureBlock written `CurrentBookmarkMask = CurrentBookmarkMask * 2; size =
//   size + 1;` gives retail's load/shl/store mask update (no `shl dword ptr [mem],1`); left: retail materialises the
//   first state value 0xF in ebx (push ebx, so every frame offset shifts by 4) and loads the stack size into esi before
//   the shift. Tried this pass: mask/count locals in retail's load order (794, same instructions with esi/edx swapped
//   for want of ebx), mask/count computed first (798)
struct C2DBoxF {
    float left, top, right, bottom;
    C2DBoxF(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) { }
};
struct C2DExtentsI { long X; long Y; };
struct CSurface {
    C2DExtentsI GetDimensions() const;             // 0x009F2F90
};
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
struct CStateInfo { unsigned char _body[0x14]; };
struct CRenderManagerCore {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x2840 - 0x2818];
    CTrackedRenderState  m_state2840;              // +0x2840
    unsigned char        _pad2854[0x2ee4 - 0x2854];
    CTrackedRenderState  m_state2ee4;              // +0x2EE4
    unsigned char        _pad2ef8[0x2f84 - 0x2ef8];
    CTrackedRenderState  m_state2f84;              // +0x2F84
    unsigned char        _pad2f98[0x30c4 - 0x2f98];
    CTrackedRenderState  m_state30c4;              // +0x30C4
    unsigned char        _pad30d8[0x3164 - 0x30d8];
    CTrackedRenderState  m_state3164;              // +0x3164
    void SetDefaultRenderStates();                 // 0x00A059E0
    const C2DBoxF& GetAWindow();                   // 0x00A0AAA0
    void SetAWindow(const C2DBoxF& window);        // 0x00A0AA80
};
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
extern unsigned long g_renderStateDefault_01396dc8;   // 0x01396DC8
extern unsigned long g_renderStateDefault_01396e58;   // 0x01396E58
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
struct CRenderStateManager {
    void*              __vftable;            // +0x00
    void*              PD3DDevice;           // +0x04
    CStateStackElement StateStack[1024];     // +0x08
    CStateInfo*        StateUpdateList[256]; // +0x2008
    CStateInfo*        AllStatesList[256];   // +0x2408
    long               StateStackSize;       // +0x2808
    long               StateUpdateListSize;  // +0x280c
    long               AllStatesListSize;    // +0x2810
    unsigned long      CurrentBookmarkMask;  // +0x2814
    unsigned char      DeviceState[0x3a24 - 0x2818];   // +0x2818 CDeviceState
    CStateInfo         BookmarkDummyState;   // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = StateStackSize;
        StateStack[size].State = &BookmarkDummyState;
        CurrentBookmarkMask = CurrentBookmarkMask * 2;
        size = size + 1;
    }
};
extern CRenderStateManager* g_pRenderStateManager; // 0x013CB508
struct CRenderTarget {
    void DetachTarget();                           // 0x00A10E10
    void AttachTarget(CSurface* surface);          // 0x00A10E60
};
struct CDisplayManager {
    void*               __vftable;                 // +0x00
    void*               SystemManager;             // +0x04
    CRenderManagerCore* PRenderManager;            // +0x08
    unsigned char       _pad0c[0x214 - 0x0c];
    bool                Rendering;                 // +0x214
    void SetRenderTarget(CRenderTarget* target);   // 0x009BF160
    void BeginRender();                            // 0x009BEF20
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;              // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct CSurfaceCompositor {
    unsigned char  _pad00[8];
    bool           m_composing;                    // +0x08
    bool           m_wasRendering;                 // +0x09
    unsigned char  _pad0a[0x0c - 0x0a];
    CSurface*      m_pSurface;                     // +0x0c
    CRenderTarget* m_pRenderTarget;                // +0x10
    unsigned char  _pad14[0x18 - 0x14];
    C2DBoxF        m_savedWindow;                  // +0x18
    long           m_quadCount;                    // +0x28
    bool           m_flag;                         // +0x2c
    void SetBaseBlendModes();                      // 0x00F38FF0
    void StartComposition(bool flag);
};

void CSurfaceCompositor::StartComposition(bool flag)
{
    m_composing = true;
    g_pRenderStateManager->CreateCaptureBlock();
    m_flag = flag;
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager;
    CRenderManagerCore* core = GFGetSystemManager()->PDisplayManager->PRenderManager;
    SetState(core->m_state2840, 0xf, core);
    m_pRenderTarget->DetachTarget();
    m_pRenderTarget->AttachTarget(m_pSurface);
    display->SetRenderTarget(m_pRenderTarget);
    m_wasRendering = true;
    if (!display->Rendering) {
        display->BeginRender();
        m_wasRendering = false;
    }
    core->SetDefaultRenderStates();
    m_savedWindow = core->GetAWindow();
    core->SetAWindow(C2DBoxF(0.0f, 0.0f, (float)m_pSurface->GetDimensions().X, (float)m_pSurface->GetDimensions().Y));
    SetState(core->m_state2ee4, g_renderStateDefault_01396dc8, core);
    SetState(core->m_state2f84, g_renderStateDefault_01396dc8, core);
    SetState(core->m_state30c4, g_renderStateDefault_01396e58, core);
    SetState(core->m_state3164, g_renderStateDefault_01396e58, core);
    SetBaseBlendModes();
    m_quadCount = 0;
}
