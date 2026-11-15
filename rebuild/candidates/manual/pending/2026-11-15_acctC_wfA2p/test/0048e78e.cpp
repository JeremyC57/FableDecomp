// Behaviour test for 0x0048E78E CSubtitleRenderer::DrawButtonGraphics: links the ROW OBJECT (no copy of the row here; the row's
// same-TU callees IsCutsceneSkippable / GetTextWidthFromText / IsTextReadyForClickPast come from it too) and drives it through raw
// object storage, stub callees and fake vtables (CIEngine slot 23 SetPersistentPrimitive, CGraphicDataBank slot 23 GetGraphicMaxWidth).
// Checks: the pulse timer is opened inactive (0.6 s, 0.3 s in cutscene action mode) and closed when open; the button positions
// (pos.X + 16 - GFVXToAX(width) chained right-to-left with the gap, Y offset in / out of the text box, graphic offset, minus half the
// pulse growth) and box extents ((pulse + 1) * graphic width); flag 4 draws cancel / confirm / third sprites on handles 0 / 1 / 2 with
// layer M2 under flag 0x800; action mode draws the huge mouse sprite (handle 1, position sprite) and the left sprite (handle 0, layer 1);
// the in-cutscene arm draws nothing and returns IsCutsceneSkippable() || Button2TextOverridden; flag 2 draws confirm on handle 0
// only when the text is ready (or flag 0x48); the last arm draws each non-empty text's button when the main player can respawn;
// the override tail (button 1 not drawn) places the confirm sprite left of button 1, or at the fishing reel anchor clamped to the
// render target.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FORCEINLINE_RELEASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPEN_OUTOFLINE
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISPERFORMINGREGIONLOADREALLY
#define FABLE_RETAIL_CPLAYER_RESPAWNDELAY
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CFontBank.h"
#include "engine/CFontManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"


float __fastcall GFVXToAX(float x) { return x * 2.0f; }
namespace NPlayerGui { C2DVector __fastcall GFAnchorToTopRight(const C2DVector& pos) { return C2DVector(pos.X + 1000.0f, pos.Y + 7.0f); } }
const float DELTA_NOTIONAL_ZERO = 0.0001f;

static double g_comp[0x80 / 8], g_world[0x200 / 8], g_pm[8], g_player[0x80 / 8], g_gui[0x400 / 8], g_bank[4], g_quest[8];
static double g_def[(sizeof(CPlayerGuiDef) + 15) / 8], g_r[(sizeof(CSubtitleRenderer) + 15) / 8], g_info[0x80 / 8], g_eng[4];
static double g_fontMgr[4], g_dispMgr[4], g_font[4];
static void* g_engVtbl[80];
static void* g_bankVtbl[80];
static unsigned char g_h[3][8];
static void* g_vec[3] = { g_h[0], g_h[0] + 24, g_h[0] + 24 };
static int g_mini; static long g_renderX = 1280;
static CCPPointerInfo g_fontInfo(1000, 0, g_font);

CMainGameComponent* CMainGameComponent::PMainGameComponent = reinterpret_cast<CMainGameComponent*>(g_comp);
static CCountedPointer<CPlayerGui> MakeGui() { CCountedPointer<CPlayerGui> p; p.Data = reinterpret_cast<CPlayerGui*>(g_gui); return p; }
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass(MakeGui());
CFontManager* PFontManager = reinterpret_cast<CFontManager*>(g_fontMgr);
CDisplayManager* GPDisplayManager = reinterpret_cast<CDisplayManager*>(g_dispMgr);

// text ids: the CWideString data pointer is (1..4) -> width 10 * id; null = empty string
bool CWideString::operator!=(const unsigned short* text) const { return PStringData != 0 || (text && text[0]); }
static unsigned long Id(const CWideString& s) { unsigned long v; memcpy(&v, &s, 4); return v; }
static void SetId(CWideString& s, unsigned long v) { memcpy(&s, &v, 4); }
float CFontBank::GetVStringWidth(const CWideString& text) const { return 10.0f * (float)Id(text); }
CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString&)
{ CCountedPointer<CFontBank> p; p.Data = reinterpret_cast<CFontBank*>(g_font); p.Info = &g_fontInfo; ++g_fontInfo.RefCount; return p; }
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { C2DExtentsI e; e.X = g_renderX; e.Y = 960; return e; }
CPlayer& CPlayerManager::GetMainPlayer() { return *reinterpret_cast<CPlayer*>(g_player); }
EMiniGameType NPlayerGui::CDrawQuestInfo::GetMiniGameType() const { return (EMiniGameType)g_mini; }
bool CFadeInFadeOutBase::IsOpen() const { return Active && !Opening && !Closing; }
const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef() { return *reinterpret_cast<const CPlayerGuiDef*>(g_def); }

struct Spr { int handle; float tlx, tly, brx, bry; long layer; unsigned long bank, frame, flags; unsigned char colour[4]; bool ispos; long pflags; };
static Spr g_spr[8]; static int g_nspr;
struct FakeEng {
    void SetPersistent(CEnginePrimitiveHandle& h, const CEnginePrimitive2DSprite& s, long flags)
    {
        Spr& o = g_spr[g_nspr++ & 7];
        o.handle = (int)(((unsigned char*)&h - g_h[0]) / 8); o.tlx = s.ClipBox.TLX; o.tly = s.ClipBox.TLY; o.brx = s.ClipBox.BRX; o.bry = s.ClipBox.BRY;
        o.layer = (long)s.Layer; o.bank = s.BankIndex; o.frame = s.Frame; o.flags = s.Flags; memcpy(o.colour, &s.Colour, 4); o.ispos = s.ClipBoxIsPos;
        o.pflags = flags;
    }
};
struct FakeBank { unsigned long GetMax(unsigned long graphic) const { return graphic == 77 ? 32 : 16; } };
template<class P> static void* PMF(P p) { void* v; memcpy(&v, &p, 4); return v; }
template<class T> static void SET(void* base, int off, T v) { memcpy((char*)base + off, &v, sizeof v); }

// DrawButtonGraphics is private: call it through a public look-alike bound to the row's symbol by the linker.
struct Caller { bool DrawButtonGraphics(const CEnginePrimitivePrepareInfo&, std::vector<CEnginePrimitiveHandle>&, const C2DVector&, bool); };
#pragma comment(linker, "/alternatename:?DrawButtonGraphics@Caller@@QAE_NABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DVector@@_N@Z=?DrawButtonGraphics@CSubtitleRenderer@@AAE_NABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DVector@@_N@Z")

static int fails;
static void Check(int ok, const char* what) { if (!ok) { ++fails; printf("FAIL %s\n", what); } }
static bool Near(float a, float b) { return fabs(a - b) < 1e-3f; }
static CSubtitleRenderer* R() { return reinterpret_cast<CSubtitleRenderer*>(g_r); }
static CPlayerGuiDef* D() { return reinterpret_cast<CPlayerGuiDef*>(g_def); }
static void Texts(int t1, int t2, int t3)
{
    SetId(R()->Button1Text, t1); SetId(R()->Button2Text, t2); SetId(R()->Button3Text, t3); SetId(R()->Button1OverrideText, 4);
}
static void Reset()
{
    memset(g_r, 0, sizeof g_r);
    R()->Button1Alpha = 200; R()->Button2Alpha = 150;
    // pulse timer opening half way: Update(0.15) -> OpenTimer 0.3 of 0.6 -> alpha 0.5, stays active and opening (no Open / Close)
    R()->ButtonPulseTimer.Active = true; R()->ButtonPulseTimer.Opening = true; R()->ButtonPulseTimer.FadeInTime = 0.6f;
    R()->ButtonPulseTimer.FadeOutTime = 0.6f; R()->ButtonPulseTimer.OpenTimer = 0.15f;
    Texts(1, 2, 3);
    g_nspr = 0; memset(g_spr, 0, sizeof g_spr); g_mini = 0; g_renderX = 1280;
    SET(g_player, 0x3c, (long)5);
    SET(g_world, 0x104, (long)0);
}
static bool Run(bool in_box)
{
    C2DVector pos(300.0f, 100.0f);
    return reinterpret_cast<Caller*>(g_r)->DrawButtonGraphics(*reinterpret_cast<const CEnginePrimitivePrepareInfo*>(g_info),
                                                               *reinterpret_cast<std::vector<CEnginePrimitiveHandle>*>(g_vec), pos, in_box);
}
static const float PULSE = 0.5f * (2.0f / 15.0f), HALF = PULSE * 16.0f * 0.5f, EXT = (PULSE + 1.0f) * 16.0f;
static bool Box(const Spr& s, float x, float y, float ext) { return Near(s.tlx, x) && Near(s.tly, y) && Near(s.brx, x + ext) && Near(s.bry, y + ext); }
static bool Look(const Spr& s, int handle, unsigned long bank, long layer, unsigned char alpha)
{ return s.handle == handle && s.bank == bank && s.layer == layer && s.colour[3] == alpha && s.colour[0] == 255 && s.flags == 2 && s.frame == 0 && s.pflags == 0xc0; }

int main()
{
    g_engVtbl[23] = PMF(&FakeEng::SetPersistent); SET(g_eng, 0, (void*)g_engVtbl); SET(g_info, 0x34, (void*)g_eng);
    g_bankVtbl[23] = PMF(&FakeBank::GetMax); SET(g_bank, 0, (void*)g_bankVtbl);
    SET(g_gui, 0x14, (void*)g_bank); SET(g_gui, 0x1c8, (void*)g_quest);
    SET(g_comp, 0x24, (void*)g_world); SET(g_comp, 0x1c, (void*)g_pm);
    SET(g_info, 0x00, 0.15f); SET(g_info, 0x10, 0.05f);          // Interpolate.Current / PausedCurrent predicted frame time
    D()->TextBoxButtonOffsetY = 5.0f; D()->TextBoxButtonInBoxOffsetY = 7.0f; D()->TextBoxButtonGapWidth = 3.0f;
    D()->TextBoxButtonGraphicOffset = C2DVector(1.0f, 2.0f); D()->TextBoxTextAreaBRPos = C2DVector(50.0f, 60.0f);
    D()->TextBoxCancelGraphic = 11; D()->TextBoxConfirmGraphic = 12; D()->TextBoxThirdGraphic = 13;
    D()->TextBoxHugeMouseButtonGraphic = 77; D()->TextBoxHugeMouseButtonLeftGraphic = 78;
    D()->TextBoxHugeButtonOffset = C2DVector(4.0f, 6.0f); D()->TextBoxHugeButtonLeftOffset = C2DVector(-8.0f, -9.0f);
    D()->TextBoxFishingReelTextPos = C2DVector(10.0f, 20.0f); D()->TextBoxFishingReelGraphicOffset = C2DVector(2.0f, 3.0f);
    // button 1 X = 300 + 16 - 2*10 = 296, button 2 = 296 - 2*20 - 3 = 253, button 3 = 253 - 2*30 - 3 = 190; + (1, 2); - HALF
    const float X1 = 297.0f - HALF, X2 = 254.0f - HALF, X3 = 191.0f - HALF, Y = 107.0f - HALF;

    // 1. flag 4: three sprites, layer 2; world paused reads PausedCurrent (0.05: OpenTimer 0.2 -> alpha 1/3)
    Reset(); R()->DisplayFlags = 4;
    Check(Run(false), "flag4 returns true");
    Check(g_nspr == 3, "flag4 three sprites");
    Check(Look(g_spr[0], 0, 11, ENGINE_2D_LAYER_2, 200) && Box(g_spr[0], X1, Y, EXT) && !g_spr[0].ispos, "flag4 cancel sprite");
    Check(Look(g_spr[1], 1, 12, ENGINE_2D_LAYER_2, 150) && Box(g_spr[1], X2, Y, EXT), "flag4 confirm sprite");
    Check(Look(g_spr[2], 2, 13, ENGINE_2D_LAYER_2, 255) && Box(g_spr[2], X3, Y, EXT), "flag4 third sprite");
    Check(Near(R()->ButtonPulseTimer.OpenTimer, 0.3f) && R()->ButtonPulseTimer.Opening, "timer updated with Current");
    // 1b. flag 0x800 -> layer M2, empty third text -> no third sprite, in the text box -> Y offset 7
    Reset(); R()->DisplayFlags = 0x804; Texts(1, 2, 0);
    Check(Run(true) && g_nspr == 2, "flag4 no third");
    Check(Look(g_spr[0], 0, 11, ENGINE_2D_LAYER_M2, 200) && Box(g_spr[0], X1, Y + 2.0f, EXT), "flag 0x800 layer M2 / in box Y");
    // 1c. paused world: PausedCurrent 0.05 -> OpenTimer 0.2
    Reset(); R()->DisplayFlags = 4; SET(g_world, 0xa4, true);
    Run(false); Check(Near(R()->ButtonPulseTimer.OpenTimer, 0.2f), "paused uses PausedCurrent");
    SET(g_world, 0xa4, false);

    // 2. inactive timer opens (0.6) -> alpha 0, no growth; an open timer is closed
    Reset(); R()->DisplayFlags = 4; R()->ButtonPulseTimer.Active = false; R()->ButtonPulseTimer.Opening = false;
    Run(false);
    Check(R()->ButtonPulseTimer.Active && R()->ButtonPulseTimer.Opening && Near(R()->ButtonPulseTimer.FadeInTime, 0.6f), "timer opened 0.6");
    Check(Box(g_spr[0], 297.0f, 107.0f, 16.0f), "no pulse growth after open");
    Reset(); R()->DisplayFlags = 4; R()->ButtonPulseTimer.Opening = false; R()->ButtonPulseTimer.CloseTimer = 0.3f;
    Run(false);
    Check(R()->ButtonPulseTimer.Closing && !R()->ButtonPulseTimer.Opening, "open timer closed");
    Check(Box(g_spr[0], 297.0f - 0.5f * (2.0f / 15.0f) * 8.0f, 107.0f - 0.5f * (2.0f / 15.0f) * 8.0f, (0.5f * (2.0f / 15.0f) + 1.0f) * 16.0f), "closing alpha");

    // 3. cutscene action mode: X1 = 1280 + 50 - 640 + 16 = 706; graphic 77 (width 32); huge mouse + left sprites; 0.3 s open
    Reset(); R()->CutsceneActionMode = true; R()->DisplayFlags = 4; R()->ButtonPulseTimer.Active = false; R()->ButtonPulseTimer.Opening = false;
    Check(Run(false), "action mode returns true");
    Check(Near(R()->ButtonPulseTimer.FadeInTime, 0.3f), "action mode timer 0.3");
    {
        const float x1 = 706.0f + 1.0f, x2 = 706.0f - 40.0f - 3.0f + 1.0f, y = 107.0f;   // alpha 0 after Open: no growth
        Check(g_nspr == 2, "action mode two sprites");
        Check(g_spr[0].handle == 1 && g_spr[0].ispos && g_spr[0].bank == 77 && g_spr[0].layer == ENGINE_2D_LAYER_2 && g_spr[0].colour[3] == 150
              && Near(g_spr[0].tlx, x2 + 4.0f) && Near(g_spr[0].tly, y + 6.0f), "huge mouse sprite");
        Check(Look(g_spr[1], 0, 78, ENGINE_2D_LAYER_1, 150) && Box(g_spr[1], x2 + 4.0f - 8.0f, y + 6.0f - 9.0f, 32.0f), "huge left sprite");
        (void)x1;
    }

    // 4. in cutscene, not skippable: nothing drawn, false; Button2TextOverridden -> true; flag 0x48 skips the arm
    Reset(); R()->InCutscene = true;
    Check(!Run(false) && g_nspr == 0, "in cutscene not skippable");
    Reset(); R()->InCutscene = true; R()->Button2TextOverridden = true;
    Check(Run(false) && g_nspr == 0, "in cutscene overridden");
    Reset(); R()->InCutscene = true; R()->CutsceneSkippable = true; R()->SkipCutsceneTimer.Active = true;
    Check(Run(false), "in cutscene skippable");
    Reset(); R()->InCutscene = true; R()->CutsceneSkippable = true; R()->SkipCutsceneTimer.Active = true; R()->SkipCutsceneTimer.Opening = true;
    Check(!Run(false), "skip prompt still opening");
    Reset(); R()->InCutscene = true; R()->CutsceneSkippable = true; R()->SkipCutsceneTimer.Active = true; SET(g_world, 0x104, (long)3);
    Check(!Run(false), "region load blocks skipping");

    // 5. flag 2: ready (TextTimer 0.5) -> confirm on handle 0; not ready -> nothing; flag 0x48 forces it
    Reset(); R()->DisplayFlags = 2; R()->TextTimer = 0.5f;
    Check(Run(false) && g_nspr == 1 && Look(g_spr[0], 0, 12, ENGINE_2D_LAYER_2, 200) && Box(g_spr[0], X1, Y, EXT), "flag2 ready");
    Reset(); R()->DisplayFlags = 2; R()->TextTimer = 0.1f;
    Check(!Run(false) && g_nspr == 0, "flag2 not ready");
    Reset(); R()->DisplayFlags = 0x4a; R()->TextTimer = 0.1f; R()->InCutscene = true;
    Check(Run(false) && g_nspr == 1, "flag2 forced by 0x48");

    // 6. last arm: respawn delay 0 -> each non-empty text; delay -> nothing
    Reset(); SET(g_player, 0x3c, (long)0); Texts(0, 2, 3);
    Check(Run(false) && g_nspr == 2, "last arm two buttons");
    Check(Look(g_spr[0], 1, 12, ENGINE_2D_LAYER_2, 150) && Look(g_spr[1], 2, 13, ENGINE_2D_LAYER_2, 255), "last arm confirm / third");
    Reset();
    Check(!Run(false) && g_nspr == 0, "last arm respawning");

    // 7. override tail: button 2 X = X1 - 2*40 - 3; fishing: anchor (1010, 27) + (2, 3) - HALF, clamped to the render target
    Reset(); R()->Button1TextOverridden = true;
    Check(Run(false) && g_nspr == 1 && Look(g_spr[0], 0, 12, ENGINE_2D_LAYER_2, 200) && Box(g_spr[0], X1 - 83.0f, Y, EXT), "override tail");
    Reset(); R()->Button1TextOverridden = true; g_mini = MINIGAME_FISHING;
    Run(false); Check(Box(g_spr[0], 1012.0f - HALF, 30.0f - HALF, EXT), "fishing anchor");
    Reset(); R()->Button1TextOverridden = true; g_mini = MINIGAME_FISHING; g_renderX = 1000;
    Run(false); Check(Box(g_spr[0], 1012.0f - HALF - (1012.0f - HALF + 80.0f - 1000.0f + 4.0f), 30.0f - HALF, EXT), "fishing clamp");
    Reset(); R()->Button1TextOverridden = true; R()->DisplayFlags = 4;
    Run(false); Check(g_nspr == 3, "override skipped when button 1 drawn");

    if (!fails) printf("0048e78e_TEST PASS\n");
    return fails;
}
