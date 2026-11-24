// 00647ab4 NPlayerGui::CDrawHealth::Draw
// Leaf: ?Draw@CDrawHealth@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: default. The original GUI translation unit optimises for size.
// Inline-budget: retail calls IS_THING_ALIVE at 0043DBF0. The secondary HasTC and bitset owners use their speed-built bodies.
// Inline-budget: the retail potion block copies the original colour inline. The original PeekTC lookups expand at all three sites.
// Fidelity: the two-component GFToScreen copy uses the same size mode as its original GUI owner, 00643BE2.
#define FABLE_RETAIL_NPLAYERGUI_CDRAWHEALTH_PASS28
#define FABLE_RETAIL_CDRAWHEALTH_ORIGINAL_EFFECTS_PASS28
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COPY_FORCEINLINE_PASS26
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_GETPLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CFontBank.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CTCHaste.h"
#include "engine/CTCInventory.h"
#include "engine/CRightHandedSet.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CParticleEmitter.h"
#include "engine/CGraphicDataBank.h"
#include "engine/C2DExtentsF.h"
#include "engine/NPlayerGui_CDrawHealth.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
#include "engine/NPlayerGui_Graphics.h"

CWideString __fastcall GFIntToWideString(long);

class CTCHeroStats;
namespace NPlayerGui {
void CDrawHealth::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* phero = GetPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(phero))
        return;

    SetAsDrawing(true);
    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    const CTCHeroStats* ptc_hero_stats;
    phero->PeekTC(TCI_HERO_STATS, &ptc_hero_stats);
    if (PreviousHealth == -1.0f)
        PreviousHealth = phero->GetHealth();
    LowHealthPulseScale = 1.0f;
    const C2DVector health_pos = gui_def.HealthBarPos + gui_def.HealthBarHealthOffset;
    CEnginePrimitive2DSprite health_sprite(0, GFToScreen(health_pos), ENGINE_2D_LAYER_2,
        gui_def.HealthBarHealthGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[3], health_sprite, 0xc0);

    long effect_index = 0;
    if (PreviousHealth < phero->GetHealth()) {
        HealthIncreaseFade.Open(1.0f, 0.0f, CRGBColour(0, 0, 0));
        effect_index = gui_def.HealthBarHealthIncreaseEffect;
    } else if (HealthIncreaseFade.IsOpening()) {
        effect_index = gui_def.HealthBarHealthIncreaseEffect;
    }
    HealthIncreaseFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    PreviousHealth = phero->GetHealth();
    if (phero->HasTC(TCI_BERSERK)) {
        effect_index = gui_def.HealthBarHealthBerserkEffect;
    } else if (phero->HasTC(TCI_HASTE)) {
        const CTCHaste* ptc_haste;
        phero->PeekTC(TCI_HASTE, &ptc_haste);
        if (const_cast<CTCHaste*>(ptc_haste)->GetActionSpeedMultiplier() > 1.0f)
            effect_index = gui_def.HealthBarHealthAdrenalineEffect;
    }
    if (effect_index == 0)
        effect_index = phero->GetHealthPercentage() < gui_def.HealthBarHealthLowPercentage
            ? gui_def.HealthBarHealthLowHealthEffect : gui_def.HealthBarHealthDefaultEffect;

    if (HealthIncreaseFade.IsOpening() && effect_index == gui_def.HealthBarHealthIncreaseEffect) {
        if (Effects[1] == 0) {
            const C2DVector health_effect_pos = health_pos + gui_def.HealthBarHealthIncreaseEffectOffset;
            Effects[1] = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(effect_index, health_effect_pos,
                    ENGINE_2D_LAYER_1, true, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
        }
    } else {
        if (Effects[1] != 0) {
            Effects[1].Get()->Kill();
            Effects[1] = 0;
        }
        if (effect_index != PreviousEffect) {
            ClearEffects();
            PreviousEffect = effect_index;
        }
    }

    const CTCInventory* ptc_inventory;
    if (phero->PeekTC(TCI_INVENTORY, &ptc_inventory)) {
        const long def_index = World.PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_RESURRECTION_PHIAL");
        if (def_index > 0) {
            const long num_potions = ptc_inventory->GetNumberOfItemsOfTypeInInventory(def_index);
            if (num_potions > 0) {
                // Fidelity: the reference preserves both retail operand orders for the potion position.
                const C2DVector& offset = gui_def.HealthBarResurrectionOffset;
                const C2DVector potion_pos = offset + gui_def.HealthBarPos;
                CRGBColour colour(gui_def.HealthBarResurrectionColour);
                colour.A = ScaleAlpha(255);
                const CFontBank& font = Component.PeekMainFont();
                PlayerGui.Draw2DText(C2DVector(GFAXToVX(potion_pos.X), GFAYToVY(potion_pos.Y)),
                    GFIntToWideString(num_potions), font, colour, 0.85f, ENGINE_2D_LAYER_1, false);
            }
        }
    }

    const CGraphicDataBank& graphic_bank = Bank;
    C2DVector health_bar_pos = gui_def.HealthBarPos;
    CEnginePrimitive2DSprite health_stamina_left_sprite(0, GFToScreen(health_bar_pos), ENGINE_2D_LAYER_5,
        gui_def.HealthBarLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[5], health_stamina_left_sprite, 0xc0);

    const float health_stamina_middle_height = (float)graphic_bank.GetGraphicFrameHeight(gui_def.HealthBarMiddleGraphic, 0);
    const float health_stamina_right_width = (float)graphic_bank.GetGraphicFrameWidth(gui_def.HealthBarRightGraphic, 0);
    health_bar_pos += gui_def.HealthBarMiddleOffset;
    float health_stamina_middle_length = gui_def.HealthBarMiddleLength - health_stamina_right_width;
    const float original_max_health = phero->PeekBaseDef().Health;
    const float current_max_health = phero->GetMaxHealth();
    health_stamina_middle_length *= current_max_health / original_max_health;
    C2DBoxF health_stamina_middle_box(health_bar_pos, C2DExtentsF(health_stamina_middle_length, health_stamina_middle_height));
    CEnginePrimitive2DSprite health_stamina_middle_sprite(0, GFToScreen(health_stamina_middle_box), ENGINE_2D_LAYER_7,
        gui_def.HealthBarMiddleGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[7], health_stamina_middle_sprite, 0xc0);
    health_bar_pos.X += health_stamina_middle_length;
    CEnginePrimitive2DSprite health_stamina_right_sprite(0,
        GFToScreen(health_bar_pos), ENGINE_2D_LAYER_7,
        gui_def.HealthBarRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[9], health_stamina_right_sprite, 0xc0);

    const float health_as_percentage = phero->GetHealthPercentage();
    C2DVector health_bar_position = gui_def.HealthBarPos + gui_def.HealthBarHealthInnerOffset;
    const float health_bar_width = (gui_def.HealthBarHealthInnerOffset.X - gui_def.HealthBarMiddleOffset.X
        + health_stamina_middle_length);
    const float health_bar_height = (float)graphic_bank.GetGraphicFrameHeight(gui_def.HealthBarHealthInnerGraphic, 0);
    C2DBoxF scaled_health_bar_box(health_bar_position, C2DExtentsF(health_as_percentage * health_bar_width, health_bar_height));
    CEnginePrimitive2DSprite health_bar_sprite(0, GFToScreen(scaled_health_bar_box), ENGINE_2D_LAYER_6,
        gui_def.HealthBarHealthInnerGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_DEFAULT,
        0.0f, 0.0f, ScaleAlpha(255), 255, 255, 255, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], health_bar_sprite, 0xc0);
}
}