// Behaviour test for 0x008A12A0 CGameScriptInterface::GetNumHousesOwned (NF435): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The hero's hero-stats component (TCI_HERO_STATS 4) holds shops +0xE0 and houses +0xE4; the row must return their sum for a live hero, else 0.
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
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include <stdio.h>
#include <string.h>
static void* g_hero = 0; static long g_player[0x40]; static long g_pm[0x40]; static int g_hero_gets = 0;
CPlayer& CPlayerManager::GetMainPlayer() { return *(CPlayer*)g_player; }
const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const { ++g_hero_gets; return (const CThingPlayerCreature*)g_hero; }
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
static long g_stats[0x80];
static int run(void* hero, bool bit, unsigned char dead, long want)
{
    nf435_plant(g_thing, 4, g_stats); if (!bit) nf435_clear(g_thing, 4); g_thing[0x91] = dead;
    g_hero = hero;
    long got = ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::GetNumHousesOwned();
    return got == want;
}
int main(void)
{
    nf435_hero(); g_stats[0xe0 / 4] = 3; g_stats[0xe4 / 4] = 40;
    if (!run(g_thing, true, 0, 43) || !run(g_thing, false, 0, 0) || !run(g_thing, true, 1, 0) || !run(0, true, 0, 0)) { printf("FAIL_008a12a0\n"); return 1; }
    g_stats[0xe0 / 4] = 7; g_stats[0xe4 / 4] = 1;
    if (!run(g_thing, true, 0, 8)) { printf("FAIL_008a12a0\n"); return 1; }
    printf("PASS_008a12a0 GET_NUM_HOUSES_OWNED_PASS\n"); return 0;
}
