#include <cstdio>
// Behaviour test for CTCInventoryAbilities::ForceAbilityLevelTo (005eef84): same logic over a copy of the model.
struct SAbilityEntry { long Level; long MaxLevel; };
class CThingPlayerCreature { public:
    long learned[8]; int count;
    bool LearnAbility(long ability, bool notify, long level, bool silent) { if (!notify || level != -1 || !silent) return false; learned[count++] = ability; return true; }
};
struct CAbilityEntryVector {
    SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage;
    long size() const { return last - first; }
    SAbilityEntry& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryAbilities_Retail {
    CThingPlayerCreature* ParentThing;
    CAbilityEntryVector Abilities;
    int morphUpdates;
    bool IsAbilityPotentiallyAvailable(long ability) { return ability != 2; }
    long GetAbilityMaximumLevel(long ability) { return Abilities[ability].MaxLevel; }
    void UpdateWillMorph() { ++morphUpdates; }
    void ForceAbilityLevelTo(long ability, long level)
    {
        long maximum = GetAbilityMaximumLevel(ability);
        if (level < 0) level = 0;
        else if (level > maximum) level = maximum;
        Abilities[ability].Level = level;
        UpdateWillMorph();
    }
    void SetAllAbilitiesAsAvailable(bool)
    {
        for (long ability = 1; ability < Abilities.size(); ++ability)
            if (IsAbilityPotentiallyAvailable(ability)) ParentThing->LearnAbility(ability, true, -1, true);
    }
    void ForceAllAbilitiesToMaxLevel()
    {
        for (long ability = 0; ability < Abilities.size(); ++ability)
            if (IsAbilityPotentiallyAvailable(ability)) ForceAbilityLevelTo(ability, GetAbilityMaximumLevel(ability));
    }
    void ForceAllAbilitiesToLevel(long level)
    {
        for (long ability = 0; ability < Abilities.size(); ++ability)
            if (IsAbilityPotentiallyAvailable(ability)) ForceAbilityLevelTo(ability, level);
    }
    void ForceAllAbilitiesToNextLevel()
    {
        for (long ability = 0; ability < Abilities.size(); ++ability)
            if (IsAbilityPotentiallyAvailable(ability)) ForceAbilityLevelTo(ability, Abilities[ability].Level + 1);
    }
};
int main()
{
    SAbilityEntry e[4] = { { 0, 3 }, { 1, 3 }, { 0, 2 }, { 2, 2 } };
    CThingPlayerCreature hero; hero.count = 0;
    CTCInventoryAbilities_Retail a; a.ParentThing = &hero; a.morphUpdates = 0;
    a.Abilities.first = e; a.Abilities.last = e + 4; a.Abilities.end_of_storage = e + 4;
    a.ForceAbilityLevelTo(1, 9);
    if (e[1].Level != 3) { std::printf("005eef84_TEST FAIL\n"); return 1; }
    a.ForceAbilityLevelTo(1, -1);
    if (e[1].Level != 0) { std::printf("005eef84_TEST FAIL\n"); return 1; }
    a.ForceAbilityLevelTo(3, 1);
    if (e[3].Level != 1 || a.morphUpdates != 3)
    { std::printf("005eef84_TEST FAIL\n"); return 1; }
    std::printf("005eef84_TEST PASS\n");
    return 0;
}