// CReactionFrequencyTraits_ControlledCount::CalculateWeight(const CThingAICreature*) -- the reaction's weight: 0 for an immediate repeat by the same creature; otherwise recharge the available count while rising (capping it ends the rising phase), and once the minimum gap has passed weigh by count / max, clamped to [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FFE10, length 231 bytes, `ret 4` (vtable slot; float in st0)
// (PDB ?CalculateWeight@CReactionFrequencyTraits_ControlledCount@@UAEMPBVCThingAICreature@@@Z).
// Callees: CWorld::GetWorldFrame 0x0049D870 (static). Constants: 2^32 0x0122DCB4 (unsigned-to-float fix-up),
// 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: engine/CReactionFrequencyTraits_ControlledCount.h (PDB = retail: AllowIndividualRepeats +4,
// PLastReactionCreature +8, InRisingPhase +0xc, MinGapFrames +0x10, CountRechargePerFrame +0x14,
// CurrentAvailableCount +0x18, MaxCount +0x1c, InvMaxCount +0x20, FrameLastStarted +0x24, FrameLastRecharged +0x28).
// Shape: the elapsed frames convert as an unsigned long; the recharge is `+=` with the recharge-frame store scheduled
// inside it; the cap is `count >= max` (`test ah,1 ; jne`); the clamp is the inline reference Clamp over 0.0f/1.0f
// temporaries in the frame (`lea eax,[esp+N] ; fld [eax]`), the weight an unnamed temporary homed in the dead argument slot (`fst` then the compare); the repeat test and the gap test share the 0.0f exit.
#include "engine/CReactionFrequencyTraits_ControlledCount.h"

struct CWorld {
    static long GetWorldFrame();                         // 0x0049D870
};

inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

struct CReactionFrequencyTraits_ControlledCount_Methods : CReactionFrequencyTraits_ControlledCount {
    float CalculateWeight(const CThingAICreature* creature);
};

float CReactionFrequencyTraits_ControlledCount_Methods::CalculateWeight(const CThingAICreature* creature)
{
    unsigned long frame = CWorld::GetWorldFrame();
    if (!AllowIndividualRepeats && creature && PLastReactionCreature && PLastReactionCreature == creature)
        return 0.0f;
    if (InRisingPhase) {
        CurrentAvailableCount += (float)(frame - FrameLastRecharged) * CountRechargePerFrame;
        FrameLastRecharged = frame;
        if (CurrentAvailableCount >= MaxCount) {
            CurrentAvailableCount = MaxCount;
            PLastReactionCreature = 0;
            InRisingPhase = false;
        }
    }
    if (!InRisingPhase && frame - FrameLastStarted >= MinGapFrames) {
        return Clamp(InvMaxCount * CurrentAvailableCount, 0.0f, 1.0f);
    }
    return 0.0f;
}