// CReactionFrequencyTraits_ControlledCount::CReactionFrequencyTraits_ControlledCount(maxCount, minGapSeconds, rechargeSeconds, allowIndividualRepeats) -- a reaction pool of maxCount uses that recharges over rechargeSeconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEE20, length 129 bytes (`ret 10h`).
// Vtable 0x01264B94 (rtti-vtables.tsv: CReactionFrequencyTraits_ControlledCount; LogReaction 0x006FEED0 and
// TransferBinaryOut 0x006FEF30 are landed). Callees: __ftol2 @ 0x00BFEA70. Globals: the game frames-per-second
// long @ 0x01375550 (as in the landed 0x00878B00).
// Layout (Ego_r.pdb, engine/CReactionFrequencyTraits_ControlledCount.h): AllowIndividualRepeats +4,
// PLastReactionCreature +8, InRisingPhase +0xc, MinGapFrames +0x10, CountRechargePerFrame +0x14,
// CurrentAvailableCount +0x18, MaxCount +0x1c, InvMaxCount +0x20, FrameLastStarted +0x24, FrameLastRecharged +0x28
// (polymorphic retail model, same names and size).
// Shape: the bool from the argument is stored before the vtable store (an initialiser from an argument), the zero
// initialisers after it with one zero register; the unsigned count is converted once and kept on the FPU for
// CurrentAvailableCount/MaxCount (initialisers), the recharge rate (`rechargeSeconds <= 0.0f` keeps the full count
// per frame; a ternary gives the shared `fstp` after `fld st(0)`) and the inverse are body assignments.
// Leaf: ??0CReactionFrequencyTraits_ControlledCount
#include "engine/CReactionFrequencyTraits_ControlledCount.h"

extern long g_framesPerSecond;                          // 0x01375550

struct CReactionFrequencyTraits_ControlledCountRetail {
    virtual void v00();
    bool              AllowIndividualRepeats;           // +0x04
    CThingAICreature* PLastReactionCreature;            // +0x08
    bool              InRisingPhase;                    // +0x0c
    unsigned long     MinGapFrames;                     // +0x10
    float             CountRechargePerFrame;            // +0x14
    float             CurrentAvailableCount;            // +0x18
    float             MaxCount;                         // +0x1c
    float             InvMaxCount;                      // +0x20
    unsigned long     FrameLastStarted;                 // +0x24
    unsigned long     FrameLastRecharged;               // +0x28
    CReactionFrequencyTraits_ControlledCountRetail(unsigned long maxCount, float minGapSeconds, float rechargeSeconds,
                                                   bool allowIndividualRepeats);
};
FABLE_STATIC_ASSERT(sizeof(CReactionFrequencyTraits_ControlledCountRetail) == sizeof(CReactionFrequencyTraits_ControlledCount));

CReactionFrequencyTraits_ControlledCountRetail::CReactionFrequencyTraits_ControlledCountRetail(unsigned long maxCount,
        float minGapSeconds, float rechargeSeconds, bool allowIndividualRepeats)
    : AllowIndividualRepeats(allowIndividualRepeats),
      PLastReactionCreature(0),
      InRisingPhase(false),
      MinGapFrames((unsigned long)(g_framesPerSecond * minGapSeconds)),
      CurrentAvailableCount((float)maxCount),
      MaxCount((float)maxCount),
      FrameLastStarted(0),
      FrameLastRecharged(0)
{
    CountRechargePerFrame = rechargeSeconds > 0.0f ? MaxCount / (g_framesPerSecond * rechargeSeconds) : MaxCount;
    InvMaxCount = 1.0f / MaxCount;
}