// CEAXManager::GetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice) -- read an EAX property of a voice through its effect object (-4 when the voice or its effect is missing)
// Leaf: ?GetVoiceProperty@CEAXManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4E380, length 51 bytes (+ int3 padding); `ret 10h`; thiscall on the
// global EAX manager @ 0x0143BB70, `this` unused.
// Callee: the effect's GetProperty 0x00C66C90 (switch on `id & 0x7FFFFFFF`, 0x1C property ids; manifest
// "ScriptEvent_DispatchByID" is propagated). Caller: the voice's EAX Get 0x00C59820 (landed under the propagated label
// CThing::AddToRenderList), passing its +0x50 handle as `voice`.
// The catalog label "ScriptEvent_DispatchIfActive" is propagated; descriptive name (libpk, no PDB/RTTI).
// Shape: the result accumulator (`result = -4` before the tests) puts `mov eax,-4` ahead of the first `je` so both
// failure exits share the call's `ret`; the out local is homed in the dead `voice` argument slot.

struct CEAXEffect {
    long GetProperty(unsigned long id, void* data, unsigned long size, unsigned long* returned);   // 0x00C66C90
};

struct CEAXVoice {
    unsigned char _pad[0x1f0];
    CEAXEffect*   m_pEffect;                                // +0x1f0
};

struct CEAXManager {
    long GetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice);
};

long CEAXManager::GetVoiceProperty(unsigned long id, void* data, unsigned long size, CEAXVoice* voice)
{
    long result = -4;
    if (voice && voice->m_pEffect) {
        unsigned long returned;
        result = voice->m_pEffect->GetProperty(id, data, size, &returned);
    }
    return result;
}