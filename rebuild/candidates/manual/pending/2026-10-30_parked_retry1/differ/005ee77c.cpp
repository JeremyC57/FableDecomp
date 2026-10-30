// Grade: IMPROVED(259v267) retry1 -- retail_asm helper + GetInterface select form; residue: Morality fild through a frame slot
// CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) -- the experience cost of the ability's next (levelsAhead) level, scaled down by the hero's morality ratio when asked; -1 when there is no such level
// Retail-asm: 0x005EE869 `fld dword ptr [ebp+10h]` + 0x005EE86C `fistp dword ptr [ebp+0Ch]` -- the engine inline-assembly float-to-long helper; VC7.1 C++ (long)x calls __ftol2.
// Retail 0x005EE77C, 267 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: std::bitset::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0 (inline GetInterface for the hero
// stats component 4), CTCHeroStats::GetMoralityMax 0x0057A67E.
// Shape: Abilities (+0x160, 8-byte {Level, PDef} entries) bounds/def/level checks share the `or eax,-1` exit; the cost is
// kept in the dead `levelsAhead` slot; morality (+0x28 of the stats component) / GetMoralityMax() is the ratio;
// the result is cost - round(cost * def->MoralityCostFactor (+0x90) * ratio).
// Layout: retail CHeroAbilityDef ExperienceCostsToUpgrade vector +0x80 (PDB +0x8C), MoralityCostFactor +0x90.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = map->last;
            if (it != map->last && !(wanted < it->type))
                found = it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CTCHeroStats {                                // interface type 4
    unsigned char _pad_0x00[0x28];
    long Morality;                                   // +0x28
    long GetMoralityMax();                           // 0x0057A67E
};
struct CLongVector {
    long* first;
    long* last;
    long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
struct CHeroAbilityDef {
    unsigned char _pad_0x00[0x80];
    CLongVector ExperienceCostsToUpgrade;            // +0x80 (PDB +0x8C)
    unsigned char _pad_0x8c[0x90 - 0x8c];
    float MoralityCostFactor;                        // +0x90
};
struct SAbilityEntry {                               // pair<long, CDefPointer<const CHeroAbilityDef>>
    long Level;                                      // +0x00
    const CHeroAbilityDef* PDef;                     // +0x04
};
struct CAbilityEntryVector {
    SAbilityEntry* first;
    SAbilityEntry* last;
    SAbilityEntry* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    SAbilityEntry& operator[](unsigned long n) { return *(first + n); }
};
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

// the engine's inline-assembly float-to-long helper: expanded in place, reads the homed value, writes a frame word
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
struct CTCInventoryAbilities {
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x160 - 0x008];
    CAbilityEntryVector Abilities;                   // +0x160
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality);
};

long CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality)
{
    if ((unsigned long)ability < Abilities.size() && Abilities[ability].PDef &&
        Abilities[ability].Level + levelsAhead <= (long)Abilities[ability].PDef->ExperienceCostsToUpgrade.size() - 1) {
        long cost = Abilities[ability].PDef->ExperienceCostsToUpgrade.first[Abilities[ability].Level + levelsAhead];
        CTCHeroStats* stats;
        if (ParentThing->GetInterface(&stats, 4)) {
            float morality = (float)stats->Morality;
            float ratio = morality / (float)stats->GetMoralityMax();
            if (applyMorality) {
                float reduction = cost * Abilities[ability].PDef->MoralityCostFactor * ratio;
                return cost - FloatToLong(reduction);
            }
        }
        return cost;
    }
    return -1;
}
