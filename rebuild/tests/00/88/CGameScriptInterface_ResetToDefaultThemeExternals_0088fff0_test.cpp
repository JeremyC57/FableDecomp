// Behaviour test for 0x0088FFF0 CGameScriptInterface::ResetToDefaultThemeExternals (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CEnvironment::TransitionToDefaultTheme is defined here; the row must reach it through World (+0x04) -> +0x18 with (0, 1, time), once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME_CHANNEL
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static long g_a = -1, g_b = -1;
static float g_time = 0.0f;
void CEnvironment::TransitionToDefaultTheme(long a, long b, float time) { ++g_calls; g_this = this; g_a = a; g_b = b; g_time = time; }
static long g_gsi[0x50 / 4];
static long g_world[0x40];
static long g_env[8];
int main(void)
{
    g_gsi[1] = (long)g_world; g_world[0x18 / 4] = (long)g_env;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::ResetToDefaultThemeExternals(4.0f);
    if (g_calls != 1 || g_this != (const void*)g_env || g_a != 0 || g_b != 1 || g_time != 4.0f) { printf("FAIL_0088fff0\n"); return 1; }
    printf("PASS_0088fff0 CGameScriptInterface_0088fff0_TEST PASS\n"); return 0;
}
