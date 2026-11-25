// Behaviour test for 0x0088F560 CGameScriptInterface::EntityCastLightningAtTarget (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The row is empty in retail: the call must return and leave the interface and both script-thing images untouched.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include <stdio.h>
#include <string.h>
static long g_gsi[0x50 / 4];
static long g_a[3], g_b[3];
int main(void)
{
    memset(g_gsi, 0x11, sizeof g_gsi); memset(g_a, 0x22, sizeof g_a); memset(g_b, 0x33, sizeof g_b);
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntityCastLightningAtTarget(*(const CScriptThing*)g_a, *(const CScriptThing*)g_b);
    for (int i = 0; i < 0x50 / 4; ++i) if (g_gsi[i] != 0x11111111) { printf("FAIL_0088f560\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (g_a[i] != 0x22222222 || g_b[i] != 0x33333333) { printf("FAIL_0088f560\n"); return 1; }
    printf("PASS_0088f560 CGameScriptInterface_0088f560_TEST PASS\n"); return 0;
}
