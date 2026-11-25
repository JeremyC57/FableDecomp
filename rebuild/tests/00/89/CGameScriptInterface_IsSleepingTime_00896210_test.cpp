// Behaviour test for 0x00896210 CGameScriptInterface::IsSleepingTime (NF435): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The village component (TCI_VILLAGE 0x22) is planted; the row must return its IsNightTime for a live thing, else false.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_CTCVILLAGE_ISNIGHTTIME
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCVillage.h"
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
static long g_village[0x40]; static int g_calls = 0; static const void* g_this = 0; static bool g_val = false;
bool CTCVillage::IsNightTime() const { ++g_calls; g_this = this; return g_val; }
static long g_gsi[0x50 / 4];
static int run(void* thing, bool bit, unsigned char dead, bool val, bool want, int calls)
{
    nf435_plant(g_thing, 0x22, g_village); if (!bit) nf435_clear(g_thing, 0x22); g_thing[0x91] = dead;
    g_ret = thing; g_val = val; g_calls = 0;
    bool got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsSleepingTime(NF435_SCRIPT);
    return got == want && g_calls == calls && (!calls || g_this == (const void*)g_village);
}
int main(void)
{
    nf435_scripts();
    if (!run(g_thing, true, 0, true, true, 1) || !run(g_thing, true, 0, false, false, 1) || !run(g_thing, false, 0, true, false, 0)
        || !run(g_thing, true, 1, true, false, 0) || !run(0, true, 0, true, false, 0)) { printf("FAIL_00896210\n"); return 1; }
    printf("PASS_00896210 CGameScriptInterface_00896210_TEST PASS\n"); return 0;
}
