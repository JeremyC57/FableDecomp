// Behaviour test for 0x0089C8A0 CGameScriptInterface::IsDeedWitnessed (NF435): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The hero's opinion deed log (0x51) is planted; IsDeedWitnessed must get (handle, true) on it and its result is returned.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_ISDEEDWITNESSED
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include <stdio.h>
#include <string.h>
static void* g_hero = 0; static long g_player[0x40]; static long g_pm[0x40]; static int g_hero_gets = 0;
CPlayer& CPlayerManager::GetMainPlayer() { return *(CPlayer*)g_player; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { ++g_hero_gets; return (CThingPlayerCreature*)g_hero; }
static long g_gsi[0x50 / 4];
static void nf435_hero(void) { g_gsi[0x14 / 4] = (long)g_pm; }
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
static long g_log[0x40]; static int g_calls = 0; static const void* g_this = 0; static long g_h = 0; static bool g_flag = false, g_val = false;
bool CTCHeroOpinionDeedLog::IsDeedWitnessed(long h, bool f) { ++g_calls; g_this = this; g_h = h; g_flag = f; return g_val; }
static int run(void* hero, bool bit, unsigned char dead, bool val, bool want, int calls)
{
    nf435_plant(g_thing, 0x51, g_log); if (!bit) nf435_clear(g_thing, 0x51); g_thing[0x91] = dead;
    g_hero = hero; g_val = val; g_calls = 0; g_flag = false;
    bool got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsDeedWitnessed(1234);
    return got == want && g_calls == calls && (!calls || (g_this == (const void*)g_log && g_h == 1234 && g_flag));
}
int main(void)
{
    nf435_hero();
    if (!run(g_thing, true, 0, true, true, 1) || !run(g_thing, true, 0, false, false, 1) || !run(g_thing, false, 0, true, false, 0)
        || !run(g_thing, true, 1, true, false, 0) || !run(0, true, 0, true, false, 0)) { printf("FAIL_0089c8a0\n"); return 1; }
    printf("PASS_0089c8a0 FSE2_0089c8a0_TEST PASS\n"); return 0;
}
