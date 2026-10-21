// Behaviour test for 00499880 (CProgressDisplay::RenderProgress): early outs, call order, viewport/target save+restore,
// full-screen clear + text flag save/restore, debug display skips drawing, capture block pushed and restored.
#include <cstdio>
#include <cstring>
#include <string>
#ifndef _MSC_VER
#define __fastcall
#endif
static std::string g_log;
static void L(const char* s) { g_log += s; g_log += ' '; }
// CProgressDisplay::RenderProgress(const CCharString& textTag, float progress, bool showText, bool force) -- render one progress-screen frame: inside a render-state capture block, (re)start rendering, set a full-screen viewport, clear to black when full screen, draw the progress unless debug display is on, then restore the viewport, render target and rendering state
// Leaf: ?RenderProgress@CProgressDisplay@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00499880, 489 bytes + padding (`ret 10h`). Callees: GFGetSystemManager 0x009A4EC0, the render
// manager's 0x00A0AA60 / 0x00A0B560 / 0x009DA9F0 / 0x00A0AA90, the display manager's 0x009BEF20 / 0x009BE420 / 0x009BF220 / 0x009BEF50 /
// 0x009BEEB0 / 0x009BF160, CSystemManager 0x009A6460, CRenderTarget(const CSurface&, const CSurface&) 0x00A10DC0, CSurface::Uninitialise
// 0x009F2E20, ~CBase 0x0099A2E0, CProgressDisplay 0x00498490, CRenderStateManager::RestoreCaptureBlock 0x00A05840. Names of the
// callees without a PDB/landed name are descriptive.
// Layout: CDisplayManager per Ego_r.pdb (PRenderManager +8, RenderTargetSurface +0xc, RenderTargetDepthBuffer +0x1c, ViewportBox +0x1f4,
// Rendering +0x214); CRenderStateManager as the landed CEngine::RenderColorFilteredQuad 0x00B25C00; CProgressDisplay as the sibling
// StartProgress 0x00499AA0 (pack(4): vfptr + doubles).
// Shape: the capture block is a scoped object (its restore runs after the render-target local's inline destructor); C2DBoxI has a
// member-wise user copy constructor (the viewport is copied field by field straight from the display manager); the display
// manager's Rendering flag is homed in the dead `force` slot.
struct CCharString {
    void* PStringData;
};
struct CBase {
    virtual ~CBase();                                       // 0x0099A2E0
};
struct CSurface {                                           // vtable 0x0122F84C, 0x10 bytes
    virtual void v00();
    void*         PD3DSurface;                              // +0x04
    unsigned long AllocationSource;                         // +0x08
    void*         PAllocatedMemory;                         // +0x0c
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F2E20
};
struct CRenderTarget : CBase {                              // 0x24 bytes
    CSurface m_colour;                                      // +0x04
    CSurface m_depth;                                       // +0x14
    CRenderTarget(const CSurface& colour, const CSurface& depth);   // 0x00A10DC0
};
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) {}
};
struct C2DBoxF {
    float X0, Y0, X1, Y1;
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRenderManager {
    bool IsDeviceLost() const;                              // 0x00A0AA60 (descriptive)
    void SetViewport(const C2DBoxF& box);                   // 0x00A0B560
    void SetViewport(const C2DBoxI& box);                   // 0x00A0AA90
    void Flush(bool wait);                                  // 0x009DA9F0 (descriptive)
};
struct CDisplayManager {
    void*          __vftable;
    void*          SystemManager;                           // +0x04
    CRenderManager* PRenderManager;                         // +0x08 scoped_ptr
    CSurface       RenderTargetSurface;                     // +0x0c
    CSurface       RenderTargetDepthBuffer;                 // +0x1c
    unsigned char  _pad2c[0x1f4 - 0x2c];
    C2DBoxI        ViewportBox;                             // +0x1f4
    unsigned char  _pad204[0x214 - 0x204];
    bool           Rendering;                               // +0x214
    void ToggleRendering();                                 // 0x009BEF20 (descriptive)
    void Clear(const CRGBColour& colour, unsigned long flags);   // 0x009BE420
    void BeginFrame();                                      // 0x009BF220 (descriptive)
    void EndFrame();                                        // 0x009BEF50 (descriptive)
    void Present();                                         // 0x009BEEB0 (descriptive)
    void SetRenderTarget(const CRenderTarget& target);      // 0x009BF160
};
struct CTextSettings {
    unsigned char _pad00[0x38];
    bool          Flag;                                     // +0x38 (descriptive)
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                        // +0x60
    unsigned char    _pad64[0x84 - 0x64];
    CTextSettings*   TextSettings;                          // +0x84 (descriptive)
    void PrepareDebugRender();                              // 0x009A6460 (descriptive)
};
CSystemManager* __fastcall GFGetSystemManager();           // 0x009A4EC0
extern double g_ProgressScreenWidth;                        // 0x013961E8 (descriptive)
extern double g_ProgressScreenHeight;                       // 0x013961F0 (descriptive)

struct CStateInfo { unsigned char _body[0x14]; };
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
struct CRenderStateManager {
    void*              __vftable;
    void*              PD3DDevice;
    CStateStackElement StateStack[1024];                    // +0x08
    CStateInfo*        StateUpdateList[256];
    CStateInfo*        AllStatesList[256];
    long               StateStackSize;                      // +0x2808
    long               StateUpdateListSize;
    long               AllStatesListSize;
    unsigned long      CurrentBookmarkMask;                 // +0x2814
    unsigned char      DeviceState[0x3a24 - 0x2818];
    CStateInfo         BookmarkDummyState;                  // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = StateStackSize;
        StateStack[size].State = &BookmarkDummyState;
        ++size;
        CurrentBookmarkMask <<= 1;
    }
    void RestoreCaptureBlock();                             // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;          // 0x013CB508
struct CRenderStateCaptureBlock {                           // scoped capture block (descriptive)
    CRenderStateCaptureBlock() { g_pRenderStateManager->CreateCaptureBlock(); }
    ~CRenderStateCaptureBlock() { g_pRenderStateManager->RestoreCaptureBlock(); }
};

#pragma pack(push, 4)
struct CProgressDisplay {
    virtual void v00();
    unsigned char _pad04[0x78 - 0x04];
    bool          DebugDisplay;                             // +0x78
    bool          Initialised;                              // +0x79
    bool          FullScreen;                               // +0x7a
    void DrawProgress(const CCharString& textTag, float progress, bool showText);   // 0x00498490 (descriptive)
    void RenderProgress(const CCharString& textTag, float progress, bool showText, bool force);
};
#pragma pack(pop)

void CProgressDisplay::RenderProgress(const CCharString& textTag, float progress, bool showText, bool force)
{
    CDisplayManager* display = GFGetSystemManager()->DisplayManager;
    if (!display)
        return;
    CRenderManager* renderer = display->PRenderManager;
    if (!renderer)
        return;
    if (!force && renderer->IsDeviceLost())
        return;
    bool wasRendering = display->Rendering;
    if (DebugDisplay && !wasRendering)
        GFGetSystemManager()->PrepareDebugRender();
    CRenderStateCaptureBlock captureBlock;
    bool savedFlag = false;
    if (FullScreen) {
        CTextSettings* settings = GFGetSystemManager()->TextSettings;
        savedFlag = settings->Flag;
        settings->Flag = false;
    }
    if (!wasRendering)
        display->ToggleRendering();
    C2DBoxI oldViewport = display->ViewportBox;
    CRenderTarget oldTarget(display->RenderTargetSurface, display->RenderTargetDepthBuffer);
    renderer->SetViewport(C2DBoxF(0.0f, 0.0f, (float)g_ProgressScreenWidth, (float)g_ProgressScreenHeight));
    if (FullScreen)
        display->Clear(CRGBColour(0, 0, 0, 0xff), 7);
    display->BeginFrame();
    if (!DebugDisplay)
        DrawProgress(textTag, progress, showText);
    renderer->Flush(true);
    display->EndFrame();
    display->Present();
    renderer->SetViewport(oldViewport);
    display->SetRenderTarget(oldTarget);
    if (wasRendering)
        display->ToggleRendering();
    if (FullScreen)
        GFGetSystemManager()->TextSettings->Flag = savedFlag;
}

CBase::~CBase() {}
void CSurface::v00() {}
void CSurface::Uninitialise() { L("unS"); }
CRenderTarget::CRenderTarget(const CSurface& c, const CSurface& d) { m_colour.PD3DSurface = c.PD3DSurface; m_depth.PD3DSurface = d.PD3DSurface; L("RT"); }
static bool s_lost = false;
static C2DBoxF s_box(0, 0, 0, 0);
static long s_restoredX1 = 0;
static void* s_targetColour = 0;
static unsigned char s_clearA = 0;
bool CRenderManager::IsDeviceLost() const { return s_lost; }
void CRenderManager::SetViewport(const C2DBoxF& b) { s_box = b; L("VF"); }
void CRenderManager::SetViewport(const C2DBoxI& b) { s_restoredX1 = b.X1; L("VI"); }
void CRenderManager::Flush(bool) { L("F"); }
void CDisplayManager::ToggleRendering() { L("T"); }
void CDisplayManager::Clear(const CRGBColour& c, unsigned long f) { s_clearA = c.A; if (f == 7 && !c.R && !c.G && !c.B) L("C"); }
void CDisplayManager::BeginFrame() { L("B"); }
void CDisplayManager::EndFrame() { L("E"); }
void CDisplayManager::Present() { L("P"); }
void CDisplayManager::SetRenderTarget(const CRenderTarget& t) { s_targetColour = t.m_colour.PD3DSurface; L("ST"); }
void CSystemManager::PrepareDebugRender() { L("DBG"); }
static CSystemManager s_sys;
CSystemManager* __fastcall GFGetSystemManager() { return &s_sys; }
double g_ProgressScreenWidth = 640.0;
double g_ProgressScreenHeight = 480.0;
static CRenderStateManager s_rsm;
CRenderStateManager* g_pRenderStateManager = &s_rsm;
void CRenderStateManager::RestoreCaptureBlock() { --StateStackSize; CurrentBookmarkMask >>= 1; L("R"); }
void CProgressDisplay::v00() {}
void CProgressDisplay::DrawProgress(const CCharString&, float p, bool) { if (p == 0.5f) L("D"); }
static int fails = 0;
static void check(bool c, int n) { if (!c) { std::printf("fail %d: %s\n", n, g_log.c_str()); ++fails; } }
int main()
{
    static CDisplayManager dm;
    static CRenderManager rm;
    static CTextSettings ts;
    static int surf;
    std::memset(&ts, 0, sizeof(ts));
    s_sys.DisplayManager = 0; s_sys.TextSettings = &ts;
    s_rsm.StateStackSize = 0; s_rsm.CurrentBookmarkMask = 1;
    CProgressDisplay pd; pd.DebugDisplay = false; pd.Initialised = true; pd.FullScreen = true;
    CCharString tag; tag.PStringData = 0;
    pd.RenderProgress(tag, 0.5f, true, false);
    check(g_log.empty(), 1);
    s_sys.DisplayManager = &dm; dm.PRenderManager = 0;
    pd.RenderProgress(tag, 0.5f, true, false);
    check(g_log.empty(), 2);
    dm.PRenderManager = &rm; s_lost = true;
    pd.RenderProgress(tag, 0.5f, true, false);
    check(g_log.empty(), 3);
    dm.RenderTargetSurface.PD3DSurface = &surf; dm.Rendering = false;
    dm.ViewportBox.X0 = 1; dm.ViewportBox.Y0 = 2; dm.ViewportBox.X1 = 33; dm.ViewportBox.Y1 = 4;
    ts.Flag = true;
    pd.RenderProgress(tag, 0.5f, true, true);   // forced past the lost device
    check(g_log == "T RT VF C B D F E P VI ST unS unS R ", 4);
    check(s_box.X1 == 640.0f && s_box.Y1 == 480.0f && s_box.X0 == 0.0f && s_restoredX1 == 33 && s_targetColour == &surf && s_clearA == 0xff, 5);
    check(ts.Flag == true && s_rsm.StateStackSize == 0 && s_rsm.CurrentBookmarkMask == 1, 6);
    g_log.erase(); s_lost = false; dm.Rendering = true; pd.FullScreen = false; pd.DebugDisplay = true;
    pd.RenderProgress(tag, 0.5f, true, false);
    check(g_log == "RT VF B F E P VI ST T unS unS R ", 7);
    g_log.erase(); dm.Rendering = false;
    pd.RenderProgress(tag, 0.5f, true, false);
    check(g_log == "DBG T RT VF B F E P VI ST unS unS R ", 8);
    if (fails == 0) std::printf("00499880_TEST PASS\n");
    return fails;
}