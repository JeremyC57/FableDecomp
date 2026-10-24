// CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat, float& fractionTowardsNext) -- how many levels the stat could be raised with its category's experience plus the spendable pool, and how far the remainder goes towards the level after
// Leaf: ?GetTrainableStatPossibleLevelUpAmount@
// Grade: DIFFER(243v220) -- loop optimisation: retail hoists only the TrainableStatLevels pointer (spilled to the `stat`
//        slot) and re-reads `TrainableStatLevels[stat]` twice per pass (no CSE across the max-level test, no strength
//        reduction: `lea eax,[eax+eax*8]` each pass, `stat` kept in ebp); ours hoists the current level into ebp and
//        strength-reduces the level index (ebx = (level+1)*72, two frame words). Tried: `for (;;)` with both exits
//        inside, a `while (level + n <= max)` loop with the capped return after it (identical), the fraction through
//        a by-reference helper (identical), `#pragma optimize("g", off)` (frame-pointer code, 249). next: find what in
//        the retail loop VC7.1 treats as a possible store to a `long` (an address-taken local or a by-reference helper
//        inside the loop) that blocks the hoist.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6FA0, length 220 bytes (+ int3 padding), thiscall `ret 8`. Ego_r.pdb
// `?GetTrainableStatPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroTrainableStatType@@AAM@Z`; 16 callers in
// CTCInventoryExperience::AddStatToList (0x005729D8) and CTCInventoryStats::ConstructExperienceList (0x005D6748). The
// manifest label "Experience_GetLevelAndFraction" is descriptive.
// Callees: none (GetCategoryTrainableStatIsIn and GetTrainableStatMaxLevel are inline; the max level is re-evaluated each
// pass and once more on the capped exit).
// Layout: CTCHeroExperience retail (ExperiencePointsAvailableToSpend +0x14, StatExperiencePoints +0x18,
// TrainableStatLevels +0x24, PHeroStatsDef +0x5C); CHeroStatsDef StatLevels vector<CAbilityTrackDef> +0xFC (0x50-byte
// tracks: ExperienceStat +0x3C, Abilities vector<CAbilityTrackLevelDef> +0x40 of 0x48-byte levels, ExperienceCost +0x3C),
// as in the landed GetTrainableStatMaxLevel 0x006D6770 / GetCostToUpgradeTrainableStat 0x006D67B0.
// Shape: `fraction = 0; experience = StatExperiencePoints[category] + pool;` then `for (levels = 1;; ++levels)`: past the
// max level return `max - current`; else take the next level's cost, return `levels - 1` with `fraction =
// (float)experience / cost` when it is not affordable, otherwise pay it.
namespace std {

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};

}

enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };

class CAbilityTrackLevelDef {
public:
    unsigned char _base[0x3C];                              // CDefBase
    long ExperienceCost;                                    // +0x3C
    long AbilityUnlocked;                                   // +0x40
    long Description;                                       // +0x44
};

class CAbilityTrackDef {
public:
    unsigned char _base[0x3C];                              // CDefBase
    EHeroExperienceStatCategory ExperienceStat;             // +0x3C
    std::vector<CAbilityTrackLevelDef> Abilities;           // +0x40
    long MaxedOutDescription;                               // +0x4C
};

class CHeroStatsDef {
public:
    unsigned char _pad00[0xFC];
    std::vector<CAbilityTrackDef> StatLevels;               // +0xFC
};

class CTCHeroExperience {
public:
    virtual void v00();
    void* ParentThing;                                      // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    std::vector<long> StatExperiencePoints;                 // +0x18
    std::vector<long> TrainableStatLevels;                  // +0x24
    unsigned char _pad30[0x2C];                             // ExperienceMultiplierParam .. PHeroCombatDef
    CHeroStatsDef* PHeroStatsDef;                           // +0x5C

    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat) const
    {
        return PHeroStatsDef->StatLevels[stat].ExperienceStat;
    }
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const
    {
        const std::vector<CAbilityTrackLevelDef>& abilities = PHeroStatsDef->StatLevels[stat].Abilities;
        long maxLevel = (abilities.end() - abilities.begin()) - 1;
        if (maxLevel < 0)
            maxLevel = 0;
        return maxLevel;
    }
    long GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat, float& fractionTowardsNext);
};

long CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat, float& fractionTowardsNext)
{
    fractionTowardsNext = 0.0f;
    long experience = StatExperiencePoints[GetCategoryTrainableStatIsIn(stat)] + ExperiencePointsAvailableToSpend;
    for (long levels = 1; ; ++levels) {
        if (TrainableStatLevels[stat] + levels > GetTrainableStatMaxLevel(stat))
            return GetTrainableStatMaxLevel(stat) - TrainableStatLevels[stat];
        long cost = PHeroStatsDef->StatLevels[stat].Abilities[TrainableStatLevels[stat] + levels].ExperienceCost;
        if (experience < cost) {
            fractionTowardsNext = (float)experience / cost;
            return levels - 1;
        }
        experience -= cost;
    }
}
