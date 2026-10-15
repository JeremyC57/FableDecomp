// Behaviour test for 0x004EB9E8 CHeroExperienceDef::CHeroExperienceDef
#include <cstdio>
static int g_baseCtor = 0, g_mapCtor = 0;
struct CDefinitionBase { CDefinitionBase() { ++g_baseCtor; } virtual ~CDefinitionBase() {} };
struct SAllocator { SAllocator() {} };
template<class T> struct SAllocProxy : SAllocator { T* m_data; SAllocProxy(const SAllocator& a, T* d) : SAllocator(a), m_data(d) {} };
template<class T> struct CVectorBase {
    T* m_pFirst; T* m_pLast; SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
};
template<class T> struct CRetailVector : CVectorBase<T> { CRetailVector() : CVectorBase<T>(SAllocator()) {} };
struct CMap { int built; CMap() : built(1) { ++g_mapCtor; } };
struct CAttackHistoryCombo { long a; };
struct CHeroExperienceDef : CDefinitionBase {
    CRetailVector<long> ExperienceLevelThresholds, HeroLevelTitles, FallbackMultipliers;
    CMap SoundIntensityThresholds, ComboMultiplierMap;
    float f1, f2;
    CMap ComboMultiplierIncrease, ComboMultiplierLock, ComboMultiplierLost, ComboMultiplierRunningHitsIncrease, StatIncreasePerHitType;
    CRetailVector<CAttackHistoryCombo> AttackHistoryCombos;
    CHeroExperienceDef() {}
};
int main()
{
    CHeroExperienceDef d;
    bool ok = g_baseCtor == 1 && g_mapCtor == 7
        && d.ExperienceLevelThresholds.m_pFirst == 0 && d.ExperienceLevelThresholds.m_pLast == 0 && d.ExperienceLevelThresholds.m_end.m_data == 0
        && d.HeroLevelTitles.m_pFirst == 0 && d.FallbackMultipliers.m_end.m_data == 0
        && d.AttackHistoryCombos.m_pFirst == 0 && d.AttackHistoryCombos.m_pLast == 0 && d.AttackHistoryCombos.m_end.m_data == 0
        && d.StatIncreasePerHitType.built == 1;
    if (!ok) { std::printf("004eb9e8_TEST FAIL\n"); return 1; }
    std::printf("004eb9e8_TEST PASS\n");
    return 0;
}
