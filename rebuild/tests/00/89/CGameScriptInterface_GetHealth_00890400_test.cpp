// Behaviour test for 0x00890400 CGameScriptInterface::GetHealth (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The CScriptThing's slot 11 is planted here; the row must return the live physical thing's health (+0xB4) and 0 otherwise.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
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
static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_gets = 0;
extern "C" void* __fastcall nf433_get_pthing(const void*) { ++g_gets; return g_ret; }   // CScriptThing slot 11 (+0x2c)
static void* g_svtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];
static void setup(void) { for (int i = 0; i < 79; ++i) g_svtbl[i] = 0; g_svtbl[11] = (void*)&nf433_get_pthing; g_script[0] = g_svtbl; }
static int run(void* ret, unsigned char dead, unsigned char tf, float want)
{
    memset(g_thing, 0, sizeof g_thing); g_thing[0x91] = dead; g_thing[0x6c] = tf; *(float*)(g_thing + 0xb4) = 37.5f; g_ret = ret; g_gets = 0;
    float got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::GetHealth(*(const CScriptThing*)g_script);
    return got == want && g_gets == 1;
}
int main(void)
{
    setup();
    if (!run(g_thing, 0, 2, 37.5f) || !run(g_thing, 1, 2, 0.0f) || !run(g_thing, 0, 1, 0.0f) || !run(0, 0, 2, 0.0f)) { printf("FAIL_00890400\n"); return 1; }
    printf("PASS_00890400 CGameScriptInterface_00890400_TEST PASS\n"); return 0;
}
