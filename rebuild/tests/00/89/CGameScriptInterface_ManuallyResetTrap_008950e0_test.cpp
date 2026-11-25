// Behaviour test for 0x008950E0 CGameScriptInterface::ManuallyResetTrap (NF435): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The trap component is planted (TCI_TRAP 0x7D); ManualReset must be called once on it and the row return true; ManualTrigger never.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTRAPBASE
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CTCTrapBase.h"
#include <stdio.h>
#include <string.h>
static void* g_ret = 0; static void* g_ret2 = 0; static bool g_alive = true; static int g_gets = 0;
extern "C" void* __fastcall nf435_get_pthing(const void*) { ++g_gets; return g_ret; }     // CScriptThing slot 11 (+0x2c)
extern "C" void* __fastcall nf435_get_pthing2(const void*) { ++g_gets; return g_ret2; }
extern "C" bool __fastcall nf435_is_alive(const void*) { return g_alive; }                // CScriptThing slot 75 (+0x12c)
static void* g_svtbl[79]; static void* g_svtbl2[79]; static void* g_script[3]; static void* g_script2[3];
static void nf435_scripts(void)
{
    g_svtbl[11] = (void*)&nf435_get_pthing; g_svtbl[75] = (void*)&nf435_is_alive; g_script[0] = g_svtbl;
    g_svtbl2[11] = (void*)&nf435_get_pthing2; g_svtbl2[75] = (void*)&nf435_is_alive; g_script2[0] = g_svtbl2;
}
#define NF435_SCRIPT (*(const CScriptThing*)g_script)
#define NF435_SCRIPT2 (*(const CScriptThing*)g_script2)
typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > NF435Map;
// CVectorMap::LowerBound comes from the row object (the header's template body, the same code retail calls out of line).
static unsigned char g_thing[0x200];                       // CThing: UsingComponents +0x20, Components +0x44 / +0x48, dead flag byte +0x91
static unsigned char g_thing2[0x200];
static NF435Map::value_type g_pairs[3];
static void nf435_plant(unsigned char* thing, int id, void* tc)   // three sorted entries around id, the bit for id set
{
    memset(thing, 0, 0x200);
    g_pairs[0].first = (ETCInterfaceType)(id - 1); g_pairs[0].second = 0;
    g_pairs[1].first = (ETCInterfaceType)id;       g_pairs[1].second = (CTCBase*)tc;
    g_pairs[2].first = (ETCInterfaceType)(id + 1); g_pairs[2].second = 0;
    *(void**)(thing + 0x44) = g_pairs; *(void**)(thing + 0x48) = g_pairs + 3;
    thing[0x20 + id / 8] |= (unsigned char)(1 << (id % 8));
}
static void nf435_clear(unsigned char* thing, int id) { thing[0x20 + id / 8] &= (unsigned char)~(1 << (id % 8)); }
static long g_trap[0x20]; static int g_trig = 0, g_reset = 0; static const void* g_this = 0;
void CTCTrapBase::ManualTrigger() { ++g_trig; g_this = this; }
void CTCTrapBase::ManualReset() { ++g_reset; g_this = this; }
static long g_gsi[0x50 / 4];
static int run(void* thing, bool bit, bool want)
{
    nf435_plant(g_thing, 0x7d, g_trap); if (!bit) nf435_clear(g_thing, 0x7d);
    g_ret = thing; g_trig = 0; g_reset = 0;
    bool got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::ManuallyResetTrap(NF435_SCRIPT);
    return got == want && g_reset == (want ? 1 : 0) && g_trig == 0 && (!want || g_this == (const void*)g_trap);
}
int main(void)
{
    nf435_scripts();
    if (!run(g_thing, true, true) || !run(g_thing, false, false) || !run(0, true, false)) { printf("FAIL_008950e0\n"); return 1; }
    printf("PASS_008950e0 PASS_008950e0\n"); return 0;
}
