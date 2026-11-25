// Behaviour test for 0x0088F850 CGameScriptInterface::EntitySetDeedReactionsEnabled (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// CCharString's constructor / destructor and the interface's slot 590 (+0x938) are planted / defined here; the row must build "SG_DEED_REACTION_ROOT",
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>
// pass the entity, that string and the flag to the virtual EntitySetStategroupEnabled once, then destroy the string.
static int g_ctors = 0, g_dtors = 0, g_calls = 0;
static const void* g_str = 0; static char g_text[64]; static long g_len = 0;
CCharString::CCharString(const char* text, long length) { ++g_ctors; g_str = this; strncpy(g_text, text, 63); g_len = length; }
CCharString::~CCharString() { if (this == g_str) ++g_dtors; }
static const void* g_ent = 0; static const void* g_sg = 0; static bool g_en = false; static const void* g_self = 0; static int g_seen_ctor = 0;
extern "C" void __fastcall nf433_slot590(const void* self, void*, const void* entity, const void* sg, bool en)
{ ++g_calls; g_self = self; g_ent = entity; g_sg = sg; g_en = en; g_seen_ctor = g_ctors - g_dtors; }
static void* g_vtbl[920];
static void* g_gsi[0x50 / 4];
static long g_entity[3];
int main(void)
{
    for (int i = 0; i < 920; ++i) g_vtbl[i] = 0; g_vtbl[0x938 / 4] = (void*)&nf433_slot590; g_gsi[0] = g_vtbl;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntitySetDeedReactionsEnabled(*(const CScriptThing*)g_entity, true);
    if (g_calls != 1 || g_ctors != 1 || g_dtors != 1 || g_seen_ctor != 1 || strcmp(g_text, "SG_DEED_REACTION_ROOT") != 0 || g_len != -1
        || g_self != (const void*)g_gsi || g_ent != (const void*)g_entity || g_sg != g_str || !g_en) { printf("FAIL_0088f850\n"); return 1; }
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::EntitySetDeedReactionsEnabled(*(const CScriptThing*)g_entity, false);
    if (g_calls != 2 || g_en) { printf("FAIL_0088f850\n"); return 1; }
    printf("PASS_0088f850 CGameScriptInterface_0088f850_TEST PASS\n"); return 0;
}
