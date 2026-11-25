// Behaviour test for 0x008908C0 CGameScriptInterface::SetFactionAsNeutralToFaction (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// GetPFaction and CFaction::SetAsNeutralToFaction are defined here; the row must look both names up on World (+0x04) -> +0x54, then relate each found
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GETFACTIONMANAGER
#define FABLE_RETAIL_CFACTION_SCRIPTRELATIONS
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/CWorld.h"
#include "engine/CFactionManager.h"
#include "engine/CFaction.h"
#include <stdio.h>
#include <string.h>
// faction to the other (first, then second), and do nothing when either is missing.
static long g_n1[4], g_n2[4], g_f1[8], g_f2[8], g_mgr[4], g_world[0x40];
static long g_gsi[0x50 / 4];
static void* g_found1 = 0; static void* g_found2 = 0; static int g_looks = 0; static const void* g_mthis = 0;
CFaction* CFactionManager::GetPFaction(const CCharString& name) const
{ ++g_looks; g_mthis = this; return (CFaction*)((const void*)&name == (const void*)g_n1 ? g_found1 : (const void*)&name == (const void*)g_n2 ? g_found2 : 0); }
static int g_calls = 0; static const void* g_this[4]; static const void* g_arg[4];
void CFaction::SetAsNeutralToFaction(const CFaction& faction) { if (g_calls < 4) { g_this[g_calls] = this; g_arg[g_calls] = &faction; } ++g_calls; }
static int run(void* f1, void* f2, int want)
{
    g_found1 = f1; g_found2 = f2; g_looks = 0; g_calls = 0;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetFactionAsNeutralToFaction(*(const CCharString*)g_n1, *(const CCharString*)g_n2);
    if (g_looks != 2 || g_mthis != (const void*)g_mgr || g_calls != want) return 0;
    return !want || (g_this[0] == f1 && g_arg[0] == f2 && g_this[1] == f2 && g_arg[1] == f1);
}
int main(void)
{
    g_gsi[1] = (long)g_world; g_world[0x54 / 4] = (long)g_mgr;
    if (!run(g_f1, g_f2, 2) || !run(g_f1, 0, 0) || !run(0, g_f2, 0) || !run(0, 0, 0)) { printf("FAIL_008908c0\n"); return 1; }
    printf("PASS_008908c0 CGameScriptInterface_008908c0_TEST PASS\n"); return 0;
}
