// Behaviour test for 0x0088E3D0 CGameScriptInterface::MiniMapAllowRouteBetweenRegions (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The world's slot 12 (GetWorldMap, +0x30) is planted and SetRegionsAsConnectedInGraph defined here; the row must pass both names and the flag in order, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_SETREGIONSASCONNECTED
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include <stdio.h>
#include <string.h>
static long g_wm[8], g_r1[4], g_r2[4];
static int g_wms = 0, g_calls = 0; static const void* g_wthis = 0; static const void* g_this = 0; static const void* g_a1 = 0; static const void* g_a2 = 0; static bool g_c = false;
extern "C" void* __fastcall nf433_getworldmap(void* self) { ++g_wms; g_wthis = self; return g_wm; }
void CWorldMap::SetRegionsAsConnectedInGraph(const CCharString& r1, const CCharString& r2, bool c) { ++g_calls; g_this = this; g_a1 = &r1; g_a2 = &r2; g_c = c; }
static void* g_wvtbl[40]; static void* g_world[0x40];
static long g_gsi[0x50 / 4];
int main(void)
{
    g_wvtbl[12] = (void*)&nf433_getworldmap; g_world[0] = g_wvtbl; g_gsi[1] = (long)g_world;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::MiniMapAllowRouteBetweenRegions(*(const CCharString*)g_r1, *(const CCharString*)g_r2, true);
    if (g_wms != 1 || g_wthis != (const void*)g_world || g_calls != 1 || g_this != (const void*)g_wm || g_a1 != (const void*)g_r1 || g_a2 != (const void*)g_r2 || !g_c) { printf("FAIL_0088e3d0\n"); return 1; }
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::MiniMapAllowRouteBetweenRegions(*(const CCharString*)g_r2, *(const CCharString*)g_r1, false);
    if (g_calls != 2 || g_a1 != (const void*)g_r2 || g_c) { printf("FAIL_0088e3d0\n"); return 1; }
    printf("PASS_0088e3d0 0088e3d0_TEST PASS\n"); return 0;
}
