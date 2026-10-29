// CReactionFrequencyTraits_PeriodicFrequency::CalculateWeight(const CThingAICreature*) -- the reaction's weight: roll the cutoff window forward, ramp the target frequency from LowFreqPerMin by the cubed ramp fraction, then weigh how far the observed frequency lags it, clamped to [0, 1]
// Leaf: CalculateWeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FFB10, length 279 bytes, `ret 4` (vtable slot; float in st0)
// (PDB ?CalculateWeight@CReactionFrequencyTraits_PeriodicFrequency@@UAEMPBVCThingAICreature@@@Z).
// Callees: CWorld::GetWorldFrame 0x0049D870 (static). Constants: 2^32 0x0122DCB4 (unsigned-to-float fix-up), 0.0f 0x0122DEDC,
// 1.0f 0x0122DED8.
// Layout: engine/CReactionFrequencyTraits_PeriodicFrequency.h (PDB = retail: LowFreqPerMin +4, HighLowFreqInterval +8,
// FramesPerMinute +0xc, LastCutoffFrame +0x10, RampUpTimeFrames +0x14, CutoffIntervalFrames +0x18, LastHappenedFrame +0x1c).
// Shape: the landed ControlledCount twin 0x006FFE10: unsigned frame counts converted through `fild` + 2^32, both clamps the
// inline reference Clamp over 0.0f/1.0f frame temporaries (`lea eax,[esp+N] ; fld [eax]`), the ramp fraction cubed in statement order
// (`f = t * t; f *= t;`, a single expression is reassociated onto the member multiply), the second clamp's argument an unnamed temporary.
#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"

class CThingAICreature;

class CWorld { public:
    static long __fastcall GetFrame();                         // 0x0049D870
};

inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

struct CReactionFrequencyTraits_PeriodicFrequency_Methods : CReactionFrequencyTraits_PeriodicFrequency {
    float CalculateWeight(const CThingAICreature* creature);
};

float CReactionFrequencyTraits_PeriodicFrequency_Methods::CalculateWeight(const CThingAICreature* creature)
{
    unsigned long frame = CWorld::GetFrame();
    if (frame - LastCutoffFrame > CutoffIntervalFrames)
        LastCutoffFrame += CutoffIntervalFrames;
    float ramp = Clamp((float)(frame - LastCutoffFrame) / (float)RampUpTimeFrames, 0.0f, 1.0f);
    float frequency = ramp * ramp;
    frequency *= ramp;
    frequency = frequency * HighLowFreqInterval + LowFreqPerMin;
    float observed = (float)FramesPerMinute / (float)(frame - LastHappenedFrame);
    return Clamp((frequency - observed) / (frequency - LowFreqPerMin), 0.0f, 1.0f);
}