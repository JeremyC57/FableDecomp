// Behaviour test for 0x0088F6E0 CGameScriptInterface::EntitySetAllStategroupsEnabled (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The CScriptThing's slot 11 and the creature's state-group call are planted / defined here; the row must forward only for a non-null thing
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SETALLSTATEGROUPSENABLED
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/GFDebugPDynamicCast.h"
#include <stdio.h>
#include <string.h>
// whose CClass Type (+0x10) is 1.
static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_gets = 0;
extern "C" void* __fastcall nf433_get_pthing(const void*) { ++g_gets; return g_ret; }   // CScriptThing slot 11 (+0x2c)
static void* g_svtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];
static void setup(void) { for (int i = 0; i < 79; ++i) g_svtbl[i] = 0; g_svtbl[11] = (void*)&nf433_get_pthing; g_script[0] = g_svtbl; }
static long g_name[4];
static int g_calls = 0; static const void* g_this = 0; static const void* g_arg = 0; static bool g_en = false;
void CThingAICreature::SetAllStateGroupsEnabled(bool enabled) { ++g_calls; g_this = this; g_arg = g_name; g_en = enabled; }
static int run(void* ret, unsigned char type, bool v, int want)
{
    memset(g_thing, 0, sizeof g_thing); g_thing[0x10] = type; g_ret = ret; g_gets = 0; g_calls = 0; g_en = !v;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntitySetAllStategroupsEnabled(*(const CScriptThing*)g_script, v);
    if (g_gets != 1 || g_calls != want) return 0;
    return !want || (g_this == (const void*)g_thing && g_arg == (const void*)g_name && g_en == v);
}
int main(void)
{
    setup();
    if (!run(g_thing, 1, true, 1) || !run(g_thing, 1, false, 1) || !run(g_thing, 2, true, 0) || !run(0, 1, true, 0)) { printf("FAIL_0088f6e0\n"); return 1; }
    printf("PASS_0088f6e0 CGameScriptInterface_0088f6e0_TEST PASS\n"); return 0;
}
