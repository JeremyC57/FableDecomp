// CDisplayEngine::DoRender(const CInterpolationInfo* info, bool, bool swap) -- render one game frame
// Grade: DIFFER(2571v2617) -- 401 md lines, 46 bytes short: retail frame 0xcc vs ours 0x90 (retail does not share stack slots between the screenshot / stats / track-name blocks, so every later slot offset differs), the track-name loop keeps its counter / count in different registers, and the debug-map block's C2DCoordI temp ordering
// Next idea: find the source shape that stops VC7.1 sharing slots (retail camera at frame 0x94, our 0x5c): try the screenshot filename built by an inline helper returning CWideString by value, the stats text through a named CWideString in the outer scope with the Sprintf inline, or the loop as for (unsigned long i...) with things[i] through an iterator
// Leaf: ?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00435530, 2617 bytes, `ret 0Ch`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW3DSCALEDTEXT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "fable_string.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CCountedPointer.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/GFFPUSetRoundingNearest.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "stlport/fable_containers.h"
#include <stdlib.h>
#include "engine/CFontBank.h"

extern double VIRTUAL_SCREEN_WIDTH;                                      // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern CRenderManager* PRenderManager;                                   // 0x013B8384
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
extern const float DELTA_NOTIONAL_ZERO;                                  // 0x0129BA3C
extern long g_DebugMapDrawLevel;                                         // 0x01375720
extern long g_013BA85C;                                                  // 0x013BA85C
extern long g_013B8A0C;                                                  // 0x013B8A0C
extern long g_013B8A10;                                                  // 0x013B8A10
extern bool g_RenderThrottleEnabled;                                     // 0x013B860C
extern bool g_RenderThrottlePaused;                                      // 0x013B8629
extern unsigned long g_RenderFrame;                                      // 0x013B7D70
extern bool g_DrawStatsText;                                             // 0x013B86EB
extern bool g_DrawBuildVersion;                                          // 0x013B86E7
extern bool g_ScreenshotMode;                                            // 0x01375455
extern long g_ScreenshotCount;                                           // 0x013B862C

class CThing { public: unsigned char _pad[0x70]; struct CTrackNodeInfo* PTrackNode; };   // +0x70
struct CTrackNodeInfo { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual const long* PeekNameIndex(); };   // slot 10 (+0x28)
class CDefStringTable { public: CCharString GetString(long index); };   // 0x009D49B0
extern CDefStringTable GDefStringTable;                  // 0x013CA828
class CThingFilter_OverlappingCircle { public: C3DVector Centre; float Radius; CThingFilter_OverlappingCircle(const C3DVector& c, float r) : Centre(c), Radius(r) { } };
class CThingSearchTools { public:
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& things) const;   // 0x00436D60
};
extern CCharString FableFullBuildVersion;               // 0x013B85E0
CWideString __fastcall GFIntToWideString(long value);   // 0x00405F00
// CSurface::SaveAsTGA 0x009F4590 (?SaveAsTGA@CSurface@@QAE_NPBVCWideString@@PBVC2DExtentsI@@_N@Z) is not in engine/CSurface.h and the
// render-target surface (+0x2C) is private in engine/CDisplayManager.h: a local view (header debt).
class CSurfaceSaveView { public: bool SaveAsTGA(const CWideString* filename, const C2DExtentsI* extents, bool flag); };
inline CSurfaceSaveView& PeekRenderTargetSurface(CDisplayManager* manager) { return *(CSurfaceSaveView*)((char*)manager + 0x2c); }
class CInterpolationInfo { public: float Time; };
class CCamera {
public:
    CCamera(const C3DVector& pos, const C3DVector& dir, float fov, long flags);   // 0x00A0BF20
    unsigned char _data[0x38];
};
class CEngineManager { public: void OnRender(); };                       // 0x00640500
template <class T> class CTBaseSingleton { public: static T* __fastcall Get(); };
class CInputManager { public: unsigned char _pad[0x38]; bool Paused; };
extern CInputManager* PInputManager;                                     // 0x013B8388
class CFadeInFadeOutBase {
public:
    bool Active;
    unsigned char _pad[0x1b];
    float GetFadeInOutAlpha() const;                                     // 0x004348D0
    void Update(float);                                                  // 0x00434870
};
class CSubtitleRenderer { public: bool IsWaitingForClickPast() const; }; // 0x0048DB46
class CPlayerGui { public: unsigned char _pad[0x18]; CSubtitleRenderer Subtitles; };
extern CPlayerGui* GlobalPlayerGui;                                      // 0x013B8790
class CPlayer {
public:
    unsigned char _pad[8];
    bool Active;
    long GetCurrentMode() const;                                         // 0x00633BE0
    void DrawFreeCamDebug(const C2DBoxF&, CFontBank*) const;             // 0x00487570
};
class CThingMapManager { public: void DebugDraw(const C2DCoordI&, long); };   // 0x00637C80
class CWorld {
public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual C3DVector GetDebugCentre(const CInterpolationInfo*);         // slot 5
    virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual struct CWorldMapHolder* GetWorldMap();                       // slot 13
    unsigned char _pad[0x1c];
    CThingSearchTools* ThingManager;                                     // +0x20
    CPlayer* PeekMyPlayer();                                             // 0x0049D9D0
};
struct CWorldMapHolder { unsigned char _pad[0x10]; CThingMapManager* MapManager; };
class CViewRenderer {
public:
    virtual void v0(); virtual void v1();
    virtual void SetWindow(const C2DBoxF&);
    virtual void Render(const CInterpolationInfo*, bool);
};
struct CDisplayViewManager { CViewRenderer* Renderer; };
class CSystemManager { public: unsigned char _pad[0xf8]; struct CDebugDrawer* Debug; };
struct CDebugDrawer { void Draw(const C2DBoxF&, CFontBank*); };         // 0x009F8D60
CSystemManager* __fastcall GFGetSystemManager();                         // 0x009A4EC0
class CMainGameComponent { public: unsigned char _pad[0x16144]; CFontBank* MainFont; unsigned char _pad2[4]; CFontBank* DebugFont; unsigned char _pad3[0x161e4 - 0x16150]; long FramesBehind; };
class CConsole { public: CConsole(); unsigned char _pad[0x78]; bool Visible; unsigned char _pad79[0x9c - 0x79]; void Render(const C2DBoxF& window); };   // ctor 0x009ECD80, Render 0x009EA550
template <> class CTBaseSingleton<CConsole> { public:
    static CCountedPointer<CConsole> Instance;           // 0x013CAA40
    static CConsole* Get() { if (!Instance.Data) Instance.Reset(new CConsole); return Instance.Data; }
};

float __fastcall GFAYToVY(float);
void SetGamma(float, bool);                                              // 0x009BEAA0 (thiscall on GPDisplayManager in retail)                                        // 0x009E1DD0

class CDisplayEngine {
public:
    void* __vftable;
    unsigned long EnginePreloadFlags;
    CMainGameComponent* Component;                                       // +0x08
    unsigned char _pad0c[0x10];
    C2DBoxF MainWindow;                                                  // +0x1c
    CIEngine* Engine3D;                                                  // +0x2c
    unsigned char Camera[0x38];
    long Frame;                                                          // +0x68
    bool DrawGame;                                                       // +0x6c
    unsigned char _pad6d[3];
    CFadeInFadeOutBase LetterBox;                                        // +0x70
    unsigned char _pad8c[0xb4 - 0x8c];
    float GammaRamp;                                                     // +0xb4
    float OldGammaRamp;                                                  // +0xb8
    CFadeInFadeOutBase ScreenFadeOutInfo;                                // +0xbc
    unsigned char _padd8[0xe0 - 0xd8];
    CDisplayViewManager* PViewManager;                                   // +0xe0
    void* PViewManagerInfo;                                              // +0xe4
    long InitialFadeReadynessCount;                                      // +0xe8
    bool InitialFadeActive;                                              // +0xec
    unsigned char _paded[3];
    float ScreenFadeDuration;                                            // +0xf0
    float TimeSinceFadeStarted;                                          // +0xf4
    CWorld* World;                                                       // +0xf8
    void SetAboutToRun();                                                // 0x00434CD0
    void EndScreenFadeOut();                                             // 0x00434C90
    void RenderHeroFuckShit(const CInterpolationInfo&) const;            // 0x00435000
    void RenderHeroDrunkenness(const CInterpolationInfo&) const;         // 0x00435070
protected:
    void DoRender(const CInterpolationInfo* info, bool arg2, bool swap);
};

void CDisplayEngine::DoRender(const CInterpolationInfo* info, bool arg2, bool swap)
{
    if (InitialFadeReadynessCount > 0)
        SetAboutToRun();
    else if (InitialFadeActive && (unsigned char)(ScreenFadeOutInfo.GetFadeInOutAlpha() * 255.0f) < 0x80)
        InitialFadeActive = false;
    {
        float bottom = (float)VIRTUAL_SCREEN_HEIGHT;
        float right = (float)VIRTUAL_SCREEN_WIDTH;
        C2DBoxF* window = &MainWindow;
        window->TLX = 0.0f;
        window->BRX = right;
        window->TLY = 0.0f;
        window->BRY = bottom;
    }
    GFFPUSetRoundingNearest();
    g_013BA85C = 0;
    CCamera camera(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f), 0.25f, 0);
    PRenderManager->SetFogEnable(false);
    PRenderManager->SetVWindow(MainWindow);
    C2DBoxF window = MainWindow;
    C2DBoxF televisionArea = GPDisplayManager->GetVVisibleTelevisionScreenArea();
    g_013B8A0C = 0;
    g_013B8A10 = 0;
    GPDisplayManager->BeginRender();
    PRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 0), 0);
    if (DrawGame) {
        CTBaseSingleton<CEngineManager>::Get()->OnRender();
        if (NUISystem::CManager::GetInstance()->HasErrorMessage())
            NUISystem::CManager::GetInstance()->RenderErrorMessage(*Engine3D);
        if (!g_RenderThrottleEnabled || g_RenderThrottlePaused || !PInputManager->Paused || g_RenderFrame % 15 == 0) {
            if (PViewManager->Renderer) {
                PViewManager->Renderer->SetWindow(window);
                PViewManager->Renderer->Render(info, arg2);
            }
        }
    }
    GFGetSystemManager()->Debug->Draw(MainWindow, Component->MainFont);
    CPlayer* player = World->PeekMyPlayer();
    if (player->Active && player->GetCurrentMode() == 0x13)
        player->DrawFreeCamDebug(MainWindow, Component->MainFont);
    if (LetterBox.Active)
        LetterBox.Update(info->Time);
    RenderHeroFuckShit(*info);
    RenderHeroDrunkenness(*info);
    if (ScreenFadeDuration > -DELTA_NOTIONAL_ZERO && !GlobalPlayerGui->Subtitles.IsWaitingForClickPast()) {
        if (TimeSinceFadeStarted <= ScreenFadeDuration)
            TimeSinceFadeStarted = TimeSinceFadeStarted + info->Time;
        if (TimeSinceFadeStarted > ScreenFadeDuration)
            EndScreenFadeOut();
    }
    if (ScreenFadeOutInfo.Active) {
        float time = 0.0f;
        if (!InitialFadeReadynessCount)
            time = info->Time;
        ScreenFadeOutInfo.Update(time);
    }
    unsigned char white = 0xff;
    if (g_DebugMapDrawLevel >= 0) {
        C3DVector centre = World->GetDebugCentre(info);
        World->GetWorldMap()->MapManager->DebugDraw(C2DCoordI((long)centre.X, (long)centre.Y), g_DebugMapDrawLevel);
        std::vector<CThing*> things;
        CThingFilter_OverlappingCircle filter(centre, 0.5f);
        long x = (long)centre.X;
        long y = (long)centre.Y;
        World->ThingManager->GetAllThingsInRoughArea<CThing>(C2DBoxI(x - 1, y - 1, x + 1, y + 1), filter, things);
        unsigned long count = things.size();
        for (long i = 0; i < count; ++i) {
            CTrackNodeInfo* node = things[i]->PTrackNode;
            if (node)
                PRenderManager->Draw3DScaledText(C3DVector((float)(VIRTUAL_SCREEN_WIDTH * 0.45), GFAYToVY(45.0f) + GFAYToVY(16.0f) * i, 0.0f), 0.7f,
                                                 GDefStringTable.GetString(*node->PeekNameIndex()).ToWideString(), *Component->MainFont, CRGBColour(white, white, white, white), 0);
        }
    }
    if (g_RenderThrottlePaused && !PInputManager->Paused)
        PRenderManager->Draw2DText(C2DVector((float)(VIRTUAL_SCREEN_WIDTH * 0.5), (float)(VIRTUAL_SCREEN_HEIGHT * 0.5)), L"Recording!", *Component->DebugFont, CRGBColour(white, white, white, white), 0);
    if (g_DrawStatsText && Component->FramesBehind) {
        CWideString count;
        count.Sprintf(L"%d", Component->FramesBehind);
        PRenderManager->Draw2DText(C2DVector((float)(VIRTUAL_SCREEN_WIDTH * 0.5), (float)(VIRTUAL_SCREEN_HEIGHT * 0.5)), L"Frames behind " + count, *Component->DebugFont, CRGBColour(white, white, white, white), 0);
    }
    if (g_RenderThrottleEnabled && !g_RenderThrottlePaused && PInputManager->Paused)
        PRenderManager->Draw2DText(C2DVector((float)(VIRTUAL_SCREEN_WIDTH * 0.5), (float)(VIRTUAL_SCREEN_HEIGHT * 0.5)), L"Skipping unrecorded frames!", *Component->DebugFont, CRGBColour(white, white, white, white), 0);
    CConsole* console = CTBaseSingleton<CConsole>::Get();
    if (console->Visible)
        console->Render(C2DBoxF(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)(VIRTUAL_SCREEN_HEIGHT * 5.0 * (1.0 / 12.0))));
    if (g_DrawBuildVersion) {
        CFontBank* font = Component->DebugFont;
        PRenderManager->Draw2DText(C2DVector((float)(VIRTUAL_SCREEN_WIDTH * 0.1), (float)(VIRTUAL_SCREEN_HEIGHT - 2.0 * GFAYToVY(font->GetFontMaxHeight()))),
                                   FableFullBuildVersion.ToWideString(), *font, CRGBColour(white, white, white, white), 0);
    }
    PRenderManager->SetTextureMipMapMode(0, TEXTURE_MIPMAP_LINEAR);
    PRenderManager->DrawBufferedPolys();
    PRenderManager->Render2DDrawList(true);
    GPDisplayManager->EndRender();
    if (g_ScreenshotMode) {
        if (g_RenderThrottlePaused && PInputManager->Paused) {
            CWideString filename(L"data\\movies\\shot");
            CWideString number = GFIntToWideString(Frame);
            for (long i = 0; i < 6 - number.GetLength(); ++i)
                filename += L"0";
            filename += number;
            filename += L".tga";
            C2DExtentsI extents(640, 480);
            PeekRenderTargetSurface(GPDisplayManager).SaveAsTGA(&filename, &extents, false);
        }
        else if (g_ScreenshotCount > 0) {
            CWideString filename(L"wishworldshot");
            CWideString number;
            number.Sprintf(L"%d", Frame);
            for (long i = 0; i < 6 - number.GetLength(); ++i)
                filename += L"0";
            filename += number;
            filename += L".tga";
            PeekRenderTargetSurface(GPDisplayManager).SaveAsTGA(&filename, 0, false);
            --g_ScreenshotCount;
        }
    }
    if (GammaRamp != OldGammaRamp) {
        OldGammaRamp = GammaRamp;
        SetGamma(GammaRamp, false);
    }
    if (swap)
        GPDisplayManager->SwapScreens();
    ++Frame;
}
