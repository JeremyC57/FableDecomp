// 00649952 NPlayerGui::CDrawBettingHUD::Draw
// Leaf: ?Draw@CDrawBettingHUD@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy-
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_REGION_DRAW_PASS29
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CDRAWBETTINGHUD_PASS30
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_COMPONENT_QUERY_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCAISCRATCHPAD_BETTING
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_DRAW_BUTTON_TEXT_PASS29

#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NPlayerGui_CDrawBettingHUD.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCGamePlayer.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CFontBank.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/SubtitleDrawGlobals.h"
#include "engine/NPlayerGuiCombatDraw.inl"

void NPlayerGui::CDrawBettingHUD::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
    const CTCGamePlayer* tc_game_player;
    phero->PeekTC(&tc_game_player);
    // Evaluating the original assertion retains the game query in retail.
    (void)((tc_game_player->GetGame()!=0 && tc_game_player->GetGame()->IsBetting()) ||
           CTCAIScratchpad::BettingActive);

    SetAsDrawing(true);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    C2DVector base_pos((float)GPDisplayManager->GetRenderTargetDimensions().X * 0.5f,
                       (float)GPDisplayManager->GetRenderTargetDimensions().Y * gui_def.TextBoxBettingGraphicPos.Y);
    C2DVector pos;
    C2DVector graphic_size;
    graphic_size.X = (float)Bank.GetGraphicMaxWidth(gui_def.MiniGameBettingGraphic);
    graphic_size.Y = (float)Bank.GetGraphicMaxHeight(gui_def.MiniGameBettingGraphic);
    pos = base_pos - graphic_size * 0.5f;
    CEnginePrimitive2DSprite betting_sprite(
        0, GFToScreen(pos), ENGINE_2D_LAYER_3, gui_def.MiniGameBettingGraphic, 0,
        ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
        ScaleAlpha(255), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], betting_sprite, 0xc0);

    const CFontBank& font = Component.PeekMainFont();
    static float font_size = 0.8f;
    CWideString text = Component.GetTextBank().GetTextBySymbol(
        CTCAIScratchpad::BettingActive ? "TEXT_GUI_DONATING_ARROW_UP" : "TEXT_GUI_BETTING_ARROW_UP");
    pos = base_pos + gui_def.TextBoxBettingUpOffset;
    pos.X -= font.GetStringWidth(text) * font_size * 0.5f;
    pos.Y -= font.GetFontMaxHeight() * font_size + graphic_size.Y * 0.5f;
    C2DVector vtext_pos;
    GFACoordToVCoord(pos, vtext_pos);
    PlayerGui.Draw2DText(vtext_pos, text, font, CRGBColour(255, 255, 255, ScaleAlpha(255)),
                         font_size, ENGINE_2D_LAYER_0, true);

    text = Component.GetTextBank().GetTextBySymbol(
        CTCAIScratchpad::BettingActive ? "TEXT_GUI_DONATING_ARROW_DOWN" : "TEXT_GUI_BETTING_ARROW_DOWN");
    pos = base_pos + gui_def.TextBoxBettingDownOffset;
    pos.X -= font.GetStringWidth(text) * font_size * 0.5f;
    pos.Y += graphic_size.Y * 0.5f;
    GFACoordToVCoord(pos, vtext_pos);
    PlayerGui.Draw2DText(vtext_pos, text, font, CRGBColour(255, 255, 255, ScaleAlpha(255)),
                         font_size, ENGINE_2D_LAYER_0, true);

    text = Component.GetTextBank().GetTextBySymbol(
        CTCAIScratchpad::BettingActive ? "TEXT_GUI_DONATING_ARROW_LEFT" : "TEXT_GUI_BETTING_ARROW_LEFT");
    pos = base_pos + gui_def.TextBoxBettingLeftOffset;
    pos.X -= font.GetStringWidth(text) * font_size + graphic_size.X * 0.5f;
    pos.Y -= font.GetFontMaxHeight() * font_size * 0.5f;
    GFACoordToVCoord(pos, vtext_pos);
    PlayerGui.Draw2DText(vtext_pos, text, font, CRGBColour(255, 255, 255, ScaleAlpha(255)),
                         font_size, ENGINE_2D_LAYER_0, true);

    text = Component.GetTextBank().GetTextBySymbol(
        CTCAIScratchpad::BettingActive ? "TEXT_GUI_DONATING_ARROW_RIGHT" : "TEXT_GUI_BETTING_ARROW_RIGHT");
    pos = base_pos + gui_def.TextBoxBettingRightOffset;
    pos.X += graphic_size.X * font_size * 0.5f;
    pos.Y -= font.GetFontMaxHeight() * font_size * 0.5f;
    GFACoordToVCoord(pos, vtext_pos);
    PlayerGui.Draw2DText(vtext_pos, text, font, CRGBColour(255, 255, 255, ScaleAlpha(255)),
                         font_size, ENGINE_2D_LAYER_0, true);
}