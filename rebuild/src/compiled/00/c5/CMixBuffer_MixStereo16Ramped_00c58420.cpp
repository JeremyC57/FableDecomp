// CMixBuffer::MixStereo16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps) -- add a stereo pair of 128-sample 16-bit blocks (left at src, right at src + 128) under per-channel ramped Q15 gains: into channel 0 at half gain and half step for a mono mix, else left/right into channels 0/1 and, with four or more channels, into 2/3
// Leaf: ?MixStereo16Ramped@CMixBuffer@@QAEXPBFPBG1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C58420, 244 bytes, thiscall `ret 0Ch`.
// Identity (descriptive; GameCODA has no PDB or RTTI): the ramped twin of the landed CMixBuffer::MixStereo16 0x00C58370
//   (same channel routing, slot 9 MixAdd16Ramp 0x00C58140 in place of slot 3 MixAdd16), as MixMono16Ramped 0x00C58210 is
//   of MixMono16 0x00C581D0. Its one caller is 0x00C5C576 (the software voice's render step). The catalog's
//   `AudioFormat_DecodeSamplesWithDeltas` is a propagated description.
// Shape: the mono arm halves the gain with the 16-bit `shr ax,1` and the step with a signed `/ 2` (`cdq ; sub ; sar`),
//   so the step words are read as signed there and zero-extended in the stereo arms (unsigned step words).
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
    void MixStereo16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps);
};

void CMixBuffer::MixStereo16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps)
{
    if (m_numChannels == 1) {
        MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 1, (short)steps[0] / 2);
        MixAdd16Ramp(src + 128, m_pChannel[0], gains[1] >> 1, (short)steps[1] / 2);
        return;
    }
    MixAdd16Ramp(src, m_pChannel[0], gains[0], steps[0]);
    MixAdd16Ramp(src + 128, m_pChannel[1], gains[1], steps[1]);
    if (m_numChannels >= 4) {
        MixAdd16Ramp(src, m_pChannel[2], gains[2], steps[2]);
        MixAdd16Ramp(src + 128, m_pChannel[3], gains[3], steps[3]);
    }
}