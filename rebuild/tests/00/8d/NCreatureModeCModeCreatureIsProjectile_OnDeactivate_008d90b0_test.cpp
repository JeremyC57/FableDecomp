// behaviour test for 0x008D90B0 NCreatureMode::CModeCreatureIsProjectile::OnDeactivate (chicken-kick record)
#include <cstdio>
#include <cmath>
#include <cstring>
struct Stats { float LongestChickenKick; };
static float g_scored; static int g_physicsOff;
static void OnDeactivate(const char* brain, bool heroAlive, Stats* stats, float sx, float sy, float px, float py)
{
    if (std::strcmp(brain, "BRAIN_CHICKEN") == 0 && heroAlive) {
        float dx = px - sx, dy = py - sy;
        float distance = (float)std::sqrt(dx * dx + dy * dy);
        if (stats->LongestChickenKick < distance) stats->LongestChickenKick = distance;
        g_scored = distance;
    }
    g_physicsOff += 2;
}
int main()
{
    Stats s = { 4.0f };
    OnDeactivate("BRAIN_CHICKEN", true, &s, 0, 0, 3, 4);
    bool ok = s.LongestChickenKick == 5.0f && g_scored == 5.0f;
    OnDeactivate("BRAIN_CHICKEN", true, &s, 0, 0, 0, 2);
    ok = ok && s.LongestChickenKick == 5.0f && g_scored == 2.0f;
    OnDeactivate("BRAIN_DOG", true, &s, 0, 0, 30, 40);
    ok = ok && s.LongestChickenKick == 5.0f && g_physicsOff == 6;
    std::printf(ok ? "008d90b0_TEST PASS\n" : "008d90b0_TEST FAIL\n");
    return ok ? 0 : 1;
}