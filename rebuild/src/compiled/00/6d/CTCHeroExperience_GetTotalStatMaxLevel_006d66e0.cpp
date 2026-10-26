// CTCHeroExperience::GetTotalStatMaxLevel(EHeroExperienceStatCategory category) const -- sum, over the stats-def ability tracks of the category, of each track's maximum level (ability count - 1, never below 0)
// Leaf: GetTotalStatMaxLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D66E0, length 144 bytes, `ret 4`.
// Shape: index loop over the def's StatLevels (0x50-byte tracks; STLport size by signed division re-evaluated each pass, the
// element walked by a strength-reduced pointer); the level count is the track's Abilities size (0x48-byte entries) minus one,
// clamped with `if (n < 0) n = 0;`; the stats def is read once through the CDefPointer.
// Layout: retail PHeroStatsDef +0x5c; CHeroStatsDef::StatLevels +0xfc; CAbilityTrackDef ExperienceStat +0x3c, Abilities +0x40.
enum EHeroExperienceStatCategory { HERO_STAT_CATEGORY_NONE };
class CAbilityTrackLevelDef { public: unsigned char _body[0x48]; };
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
class CAbilityTrackDef { public:                       // 0x50 bytes in retail
    unsigned char _base00[0x3c];
    EHeroExperienceStatCategory ExperienceStat;            // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;         // +0x40
    long MaxedOutDescription;                               // +0x4c
};
class CHeroStatsDef { public:
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;             // +0xfc
};
template <class T> class CDefPointer { public:
    T* Object;
    const T* operator->() const { return Object; }
};
class CThing;
class CTCHeroExperience {
public:
    virtual ~CTCHeroExperience();
    CThing*             ParentThing;            // +0x04
    unsigned char       _base08[0x4];
    long                FishingLevel;           // +0x0c
    long                TotalExperiencePoints;  // +0x10
    long                ExperiencePointsAvailableToSpend;   // +0x14
    CRetailVector<long> StatExperiencePoints;   // +0x18
    CRetailVector<long> TrainableStatLevels;    // +0x24
    float               ExperienceMultiplierParam;          // +0x30
    float               LastExperienceMultiplierParam;      // +0x34
    long                ExperienceAttenuationTimer;         // +0x38
    bool                AbleToGainExperience;               // +0x3c
    void*               Pickups;                // +0x40
    CRetailVector<long> ExperienceSpentOnStats; // +0x44
    void*               AttackHistory;          // +0x50
    CDefPointer<void>   PDef;                   // +0x54
    CDefPointer<void>   PHeroCombatDef;         // +0x58
    CDefPointer<CHeroStatsDef> PHeroStatsDef;   // +0x5c
    long GetTotalStatMaxLevel(EHeroExperienceStatCategory category) const;
};

long CTCHeroExperience::GetTotalStatMaxLevel(EHeroExperienceStatCategory category) const
{
    long total = 0;
    const CHeroStatsDef* statsDef = PHeroStatsDef.operator->();
    for (unsigned int i = 0; i < statsDef->StatLevels.size(); ++i) {
        const CAbilityTrackDef& track = statsDef->StatLevels[i];
        if (track.ExperienceStat == category) {
            long maxLevel = (long)track.Abilities.size() - 1;
            if (maxLevel < 0)
                maxLevel = 0;
            total += maxLevel;
        }
    }
    return total;
}