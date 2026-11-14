// CPlayerGui::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) -- per-frame HUD build: gamma / HUD alpha from the profile, the gui lighting to the engine, the gui timers, every NPlayerGui illustrator, the debug TGA sprite
// Grade: DIFFER(2613v2613) -- 17 hunks, all in the gui-lighting block 0x0043912C..0x004392E9: the three CRGBFloatColour locals get frame slots backlight 0x20 / ambient 0x30 / diffuse 0x40 (retail diffuse 0x20 / backlight 0x30 / ambient 0x40; identity-fixed: invariant under declaration order and names), and retail homes the fourth (R) channel int temp in a second slot [esp+18h] (the dead PLAYER_GUI_PC CCharString slot), ours reuses [esp+10h]. Rest of the function matches.
// Leaf: ?DrawPreparePrimitivesForEngine@CPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00439070, 2613 bytes, `ret 4`.
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
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
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

// Ego_r.pdb / FableWin.pdb ?GFVXToAX@@YIMM@Z / ?GFVYToAY@@YIMM@Z (retail 0x009E1E10 / 0x009E1E40, landed) and the virtual screen
// size doubles (0x013961E8 / 0x013961F0, rebuild/oracles/reloc_bindings.tsv VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT).
float __fastcall GFVXToAX(float x);
float __fastcall GFVYToAY(float y);
extern double VIRTUAL_SCREEN_WIDTH;
extern double VIRTUAL_SCREEN_HEIGHT;

// NPlayerGui illustrators with no shared header: Ego_r.pdb names (RTTI .?AV<Class>@NPlayerGui@@ : CDrawBase); only the members the
// row calls (Draw out of line at the retail addresses noted, ClearEffects, the two virtual Draws at slot 2).
namespace NPlayerGui {
class CDrawHealth : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };   // 0x00647AB4 / 0x00644644
class CDrawStamina : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };  // 0x0064CCB3 / 0x0064464C
class CDrawCombatMultiplier : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };            // 0x0064822A
class CDrawSpiritsHealth : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); };               // 0x0064E2CC
class CDrawMiniMap : public CDrawBase { public: void Update(const CEnginePrimitivePrepareInfo& info);                     // 0x0064401C
                                        void Draw(const CEnginePrimitivePrepareInfo& info, bool); };                      // 0x00648820
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

// Ego_r.pdb / FableWin.pdb ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ, out of line at 0x00438570 (same TU; expanded four times here).
// Text of the landed CPlayerGui ctor 0x0043B570.
const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()
{
    if (PGuiDef.IsNull())
        CGameDefinitionManager::Get().GetDef("PLAYER_GUI_PC", &PGuiDef);
    return *reinterpret_cast<const CPlayerGuiDef*>(PGuiDef.InternalGetObject());   // header debt: CPlayerGuiDef.h flattens its CDefClassBase base
}

void CPlayerGui::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)
{
    const CInterpolationInfo::CInterpolationInfoSet& set = World->IsPaused() ? info.Interpolate.PausedCurrent : info.Interpolate.Current;
    static long frameCount;
    if (frameCount <= 2 && ++frameCount == 2)
        CMainGameComponent::Get()->GetDisplayEngine().SetGammaRamp(CUserProfileManager::Get().GetGamma());
    Alpha = CUserProfileManager::Get().GetHUDAlpha();
    {
        CRGBFloatColour backlight(PeekGuiDef().BacklightColour);
        CRGBFloatColour diffuse(PeekGuiDef().DiffuseColour);
        CRGBFloatColour ambient(PeekGuiDef().AmbientColour);
        info.Engine.Set2DPrimitiveLightingParameters(PeekGuiDef().DiffuseVector, ambient, diffuse, backlight);
    }
    RenderUpdateGuiTimers(set);
    std::vector<CDrawBase*>::iterator end = Illustrators.end();
    for (std::vector<CDrawBase*>::iterator it = Illustrators.begin(); it != end; ++it)
        (*it)->SetAsDrawing(false);
    PDrawMiniMap->Update(info);
    if (GuiMode != 1 || ForceAllowSubtitleToRender)
        SubtitleRenderer.DrawPreparePrimitivesForEngine(info);
    if (GuiMode != 1) {
        PDrawScreenFade->Draw(info);
        if (Component->PeekDisplayEngine().IsLetterBoxMode())
            PDrawLetterBoxBlackBars->Draw(info);
        bool drawHud = true;
        if ((PDrawScreenFade->IsDrawing() && Component->PeekDisplayEngine().IsScreenFullyFadedOut())
            || SubtitleRenderer.IsInCutscene() || World->GetGameScriptInterface().IsInMovieSequence()
            || (SubtitleRenderer.IsDisplayingText() && SubtitleRenderer.IsFlagSet((ESubtitleFlag)0x28))
            || (Component->PeekDisplayEngine().IsLetterBoxMode() && SubtitleRenderer.IsDisplayingText())
            || PlayerManager->GetMainPlayer().GetCurrentMode() == PLAYER_MODE_FIRST_PERSON)
            drawHud = false;
        CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            if (PlayerManager->GetMainPlayer().GetCurrentMode() != PLAYER_MODE_FIRST_PERSON) {
                PDrawFishingGame->Draw(info);
                PDrawReceivedItems->Draw(info);
                if (PDrawQuestInfo->GetMiniGameType() == MINIGAME_ORACLE) {
                    PDrawQuickAccessIcons->Draw(info);
                    PDrawQuestInfo->Draw(info, true);
                }
                CTCQuickAccessMenu* menu;
                if (PlayerManager->GetMainPlayer().GetPControlledCreature()->GetTC(TCI_QUICK_ACCESS_MENU, &menu)) {
                    if (!SubtitleRenderer.IsDisplayingText() && !World->GetGameScriptInterface().IsInMovieSequence() && !PDrawFishingGame->IsDrawing() && drawHud)
                        menu->DrawPreparePrimitivesForRendering(info);
                    else
                        menu->Deactivate();
                }
                if (!SubtitleRenderer.IsDisplayingText()) {
                    if (!SubtitleRenderer.IsInCutscene() && !World->GetGameScriptInterface().IsInMovieSequence() && !PDrawFishingGame->IsDrawing()
                        && (PDrawQuestInfo->GetMiniGameType() == MINIGAME_NONE || PDrawQuestInfo->GetMiniGameType() == MINIGAME_TROPHY))
                        PDrawMiniMap->Draw(info, drawHud);
                    if (!Component->PeekDisplayEngine().IsScreenFadeOut()) {
                        if (PDrawQuestInfo->GetMiniGameType() != MINIGAME_NONE) {
                            if (PDrawQuestInfo->GetMiniGameType() == MINIGAME_ORACLE)
                                PDrawQuestInfo->Draw(info, true);
                            else
                                PDrawQuestInfo->DrawMiniGameInfo(info);
                        }
                        else if (!SubtitleRenderer.IsInCutscene() && !World->GetGameScriptInterface().IsInMovieSequence() && !PDrawFishingGame->IsDrawing())
                            PDrawQuestInfo->Draw(info, false);
                    }
                }
            }
            if (drawHud) {
                if (LiveGuiMode)
                    PDrawMouseCursor->Draw(info);
                if (DisplayBottomHalfGUI) {
                    PDrawNPCName->Draw(info);
                    if (DisplayBottomHalfGUI && PCrime->CurrentValue > 0.0f && !SubtitleRenderer.IsDisplayingText())
                        SetCrimeAsDisplaying();
                }
                PDrawHealth->Draw(info);
                if (!hero->IsChild())
                    PDrawStamina->Draw(info);
                if (DisplayBottomHalfGUI) {
                    PDrawExperience->Draw(info);
                    PDrawStatUpdates->Draw(info);
                    PDrawAutoPickups->Draw(info);
                    PDrawCombatMultiplier->Draw(info);
                    PDrawProjectileTargetingCentre->Draw(info);
                }
                if (PDrawMiniMap->IsDrawing()) {
                    if (ShowTime)
                        PDrawWorldTime->Draw(info);
                    PDrawPerceivers->Draw(info);
                }
                if (!SubtitleRenderer.IsDisplayingText() && DisplayBottomHalfGUI) {
                    if (!PDrawProjectileTargetingCentre->IsDrawing())
                        PDrawMoney->Draw(info);
                    PDrawRenown->Draw(info);
                    PDrawMorality->Draw(info);
                    PDrawCrime->Draw(info);
                    PDrawScriptScreenText->Draw(info);
                    PDrawAbilityButtons->Draw(info);
                    PDrawMeleeActionButtonEffects->Draw(info);
                    PDrawDamage->Draw(info);
                    if (!PDrawProjectileTargetingCentre->IsDrawing()) {
                        PDrawGuildSeal->Draw(info);
                        PDrawQuickAccessIcons->Draw(info);
                    }
                    PDrawTargetInfo->Draw(info);
                    PDrawHighlightingTargetingCursor->Draw(info);
                    if (PlayerManager->IsMultiplayerGameActive()) {
                        PDrawSpiritScores->Draw(info);
                        PDrawSpiritsHealth->Draw(info);
                        PDrawSpiritTargetingCursor->Draw(info);
                    }
                }
                if (ShowMoneyBag)
                    SetMoneyAsDisplaying();
                else {
                    CThing* target = PDrawHighlightingTargetingCursor->GetPTargetedThing();
                    if (IS_THING_ALIVE(target) && target->HasTC(TCI_SHOP_KEEPER))
                        SetMoneyAsDisplaying();
                    else if (hero->HasTC(TCI_VOLUME_CONTAINMENT_TRACKER)) {
                        const CTCVolumeContainmentTracker* tracker;
                        hero->PeekTC(TCI_VOLUME_CONTAINMENT_TRACKER, &tracker);
                        CThing* inside = tracker->GetPThingInside();
                        if (inside && inside->HasTC(TCI_SHOP))
                            SetMoneyAsDisplaying();
                    }
                }
            }
            else if (PDrawQuestInfo->GetMiniGameType() == MINIGAME_PICKPOCKET || PDrawQuestInfo->GetMiniGameType() == MINIGAME_PICKLOCK
                     || PDrawQuestInfo->GetMiniGameType() == MINIGAME_STEAL)
                PDrawHighlightingTargetingCursor->Draw(info);
            else
                PDrawHighlightingTargetingCursor->Clear();
        }
    }
    PDraw2DText->Draw(info);
    if (!PDrawHealth->IsDrawing()) PDrawHealth->ClearEffects();
    if (!PDrawStamina->IsDrawing()) PDrawStamina->ClearEffects();
    if (!PDrawAbilityButtons->IsDrawing()) PDrawAbilityButtons->ClearEffects();
    if (!PDrawQuickAccessIcons->IsDrawing()) PDrawQuickAccessIcons->ClearEffects();
    if (!PDrawMeleeActionButtonEffects->IsDrawing()) PDrawMeleeActionButtonEffects->ClearEffects();
    if (DrawingDebugTGA) {
        unsigned long height = DebugTGATexture.GetDimensions(0).Y;
        unsigned long width = DebugTGATexture.GetDimensions(0).X;
        C2DVector pos((GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) - (float)width) * 0.5f, (GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) - (float)height) * 0.5f);
        CEnginePrimitive2DSprite sprite(0, pos, ENGINE_2D_LAYER_0, DebugTGATexture, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0,
                                        0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(DebugTGAPrimitiveHandle, sprite, 0x40);
    }
    else
        DebugTGAPrimitiveHandle.Clear();
    std::for_each(Illustrators.begin(), Illustrators.end(), std::mem_fun(&CDrawBase::ClearPrimitivesIfNecessary));
}
