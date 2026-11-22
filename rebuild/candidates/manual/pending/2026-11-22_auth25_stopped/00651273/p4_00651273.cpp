// 0x00651273 NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::DrawAt -- draw the mini-game progress bar (left / middle / right frame, the coloured fill) and its optional icons; returns the bar height.
// Leaf: ?DrawAt@CQuestInfoElementMiniGameBar@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x00651273, 1681 bytes, thiscall.
// Flags: none
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUIDEF_MINIGAME_OFFSET
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_CORNERPOS
#define FABLE_RETAIL_C2DBOXF_TRPOS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_compiler.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NPlayerGui_Graphics.h"
using namespace NPlayerGui;
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::DrawAt(const C2DVector& pos,const CEnginePrimitivePrepareInfo& info)
{
    const CPlayerGuiDef& gui_def = CPlayerGui::Get().PeekGuiDef();
    PrimitiveHandles.resize(6);

    const CGraphicDataBank& graphic_bank = *CPlayerGui::Get().GetQuestInfo().Bank;
    float left_graphic_width = (float)graphic_bank.GetGraphicFrameWidth(gui_def.HeroSprintBarLeftGraphic, 0);
    float left_graphic_height = (float)graphic_bank.GetGraphicFrameHeight(gui_def.HeroSprintBarLeftGraphic, 0);

    float height = 16.0f * Scale;
    const float inner_width = 48.0f;

    C2DVector draw_pos(pos);


    if (Graphic > 0)
    {
        float sprite_width = (float)graphic_bank.GetGraphicFrameWidth(Graphic, 0);
        float sprite_height = (float)graphic_bank.GetGraphicFrameHeight(Graphic, 0);
        if (sprite_height > height)
        {
            sprite_height = height;
        }

        C2DVector sprite_pos(draw_pos.X - (long)(sprite_width * 0.5f), draw_pos.Y - (long)(sprite_height * (1.0f / 3.0f)));
        CEnginePrimitive2DSprite left_sprite(0, GFToScreen(sprite_pos), ENGINE_2D_LAYER_1, Graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[4], left_sprite, 0xc0);
    }

    // left
    C2DVector left_pos(draw_pos.X, draw_pos.Y - (Scale * 12.0f));
    C2DBoxF left_box(left_pos, C2DExtentsF(left_graphic_width, left_graphic_height * Scale));

    CEnginePrimitive2DSprite left_bar_sprite(0, GFToScreen(left_box), ENGINE_2D_LAYER_3, gui_def.HeroSprintBarLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], left_bar_sprite, 0xc0);

    // middle
    C2DVector middle_pos = left_pos + C2DVector(left_graphic_width, 0.0f);
    C2DBoxF middle_box(middle_pos, C2DExtentsF(inner_width, left_graphic_height * Scale));

    CEnginePrimitive2DSprite middle_bar_sprite(0, GFToScreen(middle_box), ENGINE_2D_LAYER_3, gui_def.HeroSprintBarMiddleGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], middle_bar_sprite, 0xc0);

    // right
    C2DBoxF right_box(middle_box.GetTRPos(), C2DExtentsF(left_graphic_width, left_graphic_height * Scale));

    CEnginePrimitive2DSprite right_bar_sprite(0, GFToScreen(right_box), ENGINE_2D_LAYER_3, gui_def.HeroSprintBarRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[2], right_bar_sprite, 0xc0);

    // inner
    float value_as_percentage = Percentage;

    C2DVector inner_pos = left_pos + gui_def.HeroSprintBarInnerGraphicOffset;
    float inner_bar_width = ((left_graphic_width - gui_def.HeroSprintBarInnerGraphicOffset.X) * 2.0f + inner_width);
    C2DBoxF inner_box(inner_pos, C2DExtentsF(inner_bar_width * value_as_percentage, left_graphic_height * Scale));

    CRGBColour interp_colour(Colour);
    if (Type == MINIGAME_PICKPOCKET || Type == MINIGAME_PICKLOCK || Type == MINIGAME_STEAL)
    {
        interp_colour.R = (unsigned char)(Colour.R * value_as_percentage + FadeColour.R * (1.0f - value_as_percentage));
        interp_colour.G = (unsigned char)(Colour.G * value_as_percentage + FadeColour.G * (1.0f - value_as_percentage));
        interp_colour.B = (unsigned char)(Colour.B * value_as_percentage + FadeColour.B * (1.0f - value_as_percentage));
        interp_colour.A = (unsigned char)(Colour.A * value_as_percentage + FadeColour.A * (1.0f - value_as_percentage));
    }

    CEnginePrimitive2DSprite inner_bar_sprite(0, GFToScreen(inner_box), ENGINE_2D_LAYER_2, gui_def.HeroSprintBarInnerGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(interp_colour.A), interp_colour.R, interp_colour.G, interp_colour.B, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[3], inner_bar_sprite, 0xc0);

    if (Type == MINIGAME_FISHING)
    {
        if (value_as_percentage > DELTA_NOTIONAL_ZERO)
        {

            long graphic2 = gui_def.MiniGameFishingRightGraphic;
            if (graphic2 > 0)
            {
                float sprite_width = (float)graphic_bank.GetGraphicFrameWidth(graphic2, 0);
                float sprite_height = (float)graphic_bank.GetGraphicFrameHeight(graphic2, 0);
                if (sprite_height > height)
                {
                    sprite_height = height;
                }

                C2DVector sprite_pos(draw_pos.X, draw_pos.Y - (long)(sprite_height * (1.0f / 3.0f)));
                sprite_pos.X = (float)(long)(inner_bar_width * value_as_percentage + inner_pos.X);

                CEnginePrimitive2DSprite right_sprite(0, GFToScreen(sprite_pos), ENGINE_2D_LAYER_1, graphic2, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Engine.SetPersistentPrimitive(PrimitiveHandles[5], right_sprite, 0xc0);
            }
        }
    }

    return height;
}
