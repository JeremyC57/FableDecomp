// Behaviour test for 0x00890F00 CGameScriptInterface::EntitySetAbleToBeEngagedInCombat (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The CScriptThing's slot 11 is planted here; the row must set only bit 0x20 of the physical thing's +0xBC flags byte, and only for a live
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_SCRIPTHITSETTERS
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPhysical.h"
#include "engine/GFDebugPDynamicCast.h"
#include <stdio.h>
#include <string.h>
// thing whose type flags have bit 2 (Dead is bit 0 of +0x91; ThingTypeFlags is +0x6C).
static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_gets = 0;
extern "C" void* __fastcall nf433_get_pthing(const void*) { ++g_gets; return g_ret; }   // CScriptThing slot 11 (+0x2c)
static void* g_svtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];
static void setup(void) { for (int i = 0; i < 79; ++i) g_svtbl[i] = 0; g_svtbl[11] = (void*)&nf433_get_pthing; g_script[0] = g_svtbl; }
static int run(void* ret, unsigned char dead, unsigned char tf, bool v, unsigned char start, unsigned char want)
{
    memset(g_thing, 0x5a, sizeof g_thing); g_thing[0x91] = dead; g_thing[0x6c] = tf; g_thing[0xbc] = start; g_ret = ret; g_gets = 0;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntitySetAbleToBeEngagedInCombat(*(const CScriptThing*)g_script, v);
    for (int i = 0; i < (int)sizeof g_thing; ++i) {
        unsigned char e = i == 0x91 ? dead : i == 0x6c ? tf : i == 0xbc ? want : (unsigned char)0x5a;
        if (g_thing[i] != e) return 0;
    }
    return g_gets == 1;
}
int main(void)
{
    setup();
    if (!run(g_thing, 0, 2, true, 0x00, 0x20) || !run(g_thing, 0, 2, false, 0xff, (unsigned char)(0xff & ~0x20))
        || !run(g_thing, 1, 2, true, 0x00, 0x00) || !run(g_thing, 0, 1, true, 0x00, 0x00) || !run(0, 0, 2, true, 0x00, 0x00)) { printf("FAIL_00890f00\n"); return 1; }
    printf("PASS_00890f00 CGameScriptInterface_00890f00_TEST PASS\n"); return 0;
}
