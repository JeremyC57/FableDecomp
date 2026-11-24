// Test for 0x006F3530 std::for_each<CDefendingCombatantInfo::CProng*, CGetBestFreeProngForAttacker> on the shared headers (NF238).
// Self-contained: the same header for_each body and the header functor, with the functor's out-of-line operator() (0x006F2C00)
// stubbed in this TU's anonymous namespace (a per-TU name, so the row object cannot be linked against a test stub). Cases: an empty
// range (no call, the functor comes back unchanged), a three-prong range (each prong visited once, in order, by reference), and the
// returned functor carries the state the calls left (PBestProng).
#include <stdio.h>
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
#define FABLE_RETAIL_FREE_PRONG_PASS26
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CDefendingCombatantInfo.h"
#include "engine/CGetBestFreeProngForAttacker.h"
#include "stlport/fable_algorithm.h"


static int g_calls = 0;
static CDefendingCombatantInfo::CProng* g_seen[8];

namespace {
void CGetBestFreeProngForAttacker::operator()(CDefendingCombatantInfo::CProng& prong)
{
    if (g_calls < 8) g_seen[g_calls] = &prong;
    ++g_calls;
    if (!prong.Blocked && PBestProng == 0) PBestProng = &prong;   // first unblocked prong wins (stub rule)
}
}

static int Fail(int n) { printf("006f3530_TEST FAIL case %d\n", n); return 1; }

int main()
{
    static unsigned char world[0x40], wheel[0x40], ring[0x40], attacker[0x40], defender[0x40], info[0x40];
    static unsigned char prongBuf[3 * 0x1c];
    CDefendingCombatantInfo::CProng* prongs = reinterpret_cast<CDefendingCombatantInfo::CProng*>(prongBuf);
    prongs[0].Blocked = true; prongs[1].Blocked = false; prongs[2].Blocked = false;
    CGetBestFreeProngForAttacker f(*reinterpret_cast<const CWorldMap*>(world),
        *reinterpret_cast<const CDefendingCombatantInfo::CCombatWheel*>(wheel),
        *reinterpret_cast<const CDefendingCombatantInfo::CCombatRing*>(ring),
        *reinterpret_cast<const CThingCreatureBase*>(attacker), *reinterpret_cast<const CThingCreatureBase*>(defender),
        *reinterpret_cast<const CAttackingCombatantInfo*>(info));

    // Case 1: empty range.
    CGetBestFreeProngForAttacker r0 = std::for_each(prongs, prongs, f);
    if (g_calls != 0 || r0.GetPBestProng() != 0) return Fail(1);

    // Case 2: three prongs, each once, in order, by reference; the returned copy holds the first unblocked prong.
    CGetBestFreeProngForAttacker r1 = std::for_each(prongs, prongs + 3, f);
    if (g_calls != 3 || g_seen[0] != &prongs[0] || g_seen[1] != &prongs[1] || g_seen[2] != &prongs[2]) return Fail(2);
    if (r1.GetPBestProng() != &prongs[1]) return Fail(3);
    if (f.GetPBestProng() != 0) return Fail(4);   // passed by value: the caller's functor is untouched

    printf("006f3530_TEST PASS\n");
    return 0;
}
