#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetAbilityMaximumLevel (0x005EDEA3): same logic over a copy of the model.
struct CLongVector { long* first; long* last; long* end_of_storage; unsigned long size() const { return (unsigned long)(last - first); } };
class CHeroAbilityDef { public: CLongVector ExperienceCostsToUpgrade; };
struct SAbilityEntry { long Level; const CHeroAbilityDef* PDef; };
struct CAbilityEntryVector { SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage; SAbilityEntry& operator[](unsigned long n) { return first[n]; } };
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
static const long MAX_ABILITY_LEVEL = 3;
struct CTCInventoryAbilities_Retail {
    CAbilityEntryVector Abilities;
    long GetAbilityMaximumLevel(long ability)
    {
        if (ability != 0) {
            const CHeroAbilityDef* def = Abilities[ability].PDef;
            if (def != 0) {
                long level = def->ExperienceCostsToUpgrade.size() - 1;
                if (level < 0) level = 0;
                return Min(level, MAX_ABILITY_LEVEL);
            }
        }
        return 0;
    }
};
int main()
{
    long costs[6] = { 1, 2, 3, 4, 5, 6 };
    CHeroAbilityDef d0, d2, d6;
    d0.ExperienceCostsToUpgrade.first = costs; d0.ExperienceCostsToUpgrade.last = costs;
    d2.ExperienceCostsToUpgrade.first = costs; d2.ExperienceCostsToUpgrade.last = costs + 2;
    d6.ExperienceCostsToUpgrade.first = costs; d6.ExperienceCostsToUpgrade.last = costs + 6;
    SAbilityEntry e[5] = { { 0, &d6 }, { 0, &d0 }, { 0, &d2 }, { 0, &d6 }, { 0, 0 } };
    CTCInventoryAbilities_Retail a; a.Abilities.first = e; a.Abilities.last = e + 5; a.Abilities.end_of_storage = e + 5;
    if (a.GetAbilityMaximumLevel(0) != 0 || a.GetAbilityMaximumLevel(1) != 0 || a.GetAbilityMaximumLevel(2) != 1
        || a.GetAbilityMaximumLevel(3) != 3 || a.GetAbilityMaximumLevel(4) != 0) { std::printf("005edea3_TEST FAIL\n"); return 1; }
    std::printf("005edea3_TEST PASS\n");
    return 0;
}