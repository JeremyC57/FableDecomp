// CTCHeroExperience::IsReadyToLevelUp() const -- true when the hero can afford the next level of any trainable stat track (its stat pool plus the spendable points against that track's next ExperienceCost) or the first upgrade of any potentially-available, not-maxed ability 1..19
// Leaf: IsReadyToLevelUp
//
// Retail 0x006D7CB0, 304 bytes incl. padding (plain `ret`). Ego_r.pdb `?IsReadyToLevelUp@CTCHeroExperience@@QBE_NXZ`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for
// CTCInventoryAbilities, type 0x6F, through a const thing), CTCInventoryAbilities::IsAbilityPotentiallyAvailable @ 0x005EE02E,
// ::GetAbilityMaximumLevel @ 0x005EDEA3, ::GetAbilityLevel @ 0x005EDE93, ::GetAbilityStatType @ 0x005EDEEB,
// ::GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool) @ 0x005EE77C.
// Layout (retail, 12-byte STLport vectors, 0x3C-byte CDef base): CTCHeroExperience ExperiencePointsAvailableToSpend +0x14,
// StatExperiencePoints +0x18, TrainableStatLevels +0x24, PHeroStatsDef +0x5C; CHeroStatsDef StatLevels +0xFC
// (CAbilityTrackDef, 0x50 bytes: ExperienceStat +0x3C, Abilities +0x40); CAbilityTrackLevelDef 0x48 bytes, ExperienceCost +0x3C.
// Shape: the stat loop runs 0..5 (strength-reduced: track pointer += 0x50, level pointer += 4); the next-level cost is
// `level < Abilities.size() - 1 ? Abilities[level + 1].ExperienceCost : -1` (unsigned `jae`, /72 by magic multiply).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
enum EHeroAbility { HERO_ABILITY_NONE = 0 };
struct CTCInventoryAbilities {
    bool IsAbilityPotentiallyAvailable(EHeroAbility ability);       // 0x005EE02E
    long GetAbilityMaximumLevel(EHeroAbility ability);              // 0x005EDEA3
    long GetAbilityLevel(EHeroAbility ability);                     // 0x005EDE93
    long GetAbilityStatType(EHeroAbility ability);                  // 0x005EDEEB
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool flag);   // 0x005EE77C
};
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
struct CDefBase { unsigned char _base[0x3c]; };
struct CAbilityTrackLevelDef : CDefBase {                   // 0x48
    long ExperienceCost;                                    // +0x3c
    long AbilityUnlocked;                                   // +0x40
    long Description;                                       // +0x44
};
struct CAbilityTrackDef : CDefBase {                        // 0x50
    long ExperienceStat;                                    // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;         // +0x40
    long MaxedOutDescription;                               // +0x4c
};
struct CHeroStatsDef {
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;             // +0xfc
};
template <class T> struct CDefPointer {
    T* Ptr;
    T* operator->() const { return Ptr; }
};
class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    CRetailVector<long> StatExperiencePoints;               // +0x18
    CRetailVector<long> TrainableStatLevels;                // +0x24
    unsigned char _pad30[0x5c - 0x30];
    CDefPointer<const CHeroStatsDef> PHeroStatsDef;         // +0x5c
    const CThing* GetThing() const { return ParentThing; }
    bool IsReadyToLevelUp() const;
};

bool CTCHeroExperience::IsReadyToLevelUp() const
{
    long available = ExperiencePointsAvailableToSpend;
    for (long stat = 0; stat <= 5; ++stat) {
        const CAbilityTrackDef& track = PHeroStatsDef->StatLevels[stat];
        long experience = StatExperiencePoints[track.ExperienceStat];
        unsigned int level = TrainableStatLevels[stat];
        long cost;
        if (level < track.Abilities.size() - 1)
            cost = track.Abilities[level + 1].ExperienceCost;
        else
            cost = -1;
        if (experience + available >= cost)
            return true;
    }
    CTCInventoryAbilities* abilities;
    if (!GetThing()->GetInterface(&abilities, 0x6f))
        return false;
    for (long a = 1; a < 20; ++a) {
        EHeroAbility ability = (EHeroAbility)a;
        if (abilities->IsAbilityPotentiallyAvailable(ability)) {
            long maxLevel = abilities->GetAbilityMaximumLevel(ability);
            if (abilities->GetAbilityLevel(ability) < maxLevel) {
                long experience = StatExperiencePoints[abilities->GetAbilityStatType(ability)];
                if (experience + available >= abilities->GetExperienceCostToUpgradeAbility(ability, 1, true))
                    return true;
            }
        }
    }
    return false;
}