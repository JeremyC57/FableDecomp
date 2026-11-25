// Behaviour test for 0x00890670 CGameScriptInterface::IsHeroInProjectileWeaponMode (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The main game component pointer is supplied by /alternatename; GetMainPlayer and IsFirstPersonTargeting are defined here: the row must go component -> player manager (+0x1c) -> main player and return its answer.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include <stdio.h>
#include <string.h>
static long g_mgc[0x20];
static long g_pm[8];
static long g_player[8];
extern "C" { void* nf431_mgc = g_mgc; }
#pragma comment(linker, "/alternatename:?PMainGameComponent@CMainGameComponent@@1PAV1@A=_nf431_mgc")
static const void* g_pmthis = 0;
static const void* g_plthis = 0;
static bool g_answer = false;
const CPlayer& CPlayerManager::GetMainPlayer() const { g_pmthis = this; return *(const CPlayer*)g_player; }
bool CPlayer::IsFirstPersonTargeting() const { g_plthis = this; return g_answer; }
static long g_gsi[0x50 / 4];
int main(void)
{
    g_mgc[0x1c / 4] = (long)g_pm;
    g_answer = true;
    if (!((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsHeroInProjectileWeaponMode()) { printf("FAIL_00890670\n"); return 1; }
    if (g_pmthis != (const void*)g_pm || g_plthis != (const void*)g_player) { printf("FAIL_00890670\n"); return 1; }
    g_answer = false;
    if (((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsHeroInProjectileWeaponMode()) { printf("FAIL_00890670\n"); return 1; }
    printf("PASS_00890670 HEROPROJ_OK\n"); return 0;
}
