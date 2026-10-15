// Behaviour test for CEnvironmentThemePriorityStackEntry's copy constructor (0x006B62B0): every member copied, the
// theme array through its own copy constructor.
#include <cstdio>
static int g_arrayCopies = 0;
struct CThemeWeightArray { void* m_pFirst; void* m_pLast; void* m_pEnd; CThemeWeightArray() { } CThemeWeightArray(const CThemeWeightArray& o) : m_pFirst(o.m_pFirst), m_pLast(o.m_pLast), m_pEnd(o.m_pEnd) { ++g_arrayCopies; } };
struct CEntry {
    float BaseWeight; bool BaseWeightLocked; CThemeWeightArray Themes; long ThemeTransitionTargetID; float ThemeTransitionInitialWeight;
    bool ThemeTransitionActive; double ThemeTransitionStartTime; float ThemeTransitionDuration;
    CEntry() { }
    CEntry(const CEntry& o)
        : BaseWeight(o.BaseWeight), BaseWeightLocked(o.BaseWeightLocked), Themes(o.Themes), ThemeTransitionTargetID(o.ThemeTransitionTargetID),
          ThemeTransitionInitialWeight(o.ThemeTransitionInitialWeight), ThemeTransitionActive(o.ThemeTransitionActive),
          ThemeTransitionStartTime(o.ThemeTransitionStartTime), ThemeTransitionDuration(o.ThemeTransitionDuration) { }
};
int main()
{
    int x;
    CEntry a; a.BaseWeight = 0.75f; a.BaseWeightLocked = true; a.Themes.m_pFirst = &x; a.Themes.m_pLast = &x; a.Themes.m_pEnd = &x;
    a.ThemeTransitionTargetID = 12; a.ThemeTransitionInitialWeight = 0.5f; a.ThemeTransitionActive = true; a.ThemeTransitionStartTime = 100.25; a.ThemeTransitionDuration = 2.0f;
    CEntry b(a);
    bool ok = b.BaseWeight == 0.75f && b.BaseWeightLocked && b.Themes.m_pFirst == &x && g_arrayCopies == 1 && b.ThemeTransitionTargetID == 12
           && b.ThemeTransitionInitialWeight == 0.5f && b.ThemeTransitionActive && b.ThemeTransitionStartTime == 100.25 && b.ThemeTransitionDuration == 2.0f;
    if (!ok) { std::printf("006b62b0_TEST FAIL\n"); return 1; }
    std::printf("006b62b0_TEST PASS\n");
    return 0;
}