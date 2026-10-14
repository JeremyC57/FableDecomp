#include <cstdio>
#include <cstdlib>
#include <cstring>
static double GFGetTime() { return 123.5; }
struct CEnvironmentThemePriorityStackEntry { float m_currentWeight; unsigned char _pad[0x10]; long m_targetTheme; float m_weight; bool m_transitioning; unsigned char _pad1d[3]; double m_startTime; long m_transitionParam;
    void TransitionToDefaultTheme(long a) { m_targetTheme = -1; m_transitioning = true; m_startTime = GFGetTime(); m_transitionParam = a; m_weight = m_currentWeight; } };
int main() {
    CEnvironmentThemePriorityStackEntry e; std::memset(&e, 0, sizeof e); e.m_currentWeight = 0.75f; e.TransitionToDefaultTheme(4);
    if (e.m_targetTheme != -1 || !e.m_transitioning || e.m_startTime != 123.5 || e.m_transitionParam != 4 || e.m_weight != 0.75f) { std::printf("00886400_TEST FAIL\n"); return 1; }
    std::printf("00886400_TEST PASS\n"); return 0;
}
