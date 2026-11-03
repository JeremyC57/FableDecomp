// CEAXManager::GetEffectProperty(unsigned long slot, unsigned long property, void* data, unsigned long size) -- read an EAX 4.0 FX-slot property through the listener at +0xE4 (CEAXListener::GetEAX4FXSlot 0x00C66330), with the out word homed in the size slot
// Leaf: ?GetEffectProperty@CEAXManager@@QAEJKKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4E350, 39 bytes, thiscall `ret 10h`.
// Identity: called from the EAX property-set dispatcher 0x00C4F2A0.. (0x00C4F2EF, 0x00C4F323, 0x00C4F3F0), which compares
//   the caller's property-set GUID with `repe cmpsd` and passes (0, data, size, ...) with ecx = the manager; the sibling
//   CEAXManager::GetVoiceProperty 0x00C4E380 reads a voice's effect the same way. Descriptive (libpk, no PDB/RTTI): the
//   callee is the GameCODA 2.10 CEAXListener::GetEAX4FXSlot(slot, property, data, size, returned) (EAX4Translation.obj, owner
//   decision 2026-11-03 D3), which fixes the argument roles; the manager's own name stays descriptive.
// Shape: the five pushes right to left (&returned, size, data, property, slot); `returned` lives in the dead size slot.
class CEAXListener {
public:
    int GetEAX4FXSlot(unsigned long slot, unsigned long property, void* data, unsigned long size, unsigned long* returned);   // 0x00C66330
};

struct CEAXManager {
    unsigned char        _pad[0xe4];
    CEAXListener*        m_pEffect;                             // +0xe4
    long GetEffectProperty(unsigned long slot, unsigned long property, void* data, unsigned long size);
};

long CEAXManager::GetEffectProperty(unsigned long slot, unsigned long property, void* data, unsigned long size)
{
    unsigned long returned;
    return m_pEffect->GetEAX4FXSlot(slot, property, data, size, &returned);
}