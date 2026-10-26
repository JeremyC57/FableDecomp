#pragma optimize("s",on)
// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `BestScoreCoinGolfOakVale` -> `Drunkenness` (+0x19c).
#include "engine/CTCHeroStats.h"  // retyped onto the PDB layout; byte parity re-verified
struct GBlob { char pad[0xa0]; float thresh; };
struct GHolder { char pad[0xdc]; GBlob* blob; };
extern "C" GHolder* GetGlobal(void);
bool __fastcall CTCHeroStats_IsDrunk(const CTCHeroStats* stats)
{
    GHolder* h = GetGlobal();
    return stats->Drunkenness >= h->blob->thresh;
}