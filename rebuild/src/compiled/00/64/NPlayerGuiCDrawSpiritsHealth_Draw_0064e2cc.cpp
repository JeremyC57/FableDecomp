// NPlayerGui::CDrawSpiritsHealth::Draw at 0x0064E2CC.
// Leaf: ?Draw@CDrawSpiritsHealth@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_EXTENTS_BODY_PASS26
#define FABLE_RETAIL_C2DBOXF_EXTENTS_OWNER_SPEED_PASS26
#define FABLE_RETAIL_C2DBOXF_GETTRPOS_PASS30
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE

#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NPlayerGui_CDrawSpiritsHealth.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CGraphicDataBank.h"

void NPlayerGui::CDrawSpiritsHealth::Draw(const CEnginePrimitivePrepareInfo& info)
{
    SetAsDrawing(true);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    PrimitiveHandles.clear();
    PrimitiveHandles.resize(40);
    float health_bar_offset = 0.0f;
    long primitive_offset = 0;
    for (long c1 = 0; c1 < 4; ++c1) {
        const CThingPlayerCreature* pcreature = PlayerManager.GetPlayer(c1).GetPControlledCreature();
        if (!PlayerManager.GetPlayer(c1).IsMainPlayer() && IS_THING_ALIVE(pcreature)) {
            C2DVector health_bar_pos(health_bar_offset + 250.0f, 410.0f);
            float health_bar_middle_width = 0.0f;
            CEnginePrimitive2DSprite outline_left_glow(
                0, GFToScreen(health_bar_pos), ENGINE_2D_LAYER_3, gui_def.HeroSprintBarLeftGlowGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ADDITIVE, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset + 3], outline_left_glow, 0xc0);

            CEnginePrimitive2DSprite outline_left_primitive(
                0, GFToScreen(health_bar_pos), ENGINE_2D_LAYER_2, gui_def.HeroSprintBarLeftGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset + 4], outline_left_primitive, 0xc0);

            const CGraphicDataBank& graphic_bank = Bank;
            float left_graphic_width = (float)graphic_bank.GetGraphicFrameWidth(gui_def.HeroSprintBarLeftGraphic, 0);
            float left_graphic_height = (float)graphic_bank.GetGraphicFrameHeight(gui_def.HeroSprintBarLeftGraphic, 0);
            C2DVector middle_bar_pos = health_bar_pos + C2DVector(left_graphic_width, 0.0f);
            C2DBoxF outline_middle_area(middle_bar_pos, C2DExtentsF(health_bar_middle_width, left_graphic_height));
            CEnginePrimitive2DSprite outline_middle_glow(
                0, GFToScreen(outline_middle_area), ENGINE_2D_LAYER_3, gui_def.HeroSprintBarMiddleGlowGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ADDITIVE, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset + 5], outline_middle_glow, 0xc0);

            CEnginePrimitive2DSprite outline_middle_primitive(
                0, GFToScreen(outline_middle_area), ENGINE_2D_LAYER_2, gui_def.HeroSprintBarMiddleGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset + 6], outline_middle_primitive, 0xc0);

            CEnginePrimitive2DSprite outline_right_glow(
                0, GFToScreen(outline_middle_area.GetTRPos()), ENGINE_2D_LAYER_3, gui_def.HeroSprintBarRightGlowGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ADDITIVE, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset + 7], outline_right_glow, 0xc0);

            CEnginePrimitive2DSprite outline_right_primitive(
                0, GFToScreen(outline_middle_area.GetTRPos()), ENGINE_2D_LAYER_2, gui_def.HeroSprintBarRightGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset + 8], outline_right_primitive, 0xc0);

            float health_as_percentage = pcreature->GetHealthPercentage();
            C2DVector inner_bar_position = health_bar_pos + gui_def.HeroSprintBarInnerGraphicOffset;
            float inner_bar_width = (left_graphic_width - gui_def.HeroSprintBarInnerGraphicOffset.X) * 2.0f + health_bar_middle_width;
            C2DBoxF scaled_inner_bar(inner_bar_position, C2DExtentsF(health_as_percentage * inner_bar_width, left_graphic_height));
            CEnginePrimitive2DSprite scaled_inner_bar_sprite(
                0, GFToScreen(scaled_inner_bar), ENGINE_2D_LAYER_1, gui_def.HeroSprintBarInnerGraphic,
                0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[primitive_offset], scaled_inner_bar_sprite, 0xc0);

            health_bar_offset += 77.0f;
            primitive_offset += 10;
        }
    }
}