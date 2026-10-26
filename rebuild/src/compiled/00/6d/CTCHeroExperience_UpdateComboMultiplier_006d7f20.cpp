// CTCHeroExperience::UpdateComboMultiplier(bool) -- after a combo-multiplier change: re-enables the melee flourish on a rise, records the best multiplier, reports it to the scoreboard and plays the lock / increase / running-hits sound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7F20, 435 bytes (+ int3 padding), `ret 4` (the bool parameter is unused;
// its slot homes the interface-lookup keys).
// Callees: __ftol2 0x00BFEA70, CVectorMap::LowerBound 0x0040F020 (inline lookups CTCCombat 0x30, CTCHeroStats 4,
// CTCHeroOnlineScoreboard 0x10D), CTCCombat::SetMeleeFlourishValid 0x00733E10, UpdateScoreForActivity 0x0055F3B0,
// CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier 0x006D73D0, the __fastcall PlayCombatMultiplierSound
// (CThing*, const map<long,CDefString>&, long) 0x006D7640, and 0x006D63A0 (a `long () const` member computing a count from
// the multiplier param; PDB name not pinned, modelled as GetRunningNumHits from the RunningHitsIncrease sound it feeds).
// Shape: the current and last pickup multipliers are the inline GetExperiencePickupMultiplierForParam over the two params
// (VC7.1 CSEs the shared `1 / PerUnitComboMultiplier` reciprocal: `fld 1.0 ; fdiv [def+64h]`); the multiplier is kept in a
// frame word for the scoreboard fild. The fallback comparison uses the inline GetExperienceFallbackMultiplier (the pickup
// multiplier recomputed after the calls); the three sounds are an if / else-if / else chain with the call in every arm.
// Model: CTCHeroExperience in its retail layout (12-byte STLport vectors, 4-byte lists; engine/CTCHeroExperience.h has the
// Ego_r.pdb debug-STL offsets), as in the landed 0x006D64B0. CTCHeroStats::MaximumMultiplier is +0x98 (PDB +0xA0, retail
// CTCHeroStats sits 8 below the PDB as in the landed 0x0057B327).
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
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



class CTCCombat { public: void SetMeleeFlourishValid(bool valid); };   // 0x00733E10
class CTCHeroStats : public CTCHeroStatsDonor {
 public:
};
class CTCHeroOnlineScoreboard { public: void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter); };   // 0x0055F3B0

struct CComboMultiplierSounds { unsigned char _map[0xc]; };   // map<long, CDefString>

class CHeroExperienceDef { public:
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