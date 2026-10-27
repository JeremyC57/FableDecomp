// Behaviour test for 0x00789A60: GetShotBuildupTime = aiming frames / frames per second
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static long g_framesPerSecond = 30;
struct SRanged { long AimingTimer; float GetShotBuildupTime() const { return (float)AimingTimer / g_framesPerSecond; } };
int main()
{
    SRanged r; r.AimingTimer = 45;
    CHECK(r.GetShotBuildupTime() == 1.5f);
    r.AimingTimer = 0; CHECK(r.GetShotBuildupTime() == 0.0f);
    g_framesPerSecond = 60; r.AimingTimer = 30; CHECK(r.GetShotBuildupTime() == 0.5f);
    if (!g_fail) printf("00789a60_TEST PASS\n");
    return g_fail;
}