// CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent) -- after a trainable stat changes, refresh what it drives: for stat 1 the creature's max health (per-level table + the hero stats' extra max health), for stat 9 the hero stats' max stamina (per-level table + extra max stamina); unless silent, the gain in the maximum is also added to the current value
// Leaf: UpdateAbilitiesForStat
//
// Retail 0x006D8170, 383 bytes (+17 int3 padding, `ret 8`). Ego_r.pdb `?UpdateAbilitiesForStat@CTCHeroExperience@@AAEXW4EHeroTrainableStatType@@_N@Z`;
// called for every stat by the landed UpdateAbilitiesForAllStats 0x006D8AD0.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (stat 1: the extra-health lookup goes through a
// const thing) and non-const @ 0x0040F020 (stat 9), CThingPhysical::SetMaxHealth(float, bool) @ 0x006A5260,
// CThingPhysical::ModifyHealth(float, bool) (virtual slot 59, +0xEC), CTCHeroStats::SetMaxStamina(long) @ 0x0057A730,
// CTCHeroStats::ModifyStaminaBy(long) @ 0x0057B1DB; float pool 0.0f @ 0x0122DEDC.
// Shape: the level index is STLport `min(level, (long)table.size() - 1)` returning a reference (the size temporary is homed in
// the dead `stat` slot, as is the new max health later); the increase is clamped at zero; HasTC(4) is `test byte ptr
// [thing+20h],10h`. Stat 1 reads the extra health through the const out-parameter GetInterface (component in eax; a returned
// GetTC lands it in ecx and rotates every later register), stat 9 through HasTC + the end/hit ternary GetTC; ExtraMaxStamina is
// read into a named local before the level lookup (ebx, pushed at entry). Same-TU `auto_inline(off)` LowerBound stand-ins
// (both overloads) let the stat-9 key share the dead argument slot with the size temporary.
// Layout: retail CTCHeroExperience TrainableStatLevels +0x24, PHeroStatsDef +0x5c (landed model, engine header has PDB offsets);
// CHeroStatsDef MaxHitPointsPerHealthLevel +0x108, MaxStaminaPointsPerMagicPowerLevel +0x114 (as the landed 0x006D64B0 model);
// retail CTCHeroStats (PDB -8): MaxStamina +0x5c, ExtraMaxHealth +0x8c, ExtraMaxStamina +0x90; CThingPhysical MaxHealth +0xb0.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

#pragma auto_inline(off)
// Same-TU stand-ins for CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (STLport lower_bound over the 8-byte
// {type, component} entries): VC7.1 then knows the key's address does not escape.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    virtual void _v00(); virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04();
    virtual void _v05(); virtual void _v06(); virtual void _v07(); virtual void _v08(); virtual void _v09();
    virtual void _v10(); virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14();
    virtual void _v15(); virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19();
    virtual void _v20(); virtual void _v21(); virtual void _v22(); virtual void _v23(); virtual void _v24();
    virtual void _v25(); virtual void _v26(); virtual void _v27(); virtual void _v28(); virtual void _v29();
    virtual void _v30(); virtual void _v31(); virtual void _v32(); virtual void _v33(); virtual void _v34();
    virtual void _v35(); virtual void _v36(); virtual void _v37(); virtual void _v38(); virtual void _v39();
    virtual void _v40(); virtual void _v41(); virtual void _v42(); virtual void _v43(); virtual void _v44();
    virtual void _v45(); virtual void _v46(); virtual void _v47(); virtual void _v48(); virtual void _v49();
    virtual void _v50(); virtual void _v51(); virtual void _v52(); virtual void _v53(); virtual void _v54();
    virtual void _v55(); virtual void _v56(); virtual void _v57(); virtual void _v58();
    virtual void ModifyHealth(float amount, bool flag);     // slot 59 (+0xEC): CThingPhysical::ModifyHealth @ 0x006A68F0
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    unsigned char _pad4c[0xb0 - 0x4c];
    float MaxHealth;                                        // +0xb0
    void SetMaxHealth(float health, bool flag);             // 0x006A5260
};
class CTCHeroStats : public CTCHeroStatsDonor {
 public:
    void SetMaxStamina(long maxStamina);                    // 0x0057A730
    void ModifyStaminaBy(long amount);                      // 0x0057B1DB
};

template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }   // STLport min
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) const { return _M_start[i]; }
};
class CHeroStatsDef { public:
    unsigned char _pad00[0x108];
    CRetailVector<float> MaxHitPointsPerHealthLevel;        // +0x108
    CRetailVector<long>  MaxStaminaPointsPerMagicPowerLevel;   // +0x114
};
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_HEALTH = 1, HERO_TRAINABLE_STAT_MAGIC_POWER = 9 };

class CTCHeroExperience {
public:
    virtual void v00();
    CThingPhysical* ParentThing;                            // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    unsigned char _pad0c[0x24 - 0x0c];
    CRetailVector<long> TrainableStatLevels;                // +0x24
    unsigned char _pad30[0x5c - 0x30];
    CHeroStatsDef* PHeroStatsDef;                           // +0x5c
    void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent);
};

void CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent)
{
    if (stat == HERO_TRAINABLE_STAT_HEALTH) {
        CThingPhysical* creature = ParentThing;
        float extra = 0.0f;
        const CThing* thing = creature;
        const CTCHeroStats* heroStats;
        if (thing->GetInterface(&heroStats, 4))
            extra = heroStats->ExtraMaxHealth;
        const long& level = Min(TrainableStatLevels[1], (long)PHeroStatsDef->MaxHitPointsPerHealthLevel.size() - 1);
        float maxHealth = PHeroStatsDef->MaxHitPointsPerHealthLevel[level] + extra;
        float increase = maxHealth - creature->MaxHealth;
        if (increase < 0.0f)
            increase = 0.0f;
        creature->SetMaxHealth(maxHealth, false);
        if (!silent)
            creature->ModifyHealth(increase, false);
    } else if (stat == HERO_TRAINABLE_STAT_MAGIC_POWER) {
        CThing* thing = ParentThing;
        if (thing->HasTC(4)) {
            CTCHeroStats* stats = thing->GetTC<CTCHeroStats>(4);
            long extra = stats->ExtraMaxStamina;
            const long& level = Min(TrainableStatLevels[9], (long)PHeroStatsDef->MaxStaminaPointsPerMagicPowerLevel.size() - 1);
            long maxStamina = PHeroStatsDef->MaxStaminaPointsPerMagicPowerLevel[level] + extra;
            long increase = maxStamina - stats->MaxStamina;
            if (increase < 0)
                increase = 0;
            stats->SetMaxStamina(maxStamina);
            if (!silent)
                stats->ModifyStaminaBy(increase);
        }
    }
}