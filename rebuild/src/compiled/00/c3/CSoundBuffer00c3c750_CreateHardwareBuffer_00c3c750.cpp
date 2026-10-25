// CSoundBuffer_00c3c750::CreateHardwareBuffer() -- create the device buffer of a sound buffer object once the audio device is up: with a device (global 0x0143BC2C, its +0x1d4 interface set) and the "software" bit (+0x38 bit 0) clear, a descriptor of more than two channels fails with -14, otherwise the device buffer (+0x30) is created from the descriptor; no device returns 0
// Leaf: ?CreateHardwareBuffer@CSoundBuffer_00c3c750@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C750, 69 bytes (+ int3 padding), __fastcall, plain `ret`, long status.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` derives from the landed vtable-0x012B4F74 base
//   (CVtable_012b4f74: slot 1 Close 0x00C3C450, slot 2 SetupEventNotification 0x00C3C4E0, descriptor copy +0x10, state
//   +0x2c); its own vtable is 0x012B4F84 (slot 1 0x00C3C720, slot 2 0x00C3C7A0). Callers 0x00C43E56 / 0x00C43F94.
//   The manifest's "CModeBlock_CreateAudioBuffer" is propagated.
// Callees: GetDesc 0x00C3C4A0 (`lea eax,[ecx+10h] ; ret`, called out of line twice), the device buffer's Create
//   0x00C4DC40 (it repeats the channel check itself).
// Shape: ONE `result = 0` variable returned at the end (`xor eax,eax` before the first test, the guard misses fall
//   straight to the shared `pop esi ; ret`); the channel failure is its own early return.
struct SBufferDesc_00c3c750 {                   // 28 bytes (the base's SNotificationDesc)
    unsigned long m_mode;                       // +0x00
    unsigned long m_channels;                   // +0x04
    unsigned long m_words[5];                   // +0x08
};

struct CAudioDevice_00c3c750 {
    unsigned char _pad_0x000[0x1d4];
    void* m_pInterface;                         // +0x1d4
};

extern CAudioDevice_00c3c750* g_pAudioDevice_0143bc2c;  // 0x0143BC2C

struct CDeviceBuffer_00c3c750 {
    long Create(const SBufferDesc_00c3c750* desc);      // 0x00C4DC40
};

struct CSoundBuffer_00c3c750 {
    virtual void v00();
    unsigned char _pad_0x04[0x0c];
    SBufferDesc_00c3c750 m_desc;                // +0x10
    unsigned long m_state;                      // +0x2c
    CDeviceBuffer_00c3c750 m_buffer;            // +0x30
    unsigned char _pad_0x31[7];
    unsigned char m_flags;                      // +0x38 (bit 0: software buffer)
    const SBufferDesc_00c3c750* GetDesc() const;        // 0x00C3C4A0
    long CreateHardwareBuffer();
};

long CSoundBuffer_00c3c750::CreateHardwareBuffer()
{
    long result = 0;
    if (g_pAudioDevice_0143bc2c && g_pAudioDevice_0143bc2c->m_pInterface && !(m_flags & 1)) {
        if (GetDesc()->m_channels > 2)
            return -14;
        result = m_buffer.Create(GetDesc());
    }
    return result;
}