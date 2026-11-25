// Behaviour test for 0x0088E380 CGameScriptInterface::DontPopulateNextLoadedRegion (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The row must set the World's DontPopulateNextLoadedRegion byte (retail +0xd8, asserted against engine/CWorld.h) to 1 and nothing else.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include <stdio.h>
#include <string.h>
static long g_gsi[0x50 / 4];
static unsigned char g_world[0x200];
int main(void)
{
    if (offsetof(CWorld, DontPopulateNextLoadedRegion) != 0xd8) { printf("FAIL_0088e380\n"); return 1; }
    memset(g_world, 0, sizeof g_world); g_gsi[1] = (long)g_world;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::DontPopulateNextLoadedRegion();
    for (int i = 0; i < (int)sizeof g_world; ++i) if (g_world[i] != (i == 0xd8 ? 1 : 0)) { printf("FAIL_0088e380\n"); return 1; }
    printf("PASS_0088e380 DONTPOP_OK\n"); return 0;
}
