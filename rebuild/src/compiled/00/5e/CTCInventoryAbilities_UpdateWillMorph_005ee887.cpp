// CTCInventoryAbilities::UpdateWillMorph() -- once initialised, sets the hero morph's will amount to the fraction of all potentially available ability levels (plus the will stat levels) the hero has reached
// Retail 0x005EE887, 278 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (two inline CThing::GetInterface lookups: the
// hero morph component 3 and the hero experience component 0x68), IsAbilityPotentiallyAvailable(EHeroAbility)
// 0x005EE02E, GetAbilityMaximumLevel(EHeroAbility) 0x005EDEA3 (landed), CTCHeroExperience::GetTrainableStatMaxLevel
// 0x006D6770 and the stat level getter 0x006D6520 (manifest "CTCInventoryAbilities::GetAbilityLevel" is propagated;
// called on the experience component with stat 9), the morph's will setter 0x0071BE80 (manifest "CTCHeroMorph::SetAge").
// Shape: both component pointers are out locals of the inline GetInterface, whose entry pick is ONE ternary (`(it == end ||
// wanted < it->type) ? end : it`; the if-form gives the second lookup a `jle ; mov eax,esi` select) -- the morph lookup returns early when absent;
// the two running sums are frame words (the reached-levels sum reuses the first lookup key's slot); `(float)reached /
// possible` is `fild ; fidiv`.
// Layout (retail; Ego_r.pdb names): Initialised +0x141 (PDB +0x159), Abilities vector<pair<long, CDefPointer>> +0x160.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->last || wanted < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};
class CTCHeroMorph { public:                                // interface type 3
    void SetWillMorphAmount(float amount);           // 0x0071BE80
};
class CTCHeroExperience { public:                           // interface type 0x68
    long GetTrainableStatMaxLevel(long stat);        // 0x006D6770
    long GetTrainableStatLevel(long stat);           // 0x006D6520
};
struct SAbilityEntry {                               // pair<long, CDefPointer<const CHeroAbilityDef>>
    long Level;                                      // +0x00
    void* PDef;                                      // +0x04
};
struct CAbilityEntryVector {
    SAbilityEntry* first;
    SAbilityEntry* last;
    SAbilityEntry* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    SAbilityEntry& operator[](unsigned long n) { return *(first + n); }
};

class CTCInventoryAbilities { public:
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x141 - 0x008];
    bool Initialised;                                // +0x141
    unsigned char _pad_0x142[0x160 - 0x142];
    CAbilityEntryVector Abilities;                   // +0x160
    bool IsAbilityPotentiallyAvailable(long ability);   // 0x005EE02E
    long GetAbilityMaximumLevel(long ability);          // 0x005EDEA3
    void UpdateWillMorph();
};

void CTCInventoryAbilities::UpdateWillMorph()
{
    if (!Initialised)
        return;
    CTCHeroMorph* morph;
    if (!ParentThing->GetInterface(&morph, 3))
        return;
    long possible = 0;
    long reached = 0;
    for (unsigned long i = 0; i < Abilities.size(); ++i) {
        if (IsAbilityPotentiallyAvailable(i)) {
            possible += GetAbilityMaximumLevel(i) + 1;
            reached += Abilities[i].Level + 1;
        }
    }
    CTCHeroExperience* experience;
    if (ParentThing->GetInterface(&experience, 0x68)) {
        possible += experience->GetTrainableStatMaxLevel(9);
        reached += experience->GetTrainableStatLevel(9);
    }
    if (possible > 0)
        morph->SetWillMorphAmount((float)reached / possible);
}