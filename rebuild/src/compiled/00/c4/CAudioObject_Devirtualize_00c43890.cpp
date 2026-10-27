// CAudioObject::Devirtualize(long value) -- store the word at +0xEC; when the object is virtual (flag bit 9) clear that bit, uncount its voice from the voice manager in the idle states (0, 8) and raise the state-change callback
// Leaf: ?Devirtualize@CAudioObject@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43890, 93 bytes, thiscall `ret 4`.
// Identity: called from the libpk API wrapper at 0x00C35C19; the class model (flags +0x10: state bits 0..6, bit 9
//   virtual; voice +0x50; `__stdcall` state callback +0xDC/+0xE0) is the landed CAudioObject::Reset 0x00C436C0.
//   Descriptive (libpk, no PDB/RTTI): the method name and the +0xEC word's role are not proven.
// Callees: CVoiceManager::Uncount 0x00C3EB00 on g_pVoiceManager 0x0143BB5C (the bound names).
// Shape: the bit test is the inline IsVirtual (`shr ecx,9 ; test cl,1`), the cleared word stays in eax for the state test.
class CVoice;

class CVoiceManager {
public:
    void Uncount(CVoice* voice);            // 0x00C3EB00
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
    unsigned char  m_padE4[0xec - 0xe4];
    long           m_ec;                    // +0xec
    long           m_status;                // +0xf0
    unsigned long GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    void RaiseStateChange()
    {
        if (m_stateCallback)
            m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData);
    }
    void Devirtualize(long value);
};

void CAudioObject::Devirtualize(long value)
{
    m_ec = value;
    if (IsVirtual()) {
        m_flags &= ~0x200;
        unsigned long state = GetState();
        if (state == 0 || state == 8)
            g_pVoiceManager->Uncount(m_pVoice);
        RaiseStateChange();
    }
}