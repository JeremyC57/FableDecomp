// Behaviour test for 0x0072C430 CTCSpecialAbilities::IsAbilityAvailable: the row's defines, includes and body verbatim. The component,
// the hero and the creature are raw zeroed images at retail offsets (CTCBase ParentThing +0x04, Creature +0x18, AbilityAvailability
// +0x1C; CThing UsingComponents +0x20, Components +0x44); doubles for IsAbleToUseAnyAbility, the const TC-map LowerBound,
// CTCInventoryAbilities::IsAbleToUseAbility and a fake targeting component whose vtable slot 33 (GetPTargetedThing) counts calls.
// Cases: each gate (any-ability, availability flag, inventory component present, IsAbleToUseAbility(type, true)) refuses on its own
// without reaching the later ones; past them, every ability 1..19 answers !HasTC(its component) (shield, double strike, summon: always
// true; raise dead: false), with the targeted thing fetched once per call; HERO_ABILITY_NULL and 20 fall to false.
#include <stdio.h>
#include <string.h>
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


// ---- test doubles -----------------------------------------------------------------------------------------------------------------
static int g_any = 1, g_any_calls = 0, g_use = 1, g_use_calls = 0, g_target_calls = 0;
static EHeroAbility g_any_type, g_use_type; static bool g_use_check = false; static const void* g_use_this = 0;
bool CTCSpecialAbilities::IsAbleToUseAnyAbility(EHeroAbility type) const { ++g_any_calls; g_any_type = type; return g_any != 0; }
bool CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility ability, bool checkCost) const
{ ++g_use_calls; g_use_type = ability; g_use_check = checkCost; g_use_this = this; return g_use != 0; }
typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > TCMap;
template<> TCMap::const_iterator TCMap::LowerBound(const ETCInterfaceType& key) const
{
    const std::pair<ETCInterfaceType,CTCBase*>* const* words = (const std::pair<ETCInterfaceType,CTCBase*>* const*)this;
    const std::pair<ETCInterfaceType,CTCBase*>* p = words[0];
    while (p != words[1] && p->first < key) ++p;
    return p;
}
static unsigned char g_target_thing[16];
static void* __fastcall FakeGetPTargetedThing(void* self, void*) { ++g_target_calls; return g_target_thing; }

// ---- harness ------------------------------------------------------------------------------------------------------------------------
static int g_fail = 0;
static void check(int ok, const char* what, int t) { if (!ok) { printf("FAIL %s (type %d)\n", what, t); g_fail = 1; } }
static unsigned char hero[0x400], creature[0x400], component[0x40], inventory[0x40], targeting[0x40];
static unsigned char availability[32];
static void* target_vtable[64];
static std::pair<ETCInterfaceType,CTCBase*> hero_map[24], creature_map[2];
static int hero_map_n = 0;
static void SetBit(unsigned char* thing, int type, bool on)
{
    unsigned long* w = (unsigned long*)(thing + 0x20) + (type >> 5);
    if (on) *w |= 1UL << (type & 31); else *w &= ~(1UL << (type & 31));
}
static const int tc_of[20] = { -1, 150, 146, 149, -1, 158, 151, -1, 132, -1, -1, 148, 168, 169, 153, 154, 157, 136, 133, 134 };

int main()
{
    // hero: inventory-abilities component (0x6F) in its map
    hero_map[0].first = (ETCInterfaceType)0x10; hero_map[0].second = 0;
    hero_map[1].first = (ETCInterfaceType)0x6F; hero_map[1].second = (CTCBase*)inventory;
    *(void**)(hero + 0x44) = &hero_map[0]; *(void**)(hero + 0x48) = &hero_map[2];
    SetBit(hero, 0x6F, true);
    // creature: targeting component (8) with a fake vtable, slot 33 = GetPTargetedThing
    for (int i = 0; i < 64; ++i) target_vtable[i] = 0;
    target_vtable[33] = (void*)&FakeGetPTargetedThing;
    *(void**)targeting = target_vtable;
    creature_map[0].first = (ETCInterfaceType)8; creature_map[0].second = (CTCBase*)targeting;
    *(void**)(creature + 0x44) = &creature_map[0]; *(void**)(creature + 0x48) = &creature_map[1];
    SetBit(creature, 8, true);
    // the special-abilities component
    *(void**)(component + 0x04) = hero;           // ParentThing
    *(void**)(component + 0x18) = creature;       // Creature&
    *(void**)(component + 0x1C) = availability;   // AbilityAvailability (bytewise)
    *(void**)(component + 0x20) = availability + 32;
    const CTCSpecialAbilities& abilities = *(const CTCSpecialAbilities*)component;
    for (int i = 0; i < 32; ++i) availability[i] = 1;

    // gate 1: IsAbleToUseAnyAbility
    g_any = 0;
    check(!abilities.IsAbilityAvailable(HERO_ABILITY_FORCE_PUSH), "any-ability gate", 1);
    check(g_any_calls == 1 && g_any_type == HERO_ABILITY_FORCE_PUSH && g_use_calls == 0 && g_target_calls == 0, "any-ability gate stops", 1);
    g_any = 1;
    // gate 2: availability flag
    availability[HERO_ABILITY_BERSERK] = 0;
    check(!abilities.IsAbilityAvailable(HERO_ABILITY_BERSERK), "availability gate", 8);
    check(g_use_calls == 0 && g_target_calls == 0, "availability gate stops", 8);
    availability[HERO_ABILITY_BERSERK] = 1;
    // gate 3: no inventory-abilities component
    SetBit(hero, 0x6F, false);
    check(!abilities.IsAbilityAvailable(HERO_ABILITY_BERSERK), "inventory component gate", 8);
    check(g_use_calls == 0 && g_target_calls == 0, "inventory component gate stops", 8);
    SetBit(hero, 0x6F, true);
    // gate 4: IsAbleToUseAbility(type, true) on the inventory component
    g_use = 0;
    check(!abilities.IsAbilityAvailable(HERO_ABILITY_HEAL_LIFE_SPELL), "IsAbleToUseAbility gate", 14);
    check(g_use_calls == 1 && g_use_type == HERO_ABILITY_HEAL_LIFE_SPELL && g_use_check && g_use_this == inventory && g_target_calls == 0, "IsAbleToUseAbility args", 14);
    g_use = 1;

    // past the gates: each ability, with and without its component on the hero
    for (int t = 0; t <= 20; ++t) {
        int before = g_target_calls;
        bool expect = false;
        if (t >= 1 && t <= 19) expect = (t == 4 || t == 9 || t == 10) ? true : (t == 7 ? false : true);
        check(abilities.IsAbilityAvailable((EHeroAbility)t) == expect, "free", t);
        check(g_target_calls == before + 1, "targeted thing fetched once", t);
        if (t >= 1 && t <= 19 && tc_of[t] >= 0) {
            SetBit(hero, tc_of[t], true);
            check(!abilities.IsAbilityAvailable((EHeroAbility)t), "refused while its component is on the hero", t);
            SetBit(hero, tc_of[t], false);
        }
    }
    // a different ability's component does not block
    SetBit(hero, 146, true);
    check(abilities.IsAbilityAvailable(HERO_ABILITY_FORCE_PUSH), "other component ignored", 1);
    check(!abilities.IsAbilityAvailable(HERO_ABILITY_TIME_SPELL), "time spell blocked by bullet time", 2);
    SetBit(hero, 146, false);
    if (!g_fail) printf("0072c430_TEST PASS\n");
    return g_fail;
}