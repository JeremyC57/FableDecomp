// CMixBuffer::CMixBuffer() -- constructor of the GameCODA software mixer's block accumulator: vtable, active flag 1, no channels yet, and the six planar 128-sample accumulator pointers carved from the inline store rounded up to 16 bytes
// Leaf: ??0CMixBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C591F0, length 77 bytes (+ int3 padding); `ret`, returns `this`.
// Caller: the output device constructor 0x00C3EFA0 (its out-of-line base, then vtable 0x012B5BE0 and six limiters).
// Label: the manifest's "CArenaRoundDef_InitializeBuffers" is propagated; GameCODA has no PDB/RTTI, names are
// descriptive (named from the kernels in the vtable and the 0x200-byte = 128-long channel stride).
// Shape: `lea ecx,[eax+33h] ; and ecx,-16` is `(m_storage + 15) & ~15`; the six stores come from a counted
// `for (i < 6) m_pChannel[i] = p + 128 * i` loop that VC7.1 unrolls and schedules (+8/+0xC/+0x10 before +4); six
// explicit assignments keep source order and differ at byte 2.
// The GameCODA software mixer's block accumulator (vtable 0x012B8F38, no RTTI; ctor 0x00C591F0): up to six planar
// 128-sample `long` accumulators (0x200 bytes each) carved out of an inline 16-byte-aligned store (+0x24, 0xC10
// bytes), the channel count at +0x20. The virtual slots are the add-into-accumulator kernels, named from their
// bodies: slot 2 0x00C57F00 clears every channel (`m_numChannels << 9` bytes), 3 0x00C580C0 adds 16-bit samples
// scaled by a Q15 gain, 4 0x00C58040 / 5 0x00C58000 add 32-bit samples with / without a Q15 gain, 6 0x00C57FC0 / 7
// 0x00C57F60 / 8 0x00C57F20 add float samples with a float gain / a Q15 gain / unscaled, 9 0x00C58140 adds 16-bit
// samples under a ramped Q15 gain (gain += step per sample).
// The kernels have distinct names on purpose: VC7.1 lays out OVERLOADED virtuals in reverse declaration order, so
// `MixAddFloat(src, dst, float)` / `MixAddFloat(src, dst)` overloads swap slots 6 and 8.
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
    CMixBuffer();
    void MixMono16Ramped(const short* src, const unsigned short* gains, const short* steps);
    void MixStereoFloat(const float* src);
    void MixStereo32(const long* src);
    void MixStereo16(const short* src, const unsigned short* gains);
};

CMixBuffer::CMixBuffer() : m_active(1), m_numChannels(0)
{
    long* p = (long*)(((unsigned long)m_storage + 15) & ~15);
    for (int i = 0; i < 6; ++i)
        m_pChannel[i] = p + 128 * i;
}