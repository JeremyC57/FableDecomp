// 0064fdb4 NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt -- draw a quest status bar, target icons and interpolated fill.
// Inline-budget: native 00650781..006507b4 expands Clear; plain size mode calls it. The larger budget expands the original0043dbf0 predicate too, so only its0064fe09 call site is scoped back to an out-of-line call.
// Leaf: ?DrawAt@CQuestInfoElementBar@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0064fdb4,2593 bytes,thiscall.
// Flags: /Os /d2inlS200
// Retail-asm: 0x0064FF92..0x0064FFAC `fld [ebp+64h] ; fsub [ebp+60h] ; fistp dword ptr [ebp+78h] ; fild [ebp+78h] ; fadd [ebp+48h] ; fstp ; mov ; cmp ; jne ; inc` (11 fistp sites in the leaf) -- the engine's inline-assembly GFFloatToLongFloor (engine/GFFloatToLongFloor.h) expanded at each call.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_SPEED_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
// Original floor conversion: native dword stores at 0064ff98,0064ffd3,006500c1,006500f3,006501d1,0065020c,0065024a,0065027c,00650359,0065043c,00650544.
// The existing shared helper preserves the original rounding and integer-bit equality fixup.
#include "engine/GFFloatToLongFloor.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);
// Actual null/dead predicate at0043dbf0; its visible body proves the pointer reference does not escape.
template <>
bool FABLE_FASTCALL IS_THING_ALIVE<const CThingPlayerCreature*>(const CThingPlayerCreature* const& thing) {
    if (thing && !thing->IsDead())
        return true;
    return false;
}
template <class T>
bool FABLE_FASTCALL operator==(const CBaseObject* item, const CIntelligentPointer<T>& pointer);
namespace NPlayerGui {
// Same-TU CQuestInfoElementTimer::DrawAt and CQuestInfoElementCounter::DrawAt use this conversion.
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box) {
    return box;
}
}
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt(const C2DVector& pos,
                                                               const CEnginePrimitivePrepareInfo& info) {
    if (ThingValid) {
        if (!IS_THING_ALIVE(PThing) || PThing->IsInLimbo())
            return 0.0f;
    }
    const CThingPlayerCreature* phero =
        CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (IS_THING_ALIVE(phero)) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        const CPlayerGuiDef& gui_def = CPlayerGui::Get().PeekGuiDef();
        PrimitiveHandles.resize(6);
        const CGraphicDataBank& graphic_bank = CPlayerGui::Get().PeekGraphicBank();
        float bar_sprite_width = (float)graphic_bank.GetGraphicMaxWidth(gui_def.HeroSprintBarLeftGraphic);
        float bar_sprite_height = (float)graphic_bank.GetGraphicMaxHeight(gui_def.HeroSprintBarLeftGraphic);
        float max_graphic_height = bar_sprite_height;
        float left_sprite_width = 0.0f, left_sprite_height = 0.0f;
        if (Graphic1 > 0) {
            left_sprite_width = (float)graphic_bank.GetGraphicMaxWidth(Graphic1);
            left_sprite_height = (float)graphic_bank.GetGraphicMaxHeight(Graphic1);
            max_graphic_height = std::max(max_graphic_height, left_sprite_height);
        }
        float right_sprite_width = 0.0f;
        float right_sprite_height = 0.0f;
        if (Graphic2 > 0) {
            right_sprite_width = (float)graphic_bank.GetGraphicMaxWidth(Graphic2);
            right_sprite_height = (float)graphic_bank.GetGraphicMaxHeight(Graphic2);
            max_graphic_height = std::max(max_graphic_height, right_sprite_height);
        }
        float total_bar_width = gui_def.QuestInfoGraphicWidth - 6.0f;
        C2DVector left_sprite_pos(pos), bar_tl_pos(pos), right_sprite_pos(pos);
        if (Graphic1 > 0) {
            total_bar_width -= GFFloatToLongFloor(left_sprite_width * Scale);
            bar_tl_pos.X += GFFloatToLongFloor(left_sprite_width * Scale);
            float left_sprite_scaled_width = left_sprite_width,
                  left_sprite_scaled_height = left_sprite_height;
            EEngine2DLayer layer = ENGINE_2D_LAYER_6;
            if (IS_THING_ALIVE(PThing)) {
                const CTCTargetingBase* ptc_targeting;
                phero->PeekTC(&ptc_targeting);
                if (ptc_targeting->GetPTargetedThing() == PThing) {
                    layer = ENGINE_2D_LAYER_5;
                    left_sprite_scaled_width *= 1.2f;
                    left_sprite_scaled_height *= 1.2f;
                    left_sprite_pos.X -= left_sprite_scaled_width * 0.1f;
                    left_sprite_pos.Y -= left_sprite_scaled_height * 0.1f;
                }
            }
            C2DBoxF left_sprite_box(
                left_sprite_pos, C2DExtentsF((float)GFFloatToLongFloor(left_sprite_scaled_width * Scale),
                                             (float)GFFloatToLongFloor(left_sprite_scaled_height * Scale)));
            CEnginePrimitive2DSprite left_sprite(
                0, GFToScreen(left_sprite_box), layer, Graphic1, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255,
                255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], left_sprite, 0xc0);
        }
        if (Graphic2 > 0) {
            total_bar_width -= GFFloatToLongFloor(right_sprite_width * Scale);
            right_sprite_pos.X += GFFloatToLongFloor(left_sprite_width * Scale) + total_bar_width;
            C2DBoxF right_sprite_box(right_sprite_pos,
                                     C2DExtentsF((float)GFFloatToLongFloor(right_sprite_width * Scale),
                                                 (float)GFFloatToLongFloor(left_sprite_height * Scale)));
            CEnginePrimitive2DSprite right_sprite(
                0, GFToScreen(right_sprite_box), ENGINE_2D_LAYER_6, Graphic2, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f, CPlayerGui::Get().ScaleAlpha(255), 255, 255,
                255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[1], right_sprite, 0xc0);
        }
        C2DVector inner_bar_tl_pos(bar_tl_pos);
        C2DBoxF bar_left_box(
            bar_tl_pos, C2DExtentsF(bar_sprite_width, (float)GFFloatToLongFloor(bar_sprite_height * Scale)));
        CEnginePrimitive2DSprite bar_left_sprite(
            0, GFToScreen(bar_left_box), ENGINE_2D_LAYER_6, gui_def.HeroSprintBarLeftGraphic, 0,
            ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
            CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f),
            C2DVector(0.0f, 0.0f));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[2], bar_left_sprite, 0xc0);
        bar_tl_pos.X += bar_sprite_width;

        C2DBoxF bar_middle_box(bar_tl_pos, C2DExtentsF(total_bar_width - bar_sprite_width * 2.0f,
                                                       (float)GFFloatToLongFloor(bar_sprite_height * Scale)));

        if (bar_middle_box.GetWidth() > DELTA_NOTIONAL_ZERO) {
            CEnginePrimitive2DSprite bar_middle_sprite(
                0, GFToScreen(bar_middle_box), ENGINE_2D_LAYER_6, gui_def.HeroSprintBarMiddleGraphic, 0,
                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
                CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f),
                C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[3], bar_middle_sprite, 0xc0);
        }
        bar_tl_pos.X += bar_middle_box.GetWidth();
        C2DBoxF bar_right_box(
            bar_tl_pos, C2DExtentsF(bar_sprite_width, (float)GFFloatToLongFloor(bar_sprite_height * Scale)));
        CEnginePrimitive2DSprite bar_right_sprite(
            0, GFToScreen(bar_right_box), ENGINE_2D_LAYER_6, gui_def.HeroSprintBarRightGraphic, 0,
            ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
            CPlayerGui::Get().ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f),
            C2DVector(0.0f, 0.0f));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[4], bar_right_sprite, 0xc0);
        float value_as_percentage = Value / (MaxValue - MinValue);
        if (value_as_percentage > DELTA_NOTIONAL_ZERO) {
            CRGBColour interp_colour;
            // Each channel blends the two weighted colours before conversion to a byte.
            interp_colour.R = (unsigned char)((Colour.R * value_as_percentage) +
                                              (FadeColour.R * (1.0f - value_as_percentage)));
            interp_colour.G = (unsigned char)((Colour.G * value_as_percentage) +
                                              (FadeColour.G * (1.0f - value_as_percentage)));
            interp_colour.B = (unsigned char)((Colour.B * value_as_percentage) +
                                              (FadeColour.B * (1.0f - value_as_percentage)));
            interp_colour.A = (unsigned char)((Colour.A * value_as_percentage) +
                                              (FadeColour.A * (1.0f - value_as_percentage)));
            inner_bar_tl_pos.X += 4.0f;
            total_bar_width -= 8.0f;
            C2DBoxF bar_inner_box(inner_bar_tl_pos, C2DExtentsF(total_bar_width * value_as_percentage,
                                                                bar_sprite_height * Scale));
            CEnginePrimitive2DSprite bar_inner_sprite(
                0, GFToScreen(bar_inner_box), ENGINE_2D_LAYER_5, gui_def.HeroSprintBarInnerGraphic, 0,
                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT, 0.0f, 0.0f,
                CPlayerGui::Get().ScaleAlpha(interp_colour.A), interp_colour.R, interp_colour.G,
                interp_colour.B, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[5], bar_inner_sprite, 0xc0);
        } else
            PrimitiveHandles[5].Clear();
        return max_graphic_height * Scale + 2.0f;
    }
    return 0.0f;
}
