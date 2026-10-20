// CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType) -- raises a trainable stat one level (capped by its ability track), refreshes the abilities and ages the hero
// Grade: DIFFER(182v183) -- retail loads the track's first pointer into ebx before `sub` (`mov ebx,[eax+edx+40h] ; ... ; sub edx,ebx`) and keeps the level vector base in edi; ours subtracts from memory and re-reads the base
// Leaf: UpgradeTrainableStat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8980, 183 bytes, `ret 4`.
// Callees: CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType, bool) @ 0x006D8170,
//          CTCHeroExperience::OnStatLevelChange(bool) @ 0x006D83B0, CTCBase::GetDefinitionManager() @ 0x006869D0,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCHeroStats::SetAge(float) @ 0x0057B05F.
// Shape: the cap is `(long)levels.size() - 1` floored at 0 over the stat's CAbilityTrackDef::Abilities (72-byte
// CAbilityTrackLevelDef records: the signed `imul 38E38E39h ; sar 4` divide), the track reached through a reference
// local (`lea eax,[eax+edx+40h]`). The ageing def is fetched from the definition manager before the hero-stats
// probe (inline raw bitset test, interface 4, then an inline GetTC); the new age is `stats->Age + def->X` (the
// right operand loaded first).
// Layout: retail CTCHeroExperience keeps 12-byte vectors (engine/CTCHeroExperience.h carries the PDB containers):
// TrainableStatLevels +0x24, PHeroStatsDef +0x5C (PDB +0x78); CHeroStatsDef::StatLevels +0xFC (PDB +0x114);
// CAbilityTrackDef is 0x50 bytes in retail with Abilities at +0x40; CTCHeroStats::Age +0x30. The definition
// manager's +0xD0 def and its +0xD0 float are not identified (named descriptively).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };

struct CTCHeroStats {                                // interface type 4
    unsigned char _pad_0x00[0x30];
    float         Age;                               // +0x30 (PDB +0x38)
    void SetAge(float age);                          // 0x0057B05F
};

struct CAbilityTrackLevelDef {                       // 72 bytes
    unsigned char _data[0x48];
};

struct CAbilityTrackLevelIterator {
    const CAbilityTrackLevelDef* _M_p;
    CAbilityTrackLevelIterator(const CAbilityTrackLevelDef* p) : _M_p(p) {}
};
inline long operator-(const CAbilityTrackLevelIterator& last, const CAbilityTrackLevelIterator& first)
{
    return last._M_p - first._M_p;
}

struct CAbilityTrackLevelVector {                    // retail std::vector<CAbilityTrackLevelDef> {first, last, end}
    CAbilityTrackLevelDef* _M_start;
    CAbilityTrackLevelDef* _M_finish;
    CAbilityTrackLevelDef* _M_end_of_storage;
    CAbilityTrackLevelIterator begin() const { return CAbilityTrackLevelIterator(_M_start); }
    CAbilityTrackLevelIterator end() const { return CAbilityTrackLevelIterator(_M_finish); }
    long size() const { return end() - begin(); }
};

struct CAbilityTrackDef {                            // 0x50 bytes in retail
    unsigned char            _base_0x00[0x3C];
    long                     ExperienceStat;         // +0x3C
    CAbilityTrackLevelVector Abilities;              // +0x40
    long                     MaxedOutDescription;    // +0x4C
};

struct CHeroStatsDef {
    unsigned char     _pad_0x00[0xFC];
    CAbilityTrackDef* StatLevels;                    // +0xFC vector<CAbilityTrackDef>::_M_start (PDB +0x114)
};

struct CAgeingDef_d0 {
    unsigned char _pad_0x00[0xD0];
    float         AgeIncreasePerStatLevel;           // +0xD0
};

struct CDefinitionManager {
    unsigned char  _pad_0x00[0xD0];
    CAgeingDef_d0* PAgeingDef;                       // +0xD0
};

struct CLongVector {                                 // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long& operator[](unsigned long n) { return _M_start[n]; }
};

struct CTCHeroExperience {
    void*          __vftable;                        // +0x00
    CThing*        ParentThing;                      // +0x04 CTCBase
    unsigned char  _pad_0x08[0x1C];
    CLongVector    TrainableStatLevels;              // +0x24 (PDB +0x28)
    unsigned char  _pad_0x30[0x2C];
    CHeroStatsDef* PHeroStatsDef;                    // +0x5C CDefPointer<const CHeroStatsDef>::Object (PDB +0x78)

    CDefinitionManager* GetDefinitionManager() const;              // 0x006869D0 (CTCBase)
    void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent);   // 0x006D8170
    void OnStatLevelChange(bool silent);                           // 0x006D83B0
    void UpgradeTrainableStat(EHeroTrainableStatType stat);
};

void CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat)
{
    const CAbilityTrackLevelVector& abilities = PHeroStatsDef->StatLevels[stat].Abilities;
    long maxLevel = abilities.size() - 1;
    if (maxLevel < 0)
        maxLevel = 0;
    if (TrainableStatLevels[stat] < maxLevel) {
        TrainableStatLevels[stat] = TrainableStatLevels[stat] + 1;
        UpdateAbilitiesForStat(stat, false);
        OnStatLevelChange(true);
        CAgeingDef_d0* ageing = GetDefinitionManager()->PAgeingDef;
        CThing* thing = ParentThing;
        if (thing->HasTC(4)) {
            CTCHeroStats* stats = thing->GetTC<CTCHeroStats>(4);
            stats->SetAge(stats->Age + ageing->AgeIncreasePerStatLevel);
        }
    }
}
