// CTCInventoryAbilities::GetAbilityInventoryThingName(EHeroAbility ability) -- the inventory thing def name of an ability
// Retail 0x005EDD04, 307 bytes including the 19-entry jump table at 0x005EDDEB (ret 8: hidden CCharString result
// + ability). A dense switch over abilities 1..19 (`dec eax ; cmp eax,12h ; ja default ; jmp [table]`): every
// case pushes `-1` and its literal and jumps to the shared `mov ecx,[ebp+8] ; call CCharString(const char*, long)`
// tail (0x0099EBF0); the default builds the empty string (0x0122D70E). VC7.1 lays the case bodies out in source
// order, so the cases are written 1..19 (retail's body order). Frame-pointer body: `optimize("y", off)` + `("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};

enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

struct CTCInventoryAbilities {
    CCharString GetAbilityInventoryThingName(EHeroAbility ability);
};

CCharString CTCInventoryAbilities::GetAbilityInventoryThingName(EHeroAbility ability)
{
    switch (ability) {
    case 1: return CCharString("HERO_ABILITY_FORCE_PUSH_INVENTORY_THING");
    case 2: return CCharString("HERO_ABILITY_TIME_SPELL_INVENTORY_THING");
    case 3: return CCharString("HERO_ABILITY_WILL_ENFLAME_INVENTORY_THING");
    case 4: return CCharString("HERO_ABILITY_PHYSICAL_SHIELD_SPELL_INVENTORY_THING");
    case 5: return CCharString("HERO_ABILITY_WILL_TURNCOAT_INVENTORY_THING");
    case 6: return CCharString("HERO_ABILITY_WILL_DRAIN_LIFE_INVENTORY_THING");
    case 7: return CCharString("HERO_ABILITY_WILL_RAISE_DEAD_INVENTORY_THING");
    case 8: return CCharString("HERO_ABILITY_STRENGTH_BERSERK_INVENTORY_THING");
    case 9: return CCharString("HERO_ABILITY_STRENGTH_DOUBLE_STRIKE_INVENTORY_THING");
    case 10: return CCharString("HERO_ABILITY_SUMMON_SPELL_INVENTORY_THING");
    case 11: return CCharString("HERO_ABILITY_WILL_LIGHTNING_INVENTORY_THING");
    case 12: return CCharString("HERO_ABILITY_STRENGTH_BATTLE_CHARGE_THING");
    case 13: return CCharString("HERO_ABILITY_SKILL_ASSASSIN_RUSH_THING");
    case 14: return CCharString("HERO_ABILITY_WILL_HEAL_LIFE_INVENTORY_THING");
    case 15: return CCharString("HERO_ABILITY_WILL_GHOST_SWORD_INVENTORY_THING");
    case 16: return CCharString("HERO_ABILITY_WILL_FIREBALL_INVENTORY_THING");
    case 17: return CCharString("HERO_ABILITY_SKILL_MULTI_ARROW_INVENTORY_THING");
    case 18: return CCharString("HERO_ABILITY_SKILL_DIVINE_WRATH_THING");
    case 19: return CCharString("HERO_ABILITY_SKILL_UNHOLY_POWER_THING");
    }
    return CCharString("");
}
