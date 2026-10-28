extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00887380 CEnvironmentThemePriorityStackEntry::RestoreDefaultTheme: the transition stops, the theme
// weights vector shrinks to empty (resize(0, zero weight)) and the base weight becomes 1.0.
struct CThemeWeight { long ThemeID; float Weight; bool LockWeight; };
struct Entry {
    float BaseWeight; bool BaseWeightLocked;
    CThemeWeight Themes[4]; int ThemeCount;
    bool ThemeTransitionActive;
    void resize(int n, const CThemeWeight& fill) { while (ThemeCount < n) Themes[ThemeCount++] = fill; if (n < ThemeCount) ThemeCount = n; }
    void RestoreDefaultTheme() { ThemeTransitionActive = false; CThemeWeight zero = { 0, 0.0f, false }; resize(0, zero); BaseWeight = 1.0f; }
};
int main()
{
    Entry e; e.BaseWeight = 0.25f; e.BaseWeightLocked = true; e.ThemeCount = 2; e.ThemeTransitionActive = true;
    e.Themes[0].ThemeID = 3; e.Themes[0].Weight = 0.5f;
    e.RestoreDefaultTheme();
    CHECK(e.ThemeCount == 0 && !e.ThemeTransitionActive && e.BaseWeight == 1.0f && e.BaseWeightLocked);
    if (fails == 0) printf("00887380_TEST PASS\n"); else printf("00887380_TEST FAIL\n");
    return fails;
}