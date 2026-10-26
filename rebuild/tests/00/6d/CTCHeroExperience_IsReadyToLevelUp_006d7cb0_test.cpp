// Behaviour test for CTCHeroExperience::IsReadyToLevelUp (retail 0x006D7CB0): model copy + stub ability inventory.
#include <cstdio>

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
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
class CTCInventoryAbilities { public:
    bool avail[20]; long level[20]; long maxLevel[20]; long stat[20]; long cost[20];
    long costCalls;
    bool IsAbilityPotentiallyAvailable(EHeroAbility a) { return avail[a]; }
    long GetAbilityMaximumLevel(EHeroAbility a) { return maxLevel[a]; }
    long GetAbilityLevel(EHeroAbility a) { return level[a]; }
    long GetAbilityStatType(EHeroAbility a) { return stat[a]; }
    long GetExperienceCostToUpgradeAbility(EHeroAbility a, long levels, bool flag) { ++costCalls; return (levels == 1 && flag) ? cost[a] : 99999; }
};
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
class CAbilityTrackLevelDef { public: long ExperienceCost; long AbilityUnlocked; long Description; };
class CAbilityTrackDef { public: long ExperienceStat; CRetailVector<CAbilityTrackLevelDef> Abilities; long MaxedOutDescription; };
class CHeroStatsDef { public: CRetailVector<CAbilityTrackDef> StatLevels; };
template <class T> class CDefPointer { public: T* Ptr; T* operator->() const { return Ptr; } };
class CTCHeroExperience {
public:
    CThing* ParentThing;
    long ExperiencePointsAvailableToSpend;
    CRetailVector<long> StatExperiencePoints;
    CRetailVector<long> TrainableStatLevels;
    CDefPointer<const CHeroStatsDef> PHeroStatsDef;
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

template <class T> static void SetVec(CRetailVector<T>& v, T* p, int n) { v._M_start = p; v._M_finish = p + n; v._M_end_of_storage = p + n; }

int main()
{
    int fail = 0;
    // six stat tracks with three levels each, costs 100/200/300 (cost of level n at index n)
    CAbilityTrackLevelDef levels[6][3];
    CAbilityTrackDef tracks[6];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) { levels[i][j].ExperienceCost = 100 * (j + 1) + i; levels[i][j].AbilityUnlocked = 0; levels[i][j].Description = 0; }
        tracks[i].ExperienceStat = i % 4;
        SetVec(tracks[i].Abilities, levels[i], 3);
        tracks[i].MaxedOutDescription = 0;
    }
    CHeroStatsDef def; SetVec(def.StatLevels, tracks, 6);
    long statXp[4] = { 10, 20, 30, 40 };
    long trainLevels[6] = { 0, 0, 0, 0, 0, 0 };

    CTCInventoryAbilities inv;
    for (int a = 0; a < 20; ++a) { inv.avail[a] = false; inv.level[a] = 0; inv.maxLevel[a] = 3; inv.stat[a] = a % 4; inv.cost[a] = 1000; }
    inv.costCalls = 0;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x6f, (CTCBase*)&inv } };
    CThing thing;
    for (int k = 0; k < 9; ++k) thing.m_presentBits[k] = 0;
    thing.m_presentBits[0x6f >> 5] = 1UL << (0x6f & 31);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;

    CTCHeroExperience hx;
    hx.ParentThing = &thing;
    hx.ExperiencePointsAvailableToSpend = 0;
    SetVec(hx.StatExperiencePoints, statXp, 4);
    SetVec(hx.TrainableStatLevels, trainLevels, 6);
    hx.PHeroStatsDef.Ptr = &def;

    // 1. nothing affordable anywhere: false, no ability costs asked (none available)
    if (hx.IsReadyToLevelUp() || inv.costCalls != 0) fail = 1;
    // 2. stat 3 track (stat category 3, xp 40) at level 0: next cost 203; available 163 -> 40 + 163 >= 203
    hx.ExperiencePointsAvailableToSpend = 163;
    if (!hx.IsReadyToLevelUp()) fail = 2;
    hx.ExperiencePointsAvailableToSpend = 162;            // 40+162=202 < 203; others lower xp: still false
    if (hx.IsReadyToLevelUp()) fail = 3;
    // 3. a maxed track (level 2 of 3 -> cost -1) is always "ready"
    trainLevels[4] = 2;
    hx.ExperiencePointsAvailableToSpend = 0;
    if (!hx.IsReadyToLevelUp()) fail = 4;
    trainLevels[4] = 0;
    // 4. ability path: ability 7 available, level 1 < max 3, stat 7%4=3 (40 xp) + 960 >= 1000
    hx.ExperiencePointsAvailableToSpend = 160;            // stat tracks: 40+160=200 < 203, 30+160=190 < 202 ... false
    inv.avail[7] = true; inv.level[7] = 1;
    if (hx.IsReadyToLevelUp()) fail = 5;                  // 40+160=200 < 1000
    if (inv.costCalls != 1) fail = 6;
    inv.cost[7] = 200;
    if (!hx.IsReadyToLevelUp()) fail = 7;
    // 5. maxed ability is skipped
    inv.level[7] = 3; inv.costCalls = 0;
    if (hx.IsReadyToLevelUp() || inv.costCalls != 0) fail = 8;
    // 6. ability 0 and 20+ never asked; without the inventory -> false
    inv.avail[0] = true; inv.level[0] = 0; inv.cost[0] = 0;
    if (hx.IsReadyToLevelUp()) fail = 9;
    thing.m_presentBits[0x6f >> 5] = 0;
    inv.level[7] = 0;
    if (hx.IsReadyToLevelUp()) fail = 10;

    if (fail) { std::printf("006d7cb0_TEST FAIL %d\n", fail); return 1; }
    std::printf("006d7cb0_TEST PASS\n");
    return 0;
}