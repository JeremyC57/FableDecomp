// 0048fab6 CSubtitleRenderer::DrawButtonText
// Leaf: ?DrawButtonText@CSubtitleRenderer@@AAE_NABVC2DVector@@_N@Z
// Flags: none
// Inline-budget: The native width wrapper calls GetVStringWidth while expanding the outer wrapper. The font width and its operands are checked in the row evidence.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26

#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DEXTENTSF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH_INLINE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_DRAW_BUTTON_TEXT_PASS29
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYER_RESPAWNDELAY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#include "engine/Direct3D9Types.h"
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontBank.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/SubtitleDrawGlobals.h"
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif

#include "engine/CFontManager.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#ifdef _MSC_VER
// The 16-byte native width accessor omits the frame pointer and calls GetVStringWidth.
#pragma optimize("y",on)
#endif
float CSubtitleRenderer::GetTextWidthFromText(const CWideString& text, const CFontBank& font) const
{
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
    return font.GetVStringWidth(text);
}
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma optimize("t",on)
#pragma optimize("y",on)
#endif
// The live-handle ShareData and Reset both call this release. The editor
// spells the outer scope-end destructor; its expanded body calls ReleaseData.
template<> inline void CCountedPointer<CFontBank>::ReleaseData()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
bool CSubtitleRenderer::DrawButtonText(const C2DVector& br_pos, bool drawn_box)
{
    C2DVector vbr_pos;
    GFACoordToVCoord(br_pos, vbr_pos);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    const CFontBank& font = *PFontManager->GetFont(gui_def.TextBoxButtonTextFontName);
    C2DVector cancel_pos(0.0f, vbr_pos.Y);
    C2DVector confirm_pos(0.0f, vbr_pos.Y);
    C2DVector third_pos(0.0f, vbr_pos.Y);
    if (drawn_box) {
        cancel_pos.Y += GFAYToVY(gui_def.TextBoxButtonInBoxOffsetY);
        confirm_pos.Y += GFAYToVY(gui_def.TextBoxButtonInBoxOffsetY);
        third_pos.Y += GFAYToVY(gui_def.TextBoxButtonInBoxOffsetY);
    } else {
        cancel_pos.Y += GFAYToVY(gui_def.TextBoxButtonOffsetY);
        confirm_pos.Y += GFAYToVY(gui_def.TextBoxButtonOffsetY);
        third_pos.Y += GFAYToVY(gui_def.TextBoxButtonOffsetY);
    }
    float cancel_text_width = GetTextWidthFromText(Button1Text, font);
    float confirm_text_width = GetTextWidthFromText(Button2Text, font);
    float third_text_width = GetTextWidthFromText(Button3Text, font);
    if (IsCutsceneActionModeEnabled())
        cancel_pos.X = GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X + gui_def.TextBoxTextAreaBRPos.X - 640.0f) + GFAXToVX(16.0f);
    else
        cancel_pos.X = GFAXToVX(16.0f) + vbr_pos.X - cancel_text_width;
    confirm_pos.X = cancel_pos.X - confirm_text_width - GFAXToVX(gui_def.TextBoxButtonGapWidth);
    third_pos.X = confirm_pos.X - third_text_width - GFAXToVX(gui_def.TextBoxButtonGapWidth);
    CRGBColour text_colour = gui_def.TextBoxTextColour;
    bool drawn = false;
    bool drawn_button_1 = false;
    if (IsCutsceneActionModeEnabled()) {
        drawn = true;
    } else if (IsFlagSet((ESubtitleFlag)4)) {
        text_colour.A = Button1Alpha;
        Draw2DText(cancel_pos, Button1Text, font, text_colour, true, 1.0f, 1.0f, false);
        drawn_button_1 = true;
        text_colour.A = Button2Alpha;
        Draw2DText(confirm_pos, Button2Text, font, text_colour, true, 1.0f, 1.0f, false);
        text_colour.A = 0xff;
        Draw2DText(third_pos, Button3Text, font, text_colour, true, 1.0f, 1.0f, false);
        drawn = true;
    } else if ((IsInCutscene() || Button2TextOverridden) && !(IsFlagSet((ESubtitleFlag)0x40) || IsFlagSet((ESubtitleFlag)8))) {
        if (IsCutsceneSkippable() || Button2TextOverridden) {
            CWideString button_2_override_text(Button2OverrideText);
            if (button_2_override_text != L"" && !IsCutsceneActionModeEnabled()) {
                button_2_override_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CONTROL_ESC_ABBREVIATED") + L" - " + button_2_override_text;
            }
            // Retail expands the two width accessors here and calls extents at 0048FDD8.
            cancel_pos.X = vbr_pos.X - font.GetVStringExtents(button_2_override_text).X;
            text_colour.A = Button2Alpha;
            float scale = 1.0f;
            if (IsInCutscene()) {
                text_colour.A = 0xd8;
                cancel_pos.X += GFAXToVX(55.0f);
                cancel_pos.Y = cancel_pos.Y - GFAXToVX(2.0f);
                scale = 0.75f;
            }
            if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType() == MINIGAME_FISHING)
                cancel_pos.X += GFAXToVX(55.0f);
            Draw2DText(cancel_pos, button_2_override_text, font, text_colour, true, 1.0f, scale, false);
            drawn = true;
        }
    } else if (IsFlagSet((ESubtitleFlag)2)) {
        if ((!IsInCutscene() && IsTextReadyForClickPast()) || (IsFlagSet((ESubtitleFlag)0x40) || IsFlagSet((ESubtitleFlag)8))) {
            text_colour.A = Button1Alpha;
            Draw2DText(cancel_pos, Button1Text, font, text_colour, true, 1.0f, 1.0f, false);
            drawn = true;
            drawn_button_1 = true;
        }
    } else if (!IsInCutscene()) {
        const CPlayer& player_hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();
        if (player_hero.GetPlayerRespawnDelay() == 0) {
            if (Button1Text != L"") {
                text_colour.A = Button1Alpha;
                Draw2DText(cancel_pos, Button1Text, font, text_colour, true, 1.0f, 1.0f, false);
                drawn = true;
                drawn_button_1 = true;
            }
            if (Button2Text != L"") {
                text_colour.A = Button2Alpha;
                Draw2DText(confirm_pos, Button2Text, font, text_colour, true, 1.0f, 1.0f, false);
                drawn = true;
            }
            if (Button3Text != L"") {
                text_colour.A = 0xff;
                Draw2DText(third_pos, Button3Text, font, text_colour, true, 1.0f, 1.0f, false);
                drawn = true;
            }
        }
    }
    if (!drawn_button_1 && Button1TextOverridden) {
        confirm_pos.X = cancel_pos.X - GetTextWidthFromText(Button1OverrideText, font) - GFAXToVX(gui_def.TextBoxButtonGapWidth);
        if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType() == MINIGAME_FISHING) {
            C2DVector top_right_pos = NPlayerGui::GFAnchorToTopRight(gui_def.TextBoxFishingReelTextPos);
            confirm_pos.X = GFAXToVX(top_right_pos.X);
            confirm_pos.Y = GFAXToVX(top_right_pos.Y);
            float actual_width = GetTextWidthFromText(Button1OverrideText, font);
            if ((float)(confirm_pos.X + actual_width) > GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X)) {
                float diff = confirm_pos.X + actual_width - GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X);
                confirm_pos.X -= diff + GFAXToVX(4.0f);
            }
        }
        text_colour.A = Button1Alpha;
        Draw2DText(confirm_pos, Button1OverrideText, font, text_colour, true, 1.0f, 1.0f, false);
        drawn = true;
        drawn_button_1 = true;
    }
    return drawn;
}