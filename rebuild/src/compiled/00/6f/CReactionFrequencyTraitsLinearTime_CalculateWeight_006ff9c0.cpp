// CReactionFrequencyTraits_LinearTime::CalculateWeight(const CThingAICreature* creature) -- frames since the reaction happened, mapped linearly from [MinWait, MinWait + WaitRange] onto [0, 1]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FF9C0, length 114 bytes (vtable 0x01264B64 slot 0).
// Callees: GetWorldFrame @ 0x0049D870 (returns the world frame counter global 0x013B89BC; a static getter).
// Layout (Ego_r.pdb, engine/CReactionFrequencyTraits_LinearTime.h): MinWait +4, WaitRange +8, unsigned long
// FrameHappened +0xc (polymorphic retail model, same names and size, as in the landed ctor 0x006FF9A0).
// Shape: the frame difference is unsigned (`fild ; jge ; fadd [2^32]`) and converted into a named float local
// (otherwise the clamp constants are stored before the call); the weight expression is passed straight
// to an inline reference Clamp(v, lo, hi) whose result and 0.0f/1.0f temporaries live in a 12-byte frame.
// The creature argument is unused.
// Leaf: CalculateWeight@CReactionFrequencyTraits_LinearTime
#include "engine/CReactionFrequencyTraits_LinearTime.h"

class CThingAICreature;

long GetWorldFrame();                                   // 0x0049D870

inline const float& Clamp(const float& v, const float& lo, const float& hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

struct CReactionFrequencyTraits_LinearTimeRetail {
    virtual float CalculateWeight(const CThingAICreature* creature);
    float         MinWait;                 // +0x04
    float         WaitRange;               // +0x08
    unsigned long FrameHappened;           // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CReactionFrequencyTraits_LinearTimeRetail) == sizeof(CReactionFrequencyTraits_LinearTime));

float CReactionFrequencyTraits_LinearTimeRetail::CalculateWeight(const CThingAICreature* creature)
{
    float elapsed = (float)(GetWorldFrame() - FrameHappened);
    return Clamp((elapsed - MinWait) / WaitRange, 0.0f, 1.0f);
}