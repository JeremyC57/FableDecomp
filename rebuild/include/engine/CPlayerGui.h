#ifndef FABLE_ENGINE_CPLAYERGUI_H
#define FABLE_ENGINE_CPLAYERGUI_H
#ifdef FABLE_RETAIL_CPLAYERGUI
#ifdef FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
namespace NPlayerGui { class CDrawQuestInfo; }
#endif
// Retail opt-in (Codex pass 11 consolidation 2): the retail CPlayerGui model (row 0x00667D30 CThingCreatureBase::OnHit); see below.
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CSubtitleRenderer.h"
#ifdef FABLE_RETAIL_CPLAYERGUI_MEMBERS
// Retail opt-in (chunk 853, row 0x0043A570 CPlayerGui::~CPlayerGui): the members PFont .. DisplayBottomHalfGUI with their Ego_r.pdb
// types at the retail offsets (the generated layout below, STLport sizes), for the implicit member destruction the destructor emits.
// Evidence (retail ~CPlayerGui 0x0043A570, two sources each: the generated Ego_r layout + the retail destructor's per-offset code):
// PHeroInfoScreensManager +0x32C (boost::scoped_ptr: inline checked_delete of a non-polymorphic CManager), ScreenMessages +0x328 /
// ScriptScreenText +0x324 (STLport list: _List_base::clear out of line 0x0043FE40 / 0x0043FDF0, then the head node freed),
// DebugTGAPrimitiveHandle +0x31C (vtable 0x0122F598), SafeAreaPrimitiveHandles +0x310 / TextPrimitiveHandles +0x304 (vector of
// handles destroyed through slot 0), PAutoPickups .. PMoney +0x2FC .. +0x2CC (CCountedPointer), Illustrators +0x2C0 (vector of
// pointers), DebugTGATexture +0x2B4 (inline ~CTexture = Uninitialise 0x009F9F70), QuickAccessButtonsAlpha / AbilityButtonsAlpha /
// AbilityButtons +0x26C / +0x260 / +0x254, PDrawNPCName .. PFont +0x24C .. +0x138 (CCountedPointer). Static PGuiDef 0x013B878C
// (FableWin/Ego_r "dynamic atexit destructor for 'CPlayerGui::PGuiDef'"; CDefPointer<const CPlayerGuiDef>: the PDB name sets list
// ??0?$CDefPointer@$$CBVCPlayerGuiDef@@@@QAE@W4ENullDef@@@Z and GetDef<CPlayerGuiDef>(.., CDefPointer<const CPlayerGuiDef>*)).
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CTexture.h"
#include "engine/C2DBoxF.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "stlport/fable_containers.h"
#include "boost/scoped_ptr.h"
class CFontBank; class CPlayerGuiDef;
namespace NPlayerGui {
class CDrawHealth; class CDrawStamina; class CDrawCombatMultiplier; class CDrawSpiritsHealth; class CDrawMiniMap; class CDrawScreenFade;
class CDrawMoney; class CDrawRenown; class CDrawMorality; class CDrawExperience; class CDrawCrime; class CDrawReceivedItems;
class CDrawFishingGame; class CDrawStatUpdates; class CDrawAutoPickups; class CDrawPerceivers; class CDrawDamage; class CDrawQuestInfo;
class CDrawGuildSeal; class CDrawQuickAccessIcons; class CDrawScriptScreenText; class CDrawTargetInfo; class CDrawHighlightingTargetingCursor;
class CDrawWorldTime; class CDrawAbilityButtons; class CDrawLetterBoxBlackBars; class CDrawBettingHUD; class CDrawProjectileTargetingCentre;
class CDrawMeleeActionButtonEffects; class CDrawSpiritScores; class CDrawSpiritTargetingCursor; class CDraw2DText; class CDrawMouseCursor;
class CDrawNPCName; class CDrawBase; class CStatBar; class CCrimeStatBar; class CMultiStatBar; class CAutoPickupBar;
class CScriptScreenText; class CScreenMessage;
}
enum EPlayerGuiMode { };                                             // W4EPlayerGuiMode@@ (enumerators not needed here)
#endif
#if defined(FABLE_RETAIL_CPLAYERGUI_GUIMODE) && !defined(FABLE_RETAIL_CPLAYERGUI_MEMBERS)
// Opt-in (chunk 898, row 0x00654438 NPlayerGui::CDrawQuestInfo::Draw reads its own EPlayerGuiMode GuiMode): the enum alone, for TUs
// that do not take the full member list.
enum EPlayerGuiMode { };                                             // W4EPlayerGuiMode@@ (enumerators not needed here)
#endif
class CThing;class C3DVector;
#ifdef FABLE_RETAIL_CPLAYERGUI_CTOR
class CWorld; class CPlayerManager; class CGamePlayerInterface; class CMainGameComponent; class CGraphicDataBank;   // chunk 863
#endif
namespace NHeroInformationScreens { class CManager; }
// TPI0xbf85 original singleton base; native437990 forwards through PDrawCombatMultiplier+150.
// Complete original members World through DisplayBottomHalfGUI retained opaque, except SubtitleRenderer (2026-11-03, row 0x005EB9E0):
// Ego_r.pdb CSubtitleRenderer SubtitleRenderer at +0x18 (after World, PlayerManager, InputInterface, Component, Graphics), 0x120
// bytes in retail; FableWin.pdb ?GetSubtitleRenderer@CPlayerGui@@QAEAAVCSubtitleRenderer@@XZ is inline in Ego_r.pdb and retail
// (CTCTavernGame::FrameUpdate reads the singleton's byte +0xFD = +0x18 + DisplayingText +0xE5 in place, 0x005EBA74).
class CPlayerGui : public CTBaseSingletonNoDefaultAlloc<CPlayerGui> {
#ifdef FABLE_RETAIL_CPLAYERGUI_SCREENSHOTMESSAGE
public:
    // Retail opt-in (wf19, row 0x0055D180): Ego_r.pdb / FableWin.pdb ?ShowScreenshotMessage / ?HideScreenshotMessage /
    // ?IsShowingScreenshotMessage@CPlayerGui (Ego_r 0x0044A6C0 / D0 / E0 = retail 0x004379E0 / F0 / 0x00437A00, out-of-line forwarders to
    // PDrawNPCName, +0x248). Declarations only.
    void ShowScreenshotMessage();                                        // 0x004379E0
    void HideScreenshotMessage();                                        // 0x004379F0
    bool IsShowingScreenshotMessage();                                   // 0x00437A00
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:
void AddDamageChange(float,bool);
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI_CTOR
    // Retail opt-in (chunk 863, row 0x0043B570 CPlayerGui::CPlayerGui): the first five members with their Ego_r.pdb names and
    // types (struct_layouts_egor.tsv: World +4, PlayerManager +8, InputInterface +0xC, Component +0x10, Graphics +0x14) and the
    // constructor Ego_r.pdb / FableWin.pdb ??0CPlayerGui@@QAE@ABVCMainGameComponent@@AAVCWorld@@AAVCGamePlayerInterface@@AAVCPlayerManager@@ABVCGraphicDataBank@@@Z
    // (retail 0x0043B570, `ret 14h`: stores [esp+8] at +4, [esp+0x10] at +8, [esp+0xC] at +0xC, [esp+4] at +0x10, [esp+0x14] at +0x14).
    // Same layout as the pad it replaces.
private:
    CWorld* World;                                                       // +0x04
    CPlayerManager* PlayerManager;                                       // +0x08
    CGamePlayerInterface* InputInterface;                                // +0x0c
    const CMainGameComponent* Component;                                 // +0x10
    const CGraphicDataBank* Graphics;                                    // +0x14
public:
    CPlayerGui(const CMainGameComponent& component, CWorld& world, CGamePlayerInterface& inputInterface, CPlayerManager& playerManager,
               const CGraphicDataBank& graphics);                        // 0x0043B570
private:
#else
private: unsigned char _pad_0x04[0x14];                              // +0x04 World .. Graphics
#endif
    CSubtitleRenderer SubtitleRenderer;                                  // +0x18
#ifdef FABLE_RETAIL_CPLAYERGUI_MEMBERS
    CCountedPointer<CFontBank> PFont;                                    // +0x138
    CCountedPointer<NPlayerGui::CDrawHealth> PDrawHealth;                // +0x140
    CCountedPointer<NPlayerGui::CDrawStamina> PDrawStamina;              // +0x148
    CCountedPointer<NPlayerGui::CDrawCombatMultiplier> PDrawCombatMultiplier;   // +0x150
    CCountedPointer<NPlayerGui::CDrawSpiritsHealth> PDrawSpiritsHealth;  // +0x158
    CCountedPointer<NPlayerGui::CDrawMiniMap> PDrawMiniMap;              // +0x160
    CCountedPointer<NPlayerGui::CDrawScreenFade> PDrawScreenFade;        // +0x168
    CCountedPointer<NPlayerGui::CDrawMoney> PDrawMoney;                  // +0x170
    CCountedPointer<NPlayerGui::CDrawRenown> PDrawRenown;                // +0x178
    CCountedPointer<NPlayerGui::CDrawMorality> PDrawMorality;            // +0x180
    CCountedPointer<NPlayerGui::CDrawExperience> PDrawExperience;        // +0x188
    CCountedPointer<NPlayerGui::CDrawCrime> PDrawCrime;                  // +0x190
    CCountedPointer<NPlayerGui::CDrawReceivedItems> PDrawReceivedItems;  // +0x198
    CCountedPointer<NPlayerGui::CDrawFishingGame> PDrawFishingGame;      // +0x1a0
    CCountedPointer<NPlayerGui::CDrawStatUpdates> PDrawStatUpdates;      // +0x1a8
    CCountedPointer<NPlayerGui::CDrawAutoPickups> PDrawAutoPickups;      // +0x1b0
    CCountedPointer<NPlayerGui::CDrawPerceivers> PDrawPerceivers;        // +0x1b8
    CCountedPointer<NPlayerGui::CDrawDamage> PDrawDamage;                // +0x1c0
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;          // +0x1c8
    CCountedPointer<NPlayerGui::CDrawGuildSeal> PDrawGuildSeal;          // +0x1d0
    CCountedPointer<NPlayerGui::CDrawQuickAccessIcons> PDrawQuickAccessIcons;   // +0x1d8
    CCountedPointer<NPlayerGui::CDrawScriptScreenText> PDrawScriptScreenText;   // +0x1e0
    CCountedPointer<NPlayerGui::CDrawTargetInfo> PDrawTargetInfo;        // +0x1e8
    CCountedPointer<NPlayerGui::CDrawHighlightingTargetingCursor> PDrawHighlightingTargetingCursor;   // +0x1f0
    CCountedPointer<NPlayerGui::CDrawWorldTime> PDrawWorldTime;          // +0x1f8
    CCountedPointer<NPlayerGui::CDrawAbilityButtons> PDrawAbilityButtons;   // +0x200
    CCountedPointer<NPlayerGui::CDrawLetterBoxBlackBars> PDrawLetterBoxBlackBars;   // +0x208
    CCountedPointer<NPlayerGui::CDrawBettingHUD> PDrawBettingHUD;        // +0x210
    CCountedPointer<NPlayerGui::CDrawProjectileTargetingCentre> PDrawProjectileTargetingCentre;   // +0x218
    CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects> PDrawMeleeActionButtonEffects;     // +0x220
    CCountedPointer<NPlayerGui::CDrawSpiritScores> PDrawSpiritScores;    // +0x228
    CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor> PDrawSpiritTargetingCursor;   // +0x230
    CCountedPointer<NPlayerGui::CDraw2DText> PDraw2DText;                // +0x238
    CCountedPointer<NPlayerGui::CDrawMouseCursor> PDrawMouseCursor;      // +0x240
    CCountedPointer<NPlayerGui::CDrawNPCName> PDrawNPCName;              // +0x248
    bool ShowSpecialAbilityButtons;                                      // +0x250
    bool ShowSpecialAbilityButtonsAbilities;                             // +0x251
    bool SpecialSelected;                                                // +0x252
    bool SpecialDeselected;                                              // +0x253
    std::vector<long> AbilityButtons;                                    // +0x254
    std::vector<float> AbilityButtonsAlpha;                              // +0x260
    std::vector<float> QuickAccessButtonsAlpha;                          // +0x26c
    float AbilityButtonScale;                                            // +0x278
    bool NormalToSpecial;                                                // +0x27c
    bool SpecialToNormal;                                                // +0x27d
    C2DBoxF VWindow;                                                     // +0x280
    bool DrawGui;                                                        // +0x290
    bool OldDrawGui;                                                     // +0x291
    EPlayerGuiMode GuiMode;                                              // +0x294
    float FadeInTimer;                                                   // +0x298
    bool ForcedClosed;                                                   // +0x29c
    bool ShowTime;                                                       // +0x29d
    bool ShowMoneyBag;                                                   // +0x29e
    bool ShowStatBars;                                                   // +0x29f
    float Alpha;                                                         // +0x2a0
    unsigned __int64 HeroCentreUID;                                      // +0x2a8
    bool DrawingSafeAreas;                                               // +0x2b0
    bool DrawingDebugTGA;                                                // +0x2b1
    CTexture DebugTGATexture;                                            // +0x2b4
    bool ScreenMessagingEnabled;                                         // +0x2bc
    bool ForceAllowSubtitleToRender;                                     // +0x2bd
    bool LiveGuiMode;                                                    // +0x2be
    std::vector<NPlayerGui::CDrawBase*> Illustrators;                    // +0x2c0
    CCountedPointer<NPlayerGui::CStatBar> PMoney;                        // +0x2cc
    CCountedPointer<NPlayerGui::CStatBar> PRenown;                       // +0x2d4
    CCountedPointer<NPlayerGui::CStatBar> PMorality;                     // +0x2dc
    CCountedPointer<NPlayerGui::CStatBar> PExperience;                   // +0x2e4
    CCountedPointer<NPlayerGui::CCrimeStatBar> PCrime;                   // +0x2ec
    CCountedPointer<NPlayerGui::CMultiStatBar> PStatUpdates;             // +0x2f4
    CCountedPointer<NPlayerGui::CAutoPickupBar> PAutoPickups;            // +0x2fc
    std::vector<CEnginePrimitiveHandle> TextPrimitiveHandles;            // +0x304
    std::vector<CEnginePrimitiveHandle> SafeAreaPrimitiveHandles;        // +0x310
    CEnginePrimitiveHandle DebugTGAPrimitiveHandle;                      // +0x31c
    std::list<NPlayerGui::CScriptScreenText> ScriptScreenText;           // +0x324
    std::list<NPlayerGui::CScreenMessage> ScreenMessages;                // +0x328
    boost::scoped_ptr<NHeroInformationScreens::CManager> PHeroInfoScreensManager;   // +0x32c
    bool DisplayBottomHalfGUI;                                           // +0x330
#ifdef FABLE_RETAIL_CPLAYERGUI_FRIEND_STAT_DRAWERS
    // Retail opt-in (chunk Q1, row 0x00644263 NPlayerGui::CDrawMoney::ClearPrimitivesIfNecessary): the stat-bar drawers read the
    // GUI's private bar pointers in place (PMoney +0x2CC / PRenown +0x2D4 / PMorality +0x2DC: `mov eax,[ecx+4]; mov ecx,[eax+2CCh]`),
    // so they are friends of CPlayerGui (no accessor exists in either PDB name set). Access only; no layout or code change.
    friend class NPlayerGui::CDrawMoney;
    friend class NPlayerGui::CDrawRenown;
    friend class NPlayerGui::CDrawMorality;
#endif
public:
    static CDefPointer<const CPlayerGuiDef> PGuiDef;                     // 0x013B878C
    ~CPlayerGui();                                                       // 0x0043A570 (virtual through the singleton base)
private:
#else
#ifdef FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
    // Retail opt-in (Codex 2026-11-09): donor PDrawQuestInfo and Alpha,
    // native00651298 and006513c8 read1c8 and2a0. Default storage unchanged.
    unsigned char _pad_0x138[0x90];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;
    unsigned char _pad_0x1d0[0xd0];
    float Alpha;
    unsigned char _pad_0x2a4[0x94];
#elif defined(FABLE_RETAIL_CPLAYERGUI_LIVEGUI)
    // Retail opt-in (chunk 1004, rows 0x005C90FC / 0x005CACB9): Ego_r.pdb bool LiveGuiMode at +0x2BE (the members list above) and
    // FableWin.pdb ?IsLiveGUIMode@CPlayerGui@@QBE_NXZ, inline in retail (`mov eax,[013B8790h] ; cmp byte ptr [eax+2BEh],0`,
    // 0x005C9553). Same layout as the pad it replaces.
    unsigned char _pad_0x138[0x186];
    bool LiveGuiMode;                                                    // +0x2be
    unsigned char _pad_0x2bf[0x79];
public:
    bool IsLiveGUIMode() const { return LiveGuiMode; }
private:
#else
    unsigned char _pad_0x138[0x200];                                     // +0x138 PFont .. DisplayBottomHalfGUI
#endif
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
public:
    // Retail opt-in (Codex2026-11-10): settled00438010 and both PDB signatures.
    void Draw2DText(const C2DVector&,const CWideString&,const CFontBank&,const CRGBColour&,float,EEngine2DLayer,bool);
#endif
#if defined(FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS) && defined(FABLE_RETAIL_CPLAYERGUI_MEMBERS)
public:
    // Retail opt-in (wf3, row 0x00646580 NPlayerGui::CDrawMeleeActionButtonEffects::Draw): FableWin.pdb
    // ?IsShowSpecialAbilityButtons@CPlayerGui@@QBE_NXZ (0x0180822F / 0x020F5B83), inline in retail: the row reads the flag in place
    // (`mov eax,[esi+4] ; cmp byte ptr [eax+250h],bl` at 0x0064658D). Needs FABLE_RETAIL_CPLAYERGUI_MEMBERS (the named member). No layout change.
    bool IsShowSpecialAbilityButtons() const { return ShowSpecialAbilityButtons; }
#endif
public: void AddHitByMelee(CThing&,CThing&,const C3DVector&);
    // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb / FableWin.pdb ?Update@CPlayerGui@@QAEX_N@Z (retail 0x0043A080,
    // landed) and ?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z (0x004378C0, the forward to PDrawMiniMap +0x160), both on Get().
#ifdef FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
    // FableWin01a671e7/020d93ed/020e2c6a; the native draw inlines all three.
    NPlayerGui::CDrawQuestInfo& GetQuestInfo() { return *PDrawQuestInfo; }
    unsigned char ScaleAlpha(unsigned char alpha) const { return (unsigned char)(alpha*Alpha); }
    unsigned char GetAlpha() const { return (unsigned char)(255.0f*Alpha); }
#endif
    void Update(bool paused);                                            // 0x0043A080
    void UpdateMiniMapMarkers(bool);                                     // 0x004378C0
    CSubtitleRenderer& GetSubtitleRenderer() { return SubtitleRenderer; }
#ifdef FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
    // Retail opt-in (chunk 855, row 0x00490BE0): Ego_r.pdb 0x0044B230 / FableWin.pdb 0x01A80EA0 ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ
    // (public static __fastcall), out of line in retail at 0x00438570 (no this set up at its callers). Declaration only.
    static const class CPlayerGuiDef& __fastcall PeekGuiDef();          // 0x00438570
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING
    // Opt-in (chunk 898, row 0x00654438 NPlayerGui::CDrawQuestInfo::Draw): Ego_r.pdb 0x0044A510 / FableWin.pdb 0x01A7F700
    // ?IsMiniMapDrawing@CPlayerGui@@QBE_NXZ, out of line in retail at 0x00437870 (reads PDrawMiniMap +0x160 -> CDrawBase::Drawing +0x18).
    bool IsMiniMapDrawing() const;                                       // 0x00437870
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
    // Retail opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): Ego_r.pdb 0x0044A200 / FableWin.pdb 0x01A7CC80
    // ?ChangeGraphicsBankScene@CPlayerGui@@QAEXXZ, out of line in retail at 0x00437570 (`mov ecx,[ecx+14h] ; jmp 0x009FD0C0`,
    // CGraphicDataBank::ChangeScene on Graphics; the landed StopProgress 0x00499D90 binds it under a non-PDB spelling), and FableWin.pdb
    // ?PeekGraphicBank@CPlayerGui@@QBEABVCGraphicDataBank@@XZ (0x018B1330; inline in retail: DrawRetailDisplay pushes the Graphics
    // word +0x14 read in place, 0x004986CC). PeekGraphicBank needs FABLE_RETAIL_CPLAYERGUI_CTOR (the typed Graphics member).
    void ChangeGraphicsBankScene();                                      // 0x00437570
#ifdef FABLE_RETAIL_CPLAYERGUI_CTOR
    const CGraphicDataBank& PeekGraphicBank() const { return *Graphics; }
#endif
#endif
    // Chunk 735 (row 0x00577AC0 CTCInventoryExperience::OpenInventoryInternalPC): Ego_r.pdb / FableWin.pdb
    // ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ, out of line at 0x00437CD0 (it reads
    // PHeroInfoScreensManager's scoped_ptr word at +0x32C).
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;  // 0x00437CD0
#ifdef FABLE_RETAIL_CPLAYERGUI_OPEN
    // Retail opt-in (chunk 795, row 0x005E78F0): Ego_r.pdb ?Open@CPlayerGui@@QAEXXZ, out of line at 0x004374B0 (landed callers, e.g.
    // the CTCQuestCompletionUI dtor 0x005E41E0). Declaration only.
    void Open();                                                         // 0x004374B0
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI_CLOSE
    // Retail opt-in (chunk 890, row 0x0056AD40 CTCBoastUI::Activate): Ego_r.pdb 0x0044B4D0 / FableWin.pdb 0x01A7B5F0
    // ?Close@CPlayerGui@@QAEX_N@Z (retail 0x00438400, `ret 4`) and Ego_r.pdb 0x0044B0F0 / FableWin.pdb 0x01A7CAC0
    // ?ForceClearGui@CPlayerGui@@QAEXXZ (retail 0x004382C0, 30 bytes: the illustrators' for_each only), both out of line and called
    // on a fresh read of the singleton word (Activate 0x0056AEA2 / 0x0056AEAD). 0x004382C0 is ForceClearGui, not PrepareForLoad: it
    // sits right after the landed ClearGui 0x00438230 as Ego_r orders ClearGui 0x0044B060 (0x90) / ForceClearGui 0x0044B0F0 (0x30) /
    // PrepareForLoad 0x0044B120, and PrepareForLoad is the 136-byte 0x00438460 (the sub-objects' PrepareForLoad fan-out, bound so by
    // CWorld 0x004A0220) -- NAME_FIXES. Declarations only.
    void Close(bool);                                                    // 0x00438400
    void ForceClearGui();                                                // 0x004382C0
#endif
#ifdef FABLE_RETAIL_CPLAYERGUI_CRIME
    // Nested opt-in (chunk 755, row 0x00775B40 CTCShop::FrameUpdate): Ego_r.pdb 0x0044A410 / FableWin.pdb 0x01A7EF10
    // ?SetCrimeAsDisplaying@CPlayerGui@@QAEXXZ, out of line in retail at 0x004376B0 (landed).
    void SetCrimeAsDisplaying();                                         // 0x004376B0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPlayerGui)==0x338);

#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CPlayerGui = 0x360.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CPlayerGui = 0x338.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CAutoPickupBar;
class CCPPointerInfo;
struct CCrimeStatBar;
struct CDraw2DText;
struct CDrawAbilityButtons;
struct CDrawAutoPickups;
struct CDrawBettingHUD;
struct CDrawCombatMultiplier;
struct CDrawCrime;
struct CDrawDamage;
struct CDrawExperience;
struct CDrawFishingGame;
struct CDrawGuildSeal;
struct CDrawHealth;
struct CDrawHighlightingTargetingCursor;
struct CDrawLetterBoxBlackBars;
struct CDrawMeleeActionButtonEffects;
struct CDrawMiniMap;
struct CDrawMoney;
struct CDrawMorality;
struct CDrawMouseCursor;
struct CDrawNPCName;
struct CDrawPerceivers;
struct CDrawProjectileTargetingCentre;
struct CDrawQuestInfo;
struct CDrawQuickAccessIcons;
struct CDrawReceivedItems;
struct CDrawRenown;
struct CDrawScreenFade;
struct CDrawScriptScreenText;
struct CDrawSpiritScores;
struct CDrawSpiritTargetingCursor;
struct CDrawSpiritsHealth;
struct CDrawStamina;
struct CDrawStatUpdates;
struct CDrawTargetInfo;
struct CDrawWorldTime;
class CFontBank;
class CGamePlayerInterface;
class CGraphicDataBank;
class CMainGameComponent;
struct CManager;
struct CMultiStatBar;
class CPlayerManager;
struct CStatBar;
class CWorld;
struct IDirect3DTexture9;

#pragma pack(push, 1)
class CPlayerGui {
public:
    void*                             __vftable;             // +0x00 vptr, or first dword of a flattened base subobject
    CWorld*                           World;                 // +0x04
    CPlayerManager*                   PlayerManager;         // +0x08
    CGamePlayerInterface*             InputInterface;        // +0x0c
    CMainGameComponent*               Component;             // +0x10
    CGraphicDataBank*                 Graphics;              // +0x14
    unsigned char                     SubtitleRenderer[0x120]; // +0x18 CSubtitleRenderer
    CFontBank*                        PFont_Data;            // +0x138 CCountedPointer<CFontBank>::Data
    CCPPointerInfo*                   PFont_Info;            // +0x13c CCountedPointer<CFontBank>::Info
    CDrawHealth*                      PDrawHealth_Data;      // +0x140 CCountedPointer<NPlayerGui::CDrawHealth>::Data
    CCPPointerInfo*                   PDrawHealth_Info;      // +0x144 CCountedPointer<NPlayerGui::CDrawHealth>::Info
    CDrawStamina*                     PDrawStamina_Data;     // +0x148 CCountedPointer<NPlayerGui::CDrawStamina>::Data
    CCPPointerInfo*                   PDrawStamina_Info;     // +0x14c CCountedPointer<NPlayerGui::CDrawStamina>::Info
    CDrawCombatMultiplier*            PDrawCombatMultiplier_Data; // +0x150 CCountedPointer<NPlayerGui::CDrawCombatMultiplier>::Data
    CCPPointerInfo*                   PDrawCombatMultiplier_Info; // +0x154 CCountedPointer<NPlayerGui::CDrawCombatMultiplier>::Info
    CDrawSpiritsHealth*               PDrawSpiritsHealth_Data; // +0x158 CCountedPointer<NPlayerGui::CDrawSpiritsHealth>::Data
    CCPPointerInfo*                   PDrawSpiritsHealth_Info; // +0x15c CCountedPointer<NPlayerGui::CDrawSpiritsHealth>::Info
    CDrawMiniMap*                     PDrawMiniMap_Data;     // +0x160 CCountedPointer<NPlayerGui::CDrawMiniMap>::Data
    CCPPointerInfo*                   PDrawMiniMap_Info;     // +0x164 CCountedPointer<NPlayerGui::CDrawMiniMap>::Info
    CDrawScreenFade*                  PDrawScreenFade_Data;  // +0x168 CCountedPointer<NPlayerGui::CDrawScreenFade>::Data
    CCPPointerInfo*                   PDrawScreenFade_Info;  // +0x16c CCountedPointer<NPlayerGui::CDrawScreenFade>::Info
    CDrawMoney*                       PDrawMoney_Data;       // +0x170 CCountedPointer<NPlayerGui::CDrawMoney>::Data
    CCPPointerInfo*                   PDrawMoney_Info;       // +0x174 CCountedPointer<NPlayerGui::CDrawMoney>::Info
    CDrawRenown*                      PDrawRenown_Data;      // +0x178 CCountedPointer<NPlayerGui::CDrawRenown>::Data
    CCPPointerInfo*                   PDrawRenown_Info;      // +0x17c CCountedPointer<NPlayerGui::CDrawRenown>::Info
    CDrawMorality*                    PDrawMorality_Data;    // +0x180 CCountedPointer<NPlayerGui::CDrawMorality>::Data
    CCPPointerInfo*                   PDrawMorality_Info;    // +0x184 CCountedPointer<NPlayerGui::CDrawMorality>::Info
    CDrawExperience*                  PDrawExperience_Data;  // +0x188 CCountedPointer<NPlayerGui::CDrawExperience>::Data
    CCPPointerInfo*                   PDrawExperience_Info;  // +0x18c CCountedPointer<NPlayerGui::CDrawExperience>::Info
    CDrawCrime*                       PDrawCrime_Data;       // +0x190 CCountedPointer<NPlayerGui::CDrawCrime>::Data
    CCPPointerInfo*                   PDrawCrime_Info;       // +0x194 CCountedPointer<NPlayerGui::CDrawCrime>::Info
    CDrawReceivedItems*               PDrawReceivedItems_Data; // +0x198 CCountedPointer<NPlayerGui::CDrawReceivedItems>::Data
    CCPPointerInfo*                   PDrawReceivedItems_Info; // +0x19c CCountedPointer<NPlayerGui::CDrawReceivedItems>::Info
    CDrawFishingGame*                 PDrawFishingGame_Data; // +0x1a0 CCountedPointer<NPlayerGui::CDrawFishingGame>::Data
    CCPPointerInfo*                   PDrawFishingGame_Info; // +0x1a4 CCountedPointer<NPlayerGui::CDrawFishingGame>::Info
    CDrawStatUpdates*                 PDrawStatUpdates_Data; // +0x1a8 CCountedPointer<NPlayerGui::CDrawStatUpdates>::Data
    CCPPointerInfo*                   PDrawStatUpdates_Info; // +0x1ac CCountedPointer<NPlayerGui::CDrawStatUpdates>::Info
    CDrawAutoPickups*                 PDrawAutoPickups_Data; // +0x1b0 CCountedPointer<NPlayerGui::CDrawAutoPickups>::Data
    CCPPointerInfo*                   PDrawAutoPickups_Info; // +0x1b4 CCountedPointer<NPlayerGui::CDrawAutoPickups>::Info
    CDrawPerceivers*                  PDrawPerceivers_Data;  // +0x1b8 CCountedPointer<NPlayerGui::CDrawPerceivers>::Data
    CCPPointerInfo*                   PDrawPerceivers_Info;  // +0x1bc CCountedPointer<NPlayerGui::CDrawPerceivers>::Info
    CDrawDamage*                      PDrawDamage_Data;      // +0x1c0 CCountedPointer<NPlayerGui::CDrawDamage>::Data
    CCPPointerInfo*                   PDrawDamage_Info;      // +0x1c4 CCountedPointer<NPlayerGui::CDrawDamage>::Info
    CDrawQuestInfo*                   PDrawQuestInfo_Data;   // +0x1c8 CCountedPointer<NPlayerGui::CDrawQuestInfo>::Data
    CCPPointerInfo*                   PDrawQuestInfo_Info;   // +0x1cc CCountedPointer<NPlayerGui::CDrawQuestInfo>::Info
    CDrawGuildSeal*                   PDrawGuildSeal_Data;   // +0x1d0 CCountedPointer<NPlayerGui::CDrawGuildSeal>::Data
    CCPPointerInfo*                   PDrawGuildSeal_Info;   // +0x1d4 CCountedPointer<NPlayerGui::CDrawGuildSeal>::Info
    CDrawQuickAccessIcons*            PDrawQuickAccessIcons_Data; // +0x1d8 CCountedPointer<NPlayerGui::CDrawQuickAccessIcons>::Data
    CCPPointerInfo*                   PDrawQuickAccessIcons_Info; // +0x1dc CCountedPointer<NPlayerGui::CDrawQuickAccessIcons>::Info
    CDrawScriptScreenText*            PDrawScriptScreenText_Data; // +0x1e0 CCountedPointer<NPlayerGui::CDrawScriptScreenText>::Data
    CCPPointerInfo*                   PDrawScriptScreenText_Info; // +0x1e4 CCountedPointer<NPlayerGui::CDrawScriptScreenText>::Info
    CDrawTargetInfo*                  PDrawTargetInfo_Data;  // +0x1e8 CCountedPointer<NPlayerGui::CDrawTargetInfo>::Data
    CCPPointerInfo*                   PDrawTargetInfo_Info;  // +0x1ec CCountedPointer<NPlayerGui::CDrawTargetInfo>::Info
    CDrawHighlightingTargetingCursor* PDrawHighlightingTargetingCursor_Data; // +0x1f0 CCountedPointer<NPlayerGui::CDrawHighlightingTargetingCursor>::Data
    CCPPointerInfo*                   PDrawHighlightingTargetingCursor_Info; // +0x1f4 CCountedPointer<NPlayerGui::CDrawHighlightingTargetingCursor>::Info
    CDrawWorldTime*                   PDrawWorldTime_Data;   // +0x1f8 CCountedPointer<NPlayerGui::CDrawWorldTime>::Data
    CCPPointerInfo*                   PDrawWorldTime_Info;   // +0x1fc CCountedPointer<NPlayerGui::CDrawWorldTime>::Info
    CDrawAbilityButtons*              PDrawAbilityButtons_Data; // +0x200 CCountedPointer<NPlayerGui::CDrawAbilityButtons>::Data
    CCPPointerInfo*                   PDrawAbilityButtons_Info; // +0x204 CCountedPointer<NPlayerGui::CDrawAbilityButtons>::Info
    CDrawLetterBoxBlackBars*          PDrawLetterBoxBlackBars_Data; // +0x208 CCountedPointer<NPlayerGui::CDrawLetterBoxBlackBars>::Data
    CCPPointerInfo*                   PDrawLetterBoxBlackBars_Info; // +0x20c CCountedPointer<NPlayerGui::CDrawLetterBoxBlackBars>::Info
    CDrawBettingHUD*                  PDrawBettingHUD_Data;  // +0x210 CCountedPointer<NPlayerGui::CDrawBettingHUD>::Data
    CCPPointerInfo*                   PDrawBettingHUD_Info;  // +0x214 CCountedPointer<NPlayerGui::CDrawBettingHUD>::Info
    CDrawProjectileTargetingCentre*   PDrawProjectileTargetingCentre_Data; // +0x218 CCountedPointer<NPlayerGui::CDrawProjectileTargetingCentre>::Data
    CCPPointerInfo*                   PDrawProjectileTargetingCentre_Info; // +0x21c CCountedPointer<NPlayerGui::CDrawProjectileTargetingCentre>::Info
    CDrawMeleeActionButtonEffects*    PDrawMeleeActionButtonEffects_Data; // +0x220 CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Data
    CCPPointerInfo*                   PDrawMeleeActionButtonEffects_Info; // +0x224 CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Info
    CDrawSpiritScores*                PDrawSpiritScores_Data; // +0x228 CCountedPointer<NPlayerGui::CDrawSpiritScores>::Data
    CCPPointerInfo*                   PDrawSpiritScores_Info; // +0x22c CCountedPointer<NPlayerGui::CDrawSpiritScores>::Info
    CDrawSpiritTargetingCursor*       PDrawSpiritTargetingCursor_Data; // +0x230 CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor>::Data
    CCPPointerInfo*                   PDrawSpiritTargetingCursor_Info; // +0x234 CCountedPointer<NPlayerGui::CDrawSpiritTargetingCursor>::Info
    CDraw2DText*                      PDraw2DText_Data;      // +0x238 CCountedPointer<NPlayerGui::CDraw2DText>::Data
    CCPPointerInfo*                   PDraw2DText_Info;      // +0x23c CCountedPointer<NPlayerGui::CDraw2DText>::Info
    CDrawMouseCursor*                 PDrawMouseCursor_Data; // +0x240 CCountedPointer<NPlayerGui::CDrawMouseCursor>::Data
    CCPPointerInfo*                   PDrawMouseCursor_Info; // +0x244 CCountedPointer<NPlayerGui::CDrawMouseCursor>::Info
    CDrawNPCName*                     PDrawNPCName_Data;     // +0x248 CCountedPointer<NPlayerGui::CDrawNPCName>::Data
    CCPPointerInfo*                   PDrawNPCName_Info;     // +0x24c CCountedPointer<NPlayerGui::CDrawNPCName>::Info
    bool                              ShowSpecialAbilityButtons; // +0x250
    bool                              ShowSpecialAbilityButtonsAbilities; // +0x251
    bool                              SpecialSelected;       // +0x252
    bool                              SpecialDeselected;     // +0x253
    unsigned char                     AbilityButtons[0xc];   // +0x254 vector<long,std::allocator<long>_>
    unsigned char                     AbilityButtonsAlpha[0xc];  // +0x260 vector<float,std::allocator<float>_>
    unsigned char                     QuickAccessButtonsAlpha[0xc];  // +0x26c vector<float,std::allocator<float>_>
    float                             AbilityButtonScale;    // +0x278
    bool                              NormalToSpecial;       // +0x27c
    bool                              SpecialToNormal;       // +0x27d
    unsigned char                     _pad_0x27e[0x2];       // +0x27e
    float                             VWindow_TLX;           // +0x280 C2DBoxF::TLX
    float                             VWindow_TLY;           // +0x284 C2DBoxF::TLY
    float                             VWindow_BRX;           // +0x288 C2DBoxF::BRX
    float                             VWindow_BRY;           // +0x28c C2DBoxF::BRY
    bool                              DrawGui;               // +0x290
    bool                              OldDrawGui;            // +0x291
    unsigned char                     _pad_0x292[0x2];       // +0x292
    unsigned char                     GuiMode[0x4];          // +0x294 EPlayerGuiMode
    float                             FadeInTimer;           // +0x298
    bool                              ForcedClosed;          // +0x29c
    bool                              ShowTime;              // +0x29d
    bool                              ShowMoneyBag;          // +0x29e
    bool                              ShowStatBars;          // +0x29f
    float                             Alpha;                 // +0x2a0
    unsigned char                     _pad_0x2a4[0x4];       // +0x2a4
    unsigned __int64                  HeroCentreUID;         // +0x2a8
    bool                              DrawingSafeAreas;      // +0x2b0
    bool                              DrawingDebugTGA;       // +0x2b1
    unsigned char                     _pad_0x2b2[0x2];       // +0x2b2
    unsigned char                     DebugTGATexture[0x8];  // +0x2b4 CTexture
    bool                              ScreenMessagingEnabled; // +0x2bc
    bool                              ForceAllowSubtitleToRender; // +0x2bd
    bool                              LiveGuiMode;           // +0x2be
    unsigned char                     _pad_0x2bf[0x1];       // +0x2bf
    unsigned char                     Illustrators[0xc];     // +0x2c0 vector<NPlayerGui::CDrawBase_*,std::allocator<NPlayerGui::CDrawBase_*>_>
    CStatBar*                         PMoney_Data;           // +0x2cc CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PMoney_Info;           // +0x2d0 CCountedPointer<NPlayerGui::CStatBar>::Info
    CStatBar*                         PRenown_Data;          // +0x2d4 CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PRenown_Info;          // +0x2d8 CCountedPointer<NPlayerGui::CStatBar>::Info
    CStatBar*                         PMorality_Data;        // +0x2dc CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PMorality_Info;        // +0x2e0 CCountedPointer<NPlayerGui::CStatBar>::Info
    CStatBar*                         PExperience_Data;      // +0x2e4 CCountedPointer<NPlayerGui::CStatBar>::Data
    CCPPointerInfo*                   PExperience_Info;      // +0x2e8 CCountedPointer<NPlayerGui::CStatBar>::Info
    CCrimeStatBar*                    PCrime_Data;           // +0x2ec CCountedPointer<NPlayerGui::CCrimeStatBar>::Data
    CCPPointerInfo*                   PCrime_Info;           // +0x2f0 CCountedPointer<NPlayerGui::CCrimeStatBar>::Info
    CMultiStatBar*                    PStatUpdates_Data;     // +0x2f4 CCountedPointer<NPlayerGui::CMultiStatBar>::Data
    CCPPointerInfo*                   PStatUpdates_Info;     // +0x2f8 CCountedPointer<NPlayerGui::CMultiStatBar>::Info
    CAutoPickupBar*                   PAutoPickups_Data;     // +0x2fc CCountedPointer<NPlayerGui::CAutoPickupBar>::Data
    CCPPointerInfo*                   PAutoPickups_Info;     // +0x300 CCountedPointer<NPlayerGui::CAutoPickupBar>::Info
    unsigned char                     TextPrimitiveHandles[0xc];  // +0x304 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char                     SafeAreaPrimitiveHandles[0xc];  // +0x310 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char                     DebugTGAPrimitiveHandle[0x8]; // +0x31c CEnginePrimitiveHandle
    unsigned char                     ScriptScreenText[0x4]; // +0x324 list<NPlayerGui::CScriptScreenText,std::allocator<NPlayerGui::CScriptScreenText>_>
    unsigned char                     ScreenMessages[0x4];   // +0x328 list<NPlayerGui::CScreenMessage,std::allocator<NPlayerGui::CScreenMessage>_>
    CManager*                         PHeroInfoScreensManager_ptr; // +0x32c scoped_ptr<NHeroInformationScreens::CManager>::ptr
    bool                              DisplayBottomHalfGUI;  // +0x330
    unsigned char                     _pad_0x331[0x7];       // +0x331
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CPlayerGui) == 0x338);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, World) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PlayerManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, InputInterface) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Component) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Graphics) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SubtitleRenderer) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PFont_Data) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PFont_Info) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHealth_Data) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHealth_Info) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStamina_Data) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStamina_Info) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCombatMultiplier_Data) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCombatMultiplier_Info) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritsHealth_Data) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritsHealth_Info) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMiniMap_Data) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMiniMap_Info) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScreenFade_Data) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScreenFade_Info) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMoney_Data) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMoney_Info) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawRenown_Data) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawRenown_Info) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMorality_Data) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMorality_Info) == 0x184);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawExperience_Data) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawExperience_Info) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCrime_Data) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawCrime_Info) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawReceivedItems_Data) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawReceivedItems_Info) == 0x19c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawFishingGame_Data) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawFishingGame_Info) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStatUpdates_Data) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawStatUpdates_Info) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAutoPickups_Data) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAutoPickups_Info) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawPerceivers_Data) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawPerceivers_Info) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawDamage_Data) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawDamage_Info) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuestInfo_Data) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuestInfo_Info) == 0x1cc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawGuildSeal_Data) == 0x1d0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawGuildSeal_Info) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuickAccessIcons_Data) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawQuickAccessIcons_Info) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScriptScreenText_Data) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawScriptScreenText_Info) == 0x1e4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawTargetInfo_Data) == 0x1e8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawTargetInfo_Info) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHighlightingTargetingCursor_Data) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawHighlightingTargetingCursor_Info) == 0x1f4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawWorldTime_Data) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawWorldTime_Info) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAbilityButtons_Data) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawAbilityButtons_Info) == 0x204);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawLetterBoxBlackBars_Data) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawLetterBoxBlackBars_Info) == 0x20c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawBettingHUD_Data) == 0x210);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawBettingHUD_Info) == 0x214);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawProjectileTargetingCentre_Data) == 0x218);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawProjectileTargetingCentre_Info) == 0x21c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMeleeActionButtonEffects_Data) == 0x220);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMeleeActionButtonEffects_Info) == 0x224);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritScores_Data) == 0x228);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritScores_Info) == 0x22c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritTargetingCursor_Data) == 0x230);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawSpiritTargetingCursor_Info) == 0x234);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDraw2DText_Data) == 0x238);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDraw2DText_Info) == 0x23c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMouseCursor_Data) == 0x240);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawMouseCursor_Info) == 0x244);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawNPCName_Data) == 0x248);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PDrawNPCName_Info) == 0x24c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowSpecialAbilityButtons) == 0x250);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowSpecialAbilityButtonsAbilities) == 0x251);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialSelected) == 0x252);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialDeselected) == 0x253);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtons) == 0x254);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtonsAlpha) == 0x260);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, QuickAccessButtonsAlpha) == 0x26c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, AbilityButtonScale) == 0x278);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, NormalToSpecial) == 0x27c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SpecialToNormal) == 0x27d);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_TLX) == 0x280);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_TLY) == 0x284);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_BRX) == 0x288);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, VWindow_BRY) == 0x28c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawGui) == 0x290);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, OldDrawGui) == 0x291);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, GuiMode) == 0x294);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, FadeInTimer) == 0x298);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ForcedClosed) == 0x29c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowTime) == 0x29d);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowMoneyBag) == 0x29e);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ShowStatBars) == 0x29f);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Alpha) == 0x2a0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, HeroCentreUID) == 0x2a8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawingSafeAreas) == 0x2b0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DrawingDebugTGA) == 0x2b1);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DebugTGATexture) == 0x2b4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScreenMessagingEnabled) == 0x2bc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ForceAllowSubtitleToRender) == 0x2bd);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, LiveGuiMode) == 0x2be);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, Illustrators) == 0x2c0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMoney_Data) == 0x2cc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMoney_Info) == 0x2d0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PRenown_Data) == 0x2d4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PRenown_Info) == 0x2d8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMorality_Data) == 0x2dc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PMorality_Info) == 0x2e0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PExperience_Data) == 0x2e4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PExperience_Info) == 0x2e8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PCrime_Data) == 0x2ec);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PCrime_Info) == 0x2f0);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PStatUpdates_Data) == 0x2f4);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PStatUpdates_Info) == 0x2f8);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PAutoPickups_Data) == 0x2fc);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PAutoPickups_Info) == 0x300);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, TextPrimitiveHandles) == 0x304);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, SafeAreaPrimitiveHandles) == 0x310);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DebugTGAPrimitiveHandle) == 0x31c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScriptScreenText) == 0x324);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, ScreenMessages) == 0x328);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, PHeroInfoScreensManager_ptr) == 0x32c);
FABLE_STATIC_ASSERT(offsetof(CPlayerGui, DisplayBottomHalfGUI) == 0x330);

#endif // FABLE_RETAIL_CPLAYERGUI
#endif // FABLE_ENGINE_CPLAYERGUI_H
