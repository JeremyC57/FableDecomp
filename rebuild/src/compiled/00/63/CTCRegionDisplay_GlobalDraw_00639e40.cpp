// 00639e40 CTCRegionDisplay::GlobalDraw
// Leaf: ?GlobalDraw@CTCRegionDisplay@@QBEXABVCInterpolationInfo@@@Z
// Flags: none
#define FABLE_RETAIL_CFONTBANK_VFONTMAXHEIGHT_PAREN
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_REGION_DRAW_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DEXTENTSF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCREGIONDISPLAY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGIONLOAD
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISPERFORMINGREGIONLOADREALLY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_DRAWTEXT
#include "engine/Direct3D9Types.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CFontBank.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CTCRegionDisplay.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CInterpolationInfo.h"
#include "engine/CDisplayManager.h"
#include "engine/NInventory_CTCInventoryBase.h"
void CTCRegionDisplay::GlobalDraw(const CInterpolationInfo& interp) const
{
    if (!IsDrawing() || !CPlayerGui::Get().GetDrawGuiFlag()) return;
    long current_alpha = (long)GFGetInterpolatedValue((float)LastAlpha, (float)CurrentAlpha, interp.Current.WFInterpolate);
    if (!(CurrentAreaName != L"" || CurrentAreaNameExtra != L"") || current_alpha <= 0) return;
    if (CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen() && !PeekWorld().IsPerformingRegionLoadReally()) return;
    if (CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) return;
    const CTCQuestCompletionUI* ptc_quest_ui = 0;
    GetParentThing().PeekTC(&ptc_quest_ui);
    if (ptc_quest_ui && ptc_quest_ui->IsOpen()) return;
    const CTCInGameMenu* ptc_in_game_menu = 0;
    GetParentThing().PeekTC(&ptc_in_game_menu);
    if (ptc_in_game_menu && ptc_in_game_menu->IsOpen()) return;
    if (InfoDisplay) {
        if (CMainGameComponent::Get()->GetDisplayEngine().IsScreenFadeOut()) return;
        const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
        const CFontBank& font=PeekComponent().PeekMainFont();
        C2DVector display_pos(gui_def.InfoDisplayPos);
        float x=GFAXToVX(display_pos.X)-(font.GetVStringWidth(CurrentAreaName));
        float y=GFAYToVY(display_pos.Y)-(font.GetVFontMaxHeight()/2.0f);
        NInventory::CTCInventoryBase::Draw2DText(C2DVector(x,y), CurrentAreaName, font, CRGBColour(255,255,255,(unsigned char)current_alpha), 1.0f, true);
    } else {
        if (CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) return;
        const CPlayerGuiDef& gui_def=CPlayerGui::PeekGuiDef();
        const CFontBank& font=PeekComponent().PeekCutSceneMainFont();
        C2DVector display_pos(gui_def.RegionDisplayPos);
        display_pos.X=GPDisplayManager->GetRenderTargetDimensions().X/2.0f;
        display_pos.Y=GPDisplayManager->GetRenderTargetDimensions().Y-(480.0f-display_pos.Y);
        float x=GFAXToVX(display_pos.X)-font.GetVStringWidth(CurrentAreaName)/2.0f;
        float y=GFAYToVY(display_pos.Y)-(font.GetVFontMaxHeight()/2.0f);
        NInventory::CTCInventoryBase::Draw2DText(C2DVector(x,y), CurrentAreaName, font, CRGBColour(255,255,255,(unsigned char)current_alpha), 1.0f, true);
        if (CurrentAreaNameExtra != L"") {
            display_pos.Y+=font.GetFontMaxHeight();
            x=GFAXToVX(display_pos.X)-font.GetVStringWidth(CurrentAreaNameExtra)/2.0f;
            y=GFAYToVY(display_pos.Y)-(font.GetVFontMaxHeight()/2.0f);
            NInventory::CTCInventoryBase::Draw2DText(C2DVector(x,y), CurrentAreaNameExtra, font, CRGBColour(255,255,255,(unsigned char)current_alpha), 1.0f, true);
        }
    }
}