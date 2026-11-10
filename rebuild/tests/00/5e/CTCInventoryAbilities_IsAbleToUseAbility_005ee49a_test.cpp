// Behaviour test for 005ee49a CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility, bool) const: the function body from
// src/005ee49a.cpp verbatim over self-contained C++03 stand-ins (def manager lookups by class name, PeekTC over a pointer).
// The float comparisons sit well away from equality.
#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

enum EHeroAbility { HERO_ABILITY_NULL = 0, HERO_ABILITY_FORCE_PUSH = 1, HERO_ABILITY_SUMMON_SPELL = 10, HERO_ABILITY_FIREBALL_SPELL = 16,
                    HERO_ABILITY_DIVINE_WRATH_SPELL = 18, HERO_ABILITY_UNHOLY_POWER_SPELL = 19 };
class CCharString { public:
    const char* Text;
    CCharString(const char* text) : Text(text) {}
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};
class CHeroAbilityDef { public: std::vector<long> StaminaCost; };
class CSpecialAbilitiesFireballSpellDef { public: std::vector<float> StaminaCost; };
class CSpecialAbilitiesUnholyPowerDef { public: std::vector<float> EnvChangeSecs, StaminaCostPerSec; };
class CSpecialAbilitiesDivineWrathDef { public: std::vector<float> EnvChangeSecs, StaminaCostPerSec; };
static CSpecialAbilitiesFireballSpellDef gFireball;
static CSpecialAbilitiesUnholyPowerDef gUnholy;
static CSpecialAbilitiesDivineWrathDef gWrath;
class CGameDefinitionManager { public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager manager; return manager; }
    long GetGlobalIndexFromClassIndex(const CCharString& name, long classIndex) const
    {
        if (classIndex != 1) return 0;
        if (!std::strcmp(name.Text, "SPECIAL_ABILITIES_FIREBALL_SPELL_DEF")) return 101;
        if (!std::strcmp(name.Text, "SPECIAL_ABILITIES_UNHOLY_POWER_DEF")) return 102;
        if (!std::strcmp(name.Text, "SPECIAL_ABILITIES_DIVINE_WRATH_DEF")) return 103;
        return 0;
    }
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesFireballSpellDef>* out) const { if (index != 101) return false; out->Object = &gFireball; return true; }
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesUnholyPowerDef>* out) const { if (index != 102) return false; out->Object = &gUnholy; return true; }
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesDivineWrathDef>* out) const { if (index != 103) return false; out->Object = &gWrath; return true; }
};
class CTCHeroStats { public: long Stamina; };
class CTCSummonSpell { public:
    long ResetCreatureManaCost;
    long GetResetCreatureManaCost() const { return ResetCreatureManaCost; }
};
class CThing { public:
    CTCHeroStats* Stats;
    CTCSummonSpell* Summon;
    bool PeekTC(const CTCHeroStats** out) const { if (!Stats) return false; *out = Stats; return true; }
    bool PeekTC(const CTCSummonSpell** out) const { if (!Summon) return false; *out = Summon; return true; }
};
class CTCInventoryAbilities { public:
    CThing* ParentThing;
    std::vector<std::pair<long, CDefPointer<const CHeroAbilityDef> > > Abilities;
    bool IsAbleToUseAbility(EHeroAbility ability, bool checkCost) const;
};

// ---- source body (verbatim from src/005ee49a.cpp) ----
bool CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility ability, bool checkCost) const
{
    if (ability == HERO_ABILITY_NULL || Abilities[ability].first < 0)
        return false;
    if (checkCost) {
        const CTCHeroStats* stats;
        ParentThing->PeekTC(&stats);
        if (ability == HERO_ABILITY_FIREBALL_SPELL) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", 1);
            CDefPointer<const CSpecialAbilitiesFireballSpellDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            return stats->Stamina >= def->StaminaCost[0];
        }
        if (ability == HERO_ABILITY_UNHOLY_POWER_SPELL) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", 1);
            CDefPointer<const CSpecialAbilitiesUnholyPowerDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            long level = Abilities[ability].first;
            return stats->Stamina >= def->StaminaCostPerSec[level] * def->EnvChangeSecs[level] + 20.0f;
        }
        if (ability == HERO_ABILITY_DIVINE_WRATH_SPELL) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", 1);
            CDefPointer<const CSpecialAbilitiesDivineWrathDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            long level = Abilities[ability].first;
            return stats->Stamina >= def->StaminaCostPerSec[level] * def->EnvChangeSecs[level] + 20.0f;
        }
        long cost = Abilities[ability].second->StaminaCost[Abilities[ability].first];
        if (ability == HERO_ABILITY_SUMMON_SPELL) {
            const CTCSummonSpell* summon;
            if (ParentThing->PeekTC(&summon))
                cost = summon->GetResetCreatureManaCost();
        }
        return stats->Stamina >= cost;
    }
    return true;
}

// ---- driver ----
int main()
{
    CHeroAbilityDef push; push.StaminaCost.push_back(10); push.StaminaCost.push_back(30);
    CHeroAbilityDef summonDef; summonDef.StaminaCost.push_back(50); summonDef.StaminaCost.push_back(70);
    gFireball.StaminaCost.push_back(40.0f); gFireball.StaminaCost.push_back(99.0f);
    gUnholy.StaminaCostPerSec.push_back(2.0f); gUnholy.StaminaCostPerSec.push_back(3.0f);
    gUnholy.EnvChangeSecs.push_back(10.0f); gUnholy.EnvChangeSecs.push_back(20.0f);      // level 0: 2*10+20 = 40, level 1: 3*20+20 = 80
    gWrath.StaminaCostPerSec.push_back(1.0f); gWrath.StaminaCostPerSec.push_back(5.0f);
    gWrath.EnvChangeSecs.push_back(5.0f); gWrath.EnvChangeSecs.push_back(4.0f);          // level 0: 1*5+20 = 25, level 1: 5*4+20 = 40
    CTCHeroStats stats; stats.Stamina = 60;
    CTCSummonSpell summon; summon.ResetCreatureManaCost = 55;
    CThing thing; thing.Stats = &stats; thing.Summon = 0;
    CTCInventoryAbilities inv; inv.ParentThing = &thing;
    inv.Abilities.resize(20);
    for (int i = 0; i < 20; ++i) inv.Abilities[i].first = -1;
    inv.Abilities[1].first = 1; inv.Abilities[1].second.Object = &push;          // force push, level 1 (cost 30)
    inv.Abilities[10].first = 0; inv.Abilities[10].second.Object = &summonDef;   // summon, level 0 (cost 50)
    inv.Abilities[16].first = 1;                                                 // fireball (always its first-level cost 40)
    inv.Abilities[19].first = 1;                                                 // unholy power, level 1 (80)
    inv.Abilities[18].first = 0;                                                 // divine wrath, level 0 (25)

    CHECK(!inv.IsAbleToUseAbility(HERO_ABILITY_NULL, false));
    CHECK(!inv.IsAbleToUseAbility((EHeroAbility)2, false));                     // not available
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_FORCE_PUSH, false));
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_FORCE_PUSH, true));               // 60 >= 30
    stats.Stamina = 29;
    CHECK(!inv.IsAbleToUseAbility(HERO_ABILITY_FORCE_PUSH, true));
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_FORCE_PUSH, false));              // cost not asked for
    stats.Stamina = 60;
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_FIREBALL_SPELL, true));           // 60 >= 40
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_SUMMON_SPELL, true));             // no summon active: 60 >= 50
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_DIVINE_WRATH_SPELL, true));       // 60 >= 25
    CHECK(!inv.IsAbleToUseAbility(HERO_ABILITY_UNHOLY_POWER_SPELL, true));      // 60 < 80
    stats.Stamina = 100;
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_UNHOLY_POWER_SPELL, true));       // 100 >= 80
    stats.Stamina = 35;
    CHECK(!inv.IsAbleToUseAbility(HERO_ABILITY_FIREBALL_SPELL, true));
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_DIVINE_WRATH_SPELL, true));       // 35 >= 25
    stats.Stamina = 24;
    CHECK(!inv.IsAbleToUseAbility(HERO_ABILITY_DIVINE_WRATH_SPELL, true));      // 24 < 25
    stats.Stamina = 52;
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_SUMMON_SPELL, true));
    thing.Summon = &summon;                                                      // an active summon costs its reset cost
    CHECK(!inv.IsAbleToUseAbility(HERO_ABILITY_SUMMON_SPELL, true));            // 52 < 55
    stats.Stamina = 55;
    CHECK(inv.IsAbleToUseAbility(HERO_ABILITY_SUMMON_SPELL, true));

    if (fails) { std::printf("005ee49a_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("005ee49a_TEST PASS\n");
    return 0;
}