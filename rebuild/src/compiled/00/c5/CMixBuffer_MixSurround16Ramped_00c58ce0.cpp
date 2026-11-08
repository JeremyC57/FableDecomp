// CMixBuffer::MixSurround16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps) -- add a 5.1 source (six planar 128-sample 16-bit blocks: front left, front right, centre, LFE, rear left, rear right) under per-speaker ramped Q15 gains (gains / steps: FL, FR, RL, RR, C, LFE) into the accumulators, with the routing of MixSurround16: mono folds every speaker but the LFE into channel 0 (quarter gain and step, half for the centre), stereo puts left / right into channels 0 / 1 at half and the centre into both at quarter, quad the fronts at full gain, the centre at half gain and step, and the rear pair (channels 2 / 3) at full gain but half step (retail's arithmetic, kept as is), six channels one to one
// Leaf: ?MixSurround16Ramped@CMixBuffer@@QAEXPBFPBG1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C58CE0, 1010 bytes, thiscall `ret 0Ch`.
// Identity (descriptive; GameCODA has no PDB or RTTI): the ramped twin of MixSurround16 0x00C58AB0 (as MixStereo16Ramped
//   0x00C58420 is of MixStereo16 0x00C58370), adding through virtual slot 9 MixAdd16Ramp 0x00C58140; its only caller is the
//   software stream voice's mix step 0x00C5C7CE. The catalog's "AudioFormat_DecodeMultiChannelWithDeltas" is a propagated
//   description.
// Shape: the reduced arms shift the gain in 16 bits and divide the step as a signed word; the full arms pass the step words
//   zero-extended (unsigned step words, as MixStereo16Ramped).
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
    void MixSurround16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps);
};

void CMixBuffer::MixSurround16Ramped(const short* src, const unsigned short* gains, const unsigned short* steps)
{
    if (m_numChannels == 1) {
        MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 2, (short)steps[0] / 4);
        MixAdd16Ramp(src + 128, m_pChannel[0], gains[1] >> 2, (short)steps[1] / 4);
        MixAdd16Ramp(src + 256, m_pChannel[0], gains[4] >> 1, (short)steps[4] / 2);
        MixAdd16Ramp(src + 512, m_pChannel[0], gains[2] >> 2, (short)steps[2] / 4);
        MixAdd16Ramp(src + 640, m_pChannel[0], gains[3] >> 2, (short)steps[3] / 4);
    } else if (m_numChannels == 2) {
        MixAdd16Ramp(src, m_pChannel[0], gains[0] >> 1, (short)steps[0] / 2);
        MixAdd16Ramp(src + 128, m_pChannel[1], gains[1] >> 1, (short)steps[1] / 2);
        MixAdd16Ramp(src + 256, m_pChannel[0], gains[4] >> 2, (short)steps[4] / 4);
        MixAdd16Ramp(src + 256, m_pChannel[1], gains[4] >> 2, (short)steps[4] / 4);
        MixAdd16Ramp(src + 512, m_pChannel[0], gains[2] >> 1, (short)steps[2] / 2);
        MixAdd16Ramp(src + 640, m_pChannel[1], gains[3] >> 1, (short)steps[3] / 2);
    } else if (m_numChannels == 4) {
        MixAdd16Ramp(src, m_pChannel[0], gains[0], steps[0]);
        MixAdd16Ramp(src + 128, m_pChannel[1], gains[1], steps[1]);
        MixAdd16Ramp(src + 256, m_pChannel[0], gains[4] >> 1, (short)steps[4] / 2);
        MixAdd16Ramp(src + 256, m_pChannel[1], gains[4] >> 1, (short)steps[4] / 2);
        MixAdd16Ramp(src + 512, m_pChannel[2], gains[2], (short)steps[2] / 2);
        MixAdd16Ramp(src + 640, m_pChannel[3], gains[3], (short)steps[3] / 2);
    } else {
        MixAdd16Ramp(src, m_pChannel[0], gains[0], steps[0]);
        MixAdd16Ramp(src + 128, m_pChannel[1], gains[1], steps[1]);
        MixAdd16Ramp(src + 256, m_pChannel[4], gains[4], steps[4]);
        MixAdd16Ramp(src + 384, m_pChannel[5], gains[5], steps[5]);
        MixAdd16Ramp(src + 512, m_pChannel[2], gains[2], steps[2]);
        MixAdd16Ramp(src + 640, m_pChannel[3], gains[3], steps[3]);
    }
}