// CAudioObject::Stop(unsigned long param1, unsigned long param2) -- stop the audio object's voice (+0x50, slot 11): on success a ready object (state 1) goes straight to idle through SetState(0), a stopped one (8) is left alone, any other state uncounts the voice (unless virtual, idle or 0x10) and becomes 8 (stopped, voice kept) with a state-change callback
// Leaf: ?Stop@CAudioObject@@QAEXKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43980, 130 bytes (+ int3 padding), __fastcall `ret 8`, no EH frame.
// Library: libpk audio (no PDB/RTTI): descriptive names. Callers: two `__stdcall` API wrappers 0x00C35C30 (when the
//   flags have any of 0x217) and 0x00C35C50 (state 1/2/0x10 with no loop or volume record), each forwarding its two
//   arguments. The manifest's "CCombatRenderState_OnActionComplete" is propagated.
// Callees: voice slot 11 (two arguments, bool), SetState 0x00C43570, Uncount 0x00C3EB00 (g_pVoiceManager
//   0x0143BB5C), the `__stdcall` state-change callback at +0xDC.
// Shape: the state store is `(m_flags & ~0x7F) | 8`, folded to `and 0FFFFFF88h ; or 8`; the callback load is hoisted
//   above that store and the stored register is reused for `& 0xFFFF` (early returns or nested ifs, either form).
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
};
extern CVoiceManager* g_pVoiceManager;      // 0x0143BB5C

class CAudioObject;
typedef void (__stdcall *TStateCallback)(CAudioObject* object, unsigned long flags, void* data);

class CAudioObject {
public:
    void*          m_vptr;                  // +0x00 (vtable 0x012B5A88)
    unsigned long  m_node[2];               // +0x04
    unsigned long  m_caps;                  // +0x0c
    unsigned long  m_flags;                 // +0x10 state bits 0..6, bit 9 virtual
    unsigned char  m_pad14[0x50 - 0x14];
    CVoice*        m_pVoice;                // +0x50
    unsigned char  m_pad54[0xdc - 0x54];
    TStateCallback m_stateCallback;         // +0xdc
    void*          m_stateCallbackData;     // +0xe0
    unsigned long GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    void RaiseStateChange()
    {
        if (m_stateCallback)
            m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData);
    }
    void SetState(unsigned long state);     // 0x00C43570
    void Stop(unsigned long param1, unsigned long param2);
};

void CAudioObject::Stop(unsigned long param1, unsigned long param2)
{
    if (!m_pVoice->Stop(param1, param2))
        return;
    unsigned long state = GetState();
    if (state == 1) {
        SetState(0);
        return;
    }
    if (state == 8)
        return;
    if (!IsVirtual() && state != 0 && state != 0x10)
        g_pVoiceManager->Uncount(m_pVoice);
    m_flags = (m_flags & ~0x7fUL) | 8;
    RaiseStateChange();
}