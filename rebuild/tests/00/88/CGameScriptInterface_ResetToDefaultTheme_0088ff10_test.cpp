// Behaviour test for 0x0088FF10 CGameScriptInterface::ResetToDefaultTheme (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CEnvironment::TransitionToDefaultThemeAllChannels is defined here; the row must reach it through World (+0x04) -> +0x18 with priority 1 and the time, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static long g_prio = 0;
static float g_time = 0.0f;
void CEnvironment::TransitionToDefaultThemeAllChannels(long priority, float time) { ++g_calls; g_this = this; g_prio = priority; g_time = time; }
static long g_gsi[0x50 / 4];
static long g_world[0x40];
static long g_env[8];
int main(void)
{
    g_gsi[1] = (long)g_world; g_world[0x18 / 4] = (long)g_env;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::ResetToDefaultTheme(3.0f);
    if (g_calls != 1 || g_this != (const void*)g_env || g_prio != 1 || g_time != 3.0f) { printf("FAIL_0088ff10\n"); return 1; }
    printf("PASS_0088ff10 CGameScriptInterface_0088ff10_TEST PASS\n"); return 0;
}
