#include <cstdio>
#include <cstring>
#include <new>

struct IGameInterface { virtual long GetTimeOfDay() { return m_time; } long m_time; };
struct CScript {
    IGameInterface* m_pGameInterface;
    float GetTimeOfDayFraction() {
        IGameInterface* game = m_pGameInterface; long time = game->GetTimeOfDay();
        long hours = time / 100; long minutes = time - hours * 100;
        return ((1.0f / 60.0f) * (float)minutes + (float)hours) * (1.0f / 24.0f); }
};
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    IGameInterface game; CScript script; script.m_pGameInterface = &game;
    game.m_time = 1430; if (!Near(script.GetTimeOfDayFraction(), 14.5f / 24.0f)) { std::printf("00ed43d0_TEST FAIL\n"); return 1; }
    game.m_time = 0;    if (!Near(script.GetTimeOfDayFraction(), 0.0f)) { std::printf("00ed43d0_TEST FAIL\n"); return 1; }
    game.m_time = 2359; if (!Near(script.GetTimeOfDayFraction(), (23.0f + 59.0f / 60.0f) / 24.0f)) { std::printf("00ed43d0_TEST FAIL\n"); return 1; }
    std::printf("00ed43d0_TEST PASS\n");
    return 0;
}