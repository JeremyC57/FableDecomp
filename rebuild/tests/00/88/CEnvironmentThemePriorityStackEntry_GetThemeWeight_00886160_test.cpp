// Behaviour test for 0x00886160 CEnvironmentThemePriorityStackEntry::GetThemeWeight
#include <cstdio>
struct CThemeWeight { long ThemeID; float Weight; bool LockWeight; };
template <class T> struct CArray { T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; } const T& operator[](unsigned long i) const { return m_pBegin[i]; } };
struct CEntry {
    float BaseWeight; bool BaseWeightLocked; CArray<CThemeWeight> Themes;
    float GetThemeWeight(long themeID) const
    {
        if (themeID < 0) return BaseWeight;
        for (unsigned long i = 0; i < Themes.Size(); ++i) if (Themes[i].ThemeID == themeID) return Themes[i].Weight;
        return 0.0f;
    }
};
int main()
{
    CThemeWeight t[3] = { { 4, 0.25f, false }, { 9, 0.5f, false }, { 2, 0.75f, true } };
    CEntry e; e.BaseWeight = 0.125f; e.Themes.m_pBegin = t; e.Themes.m_pEnd = e.Themes.m_pCapacity = t + 3;
    bool ok = e.GetThemeWeight(-1) == 0.125f && e.GetThemeWeight(9) == 0.5f && e.GetThemeWeight(2) == 0.75f && e.GetThemeWeight(5) == 0.0f;
    if (!ok) { std::printf("00886160_TEST FAIL\n"); return 1; }
    std::printf("00886160_TEST PASS\n");
    return 0;
}