// CThingSoundEmitter::IsSamplePlaying(unsigned long) const -- whether the thing's sound emitter still plays the sample (false when the sound manager is gone or the thing is dead)
// Leaf: ?IsSamplePlaying@CThingSoundEmitter@@QBE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836870, thiscall `ret 4`. Emitter slot 22 (+0x58) IsSpecificSamplePlaying.
// Model (agentSND2, 2026-11-10): the shared engine/CThingSoundEmitter.h with the opt-in FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
//   (+0x98 PSoundEmitter, the CASoundEmitter OnCreate gets from CASoundManager::Create3DEmitter; the Ego_r.pdb forwarder
//   declarations), engine/CThing.h's Dead bit (+0x91 bit 0, IsDead), engine/CASoundManager.h's PSoundManager and
//   engine/CASoundEmitter.h's slot names; was a local padded CThingSoundEmitter / CSoundPlayer model.
// Shape: the global test first, then the dead bit; the emitter call is a sibling-call `jmp` with the arguments in place.
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#include "engine/CASoundManager.h"
#include "engine/CThingSoundEmitter.h"

bool CThingSoundEmitter::IsSamplePlaying(unsigned long sampleID) const
{
    if (PSoundManager && !IsDead())
        return PSoundEmitter->IsSpecificSamplePlaying(sampleID);
    return false;
}
