// Behaviour test for 0x0088FE80 CGameScriptInterface::SetTimeAsStopped (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CGameTimeManager::SetTimeAsStopped is defined here; the row must reach the manager through World (+0x04) -> +0x1c and pass both arguments, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static bool g_stopped = false;
static const long* g_time = 0;
void CGameTimeManager::SetTimeAsStopped(bool stopped, long& time) { ++g_calls; g_this = this; g_stopped = stopped; g_time = &time; }
static long g_gsi[0x50 / 4];
static long g_world[0x40];
static long g_tm[8];
int main(void)
{
    long t = 77;
    g_gsi[1] = (long)g_world; g_world[0x1c / 4] = (long)g_tm;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetTimeAsStopped(true, t);
    if (g_calls != 1 || g_this != (const void*)g_tm || !g_stopped || g_time != &t) { printf("FAIL_0088fe80\n"); return 1; }
    printf("PASS_0088fe80 SET_TIME_AS_STOPPED_OK\n"); return 0;
}
