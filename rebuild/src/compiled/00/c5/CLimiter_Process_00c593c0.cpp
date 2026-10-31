// CLimiter::Process(short* out, unsigned long stride, const long* src, long shift) -- run one 128-sample block of a mixed channel through the output limiter: each shifted sample enters the 88-entry delay line, the sample leaving it is written to the interleaved 16-bit output saturated (pass-through), saturated while the attack envelope runs (state 1, 44 samples), or scaled by 32767 / envelope while the release runs (state 2)
// Leaf: ?Process@CLimiter@@QAEXPAFKPBJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C593C0, 432 bytes, thiscall, `ret 10h`.
// Identity: the landed caller CLimitedMixBuffer::WriteChannel 0x00C591C0 binds this address as
//   `?Process@CLimiter@@QAEXPAFKPBJJ@Z` (m_limiters[channel].Process(out, m_numChannels, m_pChannel[channel], m_active));
//   `this` is the 0x17C-byte GameCODA limiter (vtable 0x012B8F64) of the landed CLimiter rows (ctor 0x00C59570, Clear
//   0x00C592D0, UpdateEnvelope 0x00C592F0, the only callee). Descriptive names (GameCODA: no PDB/RTTI).
// Retail-asm: 0x00C59526 `fstp dword [esp+20h] ; fld dword [esp+20h] ; fistp dword [esp+18h]` -- libpk's inline-assembly
//   float-to-long helper (value homed in the dead `src` slot, result in the dead `out` slot), the same block as the landed
//   retail_asm CVoiceEffect::Configure 0x00C6AEF0; VC7.1 C++ `(long)x` calls __ftol2, /QIfist stores `fistp qword`.
// Shape: the byte stride `stride * 2` is hoisted into the frame word of the pushed ecx, the 128 count lives in the dead
//   `stride` slot, the delayed sample is homed in the dead `src` slot for the `fimul`; `++m_writeIndex == 88` wraps,
//   the release counter compares unsigned (`jbe`), the magnitude of a negative input is `-1 - s` (unsigned).
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

class CLimiter {                                        // vtable 0x012B8F64, 0x17C bytes
public:
    virtual ~CLimiter();                                // 0x00C592C0
    long          m_delay[88];                          // +0x004 delay line of mixed samples
    unsigned long m_writeIndex;                         // +0x164
    float         m_peak;                               // +0x168
    float         m_envelope;                           // +0x16c
    long          m_holdCount;                          // +0x170
    long          m_state;                              // +0x174 0 pass-through, 1 attack, 2 release
    unsigned long m_releaseCount;                       // +0x178
    void UpdateEnvelope();                              // 0x00C592F0
    void Process(short* out, unsigned long stride, const long* src, long shift);
};

void CLimiter::Process(short* out, unsigned long stride, const long* src, long shift)
{
    for (long count = 128; count != 0; --count) {
        long s = *src++ >> shift;
        m_delay[m_writeIndex] = s;
        if (++m_writeIndex == 88)
            m_writeIndex = 0;
        long delayed = m_delay[m_writeIndex];
        if (m_state == 0) {
            if (delayed > 32767)
                delayed = 32767;
            else if (delayed < -32768)
                delayed = -32768;
            *out = (short)delayed;
            unsigned long mag = s < 0 ? -1 - s : s;
            if (mag > 32767) {
                m_state = 1;
                m_releaseCount = 0;
                m_peak = 0.0f;
                m_envelope = 0.0f;
                m_holdCount = 0;
            }
        } else if (m_state == 1) {
            UpdateEnvelope();
            if (delayed > 32767)
                delayed = 32767;
            else if (delayed < -32768)
                delayed = -32768;
            *out = (short)delayed;
            if (++m_releaseCount > 44)
                m_state = 2;
        } else {
            UpdateEnvelope();
            if (m_envelope <= 32767.0f) {
                if (delayed > 32767)
                    delayed = 32767;
                else if (delayed < -32768)
                    delayed = -32768;
                *out = (short)delayed;
            } else {
                float gain = 32767.0f / m_envelope;
                long scaled = FloatToLong(gain * delayed);
                if (scaled > 32767)
                    scaled = 32767;
                else if (scaled < -32768)
                    scaled = -32768;
                *out = (short)scaled;
            }
        }
        out += stride;
    }
}