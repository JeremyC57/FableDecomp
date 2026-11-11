// Behaviour test for 0x007CD390 CTCMultiStrike::CTCMultiStrike: the row's body over stubs (component lookups, def tables, recorded
// effect / sound / noise calls), checking the weapon pick, the spell level, the cast-effect name per level and the sound index.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void check(bool b) { if (!b) { std::puts("007cd390_TEST FAIL"); std::exit(1); } }
class CThing;
static CThing* g_applied; static char g_effectName[32]; static long g_soundIndex; static int g_noise, g_attach, g_sounds;
class CTCInventoryWeapons { public: CThing* melee; CThing* GetActiveMeleeWeapon() const { return melee; } };
class CTCCarrying { public: CThing* primary; CThing* GetThingInPrimarySlot() const { return primary; } };
class CTCInventoryAbilities { public: long level; long GetAbilityLevel(int) const { return level; } };
class CThing { public: CTCInventoryWeapons* weapons; CTCCarrying* carrying; CTCInventoryAbilities* abilities; bool sound;
    CThing() : weapons(0), carrying(0), abilities(0), sound(true) { } };
class CTCMultiStrike { public:
    CThing* parent; long SpellLevel; bool StrikeStarted; long DelayTimer, NumHits;
    void ApplyEffectOnWeapon(CThing* w) { g_applied = w; }
    CTCMultiStrike(CThing& thing);
};
static const long g_sounds_table[4] = { 100, 101, 102, 103 };
CTCMultiStrike::CTCMultiStrike(CThing& thing) : parent(&thing), SpellLevel(0), StrikeStarted(false), DelayTimer(0), NumHits(0)
{
    CThing* weapon = 0;
    if (parent->weapons)
        weapon = parent->weapons->GetActiveMeleeWeapon();
    else if (parent->carrying)
        weapon = parent->carrying->GetThingInPrimarySlot();
    if (weapon)
        ApplyEffectOnWeapon(weapon);
    if (parent->abilities)
        SpellLevel = parent->abilities->GetAbilityLevel(9);
    else
        SpellLevel = 1;
    const char* effect_name;
    switch (SpellLevel) {
    case 0: effect_name = "CAST_EFFECT_LEVEL1"; break;
    case 1: effect_name = "CAST_EFFECT_LEVEL2"; break;
    case 2: effect_name = "CAST_EFFECT_LEVEL3"; break;
    case 3: effect_name = "CAST_EFFECT_LEVEL4"; break;
    default: effect_name = "CAST_EFFECT_LEVEL1"; break;
    }
    std::strcpy(g_effectName, effect_name); ++g_attach;
    if (parent->sound) { g_soundIndex = g_sounds_table[SpellLevel]; ++g_sounds; }
    ++g_noise;
}
int main()
{
    CThing sword, axe, hero; CTCInventoryWeapons w = { &sword }; CTCCarrying c = { &axe }; CTCInventoryAbilities a = { 2 };
    hero.weapons = &w; hero.carrying = &c; hero.abilities = &a;
    { CTCMultiStrike m(hero); check(g_applied == &sword && m.SpellLevel == 2 && !std::strcmp(g_effectName, "CAST_EFFECT_LEVEL3") && g_soundIndex == 102); }
    hero.weapons = 0; a.level = 0;
    { CTCMultiStrike m(hero); check(g_applied == &axe && !std::strcmp(g_effectName, "CAST_EFFECT_LEVEL1") && g_soundIndex == 100); }
    hero.abilities = 0; hero.carrying = 0; g_applied = 0; hero.sound = false;
    { CTCMultiStrike m(hero); check(g_applied == 0 && m.SpellLevel == 1 && !std::strcmp(g_effectName, "CAST_EFFECT_LEVEL2") && g_sounds == 2); }
    a.level = 7; hero.abilities = &a;   // out of range: default name
    { CTCMultiStrike m(hero); check(!std::strcmp(g_effectName, "CAST_EFFECT_LEVEL1") && g_noise == 4 && g_attach == 4); }
    std::puts("007cd390_TEST PASS");
    return 0;
}