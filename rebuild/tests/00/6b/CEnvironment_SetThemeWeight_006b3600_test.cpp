// Behaviour test for 0x006b3600 CEnvironment::SetThemeWeight
#include <cstdio>
static long g_calls = 0, g_lastTheme = -1, g_lastEntry = -1; static float g_lastValue = 0;
struct CDefinitionManager { long GetDefClassIndexFromGlobalIndex(long g) { return g - 100; } };
struct CEntry { long m_id; void SetWeight(long theme, float v) { ++g_calls; g_lastTheme = theme; g_lastValue = v; g_lastEntry = m_id; } };
struct CStack { CEntry* m_pBegin; CEntry* m_pEnd; CEntry* m_pCapacity; };
struct SThemeRecord { long a, b, c; };
struct CEnvironment {
    CDefinitionManager* DefinitionManager; bool ForcedEnvironmentTheme; CStack* PriorityStackChannels;
    SThemeRecord* m_pThemesBegin; SThemeRecord* m_pThemesEnd;
    void SetThemeWeight(long channel, long priority, long globalThemeIndex, float v)
    {
        if (ForcedEnvironmentTheme) return;
        long theme = DefinitionManager->GetDefClassIndexFromGlobalIndex(globalThemeIndex) - 1;
        if (theme < 0) return;
        if (theme >= m_pThemesEnd - m_pThemesBegin) return;
        if (channel < 0 || channel >= 6) return;
        if (priority < 0 || priority >= 5) return;
        PriorityStackChannels[channel].m_pBegin[priority].SetWeight(theme, v);
    }
};
int main()
{
    CEntry entries[6][5]; CStack stacks[6];
    for (int c = 0; c < 6; ++c) { for (int p = 0; p < 5; ++p) entries[c][p].m_id = c * 10 + p; stacks[c].m_pBegin = entries[c]; stacks[c].m_pEnd = stacks[c].m_pCapacity = entries[c] + 5; }
    SThemeRecord themes[4]; CDefinitionManager dm;
    CEnvironment e; e.DefinitionManager = &dm; e.ForcedEnvironmentTheme = false; e.PriorityStackChannels = stacks;
    e.m_pThemesBegin = themes; e.m_pThemesEnd = themes + 4;
    bool ok = true;
    e.SetThemeWeight(2, 3, 103, 0.5f);                     // class index 3 -> slot 2
    ok = ok && g_calls == 1 && g_lastTheme == 2 && g_lastEntry == 23 && g_lastValue == 0.5f;
    e.SetThemeWeight(6, 3, 103, 0.5f); e.SetThemeWeight(2, 5, 103, 0.5f); e.SetThemeWeight(-1, 0, 103, 0.5f);
    e.SetThemeWeight(0, 0, 100, 0.5f); e.SetThemeWeight(0, 0, 105, 0.5f);   // slot -1, slot 4 (out of range)
    ok = ok && g_calls == 1;
    e.ForcedEnvironmentTheme = true; e.SetThemeWeight(0, 0, 101, 0.5f); ok = ok && g_calls == 1;
    if (!ok) { std::printf("006b3600_TEST FAIL\n"); return 1; }
    std::printf("006b3600_TEST PASS\n");
    return 0;
}