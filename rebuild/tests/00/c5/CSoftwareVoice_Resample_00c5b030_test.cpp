// Behaviour test for CSoftwareVoice::Resample (0x00C5B030): 20.12 linear interpolation from the input ring's read
// cursor into one planar channel of the output ring's write cursor; the phase keeps the fraction and the input
// consumes the whole frames stepped over.
#include <cstdio>
struct CAudioRingBuffer {
    unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    void CommitRead(unsigned int frames)
    {
        m_queued -= frames; m_readPos += frames;
        if (m_queued == 0) { m_readPos = 0; m_writePos = 0; } else if (m_readPos >= m_capacity) m_readPos -= m_capacity;
    }
};
struct CSoftwareVoice {
    unsigned int m_step, m_phase;
    void Resample(CAudioRingBuffer* out, CAudioRingBuffer* in, unsigned int frames, unsigned int channel)
    {
        short* dst = (short*)(out->m_pData + out->m_writePos * out->m_frameBytes + out->m_capacity * channel * 2);
        unsigned int pos = m_phase;
        short* src = (short*)(in->m_pData + in->m_readPos * in->m_frameBytes);
        for (unsigned int i = 0; i < frames; ++i) {
            unsigned int frac = pos & 0xfff, index = pos >> 12;
            pos += m_step;
            dst[i] = (short)((src[index] * (0x1000 - frac) + src[index + 1] * frac) >> 12);
        }
        m_phase = pos & 0xfff;
        in->CommitRead(pos >> 12);
    }
};
int main()
{
    short inData[16]; for (int i = 0; i < 16; ++i) inData[i] = (short)(i * 100);
    short outData[2 * 8] = { 0 };
    CAudioRingBuffer in = { (unsigned char*)inData, 16, 2, 1, 2, 0, 10 };
    CAudioRingBuffer out = { (unsigned char*)outData, 8, 2, 2, 0, 1, 0 };
    CSoftwareVoice v; v.m_step = 0x800; v.m_phase = 0;          // half speed
    v.Resample(&out, &in, 4, 1);
    // src starts at inData[2] = 200: 200, 250, 300, 350 into channel 1 (outData[8 + 1 ..])
    int ok = outData[9] == 200 && outData[10] == 250 && outData[11] == 300 && outData[12] == 350 && outData[1] == 0;
    ok = ok && v.m_phase == 0 && in.m_readPos == 4 && in.m_queued == 8;                 // 4 * 0x800 = 2 frames
    v.m_step = 0x1800; v.m_phase = 0x400;
    v.Resample(&out, &in, 2, 0);                                // from inData[4] = 400
    ok = ok && outData[1] == 425 && outData[2] == 575 && v.m_phase == 0x400 && in.m_readPos == 7;
    if (!ok) { std::printf("00c5b030_TEST FAIL\n"); return 1; }
    std::printf("00c5b030_TEST PASS\n");
    return 0;
}