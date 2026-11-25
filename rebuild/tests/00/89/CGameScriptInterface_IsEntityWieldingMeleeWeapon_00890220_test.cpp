// Behaviour test for 0x00890220 CGameScriptInterface::IsEntityWieldingMeleeWeapon (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CTCCarrying::IsThingCarryingMeleeWeapon is defined here; the row must return its answer for a live thing and false otherwise.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCarrying.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_gets = 0;
extern "C" void* __fastcall nf433_get_pthing(const void*) { ++g_gets; return g_ret; }   // CScriptThing slot 11 (+0x2c)
static void* g_svtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];
static void setup(void) { for (int i = 0; i < 79; ++i) g_svtbl[i] = 0; g_svtbl[11] = (void*)&nf433_get_pthing; g_script[0] = g_svtbl; }
static int g_calls = 0; static const void* g_arg = 0; static bool g_val = false;
bool __fastcall CTCCarrying::IsThingCarryingMeleeWeapon(const CThing& thing) { ++g_calls; g_arg = &thing; return g_val; }
static int run(void* ret, unsigned char dead, bool val, bool want, int calls)
{
    g_thing[0x91] = dead; g_ret = ret; g_val = val; g_calls = 0; g_gets = 0;
    bool got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsEntityWieldingMeleeWeapon(*(const CScriptThing*)g_script);
    return got == want && g_calls == calls && g_gets == 1 && (!calls || g_arg == (const void*)g_thing);
}
int main(void)
{
    setup();
    if (!run(g_thing, 0, true, true, 1) || !run(g_thing, 0, false, false, 1) || !run(g_thing, 1, true, false, 0) || !run(0, 0, true, false, 0)) { printf("FAIL_00890220\n"); return 1; }
    printf("PASS_00890220 CGameScriptInterface_00890220_TEST PASS\n"); return 0;
}
