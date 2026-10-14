#include <cstdio>
#include <cstdlib>
#include <cstring>
static const float g_dayStart_0125d640 = 0.1667f, g_dayEnd_0124f6f4 = 0.875f, g_nightStart_0125d62c = 0.7917f, g_one_0122ded8 = 1.0f, g_nightEnd_0125d620 = 0.25f, g_zero_0122dedc = 0.0f, g_dayStart_0125d624 = 0.3333f, g_dayEnd_0125d628 = 0.7083f;
struct CGameTimeManager { unsigned char _pad[8]; float m_timeOfDay;
    bool IsDay() { if (m_timeOfDay > g_dayStart_0125d640 && m_timeOfDay <= g_dayEnd_0124f6f4) return true; return false; }
    bool IsNightTime() { if (m_timeOfDay > g_nightStart_0125d62c && m_timeOfDay <= g_one_0122ded8) return true; if (m_timeOfDay <= g_nightEnd_0125d620 && m_timeOfDay >= g_zero_0122dedc) return true; return false; }
    bool IsDay_006bb2c0() { if (m_timeOfDay > g_dayStart_0125d624 && m_timeOfDay <= g_dayEnd_0125d628) return true; return false; }
    bool IsNotDay_006bb790() { bool day; if (m_timeOfDay > g_dayStart_0125d640 && m_timeOfDay <= g_dayEnd_0124f6f4) day = true; else day = false; return !day; } };
static bool at(float t, int which) { CGameTimeManager m; m.m_timeOfDay = t; switch (which) { case 0: return m.IsDay(); case 1: return m.IsNightTime(); case 2: return m.IsDay_006bb2c0(); default: return m.IsNotDay_006bb790(); } }

int main() {
    if (!at(0.8f, 1) || !at(1.0f, 1) || !at(0.0f, 1) || !at(0.25f, 1) || at(0.7917f, 1) || at(0.26f, 1) || at(0.5f, 1) || at(1.1f, 1) || at(-0.1f, 1)) { std::printf("006bb210_TEST FAIL\n"); return 1; }
    std::printf("006bb210_TEST PASS\n"); return 0;
}
