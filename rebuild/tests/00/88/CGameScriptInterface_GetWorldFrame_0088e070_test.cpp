// Behaviour test for 0x0088E070 CGameScriptInterface::GetWorldFrame (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CWorld::GetFrame is defined here; the row must return exactly its value, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
long __fastcall CWorld::GetFrame() { ++g_calls; return 0x1234567; }
static long g_gsi[0x50 / 4];
int main(void)
{
    long got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::GetWorldFrame();
    if (got != 0x1234567 || g_calls != 1) { printf("FAIL_0088e070\n"); return 1; }
    printf("PASS_0088e070 PASS_0088e070\n"); return 0;
}
