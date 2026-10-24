// Grade: DIFFER(502v478) -- IMPOSSIBLE as plain C++: the retail body switches the x87 rounding mode in line (`fnstcw [ebp-4] ; and byte ptr [ebp-3],0F3h ; fldcw [ebp-4]`, round-to-nearest without a restore), a sequence VC7.1 only emits from inline assembly (there is no control-word intrinsic in VC7.1; _controlfp is a CRT call, as the landed __setdefaultprecision 0x0040135C shows). The rest of the frame (tracked render-state updates of the fog state, the clear colour reusing the control-word slot) is ordinary C++; this source documents it.
// CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& info) -- render one front-end frame
// Retail 0x0042DF9E, 478 bytes, `ret 4` (Ego_r.pdb ?Draw@CNewFrontendGameComponent@@AAEXABVCEngineInterpolationInfo@@@Z).
// Body: local CEngineInterpolationInfo (ctor 0x00415A60); m_fLastUpdateTime (+0xBC) = GFGetTime() (0x009E1BC0); the
// full-screen 2D box (0, 0, screen width / height doubles 0x013961E8 / 0x013961F0) narrowed to floats; round-to-nearest
// (inline asm in the original); fog render state (+0x2944 of the tracked manager 0x013B8384) set to 0 through the
// inlined capture-aware SetState; CRenderManager2D::SetViewport(box) 0x00A0B560; CDisplayManager (0x013B8390)
// GetVisibleTelevisionScreenArea 0x009BECE0; ClearRenderTarget(CRGBColour(0,0,0,255), 0) 0x009D8CF0 (the colour reuses
// the control-word word); BeginRender 0x009BEF20; the front-end manager draw (0x00595582 / 0x00595222); the error
// message when the UI manager flags one (+0x9C, CManager::RenderErrorMessage 0x0041D03C); the engine's slot 8 draw
// with the interpolation info; a second tracked state (+0x2E44 from the global 0x01396E68); two 2D draw-list flushes
// (0x009D9C80 / 0x009DA9F0(1)) around the IME render (0x00404A80 -> 0x00404C00); EndRender 0x009BEF50 and SwapScreens
// 0x009BEEB0.
// Leaf: ?Draw@CNewFrontendGameComponent

struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {} };
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CEngineInterpolationInfo { unsigned char _body[0x34]; CEngineInterpolationInfo(); };   // 0x00415A60

double __fastcall GFGetTime();                          // 0x009E1BC0
extern double g_ScreenWidth;                            // 0x013961E8
extern double g_ScreenHeight;                           // 0x013961F0
extern unsigned long g_DefaultTextureFilter;            // 0x01396E68

struct CTrackedRenderState {
    long          m_stateId;                            // +0x00
    unsigned long m_value;                              // +0x04
    unsigned long m_captureMask;                        // +0x08
    long          m_appliedValue;                       // +0x0C
    bool          m_queued;                             // +0x10
};
struct SCapturedRenderState { CTrackedRenderState* State; unsigned long Value; };
struct CRenderManager2D {
    unsigned char        _pad000[8];
    SCapturedRenderState m_captured[0x400];             // +0x0008
    CTrackedRenderState* m_queuedStates[0x200];         // +0x2008
    unsigned long        m_capturedCount;               // +0x2808
    unsigned long        m_queuedCount;                 // +0x280c
    unsigned char        _pad2810[4];
    unsigned long        m_captureMask;                 // +0x2814
    unsigned char        _pad2818[0x2944 - 0x2818];
    CTrackedRenderState  m_fogEnable;                   // +0x2944
    unsigned char        _pad2958[0x2e44 - 0x2958];
    CTrackedRenderState  m_textureFilter;               // +0x2e44
    void SetViewport(const C2DBoxF& box);               // 0x00A0B560
    void ClearRenderTarget(const CRGBColour& colour, unsigned long flags);   // 0x009D8CF0
    void DrawBufferedPolys();                           // 0x009D9C80
    void Render2DDrawList(bool flush);                  // 0x009DA9F0
};
extern CRenderManager2D* g_pRenderManager2D;            // 0x013B8384

static __forceinline void SetState(CTrackedRenderState& state, unsigned long value, CRenderManager2D* core)
{
    if (state.m_value == value)
        return;
    unsigned long mask = core->m_captureMask;
    if (!(state.m_captureMask & mask)) {
        state.m_captureMask |= mask;
        core->m_captured[core->m_capturedCount].State = &state;
        core->m_captured[core->m_capturedCount].Value = state.m_value;
        ++core->m_capturedCount;
    }
    state.m_value = value;
    if (!state.m_queued) {
        state.m_queued = true;
        core->m_queuedStates[core->m_queuedCount++] = &state;
    }
}

struct CDisplayManager {
    C2DBoxI GetVisibleTelevisionScreenArea();           // 0x009BECE0
    void BeginRender();                                 // 0x009BEF20
    void EndRender();                                   // 0x009BEF50
    void SwapScreens();                                 // 0x009BEEB0
};
extern CDisplayManager* g_pDisplayManager;              // 0x013B8390

class CIEngine {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Draw(const CEngineInterpolationInfo& info, bool b);   // slot 8 (+0x20)
};
struct CFrontEndManagerDraw { void Draw(); };          // 0x00595222
CFrontEndManagerDraw* GetFrontEndManagerFor(CIEngine* engine, long flags);   // 0x00595582
struct CUIManager {
    unsigned char _pad[0x9c];
    bool HasErrorMessage;                               // +0x9c
    void RenderErrorMessage(CIEngine* engine);          // 0x0041D03C
    static CUIManager* __fastcall GetInstance();        // 0x0041E5F2
};
struct CIMERenderer { void Render(); };                 // 0x00404C00
CIMERenderer* GetIME();                                 // 0x00404A80

class CNewFrontendGameComponent {
public:
    unsigned char _pad00[0x58];
    CIEngine*     m_pEngine;                            // +0x58 (CCountedPointer<CIEngine>::Data)
    unsigned char _pad5c[0xbc - 0x5c];
    float         m_fLastUpdateTime;                    // +0xbc
    void Draw(const CEngineInterpolationInfo& info);
};

void CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& info)
{
    CEngineInterpolationInfo localInfo;
    m_fLastUpdateTime = (float)GFGetTime();
    C2DBoxF screen;
    screen.TLX = 0.0f;
    screen.TLY = 0.0f;
    screen.BRX = (float)g_ScreenWidth;
    screen.BRY = (float)g_ScreenHeight;
    SetState(g_pRenderManager2D->m_fogEnable, 0, g_pRenderManager2D);
    g_pRenderManager2D->SetViewport(screen);
    g_pDisplayManager->GetVisibleTelevisionScreenArea();
    g_pRenderManager2D->ClearRenderTarget(CRGBColour(0, 0, 0, 0xff), 0);
    g_pDisplayManager->BeginRender();
    GetFrontEndManagerFor(m_pEngine, 0)->Draw();
    if (CUIManager::GetInstance()->HasErrorMessage)
        CUIManager::GetInstance()->RenderErrorMessage(m_pEngine);
    m_pEngine->Draw(info, false);
    SetState(g_pRenderManager2D->m_textureFilter, g_DefaultTextureFilter, g_pRenderManager2D);
    g_pRenderManager2D->DrawBufferedPolys();
    g_pRenderManager2D->Render2DDrawList(true);
    GetIME()->Render();
    g_pRenderManager2D->DrawBufferedPolys();
    g_pRenderManager2D->Render2DDrawList(true);
    g_pDisplayManager->EndRender();
    g_pDisplayManager->SwapScreens();
}
