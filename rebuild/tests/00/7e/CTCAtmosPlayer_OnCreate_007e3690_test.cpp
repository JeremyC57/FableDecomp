// Behaviour test for 0x007E3690: OnCreate applies a preset atmos def index only when set
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SAtmos { long AtmosDefIndex; long Applied; int Calls;
    void SetAtmosDefIndex(long i) { Applied = i; ++Calls; }
    void OnCreate() { if (AtmosDefIndex) SetAtmosDefIndex(AtmosDefIndex); } };
int main()
{
    SAtmos a; a.AtmosDefIndex = 0; a.Applied = -1; a.Calls = 0;
    a.OnCreate(); CHECK(a.Calls == 0);
    a.AtmosDefIndex = 7; a.OnCreate(); CHECK(a.Calls == 1 && a.Applied == 7);
    if (!g_fail) printf("007e3690_TEST PASS\n");
    return g_fail;
}