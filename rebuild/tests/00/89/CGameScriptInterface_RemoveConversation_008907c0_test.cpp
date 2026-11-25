// Behaviour test for 0x008907C0 CGameScriptInterface::RemoveConversation (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// RemoveConversation is defined here; the row must reach the manager through World (+0x04) -> +0x7c and pass both arguments, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SCRIPTCONVERSATIONMANAGER
#define FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_REMOVECONVERSATION
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CScriptConversationManager.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static long g_id = 0;
static bool g_now = false;
void CScriptConversationManager::RemoveConversation(long conversation, bool remove_now) { ++g_calls; g_this = this; g_id = conversation; g_now = remove_now; }
static long g_gsi[0x50 / 4];
static long g_world[0x100];
static long g_mgr[8];
int main(void)
{
    g_gsi[1] = (long)g_world; g_world[0x7c / 4] = (long)g_mgr;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::RemoveConversation(0x1234, true);
    if (g_calls != 1 || g_this != (const void*)g_mgr || g_id != 0x1234 || !g_now) { printf("FAIL_008907c0\n"); return 1; }
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::RemoveConversation(77, false);
    if (g_calls != 2 || g_id != 77 || g_now) { printf("FAIL_008907c0\n"); return 1; }
    printf("PASS_008907c0 RemoveConversation_PASS_FIELD7C\n"); return 0;
}
