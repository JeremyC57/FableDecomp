#include <cstdio>

// Model of CTCHeroExperience::GetCostToUpgradeTrainableStat @ 0x006D67B0: the next level's cost, or -1 at the top level
// (the level is compared unsigned against size - 1).
class CAbilityTrackLevelDef { public: long ExperienceCost; long AbilityUnlocked; long Description; };
class CAbilityTrackDef { public: long ExperienceStat; CAbilityTrackLevelDef* first; CAbilityTrackLevelDef* last;
    unsigned long size() const { return (unsigned long)(last - first); } };
class CHeroStatsDef { public: CAbilityTrackDef* StatLevels; };
class CTCHeroExperience { public:
    long TrainableStatLevels[10]; CHeroStatsDef* PHeroStatsDef;
    long GetCostToUpgradeTrainableStat(long stat) const
    {
        if ((unsigned long)TrainableStatLevels[stat] < PHeroStatsDef->StatLevels[stat].size() - 1)
            return PHeroStatsDef->StatLevels[stat].first[TrainableStatLevels[stat] + 1].ExperienceCost;
        return -1;
    }
};

int main()
{
    CAbilityTrackLevelDef levels[3]; levels[0].ExperienceCost = 0; levels[1].ExperienceCost = 500; levels[2].ExperienceCost = 1500;
    CAbilityTrackDef track; track.first = levels; track.last = levels + 3;
    CHeroStatsDef def; def.StatLevels = &track;
    CTCHeroExperience e; for (int i = 0; i < 10; ++i) e.TrainableStatLevels[i] = 0; e.PHeroStatsDef = &def;
    if (e.GetCostToUpgradeTrainableStat(0) != 500) { std::printf("006d67b0_TEST FAIL\n"); return 1; }
    e.TrainableStatLevels[0] = 1; if (e.GetCostToUpgradeTrainableStat(0) != 1500) { std::printf("006d67b0_TEST FAIL\n"); return 1; }
    e.TrainableStatLevels[0] = 2; if (e.GetCostToUpgradeTrainableStat(0) != -1) { std::printf("006d67b0_TEST FAIL\n"); return 1; }
    e.TrainableStatLevels[0] = -1; if (e.GetCostToUpgradeTrainableStat(0) != -1) { std::printf("006d67b0_TEST FAIL\n"); return 1; }   // unsigned compare
    std::printf("006d67b0_TEST PASS\n");
    return 0;
}