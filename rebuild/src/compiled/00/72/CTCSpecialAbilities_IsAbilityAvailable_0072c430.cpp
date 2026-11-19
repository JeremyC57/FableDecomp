// CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility type) const -- whether the hero may start ability `type` now: the ability must pass IsAbleToUseAnyAbility, be marked available, and pass the inventory-abilities check (IsAbleToUseAbility with the cost check); then the timed abilities are refused while their component is still on the hero (physical shield, double strike and summon always pass; raise dead and anything else fail)
// Leaf: ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072C430, 584 bytes (508 code + the 19-entry jump table at 0x0072C62C), __fastcall, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z (FableWin debug 0x0254B170; the
//   shared header's USE opt-in declares it; UseSpecialAbility 0x0072CF40 calls it). The catalog label GetAbilityAvailability is no
//   PDB name. Locals: Ego_d.pdb's record (debug length 1396): parameter `type`; ptc_inventory_abilities; { ptc_targeting, ptarget }.
// Callees: IsAbleToUseAnyAbility 0x0072AF30 (overlay opt-in FABLE_RETAIL_CTCSPECIALABILITIES_ISABLETOUSEANYABILITY), the const TC map
//   LowerBound 0x004365B0 (PeekTC, twice), CTCInventoryAbilities::IsAbleToUseAbility 0x005EE49A, CTCTargetingBase slot 33
//   GetPTargetedThing (+0x84).
// Shape: FableWin's statements: the separate `return false` for an unavailable ability, the NULL-initialised inventory pointer, the
//   PeekTC on the creature whose failure arm only reports in the debug build (release then reads the frame word), the targeted thing
//   fetched into ptarget, and the switch with constant interface types in `!GetParentThing().HasTC(TCI_..)` (FableWin `push 92h ;
//   call CThing::HasTC`). In this text retail's arms reuse the dead `this` register (`mov esi,[esi+4]`); the old local-model draft
//   (one && guard, StatGetInterfaceType arms, no ptarget) left them in rotating scratch registers.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CTCSPECIALABILITIES_ISABLETOUSEANYABILITY
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCTargetingBase.h"

bool CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility type) const
{
    if (IsAbleToUseAnyAbility(type)) {
        if (!AbilityAvailability[type])
            return false;
        const CTCInventoryAbilities* ptc_inventory_abilities = 0;
        if (GetParentThing().PeekTC(&ptc_inventory_abilities) && ptc_inventory_abilities->IsAbleToUseAbility(type, true)) {
            const CTCTargetingBase* ptc_targeting;
            Creature.PeekTC(&ptc_targeting);
            // Fidelity: ptarget is unused in release (Ego_d.pdb's record has it; FableWin stores it at [ebp-14h]); retail still makes
            //   the virtual call (0x0072C4D5) and drops the result.
            CThing* ptarget = ptc_targeting->GetPTargetedThing();
            switch (type) {
            case HERO_ABILITY_TIME_SPELL:           return !GetParentThing().HasTC(TCI_BULLET_TIME);
            case HERO_ABILITY_PHYSICAL_SHIELD_SPELL: return true;
            case HERO_ABILITY_FORCE_PUSH:           return !GetParentThing().HasTC(TCI_FORCE_PUSH_POWER);
            case HERO_ABILITY_ENFLAME_SPELL:        return !GetParentThing().HasTC(TCI_WILL_ENFLAME);
            case HERO_ABILITY_TURNCOAT_SPELL:       return !GetParentThing().HasTC(TCI_TURNCOAT_SPELL);
            case HERO_ABILITY_DRAIN_LIFE_SPELL:     return !GetParentThing().HasTC(TCI_DRAIN_LIFE);
            case HERO_ABILITY_SUMMON_SPELL:         return true;
            case HERO_ABILITY_LIGHTNING_SPELL:      return !GetParentThing().HasTC(TCI_WILL_LIGHTNING);
            case HERO_ABILITY_BERSERK:              return !GetParentThing().HasTC(TCI_BERSERK);
            case HERO_ABILITY_BATTLE_CHARGE:        return !GetParentThing().HasTC(TCI_BATTLE_CHARGE);
            case HERO_ABILITY_MULTI_ARROW:          return !GetParentThing().HasTC(TCI_MULTI_ARROW);
            case HERO_ABILITY_DOUBLE_STRIKE:        return true;
            case HERO_ABILITY_ASSASSIN_RUSH:        return !GetParentThing().HasTC(TCI_ASSASSIN_RUSH);
            case HERO_ABILITY_HEAL_LIFE_SPELL:      return !GetParentThing().HasTC(TCI_HEAL_LIFE);
            case HERO_ABILITY_GHOST_SWORD_SPELL:    return !GetParentThing().HasTC(TCI_GHOST_SWORD);
            case HERO_ABILITY_FIREBALL_SPELL:       return !GetParentThing().HasTC(TCI_FIREBALL_SPELL);
            case HERO_ABILITY_DIVINE_WRATH_SPELL:   return !GetParentThing().HasTC(TCI_DIVINE_WRATH);
            case HERO_ABILITY_UNHOLY_POWER_SPELL:   return !GetParentThing().HasTC(TCI_UNHOLY_POWER);
            // Inferred: no raise-dead case. FableWin has `case HERO_ABILITY_RAISE_DEAD_SPELL: return IsAbleToCastRaiseDead();`; retail's
            //   table sends 7 to the shared `return false` (0x0072C622) with no call, as for the default (a release-dead ASSERT).
            }
        }
    }
    return false;
}