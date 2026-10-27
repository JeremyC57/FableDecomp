// Behaviour test for 0x007899D0: AttemptToLoadRangedWeapon sets the fire-cycle and aiming flags
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SRanged { bool FireCycleInProgress; bool Aiming; bool Other;
    void AttemptToLoadRangedWeapon() { FireCycleInProgress = true; Aiming = true; } };
int main()
{
    SRanged r; r.FireCycleInProgress = false; r.Aiming = false; r.Other = false;
    r.AttemptToLoadRangedWeapon();
    CHECK(r.FireCycleInProgress && r.Aiming && !r.Other);
    if (!g_fail) printf("007899d0_TEST PASS\n");
    return g_fail;
}