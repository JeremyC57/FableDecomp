// Behaviour test for 0x0088E4F0 CGameScriptInterface::Pause (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The protected CGameScriptInterface::Rest is defined here; Pause must pass its own `this` and the seconds through, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES
#include "engine/CGameScriptInterface.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static float g_secs = 0.0f;
void CGameScriptInterface::Rest(float seconds) const { ++g_calls; g_this = this; g_secs = seconds; }
static long g_gsi[0x50 / 4];
int main(void)
{
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::Pause(2.5f);
    if (g_calls != 1 || g_this != (const void*)g_gsi || g_secs != 2.5f) { printf("FAIL_0088e4f0\n"); return 1; }
    printf("PASS_0088e4f0 PASS_0088e4f0\n"); return 0;
}
