// Behaviour test for 006d7e10: src/006d7e10.cpp copied verbatim, the declared callee stubbed so its argument is observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroExperience::GetExperienceMultiplierFallbackIndex() const -- the index of the hero's current fallback multiplier in the def's FallbackMultipliers table (std::find, so the size when absent)
// Leaf: GetExperienceMultiplierFallbackIndex
// Grade: DIFFER(ours v retail) -- 88v86: the std::find call setup: retail pushes the empty tag temporary's address first
// (`lea ecx,[esp+7] ; push ecx`), stores the value temporary, then loads first/last into ecx/edx and pushes the value's address;
// ours stores the value and loads last/first before forming the tag and pushes in another order. The inlined pick-up and
// fallback multiplier (as in 0x006D7DE0) matches.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7E10, length 86 bytes (+ int3 padding). Manifest label
// "GetExperiencePickUpMultiplier" is the inlined callee's; Ego_r.pdb ?GetExperienceMultiplierFallbackIndex@CTCHeroExperience@@QBEJXZ.
// Callees: __ftol2 @ 0x00BFEA70, GetExperienceFallbackMultiplierForMultiplier @ 0x006D73D0, the __fastcall std::find
// (first, last, const long&, tag) @ 0x006BCC10 (manifest "std::find_4unroll").
// Tried (3 grades by the first agent; only this one-expression form was kept). Next: the multiplier and/or the tag as named
// locals before the call, else lane gap: tag-temporary / argument scheduling of the __fastcall find (GOTCHAS find-tag entries).
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

class CThing { public:
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

class CThingCreatureBase : public CThing { public:
    bool IsChild() const;                       // 0x00661F70
};

struct CTCCombat_Calls {
    void SetMeleeFlourishValid(bool valid);     // 0x00733E10
};

struct CIteratorTag { CIteratorTag() { } };
#pragma auto_inline(off)
// The out-of-line std::find instantiation @ 0x006BCC10 (STLport __find: four-way unrolled loop + remainder switch,
// __fastcall), defined in the TU with its real body so VC7.1 sees it reads only the range and the value.
__declspec(noinline) long* __fastcall Find(long* first, long* last, const long& value, const CIteratorTag&)
{
    long tripCount = (last - first) >> 2;
    for (; tripCount > 0; --tripCount) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
float __cdecl FloatLerpDummy();

struct CWorld_Calls {
    static long __fastcall GetFrame();          // 0x0049D870
};
extern long g_FramesPerSecond;                  // 0x01375550

template<class T> inline const T& Min(const T& left, const T& right) { return (right < left) ? right : left; }

template<class T> struct CRetailVector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() const { return _Myfirst; }
    T* end() const { return _Mylast; }
    unsigned long size() const { return _Mylast - _Myfirst; }
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator Begin() const { return iterator(_Myfirst); }
    T& operator[](unsigned long i) const { return *(Begin() + i); }
};

struct CRetailList { void* _Myhead; };

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer()
    {
        if (Object)
            if (--Object->m_count == 0)
                Object->Destroy();
    }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other);
    bool IsValid() const { return Object != 0; }
};

class CFloatRange { public:
    float HighValue;
    float LowValue;
};

class CTCHeroExperience;
class CHeroAbilityStatWeight { public:
    unsigned char StatWeights[0xc];
    float GetParameter(const CTCHeroExperience& experience) const;   // 0x00851BD0
};

class CAttackHistoryCombo { public:
    CRetailVector<long> AttackCombo;            // +0x00
    float Multiplier;                           // +0x0c
    bool IsComboSuccess(const CRetailList& history) const;   // 0x006D7080
};

class CComboMultiplierData { public:
    unsigned char ComboMultiplier[0xc];
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0c
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

class CHeroExperienceDef { public:
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

class CHeroCombatDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x44];
    CFloatRange RangedAccuracyPercentWithStats;             // +0x4c
    CHeroAbilityStatWeight RangedAccuracyWeighting;         // +0x54
    unsigned char _pad60[0x90];
    CFloatRange MeleeStrikeAnimationSpeedFractionWithStats; // +0xf0
    CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting;  // +0xf8
};

class CAbilityTrackLevelDef { public:
    unsigned char _base[0x3c];
    long ExperienceCost;                        // +0x3c
    long AbilityUnlocked;                       // +0x40
    long Description;                           // +0x44
};

class CAbilityTrackDef { public:
    unsigned char _base[0x3c];
    long ExperienceStat;                        // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;   // +0x40
    long MaxedOutDescription;                   // +0x4c
};

class CHeroStatsDef { public:
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

    CDefPointer<CHeroCombatDef> GetPHeroCombatDef() const { return PHeroCombatDef; }

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
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const { return TrainableStatLevels[stat]; }
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
    long GetExperienceFallbackMultiplier() const
    {
        return GetExperienceFallbackMultiplierForMultiplier(GetExperiencePickUpMultiplier());
    }
    long GetExperienceMultiplierFallbackIndex() const;
    void OnComboMultiplierDecrease();
};

long CTCHeroExperience::GetExperienceMultiplierFallbackIndex() const
{
    long* found = Find(PDef->FallbackMultipliers.begin(), PDef->FallbackMultipliers.end(), GetExperienceFallbackMultiplier(), CIteratorTag());
    return found - PDef->FallbackMultipliers.begin();
}

// ---- stubs ----
static long g_seenMultiplier = -99;
static long g_fallbackResult = 0;
static int g_fallbackCalls = 0;
long CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier(long multiplier) const
{
    ++g_fallbackCalls;
    g_seenMultiplier = multiplier;
    return g_fallbackResult;
}
void CTCHeroExperience::v00() { }

int main()
{
    static unsigned char defMem[0x200];
    memset(defMem, 0, sizeof(defMem));
    CHeroExperienceDef* def = (CHeroExperienceDef*)defMem;
    long table[9] = { 5, 7, 9, 11, 13, 15, 17, 19, 21 };
    def->ExperienceMultiplierParamPerUnitComboMultiplier = 2.0f;
    static unsigned char heroMem[0x80];
    memset(heroMem, 0, sizeof(heroMem));
    CTCHeroExperience* hero = (CTCHeroExperience*)heroMem;
    hero->PDef = def;

    // every table length 0..9 (all remainder-switch cases and full trips), every present position and a miss
    for (long n = 0; n <= 9; ++n) {
        def->FallbackMultipliers._Myfirst = table;
        def->FallbackMultipliers._Mylast = table + n;
        def->FallbackMultipliers._Myend = table + n;
        for (long i = 0; i < n; ++i) {
            g_fallbackResult = table[i];
            CHECK(hero->GetExperienceMultiplierFallbackIndex() == i);
        }
        g_fallbackResult = 4;   // absent -> size
        CHECK(hero->GetExperienceMultiplierFallbackIndex() == n);
    }
    // the multiplier passed on: param / 2.0 + 0.5 truncated, clamped to at least 1
    def->FallbackMultipliers._Mylast = table + 9;
    g_fallbackCalls = 0;
    hero->ExperienceMultiplierParam = 7.0f;     // 3.5 + 0.5 = 4
    g_fallbackResult = 13;
    CHECK(hero->GetExperienceMultiplierFallbackIndex() == 4);
    CHECK(g_seenMultiplier == 4);
    hero->ExperienceMultiplierParam = 6.8f;     // 3.4 + 0.5 = 3.9 -> 3
    hero->GetExperienceMultiplierFallbackIndex();
    CHECK(g_seenMultiplier == 3);
    hero->ExperienceMultiplierParam = 0.0f;     // 0.5 -> 0 -> clamped to 1
    hero->GetExperienceMultiplierFallbackIndex();
    CHECK(g_seenMultiplier == 1);
    hero->ExperienceMultiplierParam = -10.0f;   // negative -> clamped to 1
    hero->GetExperienceMultiplierFallbackIndex();
    CHECK(g_seenMultiplier == 1);
    CHECK(g_fallbackCalls == 4);
    // duplicates: the first match wins
    long dup[6] = { 3, 8, 8, 8, 2, 8 };
    def->FallbackMultipliers._Myfirst = dup;
    def->FallbackMultipliers._Mylast = dup + 6;
    g_fallbackResult = 8;
    CHECK(hero->GetExperienceMultiplierFallbackIndex() == 1);
    g_fallbackResult = 2;
    CHECK(hero->GetExperienceMultiplierFallbackIndex() == 4);
    if (fails == 0) printf("006d7e10_TEST PASS\n");
    return fails;
}