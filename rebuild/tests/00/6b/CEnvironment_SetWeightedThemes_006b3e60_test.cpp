// Behaviour test for 0x006B3E60: CEnvironment::SetWeightedThemes (sum weights per global theme, apply in sorted order to the
// channel's priority entry, stop at the first theme the level does not hold, flag the last one, empty the static list)
#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>

struct Call { long theme; float weight; bool last; };
static std::vector<Call> g_calls;

class CDefinitionManager {
public:
    // stub: global index g maps to def-class index g / 10 (so local theme = g / 10 - 1)
    long GetDefClassIndexFromGlobalIndex(long g) const { return g / 10; }
};

class CEnvironmentThemePriorityStackEntry {
public:
    int Id;
    void SetThemeWeight(long theme, float weight, bool last) { Call c = { theme, weight, last }; c.theme += Id * 1000; g_calls.push_back(c); }
};

class CEnvironment {
public:
    CDefinitionManager* DefinitionManager;
    bool ForcedEnvironmentTheme;
    std::vector<std::vector<CEnvironmentThemePriorityStackEntry> > PriorityStackChannels;
    long ThemeCount;
    long GetLocalEnvironmentThemeIndexFromGlobal(long g) { return DefinitionManager->GetDefClassIndexFromGlobalIndex(g); }
    void SetWeightedThemes(long channel, long priority, long* themes, float* weights, long count);
};

void CEnvironment::SetWeightedThemes(long channel, long priority, long* themes, float* weights, long count)
{
    if (ForcedEnvironmentTheme)
        return;
    if (channel < 0 || channel >= 6)
        return;
    if (priority < 0 || priority >= 5)
        return;
    if (count < 1)
        return;
    static std::vector<std::pair<long, float> > ThemeWeights;
    std::pair<long, float> theme_weight(0, 0.0f);
    for (long i = 0; i < count; ++i) {
        long theme = themes[i];
        if (theme != 0) {
            theme_weight.first = theme;
            std::vector<std::pair<long, float> >::iterator it =
                std::lower_bound(ThemeWeights.begin(), ThemeWeights.end(), theme_weight);
            if (it != ThemeWeights.end() && it->first == theme) {
                it->second += weights[i];
            } else {
                theme_weight.second = weights[i];
                ThemeWeights.insert(it, theme_weight);
            }
        }
    }
    CEnvironmentThemePriorityStackEntry& entry = PriorityStackChannels[channel][priority];
    for (std::vector<std::pair<long, float> >::iterator it = ThemeWeights.begin(), end = ThemeWeights.end(); it != end; ++it) {
        long local_theme = GetLocalEnvironmentThemeIndexFromGlobal(it->first) - 1;
        if (local_theme < 0 || local_theme >= ThemeCount)
            break;
        entry.SetThemeWeight(local_theme, it->second, it + 1 == end);
    }
    ThemeWeights.clear();
}

int main()
{
    int fails = 0;
    CDefinitionManager dm;
    CEnvironment env;
    env.DefinitionManager = &dm;
    env.ForcedEnvironmentTheme = false;
    env.ThemeCount = 4;
    env.PriorityStackChannels.resize(6);
    for (int c = 0; c < 6; ++c) {
        env.PriorityStackChannels[c].resize(5);
        for (int p = 0; p < 5; ++p) env.PriorityStackChannels[c][p].Id = c * 10 + p;
    }
    long themes[5] = { 30, 0, 10, 30, 20 };
    float weights[5] = { 0.25f, 9.0f, 0.5f, 0.125f, 1.0f };
    env.SetWeightedThemes(2, 3, themes, weights, 5);
    // Retail's key is one pair<long, float> whose weight is only written on insert, and lower_bound uses pair's operator< (the
    // weight too: 0x006B58B3 fcom), so a theme seen again after a larger stale weight is inserted twice: list (10,.5) (20,1)
    // (30,.25) (30,.125) -> local 0, 1, 2, 2 on entry 23, the zero theme skipped, the last flag on the final one.
    if (g_calls.size() != 4) { std::printf("count %u\n", (unsigned)g_calls.size()); ++fails; }
    else {
        if (g_calls[0].theme != 23000 || g_calls[0].weight != 0.5f || g_calls[0].last) { std::printf("call 0\n"); ++fails; }
        if (g_calls[1].theme != 23001 || g_calls[1].weight != 1.0f || g_calls[1].last) { std::printf("call 1\n"); ++fails; }
        if (g_calls[2].theme != 23002 || g_calls[2].weight != 0.25f || g_calls[2].last) { std::printf("call 2\n"); ++fails; }
        if (g_calls[3].theme != 23002 || g_calls[3].weight != 0.125f || !g_calls[3].last) { std::printf("call 3\n"); ++fails; }
    }
    // a repeat whose stale key weight is not larger is found and summed
    g_calls.clear();
    long themes3[2] = { 20, 20 };
    float weights3[2] = { 1.0f, 2.0f };
    env.SetWeightedThemes(1, 1, themes3, weights3, 2);
    if (g_calls.size() != 1 || g_calls[0].theme != 11001 || g_calls[0].weight != 3.0f || !g_calls[0].last) { std::printf("sum\n"); ++fails; }
    // the static list is emptied: a second call sees only its own themes; theme 90 (local 8) is out of range and stops the walk
    g_calls.clear();
    long themes2[3] = { 90, 40, 10 };
    float weights2[3] = { 1.0f, 2.0f, 3.0f };
    env.SetWeightedThemes(0, 0, themes2, weights2, 3);
    if (g_calls.size() != 2 || g_calls[0].theme != 0 || g_calls[0].weight != 3.0f || g_calls[1].theme != 3 || g_calls[1].last) { std::printf("stop at bad theme\n"); ++fails; }
    // guards: forced theme, bad channel / priority, empty list
    g_calls.clear();
    env.SetWeightedThemes(6, 0, themes, weights, 5);
    env.SetWeightedThemes(0, 5, themes, weights, 5);
    env.SetWeightedThemes(0, -1, themes, weights, 5);
    env.SetWeightedThemes(0, 0, themes, weights, 0);
    env.ForcedEnvironmentTheme = true;
    env.SetWeightedThemes(0, 0, themes, weights, 5);
    if (!g_calls.empty()) { std::printf("guards\n"); ++fails; }
    if (fails) { std::printf("006b3e60_TEST FAIL\n"); return 1; }
    std::printf("006b3e60_TEST PASS\n");
    return 0;
}