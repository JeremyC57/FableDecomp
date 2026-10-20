// CTCHeroExperience::AddTrainableStatExperience(EHeroTrainableStatType, long) -- scales the experience by the combo multiplier and the weapon modifier, books it on the stat and forwards it to the hero stats
// Grade: DIFFER(173v173) -- one operand order only: retail `lea eax,[ecx+edx]` (scaled index + begin), ours `lea eax,[edx+ecx]`; tried pointer, reference-local and iterator-object operator[]
// Leaf: AddTrainableStatExperience
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8300, 173 bytes, `ret 8`.
// Callees: _ftol2 @ 0x00BFEA70 (x2), GetWeaponModifiedExperienceMultiplierForThing(CThing*) @ 0x006D76C0
//          (__fastcall, float), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//          CTCHeroStats::AddHeroStatExperience(EHeroTrainableStatType, long) @ 0x0057A8DA.
// Shape: the combo multiplier is `(long)(ExperienceMultiplierParam / def->PerUnit + 1.0)` (a double 1.0 pool
// constant) floored at 1; the gain `(long)(weaponMultiplier * (multiplier * amount))` uses `fimul` on the
// integer product spilled into the dead `amount` slot. The stat entry is reached with the STLport
// `*(begin() + stat)` form (`lea ecx,[ebx*4]`, begin re-read for the `long&` ClampMin). The hero-stats component
// (interface 4) is the inlined raw bitset probe (`test byte ptr [thing+20h],10h`) then an inline GetTC whose key
// lands in the same dead slot.
// Layout: retail CTCHeroExperience keeps 12-byte vectors and a 4-byte list head (engine/CTCHeroExperience.h
// carries the PDB containers): StatExperiencePoints +0x18, ExperienceMultiplierParam +0x30, AbleToGainExperience
// +0x3C, PDef +0x54 (PDB +0x70); CHeroExperienceDef::ExperienceMultiplierParamPerUnitComboMultiplier +0x64.
struct CTCBase;

static inline void ClampMin(long& value, long minimum)
{
    if (value < minimum)
        value = minimum;
}

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

float __fastcall GetWeaponModifiedExperienceMultiplierForThing(CThing* thing);   // 0x006D76C0

enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };

struct CTCHeroStats {                                // interface type 4
    void AddHeroStatExperience(EHeroTrainableStatType stat, long amount);   // 0x0057A8DA
};

struct CHeroExperienceDef {
    unsigned char _pad_0x00[0x64];
    float         ExperienceMultiplierParamPerUnitComboMultiplier;   // +0x64
};

struct CLongVector {                                 // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCHeroExperience {
    void*               __vftable;                   // +0x00
    CThing*             ParentThing;                 // +0x04 CTCBase
    unsigned char       _pad_0x08[0x10];
    CLongVector         StatExperiencePoints;        // +0x18
    unsigned char       _pad_0x24[0x0C];
    float               ExperienceMultiplierParam;   // +0x30 (PDB +0x38)
    unsigned char       _pad_0x34[0x08];
    bool                AbleToGainExperience;        // +0x3C (PDB +0x44)
    unsigned char       _pad_0x3d[0x17];
    CHeroExperienceDef* PDef;                        // +0x54 CDefPointer<const CHeroExperienceDef>::Object (PDB +0x70)

    void AddTrainableStatExperience(EHeroTrainableStatType stat, long amount);
};

void CTCHeroExperience::AddTrainableStatExperience(EHeroTrainableStatType stat, long amount)
{
    if (AbleToGainExperience) {
        long multiplier = (long)(ExperienceMultiplierParam / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 1.0);
        if (multiplier < 1)
            multiplier = 1;
        long gained = (long)(GetWeaponModifiedExperienceMultiplierForThing(ParentThing) * (multiplier * amount));
        StatExperiencePoints[stat] += gained;
        ClampMin(StatExperiencePoints[stat], 0);
        CThing* thing = ParentThing;
        if (thing->HasTC(4))
            thing->GetTC<CTCHeroStats>(4)->AddHeroStatExperience(stat, gained);
    }
}
