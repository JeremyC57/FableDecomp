#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __stdcall
#endif
#include <cstdio>
#include <cstdlib>
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

struct CTCHeroMorph {
    void SetStrength(float strength);           // 0x0071BE20
    void SetSkill(float skill);                 // 0x0071BF40
};
struct CTCInventoryAbilities { void UpdateWillMorph(); };          // 0x005EE887
struct CTCHeroStats { void CheckForNewExpressions(); };            // 0x0057BE71
struct CTCCreatureModeManager { void UpdateFrontMode(); };         // 0x0073A840

enum EHeroExperienceStatCategory { HERO_STAT_CATEGORY_NONE };
struct CAbilityTrackLevelDef { unsigned char _body[0x48]; };
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CAbilityTrackDef {                       // 0x50 bytes in retail
    unsigned char _base00[0x3c];
    EHeroExperienceStatCategory ExperienceStat;            // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;         // +0x40
    long MaxedOutDescription;                               // +0x4c
};
struct CHeroStatsDef {
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;             // +0xfc
};
template <class T> struct CDefPointer {
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

CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static float g_str = -1, g_skill = -1; static int g_will = 0, g_expr = 0, g_mode = 0;
void CTCHeroMorph::SetStrength(float v) { g_str = v; }
void CTCHeroMorph::SetSkill(float v) { g_skill = v; }
void CTCInventoryAbilities::UpdateWillMorph() { ++g_will; }
void CTCHeroStats::CheckForNewExpressions() { ++g_expr; }
void CTCCreatureModeManager::UpdateFrontMode() { ++g_mode; }
CTCHeroExperience::~CTCHeroExperience() { }

int main()
{
    // four tracks: strength (cat 0) with 4 abilities, strength with 3, skill (cat 1) with 5, will (cat 2) with 2
    static CAbilityTrackLevelDef levels[20];
    static CAbilityTrackDef tracks[4];
    long cats[4] = { 0, 0, 1, 2 }; long counts[4] = { 4, 3, 5, 2 };
    long* off = 0; (void)off;
    for (int i = 0; i < 4; ++i) {
        tracks[i].ExperienceStat = (EHeroExperienceStatCategory)cats[i];
        tracks[i].Abilities._M_start = levels; tracks[i].Abilities._M_finish = levels + counts[i];
    }
    static CHeroStatsDef def; def.StatLevels._M_start = tracks; def.StatLevels._M_finish = tracks + 4;
    static long trainable[4];
    static CTCHeroMorph morph; static CTCInventoryAbilities abil; static CTCHeroStats stats; static CTCCreatureModeManager modes;
    static CThing thing;
    CInterfaceMap::SEntry entries[4];
    entries[0].type = 3; entries[0].component = (CTCBaseComponent*)&morph;
    entries[1].type = 4; entries[1].component = (CTCBaseComponent*)&stats;
    entries[2].type = 0x31; entries[2].component = (CTCBaseComponent*)&modes;
    entries[3].type = 0x6f; entries[3].component = (CTCBaseComponent*)&abil;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 4;
    thing.m_presentBits[0] = (1UL << 3) | (1UL << 4); thing.m_presentBits[1] = 1UL << 17; thing.m_presentBits[3] = 1UL << 15;
    CTCHeroExperience* e = (CTCHeroExperience*)calloc(1, sizeof(CTCHeroExperience));
    e->ParentThing = &thing; e->PHeroStatsDef.Object = &def;
    e->TrainableStatLevels._M_start = trainable; e->TrainableStatLevels._M_finish = trainable + 4;
    int ok = 1;
    trainable[0] = 1; trainable[1] = 2; trainable[2] = 6; trainable[3] = 1;   // strength 3/5, skill 6/4 -> clamped 1
    e->OnStatLevelChange(false);
    if (g_str != 3.0f / 5.0f || g_skill != 1.0f || g_will != 1 || g_expr != 0 || g_mode != 1) ok = 0;
    trainable[0] = -4; trainable[1] = 0; trainable[2] = 2;                    // strength < 0 -> 0, skill 2/4
    e->OnStatLevelChange(true);
    if (g_str != 0.0f || g_skill != 0.5f || g_will != 2 || g_expr != 1 || g_mode != 2) ok = 0;
    if (!ok) { std::printf("006d83b0_TEST FAIL\n"); return 1; }
    std::printf("006d83b0_TEST PASS\n");
    return 0;
}