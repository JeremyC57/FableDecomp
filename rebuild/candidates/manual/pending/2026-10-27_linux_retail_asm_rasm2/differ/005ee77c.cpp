// CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) -- the experience cost of the ability's next (levelsAhead) level, scaled down by the hero's morality ratio when asked; -1 when there is no such level
// Retail-asm: 0x005EE869 `fld dword ptr [ebp + 0x10]` ; 0x005EE86C `fistp dword ptr [ebp + 0xc]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EE77C; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The morality reduction is rounded by the helper through the dead argument slot.
#pragma optimize("y", off)
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
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
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
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
