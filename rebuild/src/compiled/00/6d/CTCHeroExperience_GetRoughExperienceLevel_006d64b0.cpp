// CTCHeroExperience::GetRoughExperienceLevel() const -- the number of experience-level thresholds of the def that the hero's total experience has reached
// Leaf: GetRoughExperienceLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D64B0, length 56 bytes.
// Shape: an unsigned counter loop `for (level = 0; level < thresholds.size(); ++level) if (total < thresholds[level]) break;`
// over PDef->ExperienceLevelThresholds (+0x54 -> +0x28): size() is re-evaluated every iteration (`(end-begin) >> 2`, `jbe`/`jb`
// unsigned), the element walk is strength-reduced into ecx (`add ecx,4`) with the total kept in esi, aligned loop head.
// Model: CTCHeroExperience in its retail layout (VC7.1 release containers, 12-byte vectors): CTCBase +0..+9, FishingLevel +0xc,
// TotalExperiencePoints +0x10, ExperiencePointsAvailableToSpend +0x14, StatExperiencePoints +0x18, TrainableStatLevels +0x24,
// ExperienceMultiplierParam +0x30, LastExperienceMultiplierParam +0x34, ExperienceAttenuationTimer +0x38, AbleToGainExperience
// +0x3c, Pickups +0x40, ExperienceSpentOnStats +0x44, AttackHistory +0x50, PDef +0x54, PHeroCombatDef +0x58 (counted
// CDefPointer), PHeroStatsDef +0x5c. engine/CTCHeroExperience.h carries the Ego_r.pdb debug-STL layout (TrainableStatLevels
// +0x28, PDef +0x70, ...), which disagrees with the retail offsets from +0x24 on, so the class is modelled locally (as the
// landed 0x006D8CA0 Copy and 0x006D6850 GetCategoryTrainableStatIsIn do). The def classes carry Ego_r.pdb member names at
// their retail offsets; only the members the bodies read are placed. CTCCombat_Calls / CWorld_Calls: engine/CTCCombat.h and
// engine/CWorld.h hold those classes' data layouts; only calls are needed here, so they are call-only structs.
struct CTCBaseComponent;

struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x20];
    unsigned long ThingTypeFlags;               // +0x6c (bit 3: creature)
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing {
    bool IsChild() const;                       // 0x00661F70
};

struct CTCCombat_Calls {
    void SetMeleeFlourishValid(bool valid);     // 0x00733E10
};

struct CWorld_Calls {
    static long __fastcall GetFrame();          // 0x0049D870
};
extern long g_FramesPerSecond;                  // 0x01375550

template<class T> struct CRetailVector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() const { return _Myfirst; }
    T* end() const { return _Mylast; }
    unsigned long size() const { return _Mylast - _Myfirst; }
    T& operator[](unsigned long i) const { return _Myfirst[i]; }
};

struct CRetailList { void* _Myhead; };

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer()
    {
        if (Object)
            if (--Object->m_count == 0)
                Object->Destroy();
    }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
};

struct CFloatRange {
    float HighValue;
    float LowValue;
};

class CTCHeroExperience;
struct CHeroAbilityStatWeight {
    unsigned char StatWeights[0xc];
    float GetParameter(const CTCHeroExperience& experience) const;   // 0x00851BD0
};

struct CAttackHistoryCombo {
    CRetailVector<long> AttackCombo;            // +0x00
    float Multiplier;                           // +0x0c
    bool IsComboSuccess(const CRetailList& history) const;   // 0x006D7080
};

struct CComboMultiplierData {
    unsigned char ComboMultiplier[0xc];
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0c
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

struct CHeroExperienceDef {
    unsigned char _base[0x28];
    CRetailVector<long> ExperienceLevelThresholds;          // +0x28
    CRetailVector<long> HeroLevelTitles;                    // +0x34
    CRetailVector<long> FallbackMultipliers;                // +0x40
    unsigned char SoundIntensityThresholds[0xc];            // +0x4c
    unsigned char ComboMultiplierMap[0xc];                  // +0x58
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
    float ExperienceMultiplierRegionChangeMultiplier;       // +0x68
    unsigned char ComboMultiplierIncrease[0xc];             // +0x6c
    unsigned char ComboMultiplierLock[0xc];                 // +0x78
    unsigned char ComboMultiplierLost[0xc];                 // +0x84
    unsigned char ComboMultiplierRunningHitsIncrease[0xc];  // +0x90
    unsigned char StatIncreasePerHitType[0xc];              // +0x9c
    CRetailVector<CAttackHistoryCombo> AttackHistoryCombos; // +0xa8
};

struct CHeroCombatDef : CDefPointeeBase {
    unsigned char _pad08[0x44];
    CFloatRange RangedAccuracyPercentWithStats;             // +0x4c
    CHeroAbilityStatWeight RangedAccuracyWeighting;         // +0x54
    unsigned char _pad60[0x90];
    CFloatRange MeleeStrikeAnimationSpeedFractionWithStats; // +0xf0
    CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting;  // +0xf8
};

struct CAbilityTrackLevelDef {
    unsigned char _base[0x3c];
    long ExperienceCost;                        // +0x3c
    long AbilityUnlocked;                       // +0x40
    long Description;                           // +0x44
};

struct CAbilityTrackDef {
    unsigned char _base[0x3c];
    long ExperienceStat;                        // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;   // +0x40
    long MaxedOutDescription;                   // +0x4c
};

struct CHeroStatsDef {
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;             // +0xfc
    CRetailVector<float> MaxHitPointsPerHealthLevel;        // +0x108
    CRetailVector<long> MaxStaminaPointsPerMagicPowerLevel; // +0x114
    CRetailVector<float> SoundDistanceMultiplierPerStealthLevel;   // +0x120
    CRetailVector<float> VisibilityMultiplierPerStealthLevel;      // +0x12c
};

enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };

class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    CRetailVector<long> StatExperiencePoints;               // +0x18
    CRetailVector<long> TrainableStatLevels;                // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3c
    CRetailList Pickups;                                    // +0x40
    CRetailVector<long> ExperienceSpentOnStats;             // +0x44
    CRetailList AttackHistory;                              // +0x50
    CHeroExperienceDef* PDef;                               // +0x54
    CDefPointer<CHeroCombatDef> PHeroCombatDef;             // +0x58
    CHeroStatsDef* PHeroStatsDef;                           // +0x5c

    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat) const
    {
        return (EHeroExperienceStatCategory)PHeroStatsDef->StatLevels[stat].ExperienceStat;
    }
    void SpendExperience(EHeroExperienceStatCategory category, long amount);
    void SpendExperience(EHeroTrainableStatType stat, long amount);
    bool IsAbleToBreakDownDoors() const;
    long GetRoughExperienceLevel() const;
    float GetSoundRadiusMultiplier() const;
    float GetVisibilityMultiplier() const;
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const;
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat) const;
    float GetMeleeStrikeAnimationSpeedMultiplier() const;
    long GetShotAccuracyPercentage() const;
    float GetAttackVariationMultiplier() const;
    void OnRegionLoad();
    const CComboMultiplierData& GetActiveComboMultiplierData() const;   // 0x006D72D0
    long GetExperienceFallbackMultiplierForMultiplier(long multiplier) const;   // 0x006D73D0
    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    long GetExperienceFallbackMultiplier() const;
    long GetExperienceMultiplierFallbackIndex() const;
    void OnComboMultiplierDecrease();
};

long CTCHeroExperience::GetRoughExperienceLevel() const
{
    unsigned long level;
    for (level = 0; level < PDef->ExperienceLevelThresholds.size(); ++level)
        if (TotalExperiencePoints < PDef->ExperienceLevelThresholds[level])
            break;
    return level;
}