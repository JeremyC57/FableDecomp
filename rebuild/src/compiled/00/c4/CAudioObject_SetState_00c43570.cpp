// CAudioObject::SetState(unsigned long state) -- move the audio object to a new 7-bit play state (+0x10 bits 0..6): unless the object is virtual (bit 9) keep the voice manager's playing count in step (count on 2, uncount on leaving a counted state for 8 or 0) and on 0 release the voice (+0x50) unless it is kept (bit 17); then store the state, clear bit 8 on 0 and raise the state-change callback
// Leaf: ?SetState@CAudioObject@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43570, 220 bytes, __fastcall `ret 4`, no EH frame.
// Library: libpk audio (no PDB/RTTI): descriptive names. Callers 0x00C439AA (Stop 0x00C43980, from the ready state)
//   and 0x00C43AA3. The landed 0x00C43FD0 inlines the short form of the same state store + callback. The
//   manifest's "CCombatRenderState_SetStateValue" is propagated.
// Callees: the voice manager (g_pVoiceManager 0x0143BB5C): Count 0x00C3EAA0, Uncount 0x00C3EB00, Release 0x00C4D070;
//   the `__stdcall` state-change callback at +0xDC (object, low 16 flag bits, data +0xE0).
// Shape: the argument is masked first (`and edi,7Fh`) and compared with the cached old state; a three-case switch
//   (VC7.1 `sub ecx,0 / sub ecx,2 / sub ecx,6`, case bodies laid out 8, 2, 0 in either source order); the flag
//   accessors are shifts (`shr ecx,9 ; test cl,1`), the state store `and 0FFFFFF80h ; or eax,edi`, and the bit-8
//   clear works on the stored register; the callback re-reads the flags.
class CVoice;

class CVoiceManager {
public:
    void Count(CVoice* voice);              // 0x00C3EAA0
    void Uncount(CVoice* voice);            // 0x00C3EB00
    void Release(CVoice* voice);            // 0x00C4D070
};
extern CVoiceManager* g_pVoiceManager;      // 0x0143BB5C

class CAudioObject;
typedef void (__stdcall *TStateCallback)(CAudioObject* object, unsigned long flags, void* data);

class CAudioObject {
public:
    void*          m_vptr;                  // +0x00 (vtable 0x012B5A88)
    unsigned long  m_node[2];               // +0x04
    unsigned long  m_caps;                  // +0x0c
    unsigned long  m_flags;                 // +0x10 state bits 0..6, bit 8, bit 9 virtual, bit 17 keep voice
    unsigned char  m_pad14[0x50 - 0x14];
    CVoice*        m_pVoice;                // +0x50
    unsigned char  m_pad54[0xdc - 0x54];
    TStateCallback m_stateCallback;         // +0xdc
    void*          m_stateCallbackData;     // +0xe0
    unsigned long GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    bool KeepsVoice() const { return (m_flags >> 17) & 1; }
    void SetState(unsigned long state);
};

void CAudioObject::SetState(unsigned long state)
{
    state &= 0x7f;
    unsigned long old = GetState();
    if (state == old)
        return;
    if (!IsVirtual()) {
        switch (state) {
        case 0:
            if (old != 8 && old != 1 && old != 0x10)
                g_pVoiceManager->Uncount(m_pVoice);
            if (!KeepsVoice()) {
                g_pVoiceManager->Release(m_pVoice);
                m_pVoice = 0;
            }
            break;
        case 2:
            g_pVoiceManager->Count(m_pVoice);
            break;
        case 8:
            if (old != 0 && old != 1 && old != 0x10)
                g_pVoiceManager->Uncount(m_pVoice);
            break;
        }
    }
    m_flags = (m_flags & ~0x7fUL) | state;
    if (state == 0)
        m_flags &= ~0x100UL;
    if (m_stateCallback)
        m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData);
}