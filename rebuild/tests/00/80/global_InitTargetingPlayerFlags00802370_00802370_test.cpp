// Behaviour test for 0x00802370: the module initialiser sets flag 1 on and flag 2 off
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

static bool g_flagA = false, g_flagB = true;
static void InitTargetingPlayerFlags() { g_flagA = true; g_flagB = false; }
int main()
{
    InitTargetingPlayerFlags(); CHECK(g_flagA && !g_flagB);
    if (!g_fail) printf("00802370_TEST PASS\n");
    return g_fail;
}