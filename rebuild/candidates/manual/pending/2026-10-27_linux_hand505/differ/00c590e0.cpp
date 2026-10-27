// CMixBuffer::WriteChannel16(short* dst, unsigned long channel) -- vtable slot 10: write one 128-sample channel accumulator out as interleaved 16-bit PCM: each sample shifted right by the headroom shift (+0x1C), clamped to [-32768, 32767] and stored every m_numChannels shorts
// Grade: DIFFER(217v217) -- register swap only: the source walk sits in esi and the 16-bit output in edi where retail has the output in esi and the source in edi (`mov esi,[esp+0Ch] ; ... ; mov edi,[eax+ecx*4+4]`); a named output local gives the same, an indexed source `src[i]` biases the pointer (227).
// Leaf: ?WriteChannel16@CMixBuffer@@UAEXPAFK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C590E0, 217 bytes, thiscall `ret 8`.
// Identity (descriptive; GameCODA has no PDB or RTTI): slot 10 of the CMixBuffer vtable 0x012B8F38 (its only reference,
//   0x012B8F60), after the landed add kernels of slots 3..9; the accumulators +0x04 and the channel count +0x20 are the
//   landed model's; the word at +0x1C (1 after construction, the landed model's m_active) is the output shift.
//   The catalog's gapscan name is a placeholder.
// Shape: the source walks by pointer (`*src++`, offsets 0/4/8/0Ch per unrolled step, no bias); the shift and the stride
//   are re-read after every 16-bit store (the store may alias them); VC7.1 unrolls the 128-step loop by 4.
class CMixBuffer {
public:
    virtual ~CMixBuffer();                                                            // slot 0 (0x00C59250)
    virtual void Reset();                                                             // slot 1 (0x00C59240)
    virtual void Clear();                                                             // slot 2 (0x00C57F00)
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);         // slot 3 (0x00C580C0)
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);      // slot 4 (0x00C58040)
    virtual void MixAdd32(const long* src, long* dst);                                // slot 5 (0x00C58000)
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);           // slot 6 (0x00C57FC0)
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain); // slot 7 (0x00C57F60)
    virtual void MixAddFloat(const float* src, long* dst);                            // slot 8 (0x00C57F20)
    virtual void MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step);   // slot 9 (0x00C58140)
    virtual void WriteChannel16(short* dst, unsigned long channel);                   // slot 10 (0x00C590E0)
    long*         m_pChannel[6];        // +0x04 planar 128-sample accumulators
    long          m_shift;              // +0x1c output headroom shift (1 after construction)
    unsigned long m_numChannels;        // +0x20
    unsigned char m_storage[0xc10];     // +0x24 accumulator store, 16-byte aligned by hand
};

void CMixBuffer::WriteChannel16(short* dst, unsigned long channel)
{
    const long* src = m_pChannel[channel];
    for (int i = 0; i < 128; ++i) {
        long v = *src++ >> m_shift;
        if (v > 0x7fff)
            v = 0x7fff;
        else if (v < -0x8000)
            v = -0x8000;
        *dst = (short)v;
        dst += m_numChannels;
    }
}
