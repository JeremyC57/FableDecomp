// CTCHeroExperience::UpdateAbilitiesForAllStats(bool) -- re-evaluates the abilities of all ten trainable stats, then the stat-level change hook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8AD0, 46 bytes (+2 int3 padding).
// Callees: CTCHeroExperience::UpdateAbilitiesForStat(EHeroTrainableStatType, bool) @ 0x006D8170,
//          CTCHeroExperience::OnStatLevelChange(bool) @ 0x006D83B0.
//
// Shape: `xor esi,esi` loop counter over the ten EHeroTrainableStatType values (`cmp esi,0Ah ; jl`),
// the caller's flag forwarded unchanged (`mov ebx,[esp+8]` hoisted above the loop, `push ebx`), and
// the loop head 16-byte aligned (`jmp` over a `lea ecx,[ecx]` filler) -- a speed-optimised TU.
// After the loop OnStatLevelChange(false). No member is read here, so `this` is modelled with the
// shared engine header (engine/CTCHeroExperience.h) and the methods live on a derived helper struct.
#include "engine/CTCHeroExperience.h"

struct CTCHeroExperience_Methods : CTCHeroExperience {
    void UpdateAbilitiesForStat(long stat, bool silent);   // 0x006D8170
    void OnStatLevelChange(bool silent);                   // 0x006D83B0
    void UpdateAbilitiesForAllStats(bool silent);
};

void CTCHeroExperience_Methods::UpdateAbilitiesForAllStats(bool silent)
{
    for (long stat = 0; stat < 10; ++stat)
        UpdateAbilitiesForStat(stat, silent);
    OnStatLevelChange(false);
}
