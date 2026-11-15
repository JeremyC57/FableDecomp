// Behaviour test for 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine: links the ROW OBJECT (no copy of the row here) and drives
// it through stub callees, raw object storage and fake vtables (CIEngine slots 23 / 67, CGameScriptInterface slot 395).
// Checks, over five calls: the gamma ramp is applied from the profile on the 2nd call only (frame counter <= 2 && ++ == 2); Alpha =
// the profile HUD alpha (old / new profile values); the gui lighting gets DiffuseVector, ambient, diffuse, backlight converted to
// floats; the timers get Interpolate.Current or PausedCurrent (World paused); every illustrator in Illustrators is cleared; the
// mini map always updates; the subtitle renderer draws when GuiMode != 1 or ForceAllowSubtitleToRender; GuiMode != 1 draws the
// screen fade and, in letterbox mode, the black bars; the drawHud chain stops at the first true condition (movie sequence) and asks
// the player mode last; a live first-person hero draws the targeting cursor in the pickpocket mini game and clears it otherwise;
// the 2D text always draws; the five ClearEffects run only for illustrators not drawing; the debug TGA sprite is centred
// ((AX(W) - width) / 2, (AY(H) - height) / 2, texture = DebugTGATexture, flags 2, white, ENGINE_2D_LAYER_0) and sent to SetPersistentPrimitive
// with 0x40, else the handle is cleared; for_each gets the Illustrators range.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_MEM_FUN_PTR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CHILD
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_ISMULTIPLAYERGAMEACTIVE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_SETGAMMARAMP
#define FABLE_RETAIL_CDISPLAYENGINE_ISLETTERBOXMODE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_CRIME
#define FABLE_RETAIL_CPLAYERGUI_DRAWPREPAREPRIMITIVES
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_DRAWPREPAREPRIMITIVES
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_POS_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_TEXTURE_POS_CTOR_BODY
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_WATERDYNAMIC
#define FABLE_RETAIL_CTEXTURE_GETWIDTH
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_GAMMA
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_DRAW
#define FABLE_RETAIL_NPLAYERGUI_CDRAWHIGHLIGHTINGTARGETINGCURSOR_CLEAR
#define FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
#define FABLE_RETAIL_CDRAWQUESTINFO_DRAW
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_algorithm.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuickAccessMenu.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/EPlayerMode.h"
#include "engine/CUserProfileManager.h"
#include "engine/CInterpolationInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CTexture.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CStatBar.h"
#include "engine/NPlayerGui_CCrimeStatBar.h"
#include "engine/NPlayerGui_CDrawDamage.h"
#include "engine/NPlayerGui_CDrawNPCName.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/NPlayerGui_CDrawHighlightingTargetingCursor.h"
#include "engine/NPlayerGui_CDrawSpiritTargetingCursor.h"
#include "engine/CDrawMiniMap.h"

// Ego_r.pdb / FableWin.pdb ?GFVXToAX@@YIMM@Z / ?GFVYToAY@@YIMM@Z (retail 0x009E1E10 / 0x009E1E40, landed) and the virtual screen
// size doubles (0x013961E8 / 0x013961F0, rebuild/oracles/reloc_bindings.tsv VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT).
float __fastcall GFVXToAX(float x);
float __fastcall GFVYToAY(float y);
extern double VIRTUAL_SCREEN_WIDTH;
extern double VIRTUAL_SCREEN_HEIGHT;

// NPlayerGui illustrators with no shared header (CDrawMiniMap comes from engine/CDrawMiniMap.h, opt-in ..._CDRAWMINIMAP_CLASS): Ego_r.pdb names (RTTI .?AV<Class>@NPlayerGui@@ : CDrawBase); only the members the
// row calls (Draw out of line at the retail addresses noted, ClearEffects, the two virtual Draws at slot 2).
namespace NPlayerGui {
class CDrawHealth : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };   // 0x00647AB4 / 0x00644644
class CDrawStamina : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };  // 0x0064CCB3 / 0x0064464C
class CDrawCombatMultiplier : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };            // 0x0064822A
class CDrawSpiritsHealth : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };               // 0x0064E2CC
class CDrawScreenFade : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                  // 0x006495A8
class CDrawMoney : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                       // 0x006441F6
class CDrawRenown : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                      // 0x0064483D
class CDrawMorality : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                    // 0x006448A4
class CDrawExperience : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                  // 0x00646F67
class CDrawCrime : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                       // 0x006442AE
class CDrawReceivedItems : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };               // 0x0064E978
class CDrawFishingGame : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                 // 0x00649E8B
class CDrawStatUpdates : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                 // 0x00644334
class CDrawAutoPickups : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                 // 0x006443AC
class CDrawPerceivers : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                  // 0x0064499C
class CDrawGuildSeal : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                   // 0x0064A639
class CDrawQuickAccessIcons : public CDrawBase { public: virtual void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };   // slot 2 / 0x00645605
class CDrawScriptScreenText : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };            // 0x0064560D
class CDrawTargetInfo : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                  // 0x0064B8B3
class CDrawWorldTime : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                   // 0x0064CA15
class CDrawAbilityButtons : public CDrawBase { public: virtual void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };     // slot 2 / 0x0064528A
class CDrawLetterBoxBlackBars : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };          // 0x00644677
class CDrawProjectileTargetingCentre : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };   // 0x00647575
class CDrawMeleeActionButtonEffects : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };   // 0x00646580 / 0x00645821
class CDrawSpiritScores : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                // 0x00644424
class CDraw2DText : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                      // 0x006541D1
class CDrawMouseCursor : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };                 // 0x006571D0
}
using namespace NPlayerGui;

static char g_log[8192];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static bool g_lastBool, g_movie, g_child;
static int g_movieCalls, g_modeCalls, g_mini, g_mode;
static const void* g_timerSet;
static void* g_hero;
static NPlayerGui::CDrawBase** g_feFirst; static NPlayerGui::CDrawBase** g_feLast;
static float g_light[15];
static float g_spr[2]; static const void* g_sprTex; static const void* g_sprHandle; static unsigned long g_sprFlags; static long g_persistFlags, g_sprLayer;
static unsigned char g_sprColour[4]; static int g_persist;

float __fastcall GFVXToAX(float x) { return x * 2.0f; }
float __fastcall GFVYToAY(float y) { return y * 3.0f; }
double VIRTUAL_SCREEN_WIDTH = 640.0;
double VIRTUAL_SCREEN_HEIGHT = 480.0;
const float DELTA_NOTIONAL_ZERO = 0.0001f;
static double g_comp[0x80 / 8];
CMainGameComponent* CMainGameComponent::PMainGameComponent = reinterpret_cast<CMainGameComponent*>(g_comp);
CDefPointer<const CPlayerGuiDef> CPlayerGui::PGuiDef;
CCharString::CCharString(const char*, long) { }
CCharString::~CCharString() { }
template<> bool CDefinitionManager::GetDef<CPlayerGuiDef>(const CCharString&, CDefPointer<const CPlayerGuiDef>*) const { L("GetDef"); return true; }
namespace std {
template<> mem_fun_t<void, NPlayerGui::CDrawBase> FABLE_FASTCALL for_each(NPlayerGui::CDrawBase** first, NPlayerGui::CDrawBase** last,
                                                                         mem_fun_t<void, NPlayerGui::CDrawBase> f)
{ g_feFirst = first; g_feLast = last; return f; }
}
static double g_gdm[16];
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return *reinterpret_cast<CGameDefinitionManager*>(g_gdm); }
static double g_upm[0x100 / 8];
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *reinterpret_cast<CUserProfileManager*>(g_upm); }
static double g_player[64];
CPlayer& CPlayerManager::GetMainPlayer() { return *reinterpret_cast<CPlayer*>(g_player); }
EPlayerMode CPlayer::GetCurrentMode() const { ++g_modeCalls; return (EPlayerMode)g_mode; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return (CThingPlayerCreature*)g_hero; }
bool CThingCreatureBase::IsChild() const { return g_child; }
bool CPlayerManager::IsMultiplayerGameActive() const { return false; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return 0; }
void CTCQuickAccessMenu::Deactivate() { L("QADeactivate"); }
void CSubtitleRenderer::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&) { L("Subtitle"); }
EMiniGameType NPlayerGui::CDrawQuestInfo::GetMiniGameType() const { return (EMiniGameType)g_mini; }
C2DExtentsI CTexture::GetDimensions(unsigned long level) const { L("Dim"); C2DExtentsI e; e.X = 100 + (long)level; e.Y = 60; return e; }
void CPlayerGui::RenderUpdateGuiTimers(const CInterpolationInfo::CInterpolationInfoSet& set) const { g_timerSet = &set; L("Timers"); }
void CPlayerGui::SetCrimeAsDisplaying() { L("Crime"); }
void CPlayerGui::SetMoneyAsDisplaying() { L("Money"); }
void NPlayerGui::CDrawHighlightingTargetingCursor::Clear() { L("Clear:HighlightingTargetingCursor"); }
void NPlayerGui::CDrawAbilityButtons::ClearEffects() { L("ClearEffects:AbilityButtons"); }
void NPlayerGui::CDrawHealth::ClearEffects() { L("ClearEffects:Health"); }
void NPlayerGui::CDrawMeleeActionButtonEffects::ClearEffects() { L("ClearEffects:MeleeActionButtonEffects"); }
void NPlayerGui::CDrawQuickAccessIcons::ClearEffects() { L("ClearEffects:QuickAccessIcons"); }
void NPlayerGui::CDrawStamina::ClearEffects() { L("ClearEffects:Stamina"); }
void NPlayerGui::CDraw2DText::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:2DText"); }
void NPlayerGui::CDrawAutoPickups::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:AutoPickups"); }
void NPlayerGui::CDrawCombatMultiplier::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:CombatMultiplier"); }
void NPlayerGui::CDrawCrime::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Crime"); }
void NPlayerGui::CDrawDamage::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Damage"); }
void NPlayerGui::CDrawExperience::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Experience"); }
void NPlayerGui::CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:FishingGame"); }
void NPlayerGui::CDrawGuildSeal::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:GuildSeal"); }
void NPlayerGui::CDrawHealth::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Health"); }
void NPlayerGui::CDrawHighlightingTargetingCursor::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:HighlightingTargetingCursor"); }
void NPlayerGui::CDrawLetterBoxBlackBars::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:LetterBoxBlackBars"); }
void NPlayerGui::CDrawMeleeActionButtonEffects::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:MeleeActionButtonEffects"); }
void NPlayerGui::CDrawMiniMap::Draw(const CEnginePrimitivePrepareInfo&, bool b) { L("Draw:MiniMap"); g_lastBool = b; }
void NPlayerGui::CDrawMoney::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Money"); }
void NPlayerGui::CDrawMorality::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Morality"); }
void NPlayerGui::CDrawMouseCursor::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:MouseCursor"); }
void NPlayerGui::CDrawNPCName::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:NPCName"); }
void NPlayerGui::CDrawPerceivers::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Perceivers"); }
void NPlayerGui::CDrawProjectileTargetingCentre::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:ProjectileTargetingCentre"); }
void NPlayerGui::CDrawQuestInfo::Draw(const CEnginePrimitivePrepareInfo&, bool b) { L("Draw:QuestInfo"); g_lastBool = b; }
void NPlayerGui::CDrawReceivedItems::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:ReceivedItems"); }
void NPlayerGui::CDrawRenown::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Renown"); }
void NPlayerGui::CDrawScreenFade::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:ScreenFade"); }
void NPlayerGui::CDrawScriptScreenText::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:ScriptScreenText"); }
void NPlayerGui::CDrawSpiritScores::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:SpiritScores"); }
void NPlayerGui::CDrawSpiritTargetingCursor::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:SpiritTargetingCursor"); }
void NPlayerGui::CDrawSpiritsHealth::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:SpiritsHealth"); }
void NPlayerGui::CDrawStamina::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:Stamina"); }
void NPlayerGui::CDrawStatUpdates::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:StatUpdates"); }
void NPlayerGui::CDrawTargetInfo::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:TargetInfo"); }
void NPlayerGui::CDrawWorldTime::Draw(const CEnginePrimitivePrepareInfo&) { L("Draw:WorldTime"); }
void NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo(const CEnginePrimitivePrepareInfo&) { L("DrawMiniGameInfo:QuestInfo"); }
void NPlayerGui::CDrawMiniMap::Update(const CEnginePrimitivePrepareInfo&) { L("Update:MiniMap"); }

struct FakeSI { bool IsInMovieSequence() { ++g_movieCalls; return g_movie; } };
struct FakeEng {
    void Lighting(const C3DVector& v, const CRGBFloatColour& a, const CRGBFloatColour& d, const CRGBFloatColour& b)
    {
        const float* pv = reinterpret_cast<const float*>(&v); g_light[0] = pv[0]; g_light[1] = pv[1]; g_light[2] = pv[2];
        const CRGBFloatColour* c[3] = { &a, &d, &b };
        for (int i = 0; i < 3; ++i) { const float* f = reinterpret_cast<const float*>(c[i]); for (int k = 0; k < 4; ++k) g_light[3 + i * 4 + k] = f[k]; }
        L("Lighting");
    }
    void SetPersistent(CEnginePrimitiveHandle& h, const CEnginePrimitive2DSprite& s, long flags)
    {
        g_sprHandle = &h; g_spr[0] = s.ClipBox.TLX; g_spr[1] = s.ClipBox.TLY; g_sprTex = s.Texture; g_sprFlags = s.Flags;
        g_sprLayer = (long)s.Layer; memcpy(g_sprColour, &s.Colour, 4); g_persistFlags = flags; ++g_persist;
        if (s.ClipBox.BRX != s.ClipBox.TLX || s.ClipBox.BRY != s.ClipBox.TLY) g_persist += 100;
        L("Persistent");
    }
};
static void* g_siVtbl[400];
static void* g_engVtbl[80];
template<class P> static void* PMF(P p) { void* v; memcpy(&v, &p, 4); return v; }
template<class T> static void SET(void* base, int off, T v) { memcpy((char*)base + off, &v, sizeof v); }

static double g_gui[0x400 / 8], g_info[0x80 / 8], g_disp[0x200 / 8], g_world[0x100 / 8], g_si[4], g_eng[4], g_def[0x100 / 8];
static double g_heroMem[0x800 / 8], g_pm[8];
static double g_ill[40][0x200 / 8];
static NPlayerGui::CDrawBase* g_vec[3];
static const int ILL_OFF[] = { 0x140, 0x148, 0x150, 0x158, 0x160, 0x168, 0x170, 0x178, 0x180, 0x188, 0x190, 0x198, 0x1a0, 0x1a8, 0x1b0,
                               0x1b8, 0x1c0, 0x1c8, 0x1d0, 0x1d8, 0x1e0, 0x1e8, 0x1f0, 0x1f8, 0x200, 0x208, 0x210, 0x218, 0x220, 0x228,
                               0x230, 0x238, 0x240, 0x248 };
static bool Drawing(int off) { for (int i = 0; i < 34; ++i) if (ILL_OFF[i] == off) return ((unsigned char*)g_ill[i])[0x18] != 0; return false; }
static void SetDrawing(int off, bool on) { for (int i = 0; i < 34; ++i) if (ILL_OFF[i] == off) ((unsigned char*)g_ill[i])[0x18] = on; }
static int fails;
static void Check(int ok, const char* what) { if (!ok) { ++fails; printf("FAIL %s\n", what); } }
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }

static void Run(CPlayerGui* gui, const CEnginePrimitivePrepareInfo& info)
{
    g_log[0] = 0; g_movieCalls = g_modeCalls = g_persist = 0; g_timerSet = 0; g_feFirst = g_feLast = 0;
    for (int i = 0; i < 3; ++i) ((unsigned char*)g_vec[i])[0x18] = 1;
    gui->DrawPreparePrimitivesForEngine(info);
}

int main()
{
    CPlayerGui* gui = reinterpret_cast<CPlayerGui*>(g_gui);
    const CEnginePrimitivePrepareInfo& info = *reinterpret_cast<const CEnginePrimitivePrepareInfo*>(g_info);
    g_siVtbl[395] = PMF(&FakeSI::IsInMovieSequence);
    g_engVtbl[67] = PMF(&FakeEng::Lighting);
    g_engVtbl[23] = PMF(&FakeEng::SetPersistent);
    SET(g_si, 0, (void*)g_siVtbl); SET(g_eng, 0, (void*)g_engVtbl);
    SET(g_info, 0x34, (void*)g_eng);
    SET(g_world, 0x38, (void*)g_si);
    SET(g_comp, 0x28, (void*)g_disp);
    SET(g_gui, 0x04, (void*)g_world); SET(g_gui, 0x08, (void*)g_pm); SET(g_gui, 0x10, (void*)g_comp);
    for (int i = 0; i < 34; ++i) SET(g_gui, ILL_OFF[i], (void*)g_ill[i]);
    g_vec[0] = (NPlayerGui::CDrawBase*)g_ill[1];    // stamina
    g_vec[1] = (NPlayerGui::CDrawBase*)g_ill[24];   // ability buttons
    g_vec[2] = (NPlayerGui::CDrawBase*)g_ill[6];    // money
    SET(g_gui, 0x2c0, (void*)g_vec); SET(g_gui, 0x2c4, (void*)(g_vec + 3)); SET(g_gui, 0x2c8, (void*)(g_vec + 3));
    // gui def: ambient (0,0,255,51) diffuse (0,255,0,255) backlight (255,0,0,255), DiffuseVector (1,2,3)
    unsigned char amb[4] = { 255, 0, 0, 51 }, dif[4] = { 0, 255, 0, 255 }, bkl[4] = { 0, 0, 255, 255 };   // B,G,R,A
    memcpy((char*)g_def + 0x48, amb, 4); memcpy((char*)g_def + 0x4c, dif, 4); memcpy((char*)g_def + 0x50, bkl, 4);
    float vec[3] = { 1.0f, 2.0f, 3.0f }; memcpy((char*)g_def + 0x54, vec, 12);
    { void* d = g_def; memcpy(&CPlayerGui::PGuiDef, &d, 4); }
    SET(g_upm, 0x1c, 1.25f); SET(g_upm, 0x20, 0.5f); SET(g_upm, 0xdc, 2.0f); SET(g_upm, 0xe0, 0.25f);
    SetDrawing(0x140, true);   // health keeps drawing (not in Illustrators)
    SetDrawing(0x1d8, false); SetDrawing(0x220, false);

    // 1: GuiMode 1, no subtitle override, not paused, no TGA
    SET(g_gui, 0x294, 1); SET(g_disp, 0xb4, -1.0f);
    Run(gui, info);
    Check(strcmp(g_log, "Lighting Timers Update:MiniMap Draw:2DText ClearEffects:Stamina ClearEffects:AbilityButtons ClearEffects:QuickAccessIcons ClearEffects:MeleeActionButtonEffects ") == 0, "log 1");
    Check(*(float*)((char*)g_disp + 0xb4) == -1.0f, "no gamma on call 1");
    Check(*(float*)((char*)g_gui + 0x2a0) == 0.5f, "alpha old profile");
    Check(g_timerSet == &info.Interpolate.Current, "current set");
    Check(g_light[0] == 1.0f && g_light[1] == 2.0f && g_light[2] == 3.0f, "diffuse vector");
    Check(Near(g_light[3], 0.0f) && Near(g_light[4], 0.0f) && Near(g_light[5], 1.0f) && Near(g_light[6], 0.2f), "ambient");
    Check(Near(g_light[7], 0.0f) && Near(g_light[8], 1.0f) && Near(g_light[9], 0.0f) && Near(g_light[10], 1.0f), "diffuse");
    Check(Near(g_light[11], 1.0f) && Near(g_light[12], 0.0f) && Near(g_light[13], 0.0f) && Near(g_light[14], 1.0f), "backlight");
    Check(!Drawing(0x148) && !Drawing(0x200) && !Drawing(0x170) && Drawing(0x140), "illustrators cleared");
    Check(g_feFirst == g_vec && g_feLast == g_vec + 3, "for_each range");
    Check(g_persist == 0, "no sprite");

    // 2: subtitle override, paused, new profile values, TGA on
    SET(g_gui, 0x2bd, true); SET(g_world, 0xa4, true); SET(g_upm, 0x0c, true); SET(g_gui, 0x2b1, true);
    Run(gui, info);
    Check(strcmp(g_log, "Lighting Timers Update:MiniMap Subtitle Draw:2DText ClearEffects:Stamina ClearEffects:AbilityButtons ClearEffects:QuickAccessIcons ClearEffects:MeleeActionButtonEffects Dim Dim Persistent ") == 0, "log 2");
    Check(*(float*)((char*)g_disp + 0xb4) == 2.0f, "gamma on call 2");
    Check(*(float*)((char*)g_gui + 0x2a0) == 0.25f, "alpha new profile");
    Check(g_timerSet == &info.Interpolate.PausedCurrent, "paused set");
    Check(g_persist == 1 && Near(g_spr[0], (1280.0f - 100.0f) * 0.5f) && Near(g_spr[1], (1440.0f - 60.0f) * 0.5f), "sprite pos");
    Check(g_sprTex == (char*)g_gui + 0x2b4 && g_sprHandle == (char*)g_gui + 0x31c && g_persistFlags == 0x40 && g_sprFlags == 2
          && g_sprLayer == ENGINE_2D_LAYER_0 && g_sprColour[0] == 0xff && g_sprColour[1] == 0xff && g_sprColour[2] == 0xff && g_sprColour[3] == 0xff, "sprite args");

    // 3: GuiMode 0, letterbox, movie sequence ends the drawHud chain; live first-person hero in the pickpocket game
    SET(g_disp, 0xb4, -1.0f); SET(g_gui, 0x294, 0); SET(g_gui, 0x2bd, false); SET(g_world, 0xa4, false); SET(g_gui, 0x2b1, false);
    ((unsigned char*)g_disp)[0x70] = 1; g_movie = true; g_hero = g_heroMem; g_mode = PLAYER_MODE_FIRST_PERSON; g_mini = MINIGAME_PICKPOCKET;
    Run(gui, info);
    Check(strcmp(g_log, "Lighting Timers Update:MiniMap Subtitle Draw:ScreenFade Draw:LetterBoxBlackBars Draw:HighlightingTargetingCursor Draw:2DText ClearEffects:Stamina ClearEffects:AbilityButtons ClearEffects:QuickAccessIcons ClearEffects:MeleeActionButtonEffects ") == 0, "log 3");
    Check(g_movieCalls == 1 && g_modeCalls == 1, "chain stops at movie; hero block asks the mode once");
    Check(*(float*)((char*)g_disp + 0xb4) == -1.0f, "no gamma on call 3");
    Check(g_timerSet == &info.Interpolate.Current, "current set 3");

    // 4: no movie, no letterbox: the chain asks the mode; not in a mini game -> cursor cleared
    ((unsigned char*)g_disp)[0x70] = 0; g_movie = false; g_mini = MINIGAME_NONE;
    Run(gui, info);
    Check(strcmp(g_log, "Lighting Timers Update:MiniMap Subtitle Draw:ScreenFade Clear:HighlightingTargetingCursor Draw:2DText ClearEffects:Stamina ClearEffects:AbilityButtons ClearEffects:QuickAccessIcons ClearEffects:MeleeActionButtonEffects ") == 0, "log 4");
    Check(g_movieCalls == 1 && g_modeCalls == 2, "full chain");

    // 5: no hero: nothing past the chain; health stops drawing -> its ClearEffects runs; steal mini game unused
    g_hero = 0; g_mini = MINIGAME_STEAL; SetDrawing(0x140, false); SetDrawing(0x1d8, true);
    Run(gui, info);
    Check(strcmp(g_log, "Lighting Timers Update:MiniMap Subtitle Draw:ScreenFade Draw:2DText ClearEffects:Health ClearEffects:Stamina ClearEffects:AbilityButtons ClearEffects:MeleeActionButtonEffects ") == 0, "log 5");
    Check(g_modeCalls == 1, "mode asked once by the chain");
    if (fails) printf("%s\n", g_log);
    if (!fails) printf("00439070_TEST PASS\n"); else printf("00439070_TEST FAIL\n");
    return fails ? 1 : 0;
}
