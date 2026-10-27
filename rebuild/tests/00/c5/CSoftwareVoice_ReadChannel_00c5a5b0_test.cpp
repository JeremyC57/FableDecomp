// Behaviour test for 0x00C5A5B0 CSoftwareVoice::ReadChannel
#include <cstdio>
#include <cstring>
struct Desc { unsigned int m_state, m_numChannels; };
struct Ring { unsigned char* m_pData; unsigned int m_frameBytes, m_writePos, m_written, m_read;
    void CommitWrite(unsigned int n) { m_written += n; } void CommitRead(unsigned int n) { m_read += n; } };
static short g_mono[256];
static Ring g_monoRing = { (unsigned char*)g_mono, 2, 0, 0, 0 };
struct Voice {
    Desc* m_pDesc;
    void ReadChannel(Ring* source, const unsigned char* data, unsigned int frames, unsigned int channel)
    {
        const Desc* desc = m_pDesc;
        short* dst = (short*)(g_monoRing.m_pData + g_monoRing.m_writePos * g_monoRing.m_frameBytes);
        switch (desc->m_state) {
        case 1: {
            const short* src = (const short*)data + channel;
            if (desc->m_numChannels == 1) std::memcpy(dst, src, frames * 2);
            else for (unsigned int i = 0; i < frames; i++) { dst[i] = *src; src += desc->m_numChannels; }
            break; }
        case 2: {
            const signed char* src = (const signed char*)data + channel;
            if (desc->m_numChannels == 1) for (unsigned int i = 0; i < frames; i++) dst[i] = (short)((src[i] - 0x80) << 8);
            else for (unsigned int i = 0; i < frames; i++) dst[i] = (short)((src[desc->m_numChannels * i] - 0x80) << 8);
            break; }
        }
        g_monoRing.CommitWrite(frames);
        if (channel == desc->m_numChannels - 1) source->CommitRead(frames);
    }
};
static int Fail() { std::printf("00c5a5b0_TEST FAIL\n"); return 1; }
int main()
{
    Ring src = { 0, 0, 0, 0, 0 };
    short st[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    Desc d = { 1, 2 }; Voice v; v.m_pDesc = &d;
    v.ReadChannel(&src, (const unsigned char*)st, 4, 1);         // 16-bit stereo, right channel (the last: commits the read)
    if (g_mono[0] != 2 || g_mono[3] != 8 || g_monoRing.m_written != 4 || src.m_read != 4) return Fail();
    v.ReadChannel(&src, (const unsigned char*)st, 4, 0);         // left channel: no read commit
    if (g_mono[0] != 1 || g_mono[3] != 7 || src.m_read != 4) return Fail();
    d.m_numChannels = 1; v.ReadChannel(&src, (const unsigned char*)st, 3, 0);
    if (g_mono[2] != 3 || src.m_read != 7) return Fail();
    unsigned char b8[4] = { 0x10, 0x20, 0x30, 0x40 }; d.m_state = 2;
    v.ReadChannel(&src, b8, 4, 0);                               // 8-bit mono: (s - 0x80) << 8 on the signed byte
    if (g_mono[0] != (short)((0x10 - 0x80) << 8) || g_mono[3] != (short)((0x40 - 0x80) << 8)) return Fail();
    d.m_numChannels = 2; v.ReadChannel(&src, b8, 2, 1);
    if (g_mono[0] != (short)((0x20 - 0x80) << 8) || g_mono[1] != (short)((0x40 - 0x80) << 8)) return Fail();
    d.m_state = 3; g_mono[0] = 77; v.ReadChannel(&src, b8, 2, 1);
    if (g_mono[0] != 77 || g_monoRing.m_written != 4 + 4 + 3 + 4 + 2 + 2) return Fail();
    std::printf("00c5a5b0_TEST PASS\n");
    return 0;
}