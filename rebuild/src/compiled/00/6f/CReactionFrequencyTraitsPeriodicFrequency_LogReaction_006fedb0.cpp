// Leaf: ?CReactionFrequencyTraits_PeriodicFrequency_LogReaction@@YIXPAUCReactionFrequencyTraits_PeriodicFrequency@@HABVCThingAICreature@@_N@Z
#pragma optimize("t",on)

#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingAICreature;


class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

void __fastcall CReactionFrequencyTraits_PeriodicFrequency_LogReaction(CReactionFrequencyTraits_PeriodicFrequency* traits,
                            int edx_pad,
                            const CThingAICreature& creature, bool flag)
{
    if (flag)
        traits->LastHappenedFrame = CWorld::GetFrame();
}