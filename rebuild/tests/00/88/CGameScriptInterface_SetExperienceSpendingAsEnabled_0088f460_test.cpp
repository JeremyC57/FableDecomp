// Behaviour test for 0x0088F460 CGameScriptInterface::SetExperienceSpendingAsEnabled (NF431): links the row object. No class body is
// defined here: both classes come from the row's shared headers. The flag must land in the World's ExperienceSpendingEnabled byte
// (retail +0xd6, asserted against engine/CWorld.h) and nowhere else.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include <stdio.h>
#include <string.h>

static unsigned char g_world[0x200];
static long g_gsi[0x50 / 4];

static int run(bool v)
{
    memset(g_world, 0x5a, sizeof g_world);
    g_gsi[1] = (long)g_world;                        // World (+0x04)
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetExperienceSpendingAsEnabled(v);
    for (int i = 0; i < (int)sizeof g_world; ++i)
        if (g_world[i] != (i == 0xd6 ? (unsigned char)v : 0x5a)) return 0;
    return 1;
}

int main(void)
{
    if (offsetof(CWorld, ExperienceSpendingEnabled) != 0xd6 || !run(true) || !run(false)) {
        printf("FAIL_0088f460\n");
        return 1;
    }
    printf("PASS_0088f460 CGameScriptInterface_0088f460_TEST PASS\n");
    return 0;
}
