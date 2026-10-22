#pragma optimize("t",on)

#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingAICreature;


extern "C" int __fastcall Helper_LR(CReactionFrequencyTraits_PeriodicFrequency* traits);

void __fastcall LogReaction(CReactionFrequencyTraits_PeriodicFrequency* traits,
                            int edx_pad,
                            const CThingAICreature& creature, bool flag)
{
    if (flag)
        traits->LastHappenedFrame = Helper_LR(traits);
}