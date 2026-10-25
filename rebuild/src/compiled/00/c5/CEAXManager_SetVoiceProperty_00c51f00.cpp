// CEAXManager::SetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice) -- write an EAX property of a voice through its effect object and, when that succeeds, queue the deferred commit of the changed-settings flags it reports (-4 when the voice is missing)
// Leaf: ?SetVoiceProperty@CEAXManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C51F00, length 76 bytes (+ int3 padding); `ret 10h`; thiscall on the
// global EAX manager @ 0x0143BB70.
// Callees: the effect's SetProperty 0x00C68510 (switch on `id & 0x7FFFFFFF`; manifest "CDrawGuildSeal_SetProperty" is
// propagated), QueueCommit 0x00C51A20 (thiscall on the manager; manifest "PropertySerializer_WriteProperty").
// Callers: the voice's EAX Set 0x00C597E0 (landed under the propagated label CThing::AddToRenderList) and three manager
// methods (0x00C51FBB, 0x00C52218, 0x00C52A5D).
// The catalog label "Model_WritePropertyIfValid" is propagated; descriptive name (libpk, no PDB/RTTI).
// Shape: the result accumulator puts `mov eax,-4` ahead of the `je` (the failure exit skips the esi restore); the dirty
// flags are homed in the dead `voice` argument slot.

struct CEAXEffect {
    long SetProperty(unsigned long id, void* data, unsigned long size, unsigned long* dirty);       // 0x00C68510
};

struct CEAXVoice {
    unsigned char _pad[0x1f0];
    CEAXEffect*   m_pEffect;                                // +0x1f0
};

struct CEAXManager {
    void QueueCommit(unsigned long dirty, CEAXVoice* voice);                                        // 0x00C51A20
    long SetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice);
};

long CEAXManager::SetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice)
{
    long result = -4;
    if (voice) {
        unsigned long dirty;
        result = voice->m_pEffect->SetProperty(id, data, size, &dirty);
        if (!result)
            QueueCommit(dirty, voice);
    }
    return result;
}