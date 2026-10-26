// CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown(EHeroAbility) -- the ability's maximum level, one lower when the hero's morality is on the wrong side for the ability's morality cost
// Leaf: GetAbilityMaximumLevelWithRenown
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
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component = 0;
        GetInterface(&component, wanted);
        return component;
    }
};

class CTCHeroStats { public:                                // interface type 4
    float GetMoralityPercentage() const;             // 0x0057A690
};

extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)

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
    if (morality > 0.33 && costFactor < -g_epsilon_0129ba3c)
        return maxLevel - 1;
    if (morality < 0.66 && costFactor > g_epsilon_0129ba3c)
        return maxLevel - 1;
    return maxLevel;
}