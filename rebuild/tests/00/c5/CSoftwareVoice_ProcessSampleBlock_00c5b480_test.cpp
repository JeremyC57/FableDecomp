// Behaviour test for CSoftwareVoice::ProcessSampleBlock (0x00C5B480): unity step copies the read frames straight into the mix
// ring; a half-speed step reads ((step * frames + phase) >> 12) + 2 frames, interpolates, and carries the unread frames over to
// the next block; a silent (not playing) block zero-fills; the return value is the number of frames written.
#include <cstdio>
#include <cstring>
struct Ring {
    short data[6 * 64]; unsigned long capacity, frameBytes, readPos, writePos, queued;
    unsigned char* pData() { return (unsigned char*)data; }
    void Init(unsigned long cap) { std::memset(data, 0, sizeof data); capacity = cap; frameBytes = 2; readPos = writePos = queued = 0; }
    void CommitWrite(unsigned long n) { writePos += n; queued += n; }
    void CommitRead(unsigned long n) { readPos += n; queued -= n; }
    void Flush() { readPos = writePos = queued = 0; }
    void ClearChannel(unsigned long ch, unsigned long n) { std::memset(data + capacity * ch + writePos, 0, n * 2); }
    unsigned char* ReadPointer() { return pData() + readPos * frameBytes; }
};
static Ring g_mixRing, g_monoRing;
static short g_next = 1;
struct Voice {
    unsigned long m_numChannels, m_queued, m_carryFrames, m_step, m_phase; short m_carry[6][16];
    void ReadChannel(unsigned long frames) { for (unsigned long i = 0; i < frames; ++i) g_monoRing.data[g_monoRing.writePos + i] = g_next++; g_monoRing.CommitWrite(frames); }
    void Resample(unsigned long frames, unsigned long channel)
    {
        short* dst = g_mixRing.data + g_mixRing.writePos + g_mixRing.capacity * channel;
        unsigned long pos = m_phase; short* src = (short*)g_monoRing.ReadPointer();
        for (unsigned long i = 0; i < frames; ++i) {
            unsigned long frac = pos & 0xfff, index = pos >> 12; pos += m_step;
            dst[i] = (short)((src[index] * (0x1000 - frac) + src[index + 1] * frac) >> 12);
        }
        m_phase = pos & 0xfff; g_monoRing.CommitRead(pos >> 12);
    }
    unsigned long ProcessSampleBlock(bool playing, unsigned long frames)
    {
        bool resampling = m_step != 0x1000 || m_phase != 0;
        unsigned long step = m_step, phase = m_phase, needed = frames;
        if (!playing) needed = m_carryFrames;
        if (resampling) { needed = ((m_step * needed + m_phase) >> 12) + 2; if (needed > g_monoRing.capacity) needed = g_monoRing.capacity; }
        if (playing && needed > m_carryFrames + m_queued) needed = m_carryFrames + m_queued;
        unsigned long carried = 0;
        for (unsigned long channel = 0; channel < m_numChannels; ++channel) {
            if (m_carryFrames) { std::memcpy(g_monoRing.data, m_carry[channel], m_carryFrames * 2); g_monoRing.CommitWrite(m_carryFrames); }
            if (needed > m_carryFrames) {
                if (playing) ReadChannel(needed - m_carryFrames);
                else { g_monoRing.ClearChannel(0, needed - m_carryFrames); g_monoRing.CommitWrite(needed - m_carryFrames); }
            }
            if (needed > g_monoRing.queued) needed = g_monoRing.queued;
            if (resampling) {
                m_phase = phase; m_step = step;
                unsigned long most = (((needed - 1) << 12) - phase) / step;
                if (most < frames) frames = most;
                Resample(frames, channel);
            } else {
                short* dst = g_mixRing.data + g_mixRing.writePos + g_mixRing.capacity * channel;
                if (frames > needed) frames = needed;
                std::memcpy(dst, g_monoRing.ReadPointer(), g_monoRing.frameBytes * frames);
                g_monoRing.CommitRead(frames);
            }
            unsigned long left = g_monoRing.queued;
            if (!g_monoRing.pData() || left) {
                if (playing) { std::memcpy(m_carry[channel], g_monoRing.ReadPointer(), left * 2); carried = left; }
                g_monoRing.Flush();
            }
        }
        m_carryFrames = carried; g_mixRing.CommitWrite(frames);
        return frames;
    }
};
int main()
{
    bool ok = true;
    Voice v; std::memset(&v, 0, sizeof v);
    // unity, mono
    g_mixRing.Init(64); g_monoRing.Init(64); v.m_numChannels = 1; v.m_queued = 100; v.m_step = 0x1000; v.m_phase = 0; g_next = 1;
    ok = ok && v.ProcessSampleBlock(true, 8) == 8;
    for (int i = 0; i < 8; ++i) ok = ok && g_mixRing.data[i] == i + 1;
    ok = ok && v.m_carryFrames == 0 && g_mixRing.queued == 8;
    // half speed: reads 6 frames, writes 8 interpolated, carries 2 over
    g_mixRing.Init(64); g_monoRing.Init(64); v.m_step = 0x800; v.m_phase = 0; g_next = 10;
    ok = ok && v.ProcessSampleBlock(true, 8) == 8;
    ok = ok && g_mixRing.data[0] == 10 && g_mixRing.data[1] == 10 && g_mixRing.data[2] == 11 && g_mixRing.data[7] == 13;
    ok = ok && v.m_carryFrames == 2 && v.m_carry[0][0] == 14 && v.m_carry[0][1] == 15 && v.m_phase == 0;
    // the next block starts from the carried frames
    g_mixRing.Init(64); g_next = 16;
    ok = ok && v.ProcessSampleBlock(true, 4) == 4;
    ok = ok && g_mixRing.data[0] == 14 && g_mixRing.data[2] == 15;
    // silent block, unity, stereo: zero-filled, nothing carried
    g_mixRing.Init(64); g_monoRing.Init(64); v.m_numChannels = 2; v.m_step = 0x1000; v.m_phase = 0; v.m_carryFrames = 0;
    for (int i = 0; i < 128; ++i) g_mixRing.data[i] = 99;
    ok = ok && v.ProcessSampleBlock(false, 8) == 0;
    ok = ok && v.m_carryFrames == 0 && g_mixRing.data[0] == 99;
    if (!ok) { std::printf("00c5b480_TEST FAIL\n"); return 1; }
    std::printf("00c5b480_TEST PASS\n");
    return 0;
}