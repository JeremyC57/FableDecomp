// Behaviour test for 0x00890690 CGameScriptInterface::EntityAttachToScript (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The CScriptThing's slot 11 is planted and CCharString::operator= defined here; the row must assign the name to the live thing's
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include <stdio.h>
#include <string.h>
// SerialisationSectionName (+0x58) once, and do nothing for a dead or missing thing.
static unsigned char g_thing[0x100];
static void* g_ret = 0;
static int g_gets = 0;
extern "C" void* __fastcall nf433_get_pthing(const void*) { ++g_gets; return g_ret; }   // CScriptThing slot 11 (+0x2c)
static void* g_svtbl[79];
static void* g_script[3];
static long g_gsi[0x50 / 4];
static long g_name[4];
static int g_assigns = 0; static const void* g_lhs = 0; static const void* g_rhs = 0;
const CCharString& CCharString::operator=(const CCharString& other) { ++g_assigns; g_lhs = this; g_rhs = &other; return *this; }
static int run(void* ret, unsigned char dead, int want)
{
    g_thing[0x91] = dead; g_ret = ret; g_gets = 0; g_assigns = 0;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntityAttachToScript(*(const CScriptThing*)g_script, *(const CCharString*)g_name);
    if (g_gets != 1 || g_assigns != want) return 0;
    return !want || (g_lhs == (const void*)(g_thing + 0x58) && g_rhs == (const void*)g_name);
}
int main(void)
{
    for (int i = 0; i < 79; ++i) g_svtbl[i] = 0; g_svtbl[11] = (void*)&nf433_get_pthing; g_script[0] = g_svtbl;
    if (!run(g_thing, 0, 1) || !run(g_thing, 1, 0) || !run(0, 0, 0)) { printf("FAIL_00890690\n"); return 1; }
    printf("PASS_00890690 CGameScriptInterface_00890690_TEST PASS\n"); return 0;
}
