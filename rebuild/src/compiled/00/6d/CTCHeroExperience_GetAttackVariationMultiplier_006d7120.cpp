// CTCHeroExperience::GetAttackVariationMultiplier() const -- the highest multiplier among the def's attack-history combos that the hero's attack history currently satisfies (at least 1.0)
// Leaf: GetAttackVariationMultiplier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7120, length 73 bytes (+ int3 padding).
// Callees: CAttackHistoryCombo::IsComboSuccess(const list<pair<EDamageAttribute,long> >&) const @ 0x006D7080 (on AttackHistory +0x50).
// Shape: the running multiplier stays in st(0) across IsComboSuccess because that callee (integer-only) is compiled
// above this function in the same TU (VC7.1 callee knowledge); its body here is behaviour-equal to 0x006D7080 (146 bytes,
// ours 138: retail walks the list with iterator copies) and serves only as that knowledge.
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
long* __fastcall Find(long* first, long* last, const long& value, const CIteratorTag& tag);   // 0x006BCC10
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

struct CRetailList {                            // list<pair<EDamageAttribute, long> >: the head node pointer
    struct SNode { SNode* Next; SNode* Prev; long Attribute; long Value; };
    SNode* _Myhead;
};

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
    struct SEntry { long Type; long Attribute; };   // pair<EAttackHistoryType, EDamageAttribute>
    CRetailVector<SEntry> AttackCombo;          // +0x00
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
    long GetExperienceFallbackMultiplier() const;
    long GetExperienceMultiplierFallbackIndex() const;
    void OnComboMultiplierDecrease();
};

// Same-TU body of CAttackHistoryCombo::IsComboSuccess @ 0x006D7080 (retail's TU, above the caller): integer-only, so
// VC7.1 keeps the running multiplier on the FPU stack across the call, as retail does. The combo is matched backwards
// against the newest attacks: type 2 = this attribute, type 1 = a change from the previous attack, type 0 = a repeat.
__declspec(noinline) bool CAttackHistoryCombo::IsComboSuccess(const CRetailList& history) const
{
    CRetailList::SNode* head = history._Myhead;
    CRetailList::SNode* first = head->Next;
    if (first == head)
        return false;
    CRetailList::SNode* node = head->Prev;
    long current = node->Attribute;
    for (const SEntry* it = AttackCombo.end(); it != AttackCombo.begin(); --it) {
        if (current == -1)
            return false;
        long type = it[-1].Type;
        if (type == 2 && current != it[-1].Attribute)
            return false;
        long previous;
        if (node == first || (previous = node->Prev->Attribute) == -1) {
            current = -1;
            continue;
        }
        if (type == 1) {
            if (current == previous)
                return false;
        }
        else if (type == 0 && current != previous)
            return false;
        node = node->Prev;
        current = previous;
    }
    return true;
}

float CTCHeroExperience::GetAttackVariationMultiplier() const
{
    float multiplier = 1.0f;
    CAttackHistoryCombo* last = PDef->AttackHistoryCombos.end();
    for (CAttackHistoryCombo* it = PDef->AttackHistoryCombos.begin(); it != last; ++it)
        if (multiplier < it->Multiplier && it->IsComboSuccess(AttackHistory))
            multiplier = it->Multiplier;
    return multiplier;
}