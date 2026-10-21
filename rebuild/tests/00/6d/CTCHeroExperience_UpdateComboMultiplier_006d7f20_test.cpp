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



struct CTCCombat { void SetMeleeFlourishValid(bool valid); };   // 0x00733E10
struct CTCHeroStats {
    unsigned char _pad00[0x98];
    long MaximumMultiplier;                     // +0x98 (PDB +0xa0)
};
struct CTCHeroOnlineScoreboard { void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter); };   // 0x0055F3B0

struct CComboMultiplierSounds { unsigned char _map[0xc]; };   // map<long, CDefString>

struct CHeroExperienceDef {
    unsigned char _base[0x64];
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
    float ExperienceMultiplierRegionChangeMultiplier;       // +0x68
    CComboMultiplierSounds ComboMultiplierIncrease;         // +0x6c
    CComboMultiplierSounds ComboMultiplierLock;             // +0x78
    CComboMultiplierSounds ComboMultiplierLost;             // +0x84
    CComboMultiplierSounds ComboMultiplierRunningHitsIncrease;  // +0x90
};

void __fastcall PlayCombatMultiplierSound(CThing* thing, const CComboMultiplierSounds& sounds, long multiplier);   // 0x006D7640

class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3c
    void* Pickups;                                          // +0x40
    long* ExperienceSpentOnStats[3];                        // +0x44
    void* AttackHistory;                                    // +0x50
    CHeroExperienceDef* PDef;                               // +0x54

    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    long GetLastExperiencePickupMultiplier() const { return GetExperiencePickupMultiplierForParam(LastExperienceMultiplierParam); }
    long GetExperienceFallbackMultiplierForMultiplier(long multiplier) const;   // 0x006D73D0
    long GetExperienceFallbackMultiplier() const { return GetExperienceFallbackMultiplierForMultiplier(GetExperiencePickUpMultiplier()); }
    long GetRunningNumHits() const;                         // 0x006D63A0
    void UpdateComboMultiplier(bool);
};

void CTCHeroExperience::UpdateComboMultiplier(bool)
{
    long multiplier = GetExperiencePickUpMultiplier();
    long lastMultiplier = GetLastExperiencePickupMultiplier();
    if (multiplier > lastMultiplier) {
        CTCCombat* combat;
        ParentThing->GetInterface(&combat, 0x30);
        combat->SetMeleeFlourishValid(true);
    }
    CTCHeroStats* stats;
    if (ParentThing->GetInterface(&stats, 4)) {
        if (multiplier > stats->MaximumMultiplier)
            stats->MaximumMultiplier = multiplier;
    }
    CTCHeroOnlineScoreboard* scoreboard;
    ParentThing->GetInterface(&scoreboard, 0x10d);
    scoreboard->UpdateScoreForActivity(2, (float)multiplier, false);
    long fallback = GetExperienceFallbackMultiplier();
    if (fallback > GetExperienceFallbackMultiplierForMultiplier(lastMultiplier))
        PlayCombatMultiplierSound(ParentThing, PDef->ComboMultiplierLock, fallback);
    else if (multiplier > lastMultiplier)
        PlayCombatMultiplierSound(ParentThing, PDef->ComboMultiplierIncrease, multiplier);
    else
        PlayCombatMultiplierSound(ParentThing, PDef->ComboMultiplierRunningHitsIncrease, GetRunningNumHits());
}

CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static int g_flourish = -1; static long g_act = -1; static float g_score = -1;
static const CComboMultiplierSounds* g_sound = 0; static long g_soundMult = -1; static CThing* g_soundThing = 0;
void CTCCombat::SetMeleeFlourishValid(bool valid) { g_flourish = valid ? 1 : 0; }
void CTCHeroOnlineScoreboard::UpdateScoreForActivity(long activity, float score, bool) { g_act = activity; g_score = score; }
void __fastcall PlayCombatMultiplierSound(CThing* thing, const CComboMultiplierSounds& sounds, long multiplier) { g_soundThing = thing; g_sound = &sounds; g_soundMult = multiplier; }
long CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier(long multiplier) const { return multiplier / 3; }
long CTCHeroExperience::GetRunningNumHits() const { return 77; }
void CTCHeroExperience::v00() { }

int main()
{
    static CHeroExperienceDef def; def.ExperienceMultiplierParamPerUnitComboMultiplier = 1.0f;
    static CTCCombat combat; static CTCHeroStats stats; static CTCHeroOnlineScoreboard board; static CThing thing;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 4; entries[0].component = (CTCBaseComponent*)&stats;
    entries[1].type = 0x30; entries[1].component = (CTCBaseComponent*)&combat;
    entries[2].type = 0x10d; entries[2].component = (CTCBaseComponent*)&board;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    thing.m_presentBits[0] = 1UL << 4; thing.m_presentBits[1] = 1UL << 16; thing.m_presentBits[8] = 1UL << 13;
    CTCHeroExperience* e = (CTCHeroExperience*)calloc(1, sizeof(CTCHeroExperience));
    e->ParentThing = &thing; e->PDef = &def;
    int ok = 1;
    // rise 2 -> 6: fallback 2 > 0 -> lock sound with 2
    stats.MaximumMultiplier = 4; e->LastExperienceMultiplierParam = 2.0f; e->ExperienceMultiplierParam = 6.0f;
    e->UpdateComboMultiplier(false);
    if (g_flourish != 1 || stats.MaximumMultiplier != 6 || g_act != 2 || g_score != 6.0f) ok = 0;
    if (g_sound != &def.ComboMultiplierLock || g_soundMult != 2 || g_soundThing != &thing) ok = 0;
    // rise 4 -> 5: same fallback (1) -> increase sound with 5; max stays 6
    g_flourish = -1; e->LastExperienceMultiplierParam = 4.0f; e->ExperienceMultiplierParam = 5.0f;
    e->UpdateComboMultiplier(false);
    if (g_flourish != 1 || stats.MaximumMultiplier != 6 || g_sound != &def.ComboMultiplierIncrease || g_soundMult != 5) ok = 0;
    // no rise (0.2 clamps to 1, last 1): running-hits sound, no flourish
    g_flourish = -1; e->LastExperienceMultiplierParam = 1.0f; e->ExperienceMultiplierParam = 0.2f;
    e->UpdateComboMultiplier(true);
    if (g_flourish != -1 || g_score != 1.0f || g_sound != &def.ComboMultiplierRunningHitsIncrease || g_soundMult != 77) ok = 0;
    if (!ok) { std::printf("006d7f20_TEST FAIL\n"); return 1; }
    std::printf("006d7f20_TEST PASS\n");
    return 0;
}