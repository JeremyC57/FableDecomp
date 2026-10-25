// CAudioDevice_00c3e800::UpdateMeters() -- feed the output level meters for this frame: with a primary output (+0x180) and metering enabled (+0x1cc set, mode +0x1b8 not 1) the two 0x8c-byte meters (+0x5c front, +0xe8 rear) are zeroed on first use (+0x1c4 latch) and then given the mixer's front pair (mixer +4/+8) and, for four or more channels (+0x1bc), its rear pair (+0xc/+0x10); otherwise the latch is cleared
// Leaf: ?UpdateMeters@CAudioDevice_00c3e800@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3E800, 142 bytes (+ int3 padding), __fastcall, plain `ret`.
// Library: libpk audio (no PDB/RTTI); descriptive names. Same device object as BeginFrame 0x00C3E760; single caller
//   the audio system's frame update 0x00C3D582. The manifest's "Audio_ProcessMixingIfNeeded" is propagated.
// Callee: the meter's Process(left, right) 0x00C547D0 (thiscall).
// Shape: the constant 1 of the mode compare (`mov eax,1 ; cmp ecx,eax`) is reused for the latch store; two inline
//   memsets (`mov ecx,23h ; lea edi ; rep stosd` each) share the zero eax; the "else" clears the latch and falls into the
//   shared exit.
#include <string.h>

struct CMixer_00c3e800 {
    unsigned char _pad_0x00[4];
    long m_frontLeft;                           // +0x04
    long m_frontRight;                          // +0x08
    long m_rearLeft;                            // +0x0c
    long m_rearRight;                           // +0x10
};

struct CLevelMeter_00c3e800 {                   // 0x8c bytes
    long m_words[0x23];
    void Process(long left, long right);        // 0x00C547D0
};

struct CAudioDevice_00c3e800 {
    unsigned char _pad_0x000[0x58];
    CMixer_00c3e800* m_pMixer;                  // +0x058
    CLevelMeter_00c3e800 m_frontMeter;          // +0x05c
    CLevelMeter_00c3e800 m_rearMeter;           // +0x0e8
    unsigned char _pad_0x174[0x0c];
    void* m_pPrimary;                           // +0x180
    unsigned char _pad_0x184[0x34];
    long m_meterMode;                           // +0x1b8 (1: off)
    unsigned long m_channels;                   // +0x1bc
    unsigned char _pad_0x1c0[4];
    bool m_metersPrimed;                        // +0x1c4
    unsigned char _pad_0x1c5[7];
    void* m_pMeterSink;                         // +0x1cc
    void UpdateMeters();
};

void CAudioDevice_00c3e800::UpdateMeters()
{
    if (!m_pPrimary)
        return;
    if (m_pMeterSink && m_meterMode != 1) {
        if (!m_metersPrimed) {
            m_metersPrimed = true;
            memset(&m_frontMeter, 0, sizeof(m_frontMeter));
            memset(&m_rearMeter, 0, sizeof(m_rearMeter));
        }
        m_frontMeter.Process(m_pMixer->m_frontLeft, m_pMixer->m_frontRight);
        if (m_channels >= 4)
            m_rearMeter.Process(m_pMixer->m_rearLeft, m_pMixer->m_rearRight);
    } else
        m_metersPrimed = false;
}