// CTCHeroStats::GetNumRegionsExplored() const -- how many regions the hero has spent any time in (non-zero RegionTime entries)
#pragma optimize("s", on)
// Leaf: ?GetNumRegionsExplored@CTCHeroStats@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AA1B, length 64 bytes.
// Callers: the stats screen (0x005CF7C2, 0x005D3DAF).
// NF39 (2026-11-16): moved off its `CTCHeroStats_Retail::GetNumberOfRegionsVisited` stand-in (a local view of the class) onto the shared
//   engine/CTCHeroStats.h (opt-ins SERIALISE: the retail member layout, RegionTime +0x124 = Ego_r +0x130 less the 12-byte vectors; PERSONALITY,
//   which names ?GetNumRegionsExplored@CTCHeroStats@@QBEJXZ 0x0057AA1B). Evidence: FableWin.pdb name set (6 hits); the stats screen prints it under
//   TEXT_GUI_STATS_LABEL_EXPLORED_REGIONS (0x005D3Bxx); the body reads RegionTime and sits among the CTCHeroStats getters 0x0057A8AF..0x0057AAD9.
// Pragma: optimize("s", on) -- size TU (unaligned loop head; `first` re-read after the entry test).
// Shape: the count runs `i != size()` (`test edx,0FFFFFFFCh` entry test, `jne` back edge; the `<` form gives `sar ; je` and `jb`), size()
//   re-evaluated per iteration, the element pointer strength-reduced.
// Defines: the CTCHeroStats chain of the landed 0x005CE0E6 ConstructPersonalityList preamble.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CTCHEROSTATS_AGE
#define FABLE_RETAIL_CTCHEROSTATS_TROPHYWITNESSES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHeroStats.h"

long CTCHeroStats::GetNumRegionsExplored() const
{
    long visited = 0;
    for (unsigned long i = 0; i != RegionTime.size(); ++i) {
        if (RegionTime[i] != 0)
            ++visited;
    }
    return visited;
}
