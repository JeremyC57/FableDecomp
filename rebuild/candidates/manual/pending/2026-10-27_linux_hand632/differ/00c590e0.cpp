// CMixBuffer::WriteChannel(short* out, unsigned long channel) -- vtable slot 10: write one 128-sample channel accumulator out as interleaved 16-bit PCM: each sample shifted right by the output shift (+0x1C), clamped to [-32768, 32767] and stored every m_numChannels shorts
// Grade: DIFFER(217v217) -- esi/edi swap only: retail keeps the 16-bit output in esi and the source walk in edi, this source the reverse
// Leaf: ?WriteChannel@CMixBuffer@@UAEXPAFK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C590E0, 217 bytes, thiscall `ret 8`.
// Identity (descriptive; GameCODA has no PDB or RTTI): slot 10 of the CMixBuffer vtable 0x012B8F38 (its only reference,
//   0x012B8F60), after the landed add kernels of slots 3..9; the landed CLimitedMixBuffer::WriteChannel 0x00C591C0
//   overrides this slot and already spells it CMixBuffer::WriteChannel(short* out, unsigned long channel).
//   The catalog's gapscan name is a placeholder.
// Layout: the landed CMixBuffer model (CPKObject root, m_pChannel +0x04, m_active +0x1C = output shift, m_numChannels +0x20).
// Shape: the source walks by pointer (`*src++`, offsets 0/4/8/0Ch per unrolled step, no bias); the shift and the stride
//   are re-read after every 16-bit store (the store may alias them); VC7.1 unrolls the 128-step loop by 4.
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CMixBuffer : public CPKObject {                   // vtable 0x012B8F38, 0xC34 bytes
public:
    virtual ~CMixBuffer();                              // 0x00C57EF0
    virtual void Reset();                               // slot 1
    virtual void Clear();                               // slot 2
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);
    virtual void MixAdd32(const long* src, long* dst);
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain);
    virtual void MixAddFloat(const float* src, long* dst);
    virtual void MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step);
    virtual void WriteChannel(short* out, unsigned long channel);   // slot 10 (0x00C590E0)
    long*         m_pChannel[6];                        // +0x04 planar 128-sample accumulators
    long          m_active;                             // +0x1c output shift
    unsigned long m_numChannels;                        // +0x20
    unsigned char m_storage[0xc10];                     // +0x24 accumulator store, 16-byte aligned by hand
};

void CMixBuffer::WriteChannel(short* out, unsigned long channel)
{
    const long* src = m_pChannel[channel];
    for (int i = 0; i < 128; ++i) {
        long v = *src++ >> m_active;
        if (v > 0x7fff)
            v = 0x7fff;
        else if (v < -0x8000)
            v = -0x8000;
        *out = (short)v;
        out += m_numChannels;
    }
}
