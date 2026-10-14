// CSoundFormat_00c262a0::CSoundFormat_00c262a0() -- default sound-format record: one channel, 44100 Hz, no buffer, index -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C262A0, length 52 bytes (+ int3 padding).
// Callees: none.
// Shape: a non-polymorphic 0x2c-byte record initialised through one zero register: pointer 0, long -1, long 0,
// three bool flags (+0x14, +0x16, +0x17 -- +0x15 and +0x0c..+0x13 are left alone), three zero dwords, an
// unsigned short channel count of 1, an unsigned short 0 and the 44100 (0xAC44) sample rate. `mov eax,ecx`
// first: the constructor returns `this`. The manifest label "CAnimationSet::CAnimationSet" is propagated
// (the real CAnimationSet constructor is @ 0x005DD2E0); the class is named by its address and content.
struct CSoundFormat_00c262a0 {
    void* m_pData;                              // +0x00
    long m_index;                               // +0x04 -1
    long m_size;                                // +0x08
    unsigned char _unset_0x0c[8];               // +0x0c .. +0x13 not initialised
    bool m_flag14;                              // +0x14
    unsigned char _unset_0x15;                  // +0x15 not initialised
    bool m_flag16;                              // +0x16
    bool m_flag17;                              // +0x17
    long m_value18;                             // +0x18
    long m_value1c;                             // +0x1c
    long m_value20;                             // +0x20
    unsigned short m_channels;                  // +0x24 1
    unsigned short m_bits;                      // +0x26 0
    unsigned long m_sampleRate;                 // +0x28 44100
    CSoundFormat_00c262a0();
};

CSoundFormat_00c262a0::CSoundFormat_00c262a0()
    : m_pData(0), m_index(-1), m_size(0), m_flag14(false), m_flag16(false), m_flag17(false),
      m_value18(0), m_value1c(0), m_value20(0), m_channels(1), m_bits(0), m_sampleRate(44100)
{
}
