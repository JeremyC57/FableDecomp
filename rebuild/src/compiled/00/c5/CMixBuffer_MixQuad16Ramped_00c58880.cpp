// CMixBuffer::MixQuad16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps) -- add a four-channel source (four planar 128-sample 16-bit blocks at src, src + 128, + 256, + 384: front left, front right, rear left, rear right) under per-plane ramped Q15 gains: all four into channel 0 at quarter gain and step for a mono mix, left planes into channel 0 and right planes into channel 1 at half gain and step for a stereo mix, else plane i into channel i
// Leaf: ?MixQuad16Ramped@CMixBuffer@@QAEXPBFPBG1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C58880, 545 bytes, thiscall `ret 0Ch`.
// Identity (descriptive; GameCODA has no PDB or RTTI): the four-channel member of the landed CMixBuffer routing family
//   (MixMono16Ramped 0x00C58210, MixStereo16Ramped 0x00C58420; the constant-gain four-plane twin RouteShort4 0x00C58740 is
//   landed under the descriptive CUnprovenAudioBuffer_00C591F0, the same class): branches on m_numChannels (+0x20), adds through
//   virtual slot 9 MixAdd16Ramp 0x00C58140. Its only caller is the software stream voice's mix step 0x00C5C67C. The catalog's
//   "AudioFormat_DecodeWithDeltas_Shifted" is a propagated description.
// Shape: as MixStereo16Ramped: the reduced arms shift the gain in 16 bits (`shr ax,2` / `shr ax,1`) and divide the step as a
//   signed word (`cdq ; and edx,3 ; add ; sar 2` / `cdq ; sub ; sar 1`); the full arm passes the step words zero-extended.
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
    void MixQuad16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps);
};

void CMixBuffer::MixQuad16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps)
{
    if (m_numChannels == 1) {
        MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 2, (short)steps[0] / 4);
        MixAdd16Ramp(src + 128, m_pChannel[0], gains[1] >> 2, (short)steps[1] / 4);
        MixAdd16Ramp(src + 256, m_pChannel[0], gains[2] >> 2, (short)steps[2] / 4);
        MixAdd16Ramp(src + 384, m_pChannel[0], gains[3] >> 2, (short)steps[3] / 4);
    } else if (m_numChannels == 2) {
        MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 1, (short)steps[0] / 2);
        MixAdd16Ramp(src + 128, m_pChannel[1], gains[1] >> 1, (short)steps[1] / 2);
        MixAdd16Ramp(src + 256, m_pChannel[0], gains[2] >> 1, (short)steps[2] / 2);
        MixAdd16Ramp(src + 384, m_pChannel[1], gains[3] >> 1, (short)steps[3] / 2);
    } else {
        MixAdd16Ramp(src, m_pChannel[0], gains[0], steps[0]);
        MixAdd16Ramp(src + 128, m_pChannel[1], gains[1], steps[1]);
        MixAdd16Ramp(src + 256, m_pChannel[2], gains[2], steps[2]);
        MixAdd16Ramp(src + 384, m_pChannel[3], gains[3], steps[3]);
    }
}