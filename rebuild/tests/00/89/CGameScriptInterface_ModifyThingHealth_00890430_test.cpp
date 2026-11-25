// Behaviour test for 0x00890430 CGameScriptInterface::ModifyThingHealth (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The CScriptThing's slot 11 and the thing's virtual ModifyHealth (+0xEC) are planted here; the row must call it once on a live physical
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPhysical.h"
#include "engine/GFDebugPDynamicCast.h"
#include <stdio.h>
#include <string.h>
// thing with both arguments, and not at all otherwise.
static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_gets = 0;
extern "C" void* __fastcall nf433_get_pthing(const void*) { ++g_gets; return g_ret; }   // CScriptThing slot 11 (+0x2c)
static void* g_svtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];
static void setup(void) { for (int i = 0; i < 79; ++i) g_svtbl[i] = 0; g_svtbl[11] = (void*)&nf433_get_pthing; g_script[0] = g_svtbl; }
static void* g_tvtbl[80];
static int g_mods = 0; static const void* g_mthis = 0; static float g_amount = 0.0f; static bool g_kill = false;
extern "C" void __fastcall nf433_modify(void* self, void*, float amount, bool can_kill) { ++g_mods; g_mthis = self; g_amount = amount; g_kill = can_kill; }
static int run(void* ret, unsigned char dead, unsigned char tf, float amount, bool kill, int want)
{
    memset(g_thing, 0, sizeof g_thing); *(void**)g_thing = g_tvtbl; g_thing[0x91] = dead; g_thing[0x6c] = tf; g_ret = ret; g_gets = 0; g_mods = 0;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::ModifyThingHealth(*(const CScriptThing*)g_script, amount, kill);
    if (g_gets != 1 || g_mods != want) return 0;
    return !want || (g_mthis == (const void*)g_thing && g_amount == amount && g_kill == kill);
}
int main(void)
{
    setup(); for (int i = 0; i < 80; ++i) g_tvtbl[i] = 0; g_tvtbl[0xec / 4] = (void*)&nf433_modify;
    if (!run(g_thing, 0, 2, -12.5f, true, 1) || !run(g_thing, 0, 2, 3.0f, false, 1) || !run(g_thing, 1, 2, 1.0f, true, 0)
        || !run(g_thing, 0, 1, 1.0f, true, 0) || !run(0, 0, 2, 1.0f, true, 0)) { printf("FAIL_00890430\n"); return 1; }
    printf("PASS_00890430 CGameScriptInterface_00890430_TEST PASS\n"); return 0;
}
