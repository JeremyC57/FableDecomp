// Behaviour test for CTCInventoryAbilities::GetAbilityInventoryThingName 0x005EDD04 (model copy)
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* m_text;
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
};

struct CTCInventoryAbilities {
    CCharString GetAbilityInventoryThingName(long ability);
};

CCharString CTCInventoryAbilities::GetAbilityInventoryThingName(long ability)
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

int main()
{
    CTCInventoryAbilities a;
    if (std::strcmp(a.GetAbilityInventoryThingName(1).m_text, "HERO_ABILITY_FORCE_PUSH_INVENTORY_THING") != 0 ||
        std::strcmp(a.GetAbilityInventoryThingName(19).m_text, "HERO_ABILITY_SKILL_UNHOLY_POWER_THING") != 0 ||
        std::strcmp(a.GetAbilityInventoryThingName(12).m_text, "HERO_ABILITY_STRENGTH_BATTLE_CHARGE_THING") != 0 ||
        std::strcmp(a.GetAbilityInventoryThingName(0).m_text, "") != 0 ||
        std::strcmp(a.GetAbilityInventoryThingName(20).m_text, "") != 0) {
        std::printf("005edd04_TEST FAIL\n"); return 1;
    }
    std::printf("005edd04_TEST PASS\n");
    return 0;
}
