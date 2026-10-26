// CTCHeroExperience::OnStatLevelChange(bool) -- pushes the clamped strength and skill level fractions into the hero morph, refreshes the will morph and (optionally) the expression list, and updates the creature's front mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D83B0, 447 bytes (+ int3 padding), `ret 4`.
// Callees: CTCHeroExperience::GetTotalStatLevel 0x006D6660 and GetTotalStatMaxLevel 0x006D66E0 (both landed; their real
// bodies sit above under `auto_inline(off)`: retail calls GetTotalStatMaxLevel WITHOUT reloading ecx because the same-TU
// GetTotalStatLevel never writes ecx, and VC7.1 only knows that from the visible body), CVectorMap::LowerBound 0x0040F020
// (inline lookups CTCHeroMorph 3, CTCInventoryAbilities 0x6F, CTCHeroStats 4, CTCCreatureModeManager 0x31),
// CTCHeroMorph::SetStrength 0x0071BE20 / SetSkill 0x0071BF40 (manifest "SetAge" is propagated; Ego_r.pdb order SetStrength,
// SetWill, SetBerserkSkin, SetSkill at the same 0x60 spacing; they store +0x40 / +0x4C), CTCInventoryAbilities::
// UpdateWillMorph 0x005EE887, CTCHeroStats::CheckForNewExpressions 0x0057BE71, CTCCreatureModeManager 0x0073A840
// (manifest "CNavQuadTreeNode::RemoveNeighbours" is propagated; landed callers name it UpdateFrontMode / RestartCurrentMode /
// OnUnsheatheFinished -- the name is unconfirmed).
// Shape: each fraction is `(float)level / maxLevel` clamped with the landed GFLimit(T&, const T&, const T&) (`fst` into the
// frame, `test ah,5 ; jp` for < 0, `test ah,41h ; jne` for > 1); the morph lookup is checked, the mode-manager one unchecked
// (absent arm reads the dead bool slot the key was homed in), and VC7.1 duplicates the final call into both arms.
// Model: the landed 0x006D6660 / 0x006D66E0 CTCHeroExperience retail layout (12-byte STLport vectors; engine/CTCHeroExperience.h
// carries the Ego_r.pdb debug-STL offsets).
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

template <class T> __forceinline void GFLimit(T& value, const T& lo, const T& hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}

class CTCHeroMorph { public:
    void SetStrength(float strength);           // 0x0071BE20
    void SetSkill(float skill);                 // 0x0071BF40
};
class CTCInventoryAbilities { public: void UpdateWillMorph(); };          // 0x005EE887
class CTCHeroStats { public: void CheckForNewExpressions(); };            // 0x0057BE71
class CTCCreatureModeManager { public: void UpdateFrontMode(); };         // 0x0073A840

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
    long GetTotalStatLevel(EHeroExperienceStatCategory category) const;     // 0x006D6660
    long GetTotalStatMaxLevel(EHeroExperienceStatCategory category) const;  // 0x006D66E0
    void OnStatLevelChange(bool checkExpressions);
};

#pragma auto_inline(off)
long CTCHeroExperience::GetTotalStatLevel(EHeroExperienceStatCategory category) const
{
    long total = 0;
    for (unsigned int i = 0; i < TrainableStatLevels.size() && i < PHeroStatsDef->StatLevels.size(); ++i) {
        if (PHeroStatsDef->StatLevels[i].ExperienceStat == category)
            total += TrainableStatLevels[i];
    }
    return total;
}

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
#pragma auto_inline(on)

void CTCHeroExperience::OnStatLevelChange(bool checkExpressions)
{
    float strength = (float)GetTotalStatLevel((EHeroExperienceStatCategory)0) / GetTotalStatMaxLevel((EHeroExperienceStatCategory)0);
    GFLimit(strength, 0.0f, 1.0f);
    float skill = (float)GetTotalStatLevel((EHeroExperienceStatCategory)1) / GetTotalStatMaxLevel((EHeroExperienceStatCategory)1);
    GFLimit(skill, 0.0f, 1.0f);
    CTCHeroMorph* morph;
    if (ParentThing->GetInterface(&morph, 3)) {
        morph->SetStrength(strength);
        morph->SetSkill(skill);
    }
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        abilities->UpdateWillMorph();
    CTCHeroStats* stats;
    if (checkExpressions && ParentThing->GetInterface(&stats, 4))
        stats->CheckForNewExpressions();
    CTCCreatureModeManager* modes;
    ParentThing->GetInterface(&modes, 0x31);
    modes->UpdateFrontMode();
}