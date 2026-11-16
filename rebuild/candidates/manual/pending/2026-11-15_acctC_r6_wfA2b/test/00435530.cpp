// Behaviour test for 0x00435530 CDisplayEngine::DoRender: links the ROW OBJECT (DoRender, and the same-TU EndScreenFadeOut it
// defines) and drives it through stub callees, raw object storage and fake vtables (CFontBank slot 2 GetFontMaxHeight, the
// NDisplayView::CViewBase slots 2 / 3). The debug-map branch is off (g_DebugMapDrawLevel = -1). Checks, over three frames:
// SetAboutToRun only while InitialFadeReadynessCount > 0, otherwise the initial fade ends when the fade alpha drops below 0x80;
// MainWindow = (0, 0, W, H); BeginRender / ClearRenderTarget / DrawBufferedPolys / Render2DDrawList / EndRender every frame; the
// normal scene (DrawGame) renders the engine manager and the current view (SetVWindow + Render with world_updated); the profile
// overlay gets the debug font; the screen fade accumulates the frame time and ends (EndScreenFadeOut) past its duration, unless
// the subtitle waits for a click; the throttle texts (Recording / Frames behind <n> / Skipping) follow their flag tests; the
// console draws into (0, 0, W, H * 5 / 12); the build version is drawn at (W * 0.1, H - 2 * GFAYToVY(font max height)) with the
// main font and opaque white; screenshots: the movie shot "data\movies\shot" + zero padding to 6 digits + Frame + ".tga" at 640x480
// while recording, else the wish-world shot counts down; the gamma ramp is re-applied only when it changed; SwapScreens only when
// render; Frame increments.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include <math.h>
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
extern CRenderManager* PRenderManager;                                   // 0x013B8384
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


// ---- logging -------------------------------------------------------------------------------------------------------------
static char g_log[4096];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static int fails;
static void Check(int ok, const char* what) { if (!ok) { ++fails; printf("FAIL %s\n", what); } }
static bool Near(double a, double b) { return fabs(a - b) < 1e-3; }
static unsigned long ColInt(const CRGBColour& c) { unsigned long v; memcpy(&v, &c, 4); return v; }
template<class T> static void SET(void* base, int off, T v) { memcpy((char*)base + off, &v, sizeof v); }
template<class T> static T GET(const void* base, int off) { T v; memcpy(&v, (const char*)base + off, sizeof v); return v; }

// ---- wide strings: the 4-byte CWideString holds a pointer to a test buffer ------------------------------------------------
struct TS { unsigned short s[128]; };
static TS g_pool[64]; static int g_npool;
static TS* NewTS() { TS* t = &g_pool[g_npool++ & 63]; memset(t, 0, sizeof *t); return t; }
static TS* P(const CWideString& w) { TS* t; memcpy(&t, &w, 4); return t; }
static void SetP(CWideString& w, TS* t) { memcpy(&w, &t, 4); }
static int Len(const unsigned short* s) { int n = 0; while (s && s[n]) ++n; return n; }
static void Cat(TS* t, const unsigned short* s) { int n = Len(t->s); for (int i = 0; s && s[i] && n < 127; ++i) t->s[n++] = s[i]; t->s[n] = 0; }
static void Narrow(const CWideString& w, char* out) { TS* t = P(w); int i = 0; if (t) for (; t->s[i]; ++i) out[i] = (char)t->s[i]; out[i] = 0; }
CWideString::CWideString() { SetP(*this, NewTS()); }
CWideString::CWideString(const CWideString& other) { TS* t = NewTS(); Cat(t, P(other)->s); SetP(*this, t); }
CWideString::CWideString(const unsigned short* text) { TS* t = NewTS(); Cat(t, text); SetP(*this, t); }
CWideString::~CWideString() { SetP(*this, 0); }
const CWideString& CWideString::operator+=(const CWideString& other) { Cat(P(*this), P(other)->s); return *this; }
const CWideString& CWideString::operator+=(const unsigned short* text) { Cat(P(*this), text); return *this; }
long CWideString::GetLength() const { return Len(P(*this)->s); }
void CWideString::Sprintf(const unsigned short* format, ...)
{
    va_list a; va_start(a, format); long v = va_arg(a, long); va_end(a);
    char b[32]; sprintf(b, "%ld", v); TS* t = P(*this); t->s[0] = 0;
    unsigned short wb[32]; int i = 0; for (; b[i]; ++i) wb[i] = (unsigned short)b[i]; wb[i] = 0; Cat(t, wb);
    Check(format[0] == '%' && format[1] == 'd' && format[2] == 0, "GFIntToWideString formats %d");
}
CWideString FABLE_FASTCALL operator+(const unsigned short* text, const CWideString& other) { CWideString r(text); r += other; return r; }
CCharString::CCharString() { memset(this, 0, sizeof *this); }
CCharString::~CCharString() { }
static const unsigned short VERSION_TEXT[] = { 'V', '1', '.', '0', 0 };
CWideString CCharString::ToWideString() const { Check(this == &GVersionString, "ToWideString on GVersionString"); return CWideString(VERSION_TEXT); }
CCharString CDefStringTable::GetString(long) const { Check(0, "no def-string lookups (debug map off)"); return CCharString(); }

// ---- globals the row reads ----------------------------------------------------------------------------------------------
double VIRTUAL_SCREEN_WIDTH = 640.0;
double VIRTUAL_SCREEN_HEIGHT = 480.0;
const float DELTA_NOTIONAL_ZERO = 0.0001f;
static double g_rm[0x4000 / 8], g_dm[0x400 / 8], g_im[0x80 / 8], g_sys[0x100 / 8], g_comp[0x16200 / 8], g_world[0x400 / 8];
static double g_player[0x400 / 8], g_gui[0x2000 / 8], g_ui[0x200 / 8], g_vm[4], g_con[0xa0 / 8], g_em[0x40 / 8], g_eng3d[8];
CRenderManager* PRenderManager = reinterpret_cast<CRenderManager*>(g_rm);
CDisplayManager* GPDisplayManager = reinterpret_cast<CDisplayManager*>(g_dm);
CInputManager* GPInputManager = reinterpret_cast<CInputManager*>(g_im);
CCharString GVersionString;
CDefStringTable GDefStringTable;
long g_LightsDrawnThisFrame = 99, g_DebugMapDrawLevel = -1, g_013B8A0C = 5, g_013B8A10 = 6, g_ScreenshotCount;
unsigned char g_RenderThrottleEnabled_013b860c, g_RenderThrottlePaused_013b8629;
unsigned long g_RenderFrame_013b7d70;
bool g_DrawStatsText, g_DrawBuildVersion, g_ScreenshotMode, g_InitialFadeActiveMirror;
unsigned long CRenderStateManager::D3DTextureMipMapModeTable[3] = { 0, 1, 2 };
long NThingMap::g_NoMapwhoLevels;
CLayerInfo LayerInfo_0137dad8[1];
long SearchCount_013b89a8, SearchWork_013b89ac;
unsigned long SearchID_013b89b0;
static CCountedPointer<CConsole> MakeCon() { CCountedPointer<CConsole> p; p.Data = reinterpret_cast<CConsole*>(g_con); return p; }
static CCountedPointer<CEngineManager> MakeEm() { CCountedPointer<CEngineManager> p; p.Data = reinterpret_cast<CEngineManager*>(g_em); return p; }
static CCountedPointer<CPlayerGui> MakeGui() { CCountedPointer<CPlayerGui> p; p.Data = reinterpret_cast<CPlayerGui*>(g_gui); return p; }
template<> CCountedPointer<CConsole> CTBaseSingleton<CConsole>::PClass(MakeCon());
template<> CCountedPointer<CEngineManager> CTBaseSingleton<CEngineManager>::PClass(MakeEm());
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass(MakeGui());

// ---- fake polymorphic objects --------------------------------------------------------------------------------------------
static float g_fontHeight = 10.0f;
struct FakeFont { virtual void s0() { } virtual void s1() { } virtual float GetFontMaxHeight() const { return g_fontHeight; } };
static FakeFont g_mainFont, g_debugFont;
static bool g_worldUpdatedSeen; static C2DBoxF g_viewWindow;
void NDisplayView::CViewBase::OnInit() { }
void NDisplayView::CViewBase::OnUninit() { }
void NDisplayView::CViewBase::SetVWindow(const C2DBoxF& window) { g_viewWindow = window; Log("View:SetVWindow"); }
void NDisplayView::CViewBase::Render(const CInterpolationInfo&, bool world_updated) { g_worldUpdatedSeen = world_updated; Log("View:Render"); }
static NDisplayView::CViewBase g_view;

// ---- stub callees --------------------------------------------------------------------------------------------------------
float __fastcall GFAYToVY(float y) { return y * 0.5f; }
CSystemManager* __fastcall GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(g_sys); }
CCamera::CCamera(const C3DVector&, const C3DVector&, float fov, bool) { Check(fov == 0.25f, "camera fov"); }
CConsole::CConsole() { Check(0, "console already exists"); }
void CConsole::Draw(const C2DBoxF& box) { Log("Console:Draw"); Check(box.TLX == 0.0f && box.TLY == 0.0f && box.BRX == 640.0f && box.BRY == 200.0f, "console box"); }
CEngineManager::~CEngineManager() { }
template<> CTBaseSingleton<CEngineManager>::~CTBaseSingleton() { }
template<> CTBaseSingleton<CConsole>::~CTBaseSingleton() { }
void CEngineManager::OnRender() { Log("EngineManager:OnRender"); }
bool CThingFilter_OverlappingCircle::operator()(const CThing&) const { return false; }
bool CDisplayManager::BeginRender() { Log("BeginRender"); return true; }
void CDisplayManager::EndRender() { Log("EndRender"); }
bool CDisplayManager::SwapScreens() { Log("SwapScreens"); return true; }
static float g_gamma; static bool g_gammaFlag = true;
void CDisplayManager::SetGamma(float gamma, bool flag) { g_gamma = gamma; g_gammaFlag = flag; Log("SetGamma"); }
C2DBoxF CDisplayManager::GetVVisibleTelevisionScreenArea() const { return C2DBoxF(1.0f, 2.0f, 3.0f, 4.0f); }
static char g_shot[64]; static long g_shotW = -1, g_shotH = -1;
bool CSurface::SaveAsTGA(const CWideString& filename, const C2DExtentsI* extents, bool flag) const
{
    Narrow(filename, g_shot); g_shotW = extents ? extents->X : 0; g_shotH = extents ? extents->Y : 0; Check(!flag, "tga flag false");
    Check(this == &GPDisplayManager->GetBackBuffer(), "back buffer saved"); Log("SaveAsTGA"); return true;
}
void CRenderManager2D::ClearRenderTarget(const CRGBColour& colour, unsigned long flags) { Check(ColInt(colour) == 0 && flags == 0, "clear black"); Log("Clear"); }
static char g_text[8][64]; static float g_tx[8], g_ty[8]; static const CFontBank* g_tfont[8]; static unsigned long g_tcol[8]; static int g_ntext;
bool CRenderManager2D::Draw2DText(const C2DVector& pos, CWideString text, const CFontBank& font, const CRGBColour& colour, unsigned long flags)
{
    int i = g_ntext++ & 7; Narrow(text, g_text[i]); g_tx[i] = pos.X; g_ty[i] = pos.Y; g_tfont[i] = &font; g_tcol[i] = ColInt(colour);
    Check(flags == 0, "text flags 0"); Log("Draw2DText"); return true;
}
bool CRenderManager2D::Draw3DScaledText(const C3DVector&, float, CWideString, const CFontBank&, const CRGBColour&, unsigned long) { Check(0, "no 3D text (debug map off)"); return true; }
void CRenderManager2D::DrawBufferedPolys() { Log("DrawBufferedPolys"); }
void CRenderManager2D::Render2DDrawList(bool flag) { Check(flag, "draw list flag"); Log("Render2DDrawList"); }
static C2DBoxF g_rmWindow;
void CRenderManagerCore::SetVWindow(const C2DBoxF& window) { g_rmWindow = window; }
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&, const C2DBoxI& box) const { Check(0, "no search"); return box; }
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI&, std::vector<NThingMap::CThingMap*>&) const { Check(0, "no search"); }
void NThingMap::CThingMapManager::DebugDraw(const C2DCoordI&, long) const { Check(0, "no map debug draw"); }
static const CFontBank* g_profileFont;
void CProfileManager::DrawProfiles(const C2DBoxF& window, const CFontBank& font)
{ g_profileFont = &font; Check(this == reinterpret_cast<CProfileManager*>(&g_vm[2]) && window.BRX == 640.0f, "profiles: manager + main window"); Log("DrawProfiles"); }
void CPlayer::DrawFreeCamDebug(const C2DBoxF*, const CFontBank&) const { Log("FreeCamDebug"); }
static int g_mode;
EPlayerMode CPlayer::GetCurrentMode() const { return (EPlayerMode)g_mode; }
const CPlayer* CWorld::PeekMyPlayer() const { return reinterpret_cast<const CPlayer*>(g_player); }
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return reinterpret_cast<NUISystem::CManager*>(g_ui); }
void NUISystem::CManager::RenderErrorMessage(CIEngine&) { Log("ErrorMessage"); }
static bool g_waitClick;
bool CSubtitleRenderer::IsWaitingForClickPast() const { return g_waitClick; }
static int g_aboutToRun;
void CDisplayEngine::SetAboutToRun() { ++g_aboutToRun; Log("SetAboutToRun"); }
void CDisplayEngine::RenderHeroFuckShit(const CInterpolationInfo&) const { Log("HeroFuckShit"); }
void CDisplayEngine::RenderHeroDrunkenness(const CInterpolationInfo&) const { Log("HeroDrunkenness"); }

// DoRender is protected: call it through a public look-alike bound to the row's symbol by the linker.
struct Caller { void DoRender(const CInterpolationInfo* info, bool world_updated, bool render); };
#pragma comment(linker, "/alternatename:?DoRender@Caller@@QAEXPBVCInterpolationInfo@@_N1@Z=?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z")

static double g_e[0x200 / 8], g_info[0x80 / 8];
static CDisplayEngine* E() { return reinterpret_cast<CDisplayEngine*>(g_e); }
static CInterpolationInfo* I() { return reinterpret_cast<CInterpolationInfo*>(g_info); }
static void Frame(bool world_updated, bool render)
{
    g_log[0] = 0; g_ntext = 0; g_npool = 0; g_shot[0] = 0; memset(g_text, 0, sizeof g_text);
    reinterpret_cast<Caller*>(g_e)->DoRender(I(), world_updated, render);
}
static int Has(const char* s) { return strstr(g_log, s) != 0; }

int main()
{
    // wiring
    SET(g_comp, 0x1614c, (void*)&g_mainFont); SET(g_comp, 0x16144, (void*)&g_debugFont);
    SET(g_sys, 0xf8, (void*)&g_vm[2]);              // ProfileManager.Data +0xF8 (retail 0x00435779; private in the header)
    SET(g_vm, 0, (void*)&g_view);                   // CDisplayViewManager::PCurrentView_Data
    SET(g_player, 0x08, false);                     // DrawingFreeCamDebug
    E()->Component = reinterpret_cast<CMainGameComponent*>(g_comp);
    E()->World = reinterpret_cast<CWorld*>(g_world);
    E()->Engine3D = reinterpret_cast<CIEngine*>(g_eng3d);
    SET(g_e, 0xe0, (void*)g_vm);                    // PViewManager.Data
    E()->Frame = 7; E()->GammaRamp = 2.0f; E()->OldGammaRamp = 1.0f;
    E()->InitialFadeReadynessCount = 1; E()->InitialFadeActive = true;
    E()->ScreenFadeDuration = 1.0f; E()->TimeSinceFadeStarted = 0.5f;
    I()->Current.GTPredictedTimeSinceLastRenderFrame = 0.25f;

    // frame 1: no normal scene; readyness count -> SetAboutToRun; fade 0.5 -> 0.75 (still running); build version; wish-world shot
    g_DrawBuildVersion = true; g_ScreenshotMode = true; g_ScreenshotCount = 2; g_fontHeight = 10.0f;
    Frame(true, true);
    Check(g_aboutToRun == 1 && E()->InitialFadeActive, "frame 1: SetAboutToRun, initial fade kept");
    Check(E()->MainWindow.TLX == 0.0f && E()->MainWindow.BRX == 640.0f && E()->MainWindow.BRY == 480.0f, "main window");
    Check(g_rmWindow.BRX == 640.0f && g_LightsDrawnThisFrame == 0 && g_013B8A0C == 0 && g_013B8A10 == 0, "frame setup");
    Check(!Has("EngineManager") && !Has("View:"), "frame 1: no normal scene");
    Check(Near(E()->TimeSinceFadeStarted, 0.75f) && E()->ScreenFadeDuration == 1.0f, "frame 1: fade accumulates");
    Check(g_ntext == 1 && strcmp(g_text[0], "V1.0") == 0, "frame 1: build version text");
    Check(Near(g_tx[0], 64.0f) && Near(g_ty[0], 470.0f), "frame 1: build version at (W*0.1, H - 2*VY(height))");
    Check(g_tfont[0] == reinterpret_cast<const CFontBank*>(&g_mainFont) && g_tcol[0] == 0xffffffff, "frame 1: main font, white");
    Check(g_profileFont == reinterpret_cast<const CFontBank*>(&g_debugFont), "profiles use the debug font");
    Check(strcmp(g_shot, "wishworldshot000007.tga") == 0 && g_shotW == 0 && g_ScreenshotCount == 1, "frame 1: wish-world shot");
    Check(Has("SetGamma") && g_gamma == 2.0f && !g_gammaFlag && E()->OldGammaRamp == 2.0f, "frame 1: gamma applied");
    Check(Has("SwapScreens") && E()->Frame == 8, "frame 1: swap + frame");
    Check(strstr(g_log, "BeginRender Clear DrawProfiles HeroFuckShit HeroDrunkenness Draw2DText DrawBufferedPolys Render2DDrawList EndRender SaveAsTGA SetGamma SwapScreens") != 0, "frame 1: order");

    // frame 2: normal scene; fade alpha 0 < 0x80 ends the initial fade; fade passes its duration -> EndScreenFadeOut; recording
    // (paused, not loading) + stats text; console; no build version; not loading -> the last wish-world shot; no gamma, no swap
    E()->InitialFadeReadynessCount = 0; E()->DrawGame = true; g_DrawBuildVersion = false;
    E()->ScreenFadeOutInfo.Active = true; E()->TimeSinceFadeStarted = 0.9f; E()->ScreenFadeDuration = 1.0f;
    g_RenderThrottleEnabled_013b860c = 0; g_RenderThrottlePaused_013b8629 = 1; SET(g_im, 0x38, false);
    g_DrawStatsText = true; SET(g_comp, 0x161e4, (long)3); SET(g_con, 0x78, true); g_InitialFadeActiveMirror = true;
    Frame(false, false);
    Check(g_aboutToRun == 1 && !E()->InitialFadeActive, "frame 2: initial fade ended by alpha");
    Check(Has("EngineManager:OnRender") && Has("View:SetVWindow View:Render") && !g_worldUpdatedSeen, "frame 2: view rendered");
    Check(g_viewWindow.BRX == 640.0f && g_viewWindow.BRY == 480.0f, "frame 2: view window = main window");
    Check(E()->ScreenFadeDuration == -1.0f && E()->TimeSinceFadeStarted == 0.0f && !g_InitialFadeActiveMirror, "frame 2: fade ended");
    Check(g_ntext == 2 && strcmp(g_text[0], "Recording!") == 0 && strcmp(g_text[1], "Frames behind 3") == 0, "frame 2: texts");
    Check(Near(g_tx[1], 320.0f) && Near(g_ty[1], 240.0f), "frame 2: text centred");
    Check(Has("Console:Draw"), "frame 2: console drawn");
    Check(strcmp(g_shot, "wishworldshot000008.tga") == 0 && g_ScreenshotCount == 0, "frame 2: not loading -> wish-world shot counts down");
    Check(!Has("SetGamma") && !Has("SwapScreens") && E()->Frame == 9, "frame 2: no gamma change, no swap, frame");

    // frame 3: throttled (enabled, not paused, loading, frame % 15 != 0) -> no view; skipping text; gamma changed again
    g_RenderThrottleEnabled_013b860c = 1; g_RenderThrottlePaused_013b8629 = 0; SET(g_im, 0x38, true); g_RenderFrame_013b7d70 = 4;
    g_DrawStatsText = false; SET(g_con, 0x78, false); E()->GammaRamp = 3.0f;
    Frame(true, true);
    Check(Has("EngineManager:OnRender") && !Has("View:"), "frame 3: throttled view skipped");
    Check(g_ntext == 1 && strcmp(g_text[0], "Skipping unrecorded frames!") == 0, "frame 3: skipping text");
    Check(Has("SetGamma") && g_gamma == 3.0f && Has("SwapScreens") && !Has("SaveAsTGA") && g_ScreenshotCount == 0, "frame 3: tail (no shots left)");

    // frame 4: recording (paused) while loading -> movie shot "data\movies\shot000010.tga" at 640x480; view drawn again
    g_RenderThrottlePaused_013b8629 = 1; E()->Frame = 10;
    Frame(true, true);
    Check(Has("View:Render") && g_worldUpdatedSeen && g_ntext == 0, "frame 4: view, no texts");
    Check(strcmp(g_shot, "data\\movies\\shot000010.tga") == 0 && g_shotW == 640 && g_shotH == 480 && g_ScreenshotCount == 0, "frame 4: movie shot");

    // frame 5: the subtitle waits for a click -> the fade clock stands still
    E()->ScreenFadeDuration = 1.0f; E()->TimeSinceFadeStarted = 0.5f; g_waitClick = true; g_ScreenshotMode = false;
    Frame(true, true);
    Check(E()->TimeSinceFadeStarted == 0.5f, "frame 5: fade waits for the subtitle click");

    if (fails) printf("%s\n", g_log);
    printf(fails ? "00435530_TEST FAIL\n" : "00435530_TEST PASS\n");
    return fails ? 1 : 0;
}
