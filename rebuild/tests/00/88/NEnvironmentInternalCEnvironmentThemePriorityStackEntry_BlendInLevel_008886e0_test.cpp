// Behaviour test for 0x008886E0 NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::BlendInLevel: the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <vector>
static void check(bool v, int line) { if (!v) { printf("008886e0_TEST FAIL line %d\n", line); exit(1); } }
struct CBlendedEnvironmentTheme {
    float Value;
    CBlendedEnvironmentTheme() : Value(0) { }
    void SetAsWeightedBlendOfThemes(const std::vector<CBlendedEnvironmentTheme>& themes, const std::vector<float>& weights)
    {
        Value = 0;
        for (unsigned int i = 0; i < themes.size(); ++i) Value += themes[i].Value * weights[i];
    }
};
struct CEnvironmentThemeResourcePallette {
    float Day[4];
    void GetBlendedTheme(CBlendedEnvironmentTheme& result, long index, float time) const { result.Value = Day[index] + time; }
};
struct CThemeWeight { long ThemeID; float Weight; bool LockWeight; };
struct CEnvironmentThemePriorityStackEntry {
    float BaseWeight; std::vector<CThemeWeight> Themes;
    void BlendInLevel(CBlendedEnvironmentTheme& result, const CBlendedEnvironmentTheme& base, float time_of_day, const CEnvironmentThemeResourcePallette& theme_palette) const;
};
// the row's logic (src/008886e0.cpp)
void CEnvironmentThemePriorityStackEntry::BlendInLevel(CBlendedEnvironmentTheme& result, const CBlendedEnvironmentTheme& base, float time_of_day, const CEnvironmentThemeResourcePallette& theme_palette) const
{
    if (BaseWeight > 0.9990234375f) {
        result = base;
        return;
    }
    if (BaseWeight < 0.0009765625f && Themes.size() == 1) {
        theme_palette.GetBlendedTheme(result, Themes[0].ThemeID, time_of_day);
        return;
    }
    std::vector<CBlendedEnvironmentTheme> themes;
    std::vector<float> weights;
    if (BaseWeight > 0.0009765625f) {
        themes.reserve(Themes.size() + 1);
        weights.reserve(Themes.size() + 1);
        themes.push_back(base);
        weights.push_back(BaseWeight);
    } else {
        themes.reserve(Themes.size());
        weights.reserve(Themes.size());
    }
    for (long n = 0; n < (long)Themes.size(); ++n) {
        const unsigned int index = themes.size();
        themes.resize(index + 1, CBlendedEnvironmentTheme());
        theme_palette.GetBlendedTheme(themes[index], Themes[n].ThemeID, time_of_day);
        weights.push_back(Themes[n].Weight);
    }
    result.SetAsWeightedBlendOfThemes(themes, weights);
}
int main()
{
    CEnvironmentThemeResourcePallette pal = { { 10.0f, 20.0f, 30.0f, 40.0f } };
    CBlendedEnvironmentTheme base; base.Value = 100.0f;
    CBlendedEnvironmentTheme out;
    CEnvironmentThemePriorityStackEntry e;
    CThemeWeight a = { 1, 0.25f, false }, b = { 3, 0.5f, false };
    // full base weight: copy
    e.BaseWeight = 1.0f; e.Themes.push_back(a);
    e.BlendInLevel(out, base, 2.0f, pal);
    check(out.Value == 100.0f, __LINE__);
    // no base weight, one theme: the palette blend straight into the result
    e.BaseWeight = 0.0f;
    e.BlendInLevel(out, base, 2.0f, pal);
    check(out.Value == 22.0f, __LINE__);
    // base 0.25 + themes 1 (0.25) and 3 (0.5): 100*0.25 + 22*0.25 + 42*0.5
    e.BaseWeight = 0.25f; e.Themes.push_back(b);
    e.BlendInLevel(out, base, 2.0f, pal);
    check(out.Value == 25.0f + 5.5f + 21.0f, __LINE__);
    // no base weight, two themes: base left out
    e.BaseWeight = 0.0f;
    e.BlendInLevel(out, base, 2.0f, pal);
    check(out.Value == 5.5f + 21.0f, __LINE__);
    printf("008886e0_TEST PASS\n");
    return 0;
}