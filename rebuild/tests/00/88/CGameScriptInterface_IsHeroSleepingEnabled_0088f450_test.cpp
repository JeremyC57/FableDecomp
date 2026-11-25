// Behaviour test for 0x0088F450 CGameScriptInterface::IsHeroSleepingEnabled (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The World image holds HeroSleepingEnabled at the retail offset +0xd9 (asserted against engine/CWorld.h); the row returns it.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include <stdio.h>
#include <string.h>
static long g_gsi[0x50 / 4];
static unsigned char g_world[0x200];
static int run(unsigned char v)
{
    memset(g_world, 0, sizeof g_world); g_world[0xd9] = v; g_gsi[1] = (long)g_world;
    return ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsHeroSleepingEnabled() == (v != 0);
}
int main(void)
{
    if (offsetof(CWorld, HeroSleepingEnabled) != 0xd9 || !run(1) || !run(0)) { printf("FAIL_0088f450\n"); return 1; }
    memset(g_world, 1, sizeof g_world); g_world[0xd9] = 0;
    if (((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsHeroSleepingEnabled()) { printf("FAIL_0088f450\n"); return 1; }
    printf("PASS_0088f450 CGameScriptInterface_0088f450_TEST PASS\n"); return 0;
}
