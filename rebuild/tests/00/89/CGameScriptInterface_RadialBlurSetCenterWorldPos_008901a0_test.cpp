// Behaviour test for 0x008901A0 CGameScriptInterface::RadialBlurSetCenterWorldPos (NF431): links the row object (the row is called by its own
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
static const void* g_pos = 0;
void CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos) { ++g_calls; g_this = this; g_key = key; g_pos = &pos; }
static long g_gsi[0x50 / 4];
static long g_world[0x40];
static long g_env[0x1964 / 4 + 4];
static long g_interp[8];
static float g_vec[3] = { 1.0f, 2.0f, 3.0f };
int main(void)
{
    g_gsi[1] = (long)g_world; g_world[0x18 / 4] = (long)g_env; g_env[0x1964 / 4] = (long)g_interp;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::RadialBlurSetCenterWorldPos((void*)0x1234, *(const C3DVector*)g_vec);
    if (g_calls != 1 || g_this != (const void*)g_interp || g_key != (void*)0x1234 || g_pos != (const void*)g_vec) { printf("FAIL_008901a0\n"); return 1; }
    printf("PASS_008901a0 RADIALBLUR_CENTER_CHASE_OK\n"); return 0;
}
