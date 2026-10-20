// Behaviour test for CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown (0x005EE6BF): the maximum drops by one
// for a good hero (> 0.33) with an evil-cost ability (< -eps) or an evil hero (< 0.66) with a good-cost ability (> eps).
#include <cstdio>
// --- test-side copy of the CThing interface lookup (std::bitset presence + sorted {type, component} map) ---
struct CTCBase;
struct CPresenceBits {
    long bits[16];
    bool test(long bit) { return bit >= 0 && bit < 512 && ((bits[bit >> 5] >> (bit & 31)) & 1) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry entries[4];
    void Init() { for (int i = 0; i < 16; ++i) m_present.bits[i] = 0; m_interfaces.first = m_interfaces.last = entries; }
    void Add(long type, void* comp) { m_present.bits[type >> 5] |= 1L << (type & 31); m_interfaces.last->type = type; m_interfaces.last->component = (CTCBase*)comp; ++m_interfaces.last; }
    template<class T> bool GetInterface(T** out, long wanted)
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
};
// --- end lookup copy ---
struct CTCHeroStats { float Morality; float GetMoralityPercentage() const { return Morality; } };
const float g_epsilon_0129ba3c = 1e-4f;
enum EHeroAbility { HERO_ABILITY_NULL, HERO_ABILITY_X };
struct CTCInventoryAbilities {
    CThing* ParentThing; long Max; float Cost;
    long GetAbilityMaximumLevel(EHeroAbility) { return Max; }
    float GetAbilityMoralityCostFactor(EHeroAbility) { return Cost; }
    template<class T> T* GetTCOf(long wanted) { T* component = 0; ParentThing->GetInterface(&component, wanted); return component; }
    long GetAbilityMaximumLevelWithRenown(EHeroAbility ability);
};
long CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown(EHeroAbility ability)
{
    float morality;
    long maxLevel = GetAbilityMaximumLevel(ability);
    morality = GetTCOf<CTCHeroStats>(4)->GetMoralityPercentage();
    float costFactor = GetAbilityMoralityCostFactor(ability);
    if (morality > 0.33 && costFactor < -g_epsilon_0129ba3c)
        return maxLevel - 1;
    if (morality < 0.66 && costFactor > g_epsilon_0129ba3c)
        return maxLevel - 1;
    return maxLevel;
}
int main()
{
    CThing thing; thing.Init(); CTCHeroStats stats; thing.Add(4, &stats);
    CTCInventoryAbilities a; a.ParentThing = &thing; a.Max = 3;
    struct { float m, c; long want; } cases[] = {
        { 0.9f, -0.5f, 2 }, { 0.9f, 0.5f, 3 }, { 0.9f, 0.0f, 3 }, { 0.5f, -0.5f, 2 }, { 0.5f, 0.5f, 2 },
        { 0.1f, 0.5f, 2 }, { 0.1f, -0.5f, 3 }, { 0.7f, 0.5f, 3 }, { 0.2f, 0.00001f, 3 } };
    for (int i = 0; i < 9; ++i) {
        stats.Morality = cases[i].m; a.Cost = cases[i].c;
        if (a.GetAbilityMaximumLevelWithRenown(HERO_ABILITY_X) != cases[i].want) { std::printf("005ee6bf_TEST FAIL\n"); return 1; }
    }
    std::printf("005ee6bf_TEST PASS\n");
    return 0;
}