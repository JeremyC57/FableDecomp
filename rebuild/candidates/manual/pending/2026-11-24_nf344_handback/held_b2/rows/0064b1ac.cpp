// NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities
// Leaf: ?UpdateWeaponAbilities@CDrawAbilityButtons@NPlayerGui@@IAEXXZ
// Flags: /Os /Oy-
// (NF344, 2026-11-24) /d2inlT59 dropped: on the current headers the object is identical without it (every function COMDAT, bytes and relocations; NF340 /d2inl audit).
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_HASTC_SPEED_OWNER_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_THING_BITSET_SPEED_OWNER_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY
#define FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPLAYERGUI_GETINPUTINTERFACE
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_WEAPONUSE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVERANGED
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#include "fable_string.h"

// Fidelity: scaffolding, not retail. Emit the existing speed owner for
// 0x00410DE0 with its default /O2 /Oy mode and restore the caller mode.
#include "stlport/fable_containers.h"
#ifdef _MSC_VER
#pragma optimize("ty", on)
#endif
#include "stlport/fable_thing_bitset.h"
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

#include "engine/CThingPlayerCreature.h"

// Fidelity: scaffolding, not retail. The linked 0x004118C0 HasTC copy follows
// the default /O2 /Oy CThing owner flags, including speed mode and FPO.
#ifdef _MSC_VER
#pragma optimize("s", off)
#pragma optimize("ty", on)
#endif
inline bool CThing::HasTC(ETCInterfaceType type) const
{
    return UsingComponents.test(type);
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif


#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CEnginePrimitive2DViewport.h"
#include "engine/NPlayerGui_CDrawAbilityButtons.h"
#include "engine/CTCCombat.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CMeleeCombatAbilityDef.h"

void NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities()
{
    const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
    if (!IS_THING_ALIVE(phero))
        return;

    const CTCCombat* ptc_combat;
    phero->PeekTC(&ptc_combat);
    bool melee_weapon_allowed = false;
    bool ranged_weapon_allowed = false;
    bool block_allowed = false;
    const CTCInventoryWeapons* ptc_weapons_inventory;
    phero->PeekTC(&ptc_weapons_inventory);
    const CTCHero* ptc_hero;
    if (phero->PeekTC(&ptc_hero) && ptc_hero->IsWeaponUseAllowed() && ptc_weapons_inventory) {
        if (ptc_weapons_inventory->IsMeleeWeaponDrawn())
            melee_weapon_allowed = IS_THING_ALIVE(ptc_weapons_inventory->GetActiveMeleeWeapon());
        if (ptc_weapons_inventory->IsRangedWeaponDrawn())
            ranged_weapon_allowed = IS_THING_ALIVE(ptc_weapons_inventory->GetActiveRangedWeapon());
    }
    const CTCCarrying* ptc_carrying;
    if (phero->PeekTC(&ptc_carrying)) {
        const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
        if (pcarried_thing) {
            const CTCWeapon* ptc_weapon;
            if (pcarried_thing->PeekTC(&ptc_weapon) && ptc_weapon->IsUsableBy(*phero))
                block_allowed = true;
        } else {
            block_allowed = true;
        }
    }
    bool unarmed_attack_allowed = !melee_weapon_allowed && !ranged_weapon_allowed;
    if (phero->HasTC(TCI_BERSERK))
        block_allowed = false;

    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    const CGamePlayerInterface& player_interface = PlayerGui.GetInputInterface();
    for (long index = 0; index < gui_def.AbilityButtons.size(); ++index) {
        // Inferred: keep the original signed loop index and convert to the vector's size_type for subscripting.
        const std::vector<CAbilityButtonGuiDef>::size_type button_index = index;
        if (player_interface.IsButtonForGameEvent(gui_def.AbilityButtons[button_index].ControllerButton, GAME_ACTION_ATTACK) ||
            player_interface.IsMouseButtonForGameEvent(gui_def.AbilityButtons[button_index].MouseButton, GAME_ACTION_ATTACK)) {
            if (melee_weapon_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonMeleeGraphic);
            else if (ranged_weapon_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonRangedGraphic);
            else if (unarmed_attack_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonUnarmedGraphic);
        } else if (player_interface.IsButtonForGameEvent(gui_def.AbilityButtons[button_index].ControllerButton, GAME_ACTION_BLOCK) ||
                   player_interface.IsMouseButtonForGameEvent(gui_def.AbilityButtons[button_index].MouseButton, GAME_ACTION_BLOCK)) {
            if (block_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonBlockGraphic);
            else
                PlayerGui.SetAbilityButton(index, 0);
        } else if (player_interface.IsButtonForGameEvent(gui_def.AbilityButtons[button_index].ControllerButton, GAME_ACTION_SPECIAL_ATTACK) ||
                   player_interface.IsMouseButtonForGameEvent(gui_def.AbilityButtons[button_index].MouseButton, GAME_ACTION_SPECIAL_ATTACK)) {
            long flourish_override_graphic;
            if (ptc_combat->IsMeleeFlourishValid()) {
                const CDefPointer<const CMeleeCombatAbilityDef> pmelee_ability = ptc_combat->GetPMeleeFlourishAbility();
                flourish_override_graphic = pmelee_ability->AbilityButtonGraphicOverride;
            } else if (ptc_combat->IsMeleeBlockCounterValid()) {
                const CDefPointer<const CMeleeCombatAbilityDef> pmelee_ability = ptc_combat->GetPMeleeBlockCounterAbility();
                flourish_override_graphic = pmelee_ability->AbilityButtonGraphicOverride;
            } else {
                const CTCHeroRangedAttacks* ptc_ranged;
                if (phero->PeekTC(&ptc_ranged) && ptc_ranged->IsInProcessOfFiringRangedWeapon() && !GetPlayer().IsFirstPersonTargeting())
                    PlayerGui.SetAbilityButton(index, gui_def.ABXYTargetingEnterGraphic);
                else if (PlayerGui.IsDrawingTargetingCentre())
                    PlayerGui.SetAbilityButton(index, gui_def.ABXYTargetingCancelGraphic);
                else
                    PlayerGui.SetAbilityButton(index, gui_def.ABXYRunGraphic);
                continue;
            }
            PlayerGui.SetFlourishEffect(flourish_override_graphic);
            PlayerGui.SetAbilityButton(index, 0);
        }
    }
}