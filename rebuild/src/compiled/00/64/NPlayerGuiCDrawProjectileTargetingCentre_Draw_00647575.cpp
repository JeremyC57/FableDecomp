// NPlayerGui::CDrawProjectileTargetingCentre::Draw at 0x00647575.
// Leaf: ?Draw@CDrawProjectileTargetingCentre@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy
// Inline-budget: the handle release and smart-pointer destructor expand into the 93- and 114-byte cleanup paths, verified at both native addresses.
#define FABLE_RETAIL_DRAWPROJECTILECENTRE_PASS30
#define FABLE_RETAIL_PROJECTILE_HANDLE_SPEED_PASS30
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_MORPHING_SPRITE_DRAW_PASS29
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_PROJECTILE_COUNTED_DTOR_PASS30
#define FABLE_RETAIL_PROJECTILE_COUNTED_RELEASE_PASS30
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_FORCEINLINE
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
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYER_ISTARGETLOCKED
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
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_SUBTITLE_DRAW_PREPARE_PASS30

#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NPlayerGui_CDrawProjectileTargetingCentre.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCEnemy.h"
#include "engine/SubtitleDrawGlobals.h"

void NPlayerGui::CDrawProjectileTargetingCentre::Draw(const CEnginePrimitivePrepareInfo& info)
{
    std::vector<std::pair<long, C2DVector> > graphics;
    PrimitiveHandles[0] = CEnginePrimitiveHandle();
    PrimitiveHandles[1] = CEnginePrimitiveHandle();

    C2DVector target_centre(GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) * 0.5f,
                           GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) * 0.5f);
    bool draw_target = false;
    bool targeting_thing = false;
    if (GetPlayer().IsFirstPersonTargeting()) {
        CThing* pcontrolled = GetPlayer().GetPControlledCreature();
        const CTCTargetingBase* ptc_targ;
        if (IS_THING_ALIVE(pcontrolled) && pcontrolled->PeekTC(&ptc_targ)) {
            C3DVector pos;
            if (ptc_targ->GetTargetedPos(pos, info.Interpolate.Current.WFInterpolate)) {
                if (info.Engine.ConvertWorldPosToScreenPos(pos, target_centre))
                    draw_target = true;
                if (IS_THING_ALIVE(ptc_targ->GetPTargetedThing())) {
                    if (CTCEnemy::AreThingsEnemies(*pcontrolled, *ptc_targ->GetPTargetedThing()) || GetPlayer().IsTargetLocked())
                        targeting_thing = true;
                }
            }
        }
    }

    if (draw_target) {
        SetAsDrawing(true);
        const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
        const std::vector<long>& cross_graphics = targeting_thing ? gui_def.CrosshairGraphics : gui_def.CrosshairGreenGraphics;
        target_centre = C2DVector((float)(long)target_centre.X, (float)(long)target_centre.Y);
        C2DVector left_pos = target_centre + gui_def.CrosshairOffsets[0];
        CEnginePrimitive2DSprite sprite_left(
            0, GFToScreen(left_pos), ENGINE_2D_LAYER_1, cross_graphics[0], 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], sprite_left, 0xc0);

        C2DVector right_pos = target_centre + gui_def.CrosshairOffsets[1];
        CEnginePrimitive2DSprite sprite_right(
            0, GFToScreen(right_pos), ENGINE_2D_LAYER_1, cross_graphics[1], 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], sprite_right, 0xc0);

        C2DVector up_pos = target_centre + gui_def.CrosshairOffsets[2];
        CEnginePrimitive2DSprite sprite_up(
            0, GFToScreen(up_pos), ENGINE_2D_LAYER_1, cross_graphics[2], 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[2], sprite_up, 0xc0);

        C2DVector down_pos = target_centre + gui_def.CrosshairOffsets[3];
        CEnginePrimitive2DSprite sprite_down(
            0, GFToScreen(down_pos), ENGINE_2D_LAYER_1, cross_graphics[3], 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 255, 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[3], sprite_down, 0xc0);
    }
}