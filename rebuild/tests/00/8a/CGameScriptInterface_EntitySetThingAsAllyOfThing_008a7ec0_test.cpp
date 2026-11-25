// Behaviour test for 0x008A7EC0 CGameScriptInterface::EntitySetThingAsAllyOfThing (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The private ImpSetEntityAsAllyOfThing is defined here; the row must pass its own `this`, both script things in order and true, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_IMPSETENTITYASALLY
#include "engine/CGameScriptInterface.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static const void* g_e = 0;
static const void* g_a = 0;
static bool g_flag = false;
void CGameScriptInterface::ImpSetEntityAsAllyOfThing(const CScriptThing& e, const CScriptThing& a, bool flag) const { ++g_calls; g_this = this; g_e = &e; g_a = &a; g_flag = flag; }
static long g_gsi[0x50 / 4];
static long g_t1[3], g_t2[3];
int main(void)
{
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntitySetThingAsAllyOfThing(*(const CScriptThing*)g_t1, *(const CScriptThing*)g_t2);
    if (g_calls != 1 || g_this != (const void*)g_gsi || g_e != (const void*)g_t1 || g_a != (const void*)g_t2 || !g_flag) { printf("FAIL_008a7ec0\n"); return 1; }
    printf("PASS_008a7ec0 CGameScriptInterface_008a7ec0_TEST PASS\n"); return 0;
}
