#include <cstdio>

// Model of CTCHeroExperience::GetTrainableStatMaxLevel @ 0x006D6770: max(levels in the stat's ability track - 1, 0).
struct CAbilityTrackLevelDef { long ExperienceCost; long AbilityUnlocked; long Description; };
struct CAbilityTrackDef { long ExperienceStat; CAbilityTrackLevelDef* first; CAbilityTrackLevelDef* last; };
struct CHeroStatsDef { CAbilityTrackDef* StatLevels; };
struct CTCHeroExperience {
    CHeroStatsDef* PHeroStatsDef;
    long GetTrainableStatMaxLevel(long stat) const
    {
        const CAbilityTrackDef& track = PHeroStatsDef->StatLevels[stat];
        long maxLevel = (long)(track.last - track.first) - 1;
        if (maxLevel < 0) maxLevel = 0;
        return maxLevel;
    }
};

int main()
{
    CAbilityTrackLevelDef levels[4];
    CAbilityTrackDef tracks[2]; tracks[0].first = levels; tracks[0].last = levels + 4; tracks[1].first = levels; tracks[1].last = levels;
    CHeroStatsDef def; def.StatLevels = tracks; CTCHeroExperience e; e.PHeroStatsDef = &def;
    if (e.GetTrainableStatMaxLevel(0) != 3 || e.GetTrainableStatMaxLevel(1) != 0) { std::printf("006d6770_TEST FAIL\n"); return 1; }
    std::printf("006d6770_TEST PASS\n");
    return 0;
}
