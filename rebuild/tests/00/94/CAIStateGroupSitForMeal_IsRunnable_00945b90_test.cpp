#include <cstdio>
// Model of CAIStateGroup_SitForMeal::IsRunnable @ 0x00945B90: new day && meal hours && cooking pot.
struct CWorldClock { long m_day; }; class CWorld { public: CWorldClock* m_pClock; };
static CWorldClock g_clock; static CWorld g_world = { &g_clock }; static bool g_meal; static int g_pot;
struct Group {
    long DayLastAte;
    CWorld* GetPWorld() { return &g_world; }
    bool IsInMealHours() { return g_meal; }
    int* GetPCookingPot() { return g_pot ? &g_pot : 0; }
    bool IsRunnable()
    {
        if (GetPWorld()->m_pClock->m_day != DayLastAte) { if (IsInMealHours()) { if (GetPCookingPot() != 0) return true; } }
        return false;
    }
};
int main()
{
    Group g; g.DayLastAte = 4; g_clock.m_day = 4; g_meal = true; g_pot = 1;
    if (g.IsRunnable()) { std::printf("00945b90_TEST FAIL\n"); return 1; }
    g_clock.m_day = 5; if (!g.IsRunnable()) { std::printf("00945b90_TEST FAIL\n"); return 1; }
    g_meal = false; if (g.IsRunnable()) { std::printf("00945b90_TEST FAIL\n"); return 1; }
    g_meal = true; g_pot = 0; if (g.IsRunnable()) { std::printf("00945b90_TEST FAIL\n"); return 1; }
    std::printf("00945b90_TEST PASS\n"); return 0;
}