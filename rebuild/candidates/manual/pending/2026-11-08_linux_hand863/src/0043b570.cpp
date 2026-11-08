// CPlayerGui::CPlayerGui(const CMainGameComponent& component, CWorld& world, CGamePlayerInterface& inputInterface, CPlayerManager& playerManager, const CGraphicDataBank& graphics) -- build the player GUI: store the game objects, build the subtitle renderer and the GUI font, size the three button arrays to the PLAYER_GUI_PC definition's ability-button table, create the hero information screens manager, the seven stat bars and the 34 draw illustrators
// Leaf: ??0CPlayerGui@@QAE@ABVCMainGameComponent@@AAVCWorld@@AAVCGamePlayerInterface@@AAVCPlayerManager@@ABVCGraphicDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043B570, 3661 bytes, thiscall, `ret 14h`, no EH frame (the PlayerGui TU is built without /EHsc,
//   like ~CPlayerGui 0x0043A570).
// Identity: Ego_r.pdb / FableWin.pdb ??0CPlayerGui@@QAE@ABVCMainGameComponent@@AAVCWorld@@AAVCGamePlayerInterface@@AAVCPlayerManager@@ABVCGraphicDataBank@@@Z
//   (five reference parameters = `ret 14h`); it stores the CPlayerGui vtable 0x0123177C (rtti-vtables.tsv) and builds exactly the members
//   chunk 853's ~CPlayerGui destroys (engine/CPlayerGui.h FABLE_RETAIL_CPLAYERGUI_MEMBERS), in declaration order.
// Shape: the member-initialiser part is the implicit construction of every member (CCountedPointer zeroing, the bools / floats the
//   class's initialiser list sets, the inline CTexture / CEnginePrimitiveHandle / STLport vector and list constructors); the body sizes
//   AbilityButtons / AbilityButtonsAlpha / QuickAccessButtonsAlpha with the GUI def's ability-button count (PeekGuiDef, defined above in
//   the same TU as in retail and expanded here: the lazy GetDef<CPlayerGuiDef>("PLAYER_GUI_PC") then the def's (finish - start) / 0x5C),
//   resets the hero information screens manager (boost::scoped_ptr::reset: the old manager deleted inline), asks the component
//   IsEditorActive (slot 9; the result unused in the release build), then `new`s every stat bar and draw illustrator into its counted
//   pointer through the out-of-line CCountedPointer<T>::Reset(T*) COMDATs.
// Callees: CSubtitleRenderer(const CGraphicDataBank&) 0x0049166E, CFontManager::GetFont 0x009E2C80 (PFontManager 0x013B838C, font name
//   g_CCharString_013b8788 "ENG_ARIAL_16"), CBaseClass() 0x0099A2D0, malloc 0x00BFEA0E (list heads), vector<long>::resize 0x004428F0,
//   vector<float>::resize 0x00442770, operator new 0x00BFEA1A, CManager(CWorld&) 0x0066EAB7, free 0x00BFEA14, operator delete 0x00BFE9BC,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDef<CPlayerGuiDef> 0x0043FF30, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   the stat bar / illustrator ctors and Reset COMDATs listed at each line, CDrawBase ctor 0x0064CC65, CWideString() 0x0099AED0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_ABILITYBUTTONS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CSUBTITLERENDERER_DTOR
#define FABLE_RETAIL_CSUBTITLERENDERER_CTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NPlayerGui_CDrawNPCName.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"

class CFontManager {
public:
    CCountedPointer<CFontBank> GetFont(const CCharString& name);         // 0x009E2C80 ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z
};
extern CFontManager* PFontManager;                                       // 0x013B838C
extern CCharString g_CCharString_013b8788;                               // 0x013B8788 "ENG_ARIAL_16" (the PlayerGui TU's font name static)

namespace NPlayerGui {
// List elements (Ego_r.pdb sizes; only the node size matters to the list constructors here).
class CScriptScreenText { public: unsigned char _body[0x18]; };          // Text, VPos, Colour, FontName, FrameAdded
class CScreenMessage { public: unsigned char _body[0x10]; };             // Text, FramesToShowFor, NumberOfCharactersShowing, LinesInText
// Stat bars (ctor ??0<bar>@NPlayerGui@@QAE@ABVCPlayerGuiDef@@@Z; sizes from the `new` here).
class CStatBar { public: CStatBar(const CPlayerGuiDef& def); unsigned char _body[0x68]; };                // 0x0065431D
class CCrimeStatBar { public: CCrimeStatBar(const CPlayerGuiDef& def); unsigned char _body[0x70]; };      // 0x00654392
class CMultiStatBar { public: CMultiStatBar(const CPlayerGuiDef& def); unsigned char _body[0x84]; };      // 0x006543AF
class CAutoPickupBar { public: CAutoPickupBar(const CPlayerGuiDef& def); unsigned char _body[0x7c]; };    // 0x006543FF
// Illustrators: ??0<draw>@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@...@std@@@Z, sizes from the `new` here.
class CDrawHealth : public CDrawBase { public: CDrawHealth(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x6c - 0x28]; };   // 0x0064E1E4
class CDrawStamina : public CDrawBase { public: CDrawStamina(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x64 - 0x28]; };   // 0x0064E24F
class CDrawCombatMultiplier : public CDrawBase { public: CDrawCombatMultiplier(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064E2A6
class CDrawSpiritsHealth : public CDrawBase { public: CDrawSpiritsHealth(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D2A9
class CDrawMiniMap : public CDrawBase { public: CDrawMiniMap(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x160 - 0x28]; };   // 0x0064E83F
class CDrawScreenFade : public CDrawBase { public: CDrawScreenFade(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x2c - 0x28]; };   // 0x0064E8DC
class CDrawMoney : public CDrawBase { public: CDrawMoney(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D2C5
class CDrawRenown : public CDrawBase { public: CDrawRenown(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D2E1
class CDrawMorality : public CDrawBase { public: CDrawMorality(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D2FD
class CDrawExperience : public CDrawBase { public: CDrawExperience(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064E952
class CDrawCrime : public CDrawBase { public: CDrawCrime(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D319
class CDrawReceivedItems : public CDrawBase { public: CDrawReceivedItems(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D335
class CDrawFishingGame : public CDrawBase { public: CDrawFishingGame(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D351
class CDrawStatUpdates : public CDrawBase { public: CDrawStatUpdates(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x40 - 0x28]; };   // 0x0064E0A1
class CDrawAutoPickups : public CDrawBase { public: CDrawAutoPickups(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D36D
class CDrawPerceivers : public CDrawBase { public: CDrawPerceivers(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x40 - 0x28]; };   // 0x0064ED06
class CDrawDamage : public CDrawBase { public: CDrawDamage(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x2c - 0x28]; };   // 0x0064D3DA
class CDrawQuestInfo : public CDrawBase { public: CDrawQuestInfo(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x5c - 0x28]; };   // 0x0064F659
class CDrawGuildSeal : public CDrawBase { public: CDrawGuildSeal(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x90 - 0x28]; };   // 0x00651904
class CDrawQuickAccessIcons : public CDrawBase { };
class CDrawQuickAccessIconsPC : public CDrawQuickAccessIcons { public: CDrawQuickAccessIconsPC(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0xa0 - 0x28]; };   // 0x00657D60
class CDrawScriptScreenText : public CDrawBase { public: CDrawScriptScreenText(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064DA6E
class CDrawTargetInfo : public CDrawBase { public: CDrawTargetInfo(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x60 - 0x28]; };   // 0x0064DAB4
class CDrawHighlightingTargetingCursor : public CDrawBase { public: CDrawHighlightingTargetingCursor(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x48 - 0x28]; };   // 0x006542A4
class CDrawWorldTime : public CDrawBase { public: CDrawWorldTime(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x006542F7
class CDrawAbilityButtons : public CDrawBase { };
class CDrawAbilityButtonsPC : public CDrawAbilityButtons { public: CDrawAbilityButtonsPC(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x54 - 0x28]; };   // 0x006579B0
class CDrawLetterBoxBlackBars : public CDrawBase { public: CDrawLetterBoxBlackBars(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064E906
class CDrawBettingHUD : public CDrawBase { public: CDrawBettingHUD(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064E92C
class CDrawProjectileTargetingCentre : public CDrawBase { public: CDrawProjectileTargetingCentre(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064E1BE
class CDrawMeleeActionButtonEffects : public CDrawBase { public: CDrawMeleeActionButtonEffects(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x48 - 0x28]; };   // 0x00654258
class CDrawSpiritScores : public CDrawBase { public: CDrawSpiritScores(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); };   // 0x0064D389
class CDrawSpiritTargetingCursor : public CDrawBase { public: CDrawSpiritTargetingCursor(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x34 - 0x28]; };   // 0x0064DD4D
class CDraw2DText : public CDrawBase { public: CDraw2DText(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x2c - 0x28]; };   // 0x0064DA8A
class CDrawMouseCursor : public CDrawBase { public: CDrawMouseCursor(CPlayerGui& gui, std::vector<CDrawBase*>& illustrators); unsigned char _body[0x30 - 0x28]; };   // 0x00658240
}
using namespace NPlayerGui;

// Ego_r.pdb / FableWin.pdb ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ, out of line at 0x00438570 (same TU; expanded in this ctor).
const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()
{
    if (PGuiDef.IsNull())
        CGameDefinitionManager::Get().GetDef("PLAYER_GUI_PC", &PGuiDef);
    return *reinterpret_cast<const CPlayerGuiDef*>(PGuiDef.InternalGetObject());   // header debt: CPlayerGuiDef.h flattens its CDefClassBase base
}

CPlayerGui::CPlayerGui(const CMainGameComponent& component, CWorld& world, CGamePlayerInterface& inputInterface,
                       CPlayerManager& playerManager, const CGraphicDataBank& graphics)
    : World(&world), PlayerManager(&playerManager), InputInterface(&inputInterface), Component(&component), Graphics(&graphics),
      SubtitleRenderer(graphics), PFont(PFontManager->GetFont(g_CCharString_013b8788)),
      ShowSpecialAbilityButtons(false), ShowSpecialAbilityButtonsAbilities(true), SpecialSelected(false), SpecialDeselected(false),
      AbilityButtonScale(0.0f), NormalToSpecial(false), SpecialToNormal(false),
      DrawGui(true), OldDrawGui(true), GuiMode((EPlayerGuiMode)0), FadeInTimer(0.0f), ForcedClosed(false), ShowTime(true),
      ShowMoneyBag(false), Alpha(1.0f), HeroCentreUID(0), DrawingSafeAreas(false), DrawingDebugTGA(false),
      ScreenMessagingEnabled(true), ForceAllowSubtitleToRender(false), LiveGuiMode(false), DisplayBottomHalfGUI(true)
{
    AbilityButtons.resize(PeekGuiDef().AbilityButtons.size());
    AbilityButtonsAlpha.resize(PeekGuiDef().AbilityButtons.size(), 1.0f);
    QuickAccessButtonsAlpha.resize(PeekGuiDef().AbilityButtons.size(), 1.0f);
    PHeroInfoScreensManager.reset(new NHeroInformationScreens::CManager(*World));
    Component->IsEditorActive();
    PMoney.Reset(new CStatBar(PeekGuiDef()));
    PRenown.Reset(new CStatBar(PeekGuiDef()));
    PMorality.Reset(new CStatBar(PeekGuiDef()));
    PExperience.Reset(new CStatBar(PeekGuiDef()));
    PCrime.Reset(new CCrimeStatBar(PeekGuiDef()));
    PStatUpdates.Reset(new CMultiStatBar(PeekGuiDef()));
    PAutoPickups.Reset(new CAutoPickupBar(PeekGuiDef()));
    PDrawHealth.Reset(new CDrawHealth(*this, Illustrators));
    PDrawStamina.Reset(new CDrawStamina(*this, Illustrators));
    PDrawCombatMultiplier.Reset(new CDrawCombatMultiplier(*this, Illustrators));
    PDrawSpiritsHealth.Reset(new CDrawSpiritsHealth(*this, Illustrators));
    PDrawMiniMap.Reset(new CDrawMiniMap(*this, Illustrators));
    PDrawScreenFade.Reset(new CDrawScreenFade(*this, Illustrators));
    PDrawMoney.Reset(new CDrawMoney(*this, Illustrators));
    PDrawRenown.Reset(new CDrawRenown(*this, Illustrators));
    PDrawMorality.Reset(new CDrawMorality(*this, Illustrators));
    PDrawExperience.Reset(new CDrawExperience(*this, Illustrators));
    PDrawCrime.Reset(new CDrawCrime(*this, Illustrators));
    PDrawReceivedItems.Reset(new CDrawReceivedItems(*this, Illustrators));
    PDrawFishingGame.Reset(new CDrawFishingGame(*this, Illustrators));
    PDrawStatUpdates.Reset(new CDrawStatUpdates(*this, Illustrators));
    PDrawAutoPickups.Reset(new CDrawAutoPickups(*this, Illustrators));
    PDrawPerceivers.Reset(new CDrawPerceivers(*this, Illustrators));
    PDrawDamage.Reset(new CDrawDamage(*this, Illustrators));
    PDrawQuestInfo.Reset(new CDrawQuestInfo(*this, Illustrators));
    PDrawGuildSeal.Reset(new CDrawGuildSeal(*this, Illustrators));
    PDrawScriptScreenText.Reset(new CDrawScriptScreenText(*this, Illustrators));
    PDrawTargetInfo.Reset(new CDrawTargetInfo(*this, Illustrators));
    PDrawHighlightingTargetingCursor.Reset(new CDrawHighlightingTargetingCursor(*this, Illustrators));
    PDrawWorldTime.Reset(new CDrawWorldTime(*this, Illustrators));
    PDrawLetterBoxBlackBars.Reset(new CDrawLetterBoxBlackBars(*this, Illustrators));
    PDrawBettingHUD.Reset(new CDrawBettingHUD(*this, Illustrators));
    PDrawProjectileTargetingCentre.Reset(new CDrawProjectileTargetingCentre(*this, Illustrators));
    PDrawMeleeActionButtonEffects.Reset(new CDrawMeleeActionButtonEffects(*this, Illustrators));
    PDrawSpiritScores.Reset(new CDrawSpiritScores(*this, Illustrators));
    PDrawSpiritTargetingCursor.Reset(new CDrawSpiritTargetingCursor(*this, Illustrators));
    PDraw2DText.Reset(new CDraw2DText(*this, Illustrators));
    PDrawMouseCursor.Reset(new CDrawMouseCursor(*this, Illustrators));
    PDrawNPCName.Reset(new CDrawNPCName(*this, Illustrators));
    PDrawAbilityButtons.Reset(new CDrawAbilityButtonsPC(*this, Illustrators));
    PDrawQuickAccessIcons.Reset(new CDrawQuickAccessIconsPC(*this, Illustrators));
}
