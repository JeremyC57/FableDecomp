// CMixBuffer::MixMono16Ramped(const short* src, const unsigned short* gains, const short* steps) -- add one mono block of 16-bit samples into every channel accumulator under that channel's ramped Q15 gain (start gain and per-sample step per channel)
// Leaf: ?MixMono16Ramped@CMixBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C58210, length 85 bytes (+ int3 padding); `ret 0Ch`.
// Callees: virtual slot 9 (+0x24) MixAdd16Ramp (0x00C58140). Caller: 0x00C5C470.
// Label: the manifest's "AudioFormat_DecodeSamples" is propagated; the class is the mixer accumulator of 0x00C591F0
// (twin of the landed un-ramped loop 0x00C581D0).
// Shape: the unsigned count is re-read each pass; the gain array walks in edi, the step array is addressed as
// `steps - gains` homed in the dead `steps` argument slot, and the channel pointer walks in ebx. `unsigned short`
// gains push `xor ecx,ecx ; mov cx,[edi]`, `short` steps push `movsx` (a `long` step parameter gives the same bytes).
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

void CMixBuffer::MixMono16Ramped(const short* src, const unsigned short* gains, const short* steps)
{
    for (unsigned long i = 0; i < m_numChannels; ++i)
        MixAdd16Ramp(src, m_pChannel[i], gains[i], steps[i]);
}