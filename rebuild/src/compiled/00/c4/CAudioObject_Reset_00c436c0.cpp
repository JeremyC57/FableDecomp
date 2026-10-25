// CAudioObject::Reset() -- drop the audio object back to idle: with any low flag bits set, uncount its voice unless it sits in an uncounted state (8, 0, 1) of a non-virtual object, stop a kept voice (bit 17) or release it to the voice manager, clear the low 16 flag bits and raise the state-change callback; always zero the status word (+0xF0)
// Leaf: ?Reset@CAudioObject@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C436C0, 160 bytes, __fastcall `ret`, no EH frame.
// Library: libpk audio (no PDB/RTTI): descriptive names. Callers 0x00C437C7, Shutdown 0x00C43810 (landed),
//   0x00C43E20, 0x00C43EC0 (landed) and Close 0x00C44170 (landed, which calls it `Cleanup`). The manifest's
//   "Thing_Cleanup" is propagated.
// Callees: the voice manager (g_pVoiceManager 0x0143BB5C): Uncount 0x00C3EB00, Release 0x00C4D070; voice slot 11
//   Stop(0, 0); the `__stdcall` state-change callback at +0xDC.
// Shape: one `if (m_flags & 0xffff) { ... }` around the body with the status store after it (VC7.1 copies the
//   store into the skip arm; an early-return spelling is 152 bytes); `m_flags &= 0xFFFF0000` is the `mov word ptr
//   [esi+10h],di` store; the uncount test is `(state != 8 && state != 0 && state != 1) || IsVirtual()`.
class CVoice {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool IsHardware() const;                        // slot 4
    virtual bool Is3D() const;                              // slot 5
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual bool Stop(unsigned long param1, unsigned long param2);  // slot 11
};

class CVoiceManager {
public:
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
    unsigned long  m_flags;                 // +0x10 state bits 0..6, bit 9 virtual, bit 17 keep voice
    unsigned char  m_pad14[0x50 - 0x14];
    CVoice*        m_pVoice;                // +0x50
    unsigned char  m_pad54[0xdc - 0x54];
    TStateCallback m_stateCallback;         // +0xdc
    void*          m_stateCallbackData;     // +0xe0
    unsigned char  m_padE4[0xf0 - 0xe4];
    long           m_status;                // +0xf0
    unsigned long GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    bool KeepsVoice() const { return (m_flags >> 17) & 1; }
    void RaiseStateChange()
    {
        if (m_stateCallback)
            m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData);
    }
    void Reset();
};

void CAudioObject::Reset()
{
    if (m_flags & 0xffff) {
        unsigned long state = GetState();
        if ((state != 8 && state != 0 && state != 1) || IsVirtual())
            g_pVoiceManager->Uncount(m_pVoice);
        if (m_pVoice) {
            if (KeepsVoice())
                m_pVoice->Stop(0, 0);
            else {
                g_pVoiceManager->Release(m_pVoice);
                m_pVoice = 0;
            }
        }
        m_flags &= 0xffff0000;
        RaiseStateChange();
    }
    m_status = 0;
}