// CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) -- the experience cost of the ability's next (levelsAhead) level, reduced by the hero's morality ratio times the ability's morality cost factor when asked; -1 when there is no such level
// Grade: DIFFER(259v267) -- best b dl=14: GetInterface select keeps last in edi and moves the hit into it (jg; mov edi,eax), morality int copied through the key slot before fild, cost preloaded in eax (best try b; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?GetExperienceCostToUpgradeAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@J_N@Z
// Flags: none
// Retail-asm: 0x005EE867 `fistp dword ptr [ebp+0Ch]` -- `cost * factor * ratio` stored to the dead applyMorality slot, reloaded and
//   stored with a bare dword fistp: the engine's inline-assembly float-to-long GFFloatToLongNear (shared engine/GFFloatToLongNear.h).
//   VC7.1 emits no bare dword fistp from C++ (NOTES.md "C++ attempts before helper": `(long)x` = __ftol2, /QIfist = `fistp qword`).
//   This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EE77C, 267 bytes, thiscall `ret 0Ch`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Identity: Ego_r.pdb ?GetExperienceCostToUpgradeAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@J_N@Z (public const).
// Callees: std::bitset<274>::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0 (inline GetInterface for the hero stats
//   component 4, the landed IsAbilityQuickAccessEventInQueue 0x005EE99D model), CTCHeroStats::GetMoralityMax 0x0057A67E (QBE).
// Layout: Abilities vector<pair<long, CDefPointer<const CHeroAbilityDef>>> +0x160 (8-byte entries); def ExperienceCostsToUpgrade
//   vector +0x80 (PDB +0x8C), MoralityCostFactor +0x90; hero stats Morality +0x28.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/GFFloatToLongNear.h"
class CTCBase;
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->last || wanted < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};
class CTCHeroStats { public:                         // interface type 4
    unsigned char _pad_0x00[0x28];
    long Morality;                                   // +0x28
    long GetMoralityMax() const;                     // 0x0057A67E
};
struct CLongVector {
    long* first;
    long* last;
    long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
class CHeroAbilityDef { public:
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
    const SAbilityEntry& operator[](unsigned long n) const { return *(first + n); }
};
enum EHeroAbility { HERO_ABILITY_NULL };
class CTCInventoryAbilities { public:
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x160 - 0x008];
    CAbilityEntryVector Abilities;                   // +0x160
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) const;
};
long CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levelsAhead, bool applyMorality) const
{
    if ((unsigned long)ability < Abilities.size() && Abilities[ability].PDef &&
        Abilities[ability].Level + levelsAhead <= (long)Abilities[ability].PDef->ExperienceCostsToUpgrade.size() - 1) {
        long cost = Abilities[ability].PDef->ExperienceCostsToUpgrade.first[Abilities[ability].Level + levelsAhead];
        CTCHeroStats* stats;
        if (ParentThing->GetInterface(&stats, 4)) {
            float morality = (float)stats->Morality;
            float ratio = morality / (float)stats->GetMoralityMax();
            if (applyMorality)
                return cost - GFFloatToLongNear(cost * Abilities[ability].PDef->MoralityCostFactor * ratio);
        }
        return cost;
    }
    return -1;
}
