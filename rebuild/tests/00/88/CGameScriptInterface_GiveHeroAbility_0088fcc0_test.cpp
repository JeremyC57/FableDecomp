// Behaviour test for 0x0088FCC0 CGameScriptInterface::GiveHeroAbility (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// GetMainPlayer, GetPControlledCreature and LearnAbility are defined here; the row must reach the main player's creature through PlayerManager
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_LEARNABILITY
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include <stdio.h>
#include <string.h>
// (+0x14) and, when it is alive (bit 0 of +0x91 clear), call LearnAbility(ability, silent, -1, true) on it once.
static long g_pm[8], g_player[8]; static unsigned char g_cr[0x100]; static void* g_crp = 0;
static int g_mains = 0, g_peeks = 0, g_calls = 0; static const void* g_pmthis = 0; static const void* g_plthis = 0; static const void* g_lthis = 0;
static EHeroAbility g_ab; static bool g_silent = false; static long g_slot = 0; static bool g_add = false;
CPlayer& CPlayerManager::GetMainPlayer() { ++g_mains; g_pmthis = this; return *(CPlayer*)g_player; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { ++g_peeks; g_plthis = this; return (CThingPlayerCreature*)g_crp; }
bool CThingPlayerCreature::LearnAbility(EHeroAbility ability, bool silent, long slot, bool add) { ++g_calls; g_lthis = this; g_ab = ability; g_silent = silent; g_slot = slot; g_add = add; return true; }
static long g_gsi[0x50 / 4];
static int run(void* cr, unsigned char dead, EHeroAbility ab, bool silent, int want)
{
    g_crp = cr; g_cr[0x91] = dead; g_mains = g_peeks = g_calls = 0;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::GiveHeroAbility(ab, silent);
    if (g_mains != 1 || g_peeks != 1 || g_pmthis != (const void*)g_pm || g_plthis != (const void*)g_player || g_calls != want) return 0;
    return !want || (g_lthis == (const void*)g_cr && g_ab == ab && g_silent == silent && g_slot == -1 && g_add);
}
int main(void)
{
    g_gsi[0x14 / 4] = (long)g_pm;
    if (!run(g_cr, 0, (EHeroAbility)7, true, 1) || !run(g_cr, 0, (EHeroAbility)3, false, 1) || !run(g_cr, 1, (EHeroAbility)7, true, 0)
        || !run(0, 0, (EHeroAbility)7, true, 0)) { printf("FAIL_0088fcc0\n"); return 1; }
    printf("PASS_0088fcc0 CGameScriptInterface_0088fcc0_TEST PASS\n"); return 0;
}
