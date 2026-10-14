#include <cstdio>
#include <cstdlib>
#include <cstring>
static double GFGetTime() { return 123.5; } static long g_asked = -1;
struct CEnvironmentThemePriorityStackEntry { unsigned char _pad[0x14]; long m_targetTheme; float m_weight; bool m_transitioning; unsigned char _pad1d[3]; double m_startTime; long m_transitionParam;
    float GetThemeWeight(long theme) { g_asked = theme; return 2.5f; }
    void StartThemeTransition(long theme, long a) { m_targetTheme = theme; m_transitioning = true; m_startTime = GFGetTime(); m_transitionParam = a; m_weight = GetThemeWeight(theme); } };
int main() {
    CEnvironmentThemePriorityStackEntry e; std::memset(&e, 0, sizeof e); e.StartThemeTransition(5, 9);
    if (e.m_targetTheme != 5 || !e.m_transitioning || e.m_startTime != 123.5 || e.m_transitionParam != 9 || e.m_weight != 2.5f || g_asked != 5) { std::printf("008861d0_TEST FAIL\n"); return 1; }
    std::printf("008861d0_TEST PASS\n"); return 0;
}
