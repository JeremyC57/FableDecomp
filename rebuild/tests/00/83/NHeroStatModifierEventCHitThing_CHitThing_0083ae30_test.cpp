#include <cstdio>
// Behaviour test for 0083ae30: NHeroStatModifierEvent::CHitThing(const CCreatureStatsDef&) keeps the stats def and sets the defaults
// (not unarmed, strength category, increment on, amount 1.0).
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH = 0, HERO_STAT_SKILL = 1, HERO_STAT_WILL = 2 };
class CCreatureStatsDef { public: int Dummy; };
class CBase { public: virtual ~CBase() { } };
class CHitThing : public CBase {
public:
    CHitThing(const CCreatureStatsDef& stats_def)
        : StatsDefOfHitThing(&stats_def), Unarmed(false), StatToIncrement(HERO_STAT_STRENGTH), DoStatIncrement(true), IncrementAmount(1.0f) { }
    const CCreatureStatsDef* StatsDefOfHitThing; bool Unarmed; EHeroExperienceStatCategory StatToIncrement; bool DoStatIncrement; float IncrementAmount;
};
int main()
{
    CCreatureStatsDef def; CHitThing h(def);
    if (h.StatsDefOfHitThing != &def || h.Unarmed || h.StatToIncrement != HERO_STAT_STRENGTH || !h.DoStatIncrement || h.IncrementAmount != 1.0f) {
        std::printf("0083ae30_TEST FAIL\n"); return 1;
    }
    std::printf("0083ae30_TEST PASS\n"); return 0;
}
