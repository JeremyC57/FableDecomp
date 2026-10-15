// CReactionFrequencyTraits_ControlledCount::LogReaction(CThingAICreature* creature, bool started) -- on a started reaction remember the creature and the frame, spend one count (clamped to the maximum) and enter the recharge phase when the pool is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEED0, length 82 bytes (virtual, `ret 8`).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`mov eax,[0x013B89BC] ; ret`; the manifest label
// "GFGetPreMainMemoryUsed" is a propagated name). Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Layout: engine/CReactionFrequencyTraits_ControlledCount.h (PDB names): PLastReactionCreature +0x08, InRisingPhase
// +0x0c, CurrentAvailableCount +0x18, MaxCount +0x1c, FrameLastStarted +0x24, FrameLastRecharged +0x28.
// Shape: the frame is a local (kept in ecx for both stamps); `CurrentAvailableCount -= 1.0f` is stored (`fst`) and
// then re-stored by an inline ternary clamp (`count > MaxCount ? MaxCount : count`: the value stays on the FPU stack,
// `fcom [MaxCount] ; test ah,41h ; jne` keeps it, else `fstp st(0) ; fld [MaxCount]`); a const-reference min
// template goes through member addresses and a by-value one spills MaxCount to the stack. `count <= 0.0f` is
// `test ah,41h ; jp` over the 0.0f pool constant (not fldz).
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: LogReaction
#include "engine/CReactionFrequencyTraits_ControlledCount.h"

long __fastcall GFGetCurrentFrame();                    // 0x0049D870

struct CReactionFrequencyTraits_ControlledCount_LogReaction : CReactionFrequencyTraits_ControlledCount {
    void LogReaction(CThingAICreature* creature, bool started);
};

void CReactionFrequencyTraits_ControlledCount_LogReaction::LogReaction(CThingAICreature* creature, bool started)
{
    if (started) {
        unsigned long frame = GFGetCurrentFrame();
        PLastReactionCreature = creature;
        FrameLastStarted = frame;
        CurrentAvailableCount -= 1.0f;
        CurrentAvailableCount = CurrentAvailableCount > MaxCount ? MaxCount : CurrentAvailableCount;
        if (CurrentAvailableCount <= 0.0f) {
            InRisingPhase = true;
            FrameLastRecharged = frame;
        }
    }
}
