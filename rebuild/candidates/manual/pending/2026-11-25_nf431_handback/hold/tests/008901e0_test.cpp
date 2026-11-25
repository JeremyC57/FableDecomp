// Behaviour test for 0x008901E0 CGameScriptInterface::DisplacementMonochromeEffectColourFadeOut (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The interpolator member is defined here; the row must reach it through World (+0x04) -> +0x18 environment -> +0x1964 and pass both arguments, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_SCRIPTFORWARDS
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static void* g_key = 0;
static float g_time = 0.0f;
void CEnvironmentSpellEffectInterpolator::DisplacementMonochromeEffectColourFadeOut(float time, void* key) { ++g_calls; g_this = this; g_key = key; g_time = time; }
static long g_gsi[0x50 / 4];
static long g_world[0x40];
static long g_env[0x1964 / 4 + 4];
static long g_interp[8];
int main(void)
{
    g_gsi[1] = (long)g_world; g_world[0x18 / 4] = (long)g_env; g_env[0x1964 / 4] = (long)g_interp;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::DisplacementMonochromeEffectColourFadeOut(1.5f, (void*)0x5678);
    if (g_calls != 1 || g_this != (const void*)g_interp || g_key != (void*)0x5678 || g_time != 1.5f) { printf("FAIL_008901e0\n"); return 1; }
    printf("PASS_008901e0 CGameScriptInterface_008901e0_TEST PASS\n"); return 0;
}
