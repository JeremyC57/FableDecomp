// Behaviour test for 0x0088E090 CGameScriptInterface::SetAllSoundsAsMuted (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CUserProfileManager::Get and SetSoundAsMuted are defined here; the row must pass the flag to the singleton Get returns, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETSOUNDASMUTED
#include "engine/CGameScriptInterface.h"
#include "engine/CUserProfileManager.h"
#include <stdio.h>
#include <string.h>
static long g_mgr[4];
static int g_gets = 0, g_calls = 0;
static const void* g_this = 0;
static bool g_muted = false;
CUserProfileManager& __fastcall CUserProfileManager::Get() { ++g_gets; return *(CUserProfileManager*)g_mgr; }
void CUserProfileManager::SetSoundAsMuted(bool muted) { ++g_calls; g_this = this; g_muted = muted; }
static long g_gsi[0x50 / 4];
int main(void)
{
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetAllSoundsAsMuted(true);
    if (g_gets != 1 || g_calls != 1 || g_this != (const void*)g_mgr || !g_muted) { printf("FAIL_0088e090\n"); return 1; }
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetAllSoundsAsMuted(false);
    if (g_calls != 2 || g_muted) { printf("FAIL_0088e090\n"); return 1; }
    printf("PASS_0088e090 CGameScriptInterface_0088e090_TEST PASS\n"); return 0;
}
