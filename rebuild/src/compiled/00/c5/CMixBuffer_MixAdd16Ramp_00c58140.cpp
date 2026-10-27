// CMixBuffer::MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step) -- vtable slot 9: add a 128-sample block of 16-bit samples into a long accumulator under a ramped Q15 gain (`(s * gain) >> 15`, gain += step after every sample, wrapping at 16 bits)
// Leaf: ?MixAdd16Ramp@CMixBuffer@@UAEXPBFPAJGF@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C58140, 137 bytes, thiscall `ret 10h` (`this` unused).
// Identity (descriptive, as the landed CMixBuffer model names it; GameCODA has no PDB or RTTI): slot 9 of the CMixBuffer
//   vtable 0x012B8F38 (reference 0x012B8F5C; also 0x012B5C04 in a derived table), called through that slot by the landed
//   MixMono16Ramped 0x00C58210. The catalog's gapscan name is a placeholder.
// Shape: the indexed form (`dst[i] += (src[i] * gain) >> 15`, as the landed MixAdd16 0x00C580C0) gives retail's pointers
//   biased by two elements; the bumped-pointer form `*dst++` does not; VC7.1 unrolls the 128-step loop by 4 itself; the gain stays a 32-bit register sum zero-extended at each use.
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
    long*         m_pChannel[6];        // +0x04 planar 128-sample accumulators
    long          m_active;             // +0x1c (1 after construction)
    unsigned long m_numChannels;        // +0x20
    unsigned char m_storage[0xc10];     // +0x24 accumulator store, 16-byte aligned by hand
};

void CMixBuffer::MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step)
{
    for (int i = 0; i < 128; ++i) {
        dst[i] += (src[i] * gain) >> 15;
        gain += step;
    }
}