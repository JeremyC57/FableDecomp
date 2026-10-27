// CEAXManager::GetEffectProperty(unsigned long id, void* data, unsigned long size, unsigned long flags) -- read an EAX property through the effect object at +0xE4 (callee 0x00C66330), with an out word homed in the flags slot
// Leaf: ?GetEffectProperty@CEAXManager@@QAEJKPAXKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4E350, 39 bytes, thiscall `ret 10h`.
// Identity: called from the EAX property-set dispatcher 0x00C4F2A0.. (0x00C4F2EF, 0x00C4F323, 0x00C4F3F0), which compares
//   the caller's property-set GUID with `repe cmpsd` and passes (0, data, size, ...) with ecx = the manager; the sibling
//   CEAXManager::GetVoiceProperty 0x00C4E380 reads a voice's effect the same way. Descriptive (libpk, no PDB/RTTI): the
//   callee's name and the fourth argument's role are not proven.
// Shape: the five pushes right to left (&returned, flags, size, data, id); `returned` lives in the dead flags slot.
struct CEAXEffect_00c66330 {
    long GetProperty(unsigned long id, void* data, unsigned long size, unsigned long flags, unsigned long* returned);   // 0x00C66330
};

struct CEAXManager {
    unsigned char        _pad[0xe4];
    CEAXEffect_00c66330* m_pEffect;                             // +0xe4
    long GetEffectProperty(unsigned long id, void* data, unsigned long size, unsigned long flags);
};

long CEAXManager::GetEffectProperty(unsigned long id, void* data, unsigned long size, unsigned long flags)
{
    unsigned long returned;
    return m_pEffect->GetProperty(id, data, size, flags, &returned);
}