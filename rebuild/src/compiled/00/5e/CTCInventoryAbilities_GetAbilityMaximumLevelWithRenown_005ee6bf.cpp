// CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown(EHeroAbility) -- the ability's maximum level, one lower when the hero's morality is on the wrong side for the ability's morality cost
// Leaf: GetAbilityMaximumLevelWithRenown
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE6BF, 189 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility) @ 0x005EDEA3 (landed), std::bitset::test
//          @ 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCHeroStats::
//          GetMoralityPercentage() const @ 0x0057A690 (landed), CTCInventoryAbilities::GetAbilityMoralityCostFactor
//          (EHeroAbility) @ 0x005EDFF7 (landed); globals 1e-4f @ 0x0129BA3C, the double literals 0.33 / 0.66.
// Shape: the maximum and the morality are named locals spilled to the frame -- the morality declared first so it
// takes [ebp-8], the word the zeroed hero-stats out pointer (interface 4, the `__forceinline` GetTC helper) and its
// key also use; the cost factor stays on the FPU. Two separate `if (...) return max - 1;` tests
// (`morality > 0.33 && cost < -eps`, `morality < 0.66 && cost > eps`; `-eps` is `fld ; fchs` of the extern
// epsilon) give retail's block order and the merged `max - 1` tail (one `||` condition inverts the first block).
// Layout: CTCBase ParentThing +0x04; the thing's interface bitset +0x20, map +0x44.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        T* component = 0;
        GetInterface(&component, wanted);
        return component;
    }
};

class CTCHeroStats { public:                                // interface type 4
    float GetMoralityPercentage() const;             // 0x0057A690
};

extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (1e-4f)

enum EHeroAbility { HERO_ABILITY_NULL };

class CTCInventoryAbilities { public:
    void*   __vftable;                               // +0x00
    CThing* ParentThing;                             // +0x04 CTCBase

    float GetAbilityMoralityCostFactor(EHeroAbility ability);    // 0x005EDFF7
    long GetAbilityMaximumLevelWithRenown(EHeroAbility ability);
public:
    long GetAbilityMaximumLevel(EHeroAbility) const;   // 0x005EDEA3
};

long CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown(EHeroAbility ability)
{
    float morality;
    long maxLevel = GetAbilityMaximumLevel(ability);
    morality = ParentThing->GetTC<CTCHeroStats>(4)->GetMoralityPercentage();
    float costFactor = GetAbilityMoralityCostFactor(ability);
    if (morality > 0.33 && costFactor < -DELTA_NOTIONAL_ZERO)
        return maxLevel - 1;
    if (morality < 0.66 && costFactor > DELTA_NOTIONAL_ZERO)
        return maxLevel - 1;
    return maxLevel;
}