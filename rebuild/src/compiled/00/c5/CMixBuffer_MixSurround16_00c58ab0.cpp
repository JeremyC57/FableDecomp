// CMixBuffer::MixSurround16(const short* src, const unsigned short* gains) -- add a 5.1 source (six planar 128-sample 16-bit blocks: front left +0, front right +128, centre +256, LFE +384, rear left +512, rear right +640 samples) under per-speaker Q15 gains (gains: FL, FR, RL, RR, C, LFE) into the accumulators: a mono mix folds every speaker but the LFE into channel 0 (quarter gain, half for the centre); stereo puts the left / right speakers into channels 0 / 1 at half gain and the centre into both at quarter gain; quad does the same at full / half gain with the rear pair in channels 2 / 3; six channels map one to one
// Leaf: ?MixSurround16@CMixBuffer@@QAEXPBFPBG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C58AB0, 555 bytes, thiscall `ret 8`.
// Identity (descriptive; GameCODA has no PDB or RTTI): the six-plane member of the landed CMixBuffer routing family
//   (MixStereo16 0x00C58370, the four-plane RouteShort4 0x00C58740, MixQuad16Ramped 0x00C58880): branches on m_numChannels (+0x20)
//   1 / 2 / 4 / other and adds through virtual slot 3 MixAdd16 0x00C580C0. Its only caller is the software stream voice's mix
//   step 0x00C5C7E0. The catalog's "AudioFormat_DecodeMultiChannel_Shifted" is a propagated description.
// Shape: the stereo / quad / 5.1 arms end with the same rear-pair calls, which VC7.1 cross-jumps into one tail.
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
    void MixSurround16(const short* src, const unsigned short* gains);
};

void CMixBuffer::MixSurround16(const short* src, const unsigned short* gains)
{
    if (m_numChannels == 1) {
        MixAdd16(src, m_pChannel[0], gains[0] >> 2);
        MixAdd16(src + 128, m_pChannel[0], gains[1] >> 2);
        MixAdd16(src + 256, m_pChannel[0], gains[4] >> 1);
        MixAdd16(src + 512, m_pChannel[0], gains[2] >> 2);
        MixAdd16(src + 640, m_pChannel[0], gains[3] >> 2);
    } else if (m_numChannels == 2) {
        MixAdd16(src, m_pChannel[0], gains[0] >> 1);
        MixAdd16(src + 128, m_pChannel[1], gains[1] >> 1);
        MixAdd16(src + 256, m_pChannel[0], gains[4] >> 2);
        MixAdd16(src + 256, m_pChannel[1], gains[4] >> 2);
        MixAdd16(src + 512, m_pChannel[0], gains[2] >> 1);
        MixAdd16(src + 640, m_pChannel[1], gains[3] >> 1);
    } else if (m_numChannels == 4) {
        MixAdd16(src, m_pChannel[0], gains[0]);
        MixAdd16(src + 128, m_pChannel[1], gains[1]);
        MixAdd16(src + 256, m_pChannel[0], gains[4] >> 1);
        MixAdd16(src + 256, m_pChannel[1], gains[4] >> 1);
        MixAdd16(src + 512, m_pChannel[2], gains[2]);
        MixAdd16(src + 640, m_pChannel[3], gains[3]);
    } else {
        MixAdd16(src, m_pChannel[0], gains[0]);
        MixAdd16(src + 128, m_pChannel[1], gains[1]);
        MixAdd16(src + 256, m_pChannel[4], gains[4]);
        MixAdd16(src + 384, m_pChannel[5], gains[5]);
        MixAdd16(src + 512, m_pChannel[2], gains[2]);
        MixAdd16(src + 640, m_pChannel[3], gains[3]);
    }
}