// Behaviour test for 0x0088F960 CGameScriptInterface::SetSaveGameMarkerPos (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CWorld::SetSaveGameMarkerPos is defined here; the row must forward the same position reference to the World at +0x04, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_UPDATEREGIONLOAD_CALLEES
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static const void* g_pos = 0;
void CWorld::SetSaveGameMarkerPos(const C3DVector& pos) { ++g_calls; g_this = this; g_pos = &pos; }
static long g_gsi[0x50 / 4];
static long g_world[4];
static float g_vec[3] = { 1.0f, 2.0f, 3.0f };
int main(void)
{
    g_gsi[1] = (long)g_world;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetSaveGameMarkerPos(*(const C3DVector*)g_vec);
    if (g_calls != 1 || g_this != (const void*)g_world || g_pos != (const void*)g_vec) { printf("FAIL_0088f960\n"); return 1; }
    printf("PASS_0088f960 TEST_PASS_MARKER_OK\n"); return 0;
}
