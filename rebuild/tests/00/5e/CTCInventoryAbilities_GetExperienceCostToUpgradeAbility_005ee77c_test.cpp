// Behaviour test for 005ee77c CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool) const: the
// function body from src/005ee77c.cpp verbatim over self-contained C++03 stand-ins. GFFloatToLongNear is modelled by a plain
// round-to-nearest (the shared helper's portable twin semantics); no value tested sits on a rounding boundary.
#include <cstdio>
#include <cmath>
#include <vector>
#include <utility>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static long GFFloatToLongNear(float value) { return (long)std::floor((double)value + 0.5); }
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* Get() const { return Object; }
    T* operator->() const { return Object; }
};
class CHeroAbilityDef { public:
    std::vector<long> ExperienceCostsToUpgrade;
    float MoralityCostFactor;
};
class CTCHeroStats { public:
    long Morality;
    long MoralityMax;
    long GetMorality() const { return Morality; }
    long GetMoralityMax() const { return MoralityMax; }
};
class CThing { public:
    CTCHeroStats* Stats;
    bool PeekTC(const CTCHeroStats** out) const { if (!Stats) return false; *out = Stats; return true; }
};
class CTCInventoryAbilities { public:
    CThing* ParentThing;
    std::vector<std::pair<long, CDefPointer<const CHeroAbilityDef> > > Abilities;
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) const;
};

// ---- source body (verbatim from src/005ee77c.cpp) ----
long CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) const
{
    if ((unsigned long)ability < Abilities.size() && Abilities[ability].second.Get() != 0 &&
        Abilities[ability].first + levelsAhead <= (long)Abilities[ability].second->ExperienceCostsToUpgrade.size() - 1) {
        long cost = Abilities[ability].second->ExperienceCostsToUpgrade[Abilities[ability].first + levelsAhead];
        const CTCHeroStats* stats;
        if (ParentThing->PeekTC(&stats)) {
            float ratio = (float)stats->GetMorality() / (float)stats->GetMoralityMax();
            long result = cost;
            if (applyMorality)
                result -= GFFloatToLongNear(cost * Abilities[ability].second->MoralityCostFactor * ratio);
            return result;
        }
        return cost;
    }
    return -1;
}

// ---- driver ----
int main()
{
    CHeroAbilityDef def;
    def.ExperienceCostsToUpgrade.push_back(100);
    def.ExperienceCostsToUpgrade.push_back(200);
    def.ExperienceCostsToUpgrade.push_back(400);
    def.MoralityCostFactor = 0.5f;
    CTCHeroStats stats; stats.Morality = 250; stats.MoralityMax = 1000;     // ratio 0.25
    CThing thing; thing.Stats = &stats;
    CTCInventoryAbilities inv; inv.ParentThing = &thing;
    std::pair<long, CDefPointer<const CHeroAbilityDef> > empty; empty.first = 0;
    std::pair<long, CDefPointer<const CHeroAbilityDef> > known; known.first = 1; known.second.Object = &def;
    inv.Abilities.push_back(empty);     // 0: no def
    inv.Abilities.push_back(known);     // 1: level 1

    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 0, false) == 200);
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 1, false) == 400);
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, -1, false) == 100);
    // with morality: cost - round(cost * 0.5 * 0.25)
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 0, true) == 200 - 25);
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 1, true) == 400 - 50);
    stats.Morality = -300;            // evil hero: ratio -0.3, the cost goes up
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 1, true) == 400 + 60);
    def.MoralityCostFactor = -1.0f;
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 0, true) == 200 - 60);
    // past the last level, an unknown ability, an ability without a def: -1
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 2, true) == -1);
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)2, 0, false) == -1);
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)0, 0, false) == -1);
    // no hero stats component: the plain cost even when morality is asked for
    thing.Stats = 0;
    CHECK(inv.GetExperienceCostToUpgradeAbility((EHeroAbility)1, 0, true) == 200);

    if (fails) { std::printf("005ee77c_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("005ee77c_TEST PASS\n");
    return 0;
}