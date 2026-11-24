// CDisplayEngine::DoRender(const CInterpolationInfo* info, bool world_updated, bool render) -- render one game frame
// Grade: RELOCATION_MATCH (wfA2-9) -- closed by the CFontBank::GetVFontMaxHeight body opt-in FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN (`return (GFAYToVY(GetFontMaxHeight()));`): its no-code paren tuple is the one extra integer-unit scheduler node retail had in the build-version block (0x00435C9F `fstp` before `push edi`, 0x00435CA5 `fadd` before `push esp`)
// Leaf: ?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z
// Flags: /d2inlT63
// Inline-budget: /d2inlT63 (per-row flag, source untouched) -- the inline GFIntToWideString (engine/GFIntToWideString.h, COMDAT 0x00405F00)
//   is expanded at the FramesBehind text and the wishworldshot branch and CALLED at the movie-shot branch (0x00435D9D); the default T74
//   expands all three (2649 bytes), T61..T66 match, T60 also calls a second one. T63 = the flag the matched same-area row 0x00439070 uses.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00435530, 2617 bytes, `ret 0Ch`.
// Retail-asm: 0x00435594 `fnstcw word ptr [esp+14h]` + 0x00435598 `and dword ptr [esp+14h],0FFFFF3FFh` + 0x004355A0 `fldcw word ptr [esp+14h]`,
//   through the shared helper engine/GFFPUSetRoundingNearest.h (FableWin.pdb ?GFFPUSetRoundingNearest@@YIXXZ; landed callers 0x0042DF9E /
//   0x00418289 carry the same evidence: no VC7.1 C++ spelling emits fnstcw / fldcw).
// GetVFontMaxHeight paren (wfA2-9): EVIDENCED by two rows' retail bytes, not by a PDB text: this row matches only with it, and the
//   landed CFrontendGameComponent::Output 0x00494900 matches with it AND its natural `y += MainFont->GetVFontMaxHeight();` (the plain
//   body needs a named line_height temporary there); CPlayer::DrawFreeCamDebug 0x00487570 matches with either body (hdr/README.md).
// Same TU (evidence only, not used): retail keeps CDisplayEngine::PeekMainFont / PeekDebugFont out of line at 0x00434DE0 / 0x00434DF0;
//   defining them above DoRender instead of the header's inline bodies gives the same bytes.
// FITTED: `long i = 0;` declared before `unsigned long count = things.size();` (orders `xor eax,eax` before `sar edi,2`, 0x00435951);
//   the named `const CThing* thing = things[i];` (retail reuses the PBaseDef word of the HasBaseDef test, 0x00435965).
// Same-TU callee (wfA2-7, closes the loop-latch reload order 0x00435A14 together with ..._ROUGHAREABODY): EndScreenFadeOut 0x00434C90 is
//   compiled earlier in retail's TU; its landed body is defined above the row under #pragma auto_inline(off) (retail calls it out of
//   line; it grades RELOCATION_MATCH v 0x00434C90 from this file). c2's split-piece ids carry state from the previous function.
// GetAllThingsInRoughArea<CThing> (retail instance 0x00436D60, same TU): its template body is visible (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY),
//   so c2 knows `things` is not captured (things._M_start becomes a split candidate like i: same bytes, retail reload order).
// CRGBColour(r,g,b,a) through the existing opt-in ..._CTOR4_INITIALISE (FableWin /Od 18i = ours; same release bytes).
// Names: 0x013B85E0 is GVersionString (Ego_r.pdb dynamic initializer; landed spelling since agentNF26; was FableFullBuildVersion).
// Descriptive names: the globals g_DebugMapDrawLevel / g_013B8A0C / g_013B8A10 / g_DrawStatsText / g_DrawBuildVersion / g_ScreenshotMode /
//   g_ScreenshotCount (no PDB / FSE name found); g_LightsDrawnThisFrame / g_RenderThrottle*_013b86xx / g_RenderFrame_013b7d70 are landed spellings.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_TOWIDESTRING_INLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASBASEDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_FRIEND_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
#define FABLE_RETAIL_CDISPLAYMANAGER_SETGAMMA
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_SAVEASTGA
#define FABLE_RETAIL_CDISPLAYVIEWMANAGER_GETCURRENTVIEW
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYER_ISDRAWINGFREECAMDEBUG
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_STEALTH_DRAW
#define FABLE_RETAIL_CDISPLAYENGINE_DORENDER
#define FABLE_RETAIL_CDISPLAYENGINE_DORENDER_BODY
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLICKPAST
#define FABLE_RETAIL_CENGINEMANAGER_ONRENDER
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CTHINGMAPMANAGER_DEBUGDRAW
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CCAMERA_TARGET_CTOR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_PROFILEMANAGER
#define FABLE_RETAIL_CINPUTMANAGER_EVENTFLAGS
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_INITIALISE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW3DSCALEDTEXT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN       // wfA2-9 (see the header notes above)
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "fable_string.h"
#include "engine/GFIntToWideString.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefString.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingMapManager.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CCamera.h"
#include "engine/CInterpolationInfo.h"
#include "engine/CEngineManager.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CDisplayViewManager.h"
#include "engine/CSystemManager.h"
#include "engine/CInputManager.h"
#include "engine/CRGBColour.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/GFFPUSetRoundingNearest.h"
#include "engine/CFontBank.h"
#include "stlport/fable_containers.h"

extern double VIRTUAL_SCREEN_WIDTH;                                      // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern CRenderManager* GPRenderManager;                                   // 0x013B8384 (Ego_r / FableWin / Ego_d publics; NF236: was PRenderManager)
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
extern CInputManager* GPInputManager;                                    // 0x013B8388
extern CCharString GVersionString;                                       // 0x013B85E0
extern long g_LightsDrawnThisFrame;                                      // 0x013BA85C (landed spelling, descriptive)
extern long g_DebugMapDrawLevel;                                         // 0x01375720 (descriptive)
extern long g_013B8A0C;                                                  // 0x013B8A0C
extern long g_013B8A10;                                                  // 0x013B8A10
extern unsigned char g_RenderThrottleEnabled_013b860c;                   // 0x013B860C (landed spelling)
extern unsigned char g_RenderThrottlePaused_013b8629;                    // 0x013B8629 (landed spelling)
extern unsigned long g_RenderFrame_013b7d70;                             // 0x013B7D70 (landed spelling)
extern bool g_DrawStatsText;                                             // 0x013B86EB (descriptive)
extern bool g_DrawBuildVersion;                                          // 0x013B86E7 (descriptive)
extern bool g_ScreenshotMode;                                            // 0x01375455 (descriptive)
extern long g_ScreenshotCount;                                           // 0x013B862C (descriptive)

// NDisplayView::CViewBase / CProfileManager / CConsole: no shared header (local declarations of the members DoRender calls).
namespace NDisplayView {
class CViewBase {
public:
    virtual void OnInit();                                               // slot 0
    virtual void OnUninit();                                             // slot 1
    virtual void SetVWindow(const C2DBoxF& window);                      // slot 2 (+0x08)
    virtual void Render(const CInterpolationInfo& info, bool world_updated);   // slot 3 (+0x0C)
};
}
class CProfileManager { public: void DrawProfiles(const C2DBoxF& window, const CFontBank& font); };   // 0x009F8D60
class CConsole : public CTBaseSingleton<CConsole> {
public:
    CConsole();                                                          // 0x009ECD80
    bool IsActive() const { return Active; }
    void Draw(const C2DBoxF& window);                                    // 0x009EA550
private:
    unsigned char _pad_0x04[0x78 - 0x04];
    bool Active;                                                         // +0x78
    unsigned char _pad_0x79[0x9c - 0x79];
};

extern bool g_InitialFadeActiveMirror;                                   // 0x01375CDC (landed spelling)

#pragma auto_inline(off)
void CDisplayEngine::EndScreenFadeOut()                                  // 0x00434C90 (landed body, same TU)
{
    InitialFadeActive = false;
    g_InitialFadeActiveMirror = false;
    if (ScreenFadeOutInfo.Active && !ScreenFadeOutLocked) {
        ScreenFadeDuration = -1.0f;
        TimeSinceFadeStarted = 0.0f;
        ScreenFadeOutInfo.Opening = false;
        ScreenFadeOutInfo.Closing = true;
    }
}
#pragma auto_inline(on)

void CDisplayEngine::DoRender(const CInterpolationInfo* info, bool world_updated, bool render)
{
    if (InitialFadeReadynessCount > 0)
        SetAboutToRun();
    else if (IsInitialFadeActive() && GetScreenFadeOutAlpha() < 0x80)
        InitialFadeActive = false;
    MainWindow.Initialise(0.0f, 0.0f, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    GFFPUSetRoundingNearest();
    g_LightsDrawnThisFrame = 0;
    CCamera camera(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f), 0.25f, false);
    GPRenderManager->SetFogEnable(false);
    GPRenderManager->SetVWindow(MainWindow);
    C2DBoxF window = MainWindow;
    C2DBoxF television_area = GPDisplayManager->GetVVisibleTelevisionScreenArea();
    g_013B8A0C = 0;
    g_013B8A10 = 0;
    GPDisplayManager->BeginRender();
    GPRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 0), 0);
    if (IsRenderingNormalScene()) {
        CTBaseSingleton<CEngineManager>::Get().OnRender();
        if (NUISystem::CManager::GetInstance()->HasErrorMessage())
            NUISystem::CManager::GetInstance()->RenderErrorMessage(*Engine3D);
        if (!g_RenderThrottleEnabled_013b860c || g_RenderThrottlePaused_013b8629 || !GPInputManager->IsLoadingEvents() || g_RenderFrame_013b7d70 % 15 == 0) {
            if (GetViewManager().GetCurrentView()) {
                GetViewManager().GetCurrentView()->SetVWindow(window);
                GetViewManager().GetCurrentView()->Render(*info, world_updated);
            }
        }
    }
    GFGetSystemManager()->GetProfileManager()->DrawProfiles(MainWindow, PeekDebugFont());
    const CPlayer* player = World->PeekMyPlayer();
    if (player->IsDrawingFreeCamDebug() && player->GetCurrentMode() == 0x13)
        player->DrawFreeCamDebug(&MainWindow, PeekDebugFont());
    if (IsLetterBoxMode())
        LetterBox.Update(info->Current.GTPredictedTimeSinceLastRenderFrame);
    RenderHeroFuckShit(*info);
    RenderHeroDrunkenness(*info);
    if (ScreenFadeDuration > -DELTA_NOTIONAL_ZERO && !CPlayerGui::Get().GetSubtitleRenderer().IsWaitingForClickPast()) {
        if (TimeSinceFadeStarted <= ScreenFadeDuration)
            TimeSinceFadeStarted += info->Current.GTPredictedTimeSinceLastRenderFrame;
        if (TimeSinceFadeStarted > ScreenFadeDuration)
            EndScreenFadeOut();
    }
    if (IsScreenFadeOut()) {
        float time = 0.0f;
        if (!InitialFadeReadynessCount)
            time = info->Current.GTPredictedTimeSinceLastRenderFrame;
        ScreenFadeOutInfo.Update(time);
    }
    unsigned char white = 0xff;
    if (g_DebugMapDrawLevel >= 0) {
        CCamera render_camera = World->GetCurrentRenderCamera(*info);
        const C3DVector& pos = render_camera.PeekWorldPos();
        World->PeekWorldMap().GetThingMapManager().DebugDraw(pos.ToC2DCoordI(), g_DebugMapDrawLevel);
        std::vector<CThing*> things;
        World->PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(pos.ToC2DCoordI(), 1), CThingFilter_OverlappingCircle(pos, 0.5f), things);
        long i = 0;
        unsigned long count = things.size();
        for (; i < count; ++i) {
            const CThing* thing = things[i];
            if (thing->HasBaseDef())
                GPRenderManager->Draw3DScaledText(C3DVector(VIRTUAL_SCREEN_WIDTH * 0.45f, GFAYToVY(45.0f) + GFAYToVY(16.0f) * i, 0.0f), 0.7f,
                                                 thing->PeekBaseDef().GetInstantiationName().ToWideString(), PeekDebugFont(), CRGBColour(white, white, white, white), 0);
        }
    }
    if (g_RenderThrottlePaused_013b8629 && !GPInputManager->IsLoadingEvents())
        GPRenderManager->Draw2DText(C2DVector(VIRTUAL_SCREEN_WIDTH * 0.5, VIRTUAL_SCREEN_HEIGHT * 0.5), L"Recording!", PeekMainFont(), CRGBColour(white, white, white, white), 0);
    if (g_DrawStatsText && Component->DebugNoFramesUnableToRender) {
        GPRenderManager->Draw2DText(C2DVector(VIRTUAL_SCREEN_WIDTH * 0.5, VIRTUAL_SCREEN_HEIGHT * 0.5), L"Frames behind " + GFIntToWideString(Component->DebugNoFramesUnableToRender), PeekMainFont(), CRGBColour(white, white, white, white), 0);
    }
    if (g_RenderThrottleEnabled_013b860c && !g_RenderThrottlePaused_013b8629 && GPInputManager->IsLoadingEvents())
        GPRenderManager->Draw2DText(C2DVector(VIRTUAL_SCREEN_WIDTH * 0.5, VIRTUAL_SCREEN_HEIGHT * 0.5), L"Skipping unrecorded frames!", PeekMainFont(), CRGBColour(white, white, white, white), 0);
    if (CConsole::Get().IsActive())
        CConsole::Get().Draw(C2DBoxF(0.0f, 0.0f, VIRTUAL_SCREEN_WIDTH, (VIRTUAL_SCREEN_HEIGHT * 5.0) / 12.0));
    if (g_DrawBuildVersion) {
        GPRenderManager->Draw2DText(C2DVector(VIRTUAL_SCREEN_WIDTH * 0.1, VIRTUAL_SCREEN_HEIGHT - 2.0 * PeekMainFont().GetVFontMaxHeight()),
                                   GVersionString.ToWideString(), PeekMainFont(), CRGBColour(white, white, white, white), 0);
    }
    GPRenderManager->SetTextureMipMapMode(0, TEXTURE_MIPMAP_LINEAR);
    GPRenderManager->DrawBufferedPolys();
    GPRenderManager->Render2DDrawList(true);
    GPDisplayManager->EndRender();
    if (g_ScreenshotMode) {
        if (g_RenderThrottlePaused_013b8629 && GPInputManager->IsLoadingEvents()) {
            CWideString filename(L"data\\movies\\shot");
            CWideString number = GFIntToWideString(Frame);
            for (long i = 0; i < 6 - number.GetLength(); ++i)
                filename += L"0";
            filename += number;
            filename += L".tga";
            C2DExtentsI extents(640, 480);
            GPDisplayManager->GetBackBuffer().SaveAsTGA(filename, &extents, false);
        }
        else if (g_ScreenshotCount > 0) {
            CWideString filename(L"wishworldshot");
            CWideString number = GFIntToWideString(Frame);
            for (long i = 0; i < 6 - number.GetLength(); ++i)
                filename += L"0";
            filename += number;
            filename += L".tga";
            GPDisplayManager->GetBackBuffer().SaveAsTGA(filename, 0, false);
            --g_ScreenshotCount;
        }
    }
    if (OldGammaRamp != GammaRamp) {
        OldGammaRamp = GammaRamp;
        GPDisplayManager->SetGamma(GammaRamp, false);
    }
    if (render)
        GPDisplayManager->SwapScreens();
    ++Frame;
}