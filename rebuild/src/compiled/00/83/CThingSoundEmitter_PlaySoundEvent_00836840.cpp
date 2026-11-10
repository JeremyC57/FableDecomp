// CThingSoundEmitter::PlaySoundEvent(CASoundBank&, const CCharString&, bool, CSoundInit&, bool) -- plays the named sound event through the thing's sound emitter unless the sound manager is gone or the thing is dead (then the invalid handle)
// Leaf: ?PlaySoundEvent@CThingSoundEmitter@@QAEKAAVCASoundBank@@ABVCCharString@@_NAAVCSoundInit@@2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836840, 37 bytes, thiscall `ret 14h`. Emitter slot 4 (+0x10) PlaySoundEvent.
// Model (agentSND2, 2026-11-10): the shared engine/CThingSoundEmitter.h with the opt-in FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
//   (+0x98 PSoundEmitter, the CASoundEmitter OnCreate gets from CASoundManager::Create3DEmitter; the Ego_r.pdb forwarder
//   declarations), engine/CThing.h's Dead bit (+0x91 bit 0, IsDead), engine/CASoundManager.h's PSoundManager and
//   engine/CASoundEmitter.h's slot names; was a local padded CThingSoundEmitter / CSoundPlayer model.
// Shape: the global test first, then the dead bit; the emitter call is a sibling-call `jmp` with the arguments in place.
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#include "engine/CASoundManager.h"
#include "engine/CThingSoundEmitter.h"
extern unsigned long g_InvalidSoundHandle;                // 0x0129D25C (the landed spelling)

unsigned long CThingSoundEmitter::PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached)
{
    if (PSoundManager && !IsDead())
        return PSoundEmitter->PlaySoundEvent(bank, name, loop, init, attached);
    return g_InvalidSoundHandle;
}
