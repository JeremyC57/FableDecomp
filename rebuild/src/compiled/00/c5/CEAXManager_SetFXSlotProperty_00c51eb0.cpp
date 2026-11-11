// CEAXManager::SetFXSlotProperty(unsigned long slot, unsigned long id, void* data, unsigned long size, CEAXVoice* voice) -- write an EAX 4 effect-slot property through the manager's slot object and queue the deferred commit of whatever settings it changed
// Leaf: ?SetFXSlotProperty@CEAXManager@@QAEJKKPAXKPAUCEAXVoice@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C51EB0, 74 bytes, thiscall `ret 14h`, one local (`push ecx`).
// Identity (descriptive; libpk/CODA EAX layer, no PDB or RTTI): the landed QueueCommit 0x00C51A20 row describes this body
//   behind its jump tables ("calls ... on the +0xE4 listener with a local out word, then this QueueCommit -- an EAX 4 property
//   setter"); sibling of the landed SetVoiceProperty 0x00C51F00. The callee 0x00C68F90 (thiscall, `ret 14h`) rejects a first
//   argument of 4 or more with -3: an effect-slot index. No direct caller in the image. Names are descriptive (NOTES).
// Shape: the change list starts empty (`mov [esp+1Ch],0` between the pushes and the call), the result waits in edi over the
//   commit, which runs whatever the setter returned.
struct _CHANGES;
struct CEAXVoice;
class CEAXFXSlots {
public:
    long SetSlotProperty(unsigned long slot, unsigned long id, void* data, unsigned long size, _CHANGES** changes);   // 0x00C68F90
};
struct CEAXManager {
    unsigned char _pad0[0xe4];
    CEAXFXSlots*  m_pFXSlots;                               // +0xe4
    void QueueCommit(_CHANGES* changes, CEAXVoice* voice);                                        // 0x00C51A20
    long SetFXSlotProperty(unsigned long slot, unsigned long id, void* data, unsigned long size, CEAXVoice* voice);
};

long CEAXManager::SetFXSlotProperty(unsigned long slot, unsigned long id, void* data, unsigned long size, CEAXVoice* voice)
{
    _CHANGES* changes = 0;
    long result = m_pFXSlots->SetSlotProperty(slot, id, data, size, &changes);
    QueueCommit(changes, voice);
    return result;
}