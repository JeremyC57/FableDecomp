// Behaviour test for 0x0088F970 CGameScriptInterface::ResetToFrontEnd (NF431): links the row object. No class body is defined here:
// both classes come from the row's shared headers. CWorld::ResetToFrontEnd is defined here and records its `this`, so the test
// checks the row forwards exactly once to the World pointer at +0x04 (another member, a doubled or a missing call fails).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include <stdio.h>

static const void* g_world = 0;
static int g_calls = 0;
void CWorld::ResetToFrontEnd() { g_world = this; ++g_calls; }

static long g_gsi[0x50 / 4];
static long g_worldObj[4];

int main(void)
{
    g_gsi[1] = (long)g_worldObj;                     // World (+0x04)
    const CGameScriptInterface* gsi = (const CGameScriptInterface*)g_gsi;
    gsi->CGameScriptInterface::ResetToFrontEnd();    // qualified: the row's own symbol, not the vftable
    if (g_calls != 1 || g_world != (const void*)g_worldObj) {
        printf("FAIL_0088f970 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_0088f970 RESET_FRONTEND_OK\n");
    return 0;
}
