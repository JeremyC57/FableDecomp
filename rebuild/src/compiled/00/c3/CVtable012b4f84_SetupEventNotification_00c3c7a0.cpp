// CVtable_012b4f84::SetupEventNotification(const SBufferDesc_00c3c7a0* desc) -- slot-2 override of the sound buffer: adopt the descriptor through the base (0x00C3C4E0, a negative status is returned as is); without a live audio device report 0; a software buffer (+0x38 bit 0) given more than two channels drops its device buffer and returns the base status; otherwise the device buffer (+0x30) is rebuilt from the adopted descriptor (+0x10), and a failure closes the object (base Close) and is returned
// Leaf: ?SetupEventNotification@CVtable_012b4f84@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C7A0, 107 bytes (+ int3 padding), __fastcall `ret 4`, long status.
// Library: libpk audio (no PDB/RTTI); descriptive names (the slot name follows the landed base 0x00C3C4E0). Slot 2 of
//   the derived vtable 0x012B4F84 (slot 0 0x00C33330, slot 1 0x00C3C720, slot 3 0x00C3C4D0) over the landed base vtable
//   0x012B4F74. The manifest's "Audio_ProcessEventNotification" is propagated.
// Callees: the base SetupEventNotification 0x00C3C4E0 and Close 0x00C3C450 (qualified, direct calls), the device
//   buffer's Destroy 0x00C4D9B0 and Rebuild 0x00C4E1B0; the device global 0x0143BC2C (+0x1d4 interface).
// Shape: the status lives in edi; the device-missing arms share one `xor eax,eax` exit at the end; the software arm
//   tests the flag first and reads the channel count through the argument (ebx).
struct SBufferDesc_00c3c7a0 {                   // 28 bytes (the base's SNotificationDesc)
    unsigned long m_mode;                       // +0x00
    unsigned long m_channels;                   // +0x04
    unsigned long m_words[5];                   // +0x08
};

struct CAudioDevice_00c3c7a0 {
    unsigned char _pad_0x000[0x1d4];
    void* m_pInterface;                         // +0x1d4
};

extern CAudioDevice_00c3c7a0* g_pAudioDevice_0143bc2c;  // 0x0143BC2C

struct CDeviceBuffer_00c3c7a0 {
    void Destroy();                                     // 0x00C4D9B0
    long Rebuild(const SBufferDesc_00c3c7a0* desc);     // 0x00C4E1B0
};

class CVtable_012b4f74 {               // vtable 0x012B4F74
public:
    virtual void v00();
    virtual void Close();                               // slot 1 (0x00C3C450)
    virtual long SetupEventNotification(const SBufferDesc_00c3c7a0* desc);   // slot 2 (0x00C3C4E0)
    unsigned char _pad_0x04[0x0c];
    SBufferDesc_00c3c7a0 m_desc;                // +0x10
    unsigned long m_state;                      // +0x2c
};

class CVtable_012b4f84 : public CVtable_012b4f74 {   // vtable 0x012B4F84
public:
    CDeviceBuffer_00c3c7a0 m_buffer;            // +0x30
    unsigned char _pad_0x31[7];
    unsigned char m_flags;                      // +0x38 (bit 0: software buffer)
    virtual long SetupEventNotification(const SBufferDesc_00c3c7a0* desc);
};

long CVtable_012b4f84::SetupEventNotification(const SBufferDesc_00c3c7a0* desc)
{
    long result = CVtable_012b4f74::SetupEventNotification(desc);
    if (result < 0)
        return result;
    if (!g_pAudioDevice_0143bc2c || !g_pAudioDevice_0143bc2c->m_pInterface)
        return 0;
    if ((m_flags & 1) && desc->m_channels > 2) {
        m_buffer.Destroy();
        return result;
    }
    result = m_buffer.Rebuild(&m_desc);
    if (result >= 0)
        return 0;
    CVtable_012b4f74::Close();
    return result;
}