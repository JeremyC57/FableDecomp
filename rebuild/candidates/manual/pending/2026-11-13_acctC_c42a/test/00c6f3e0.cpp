// Behaviour test for CVtable_012b8aa0::Process(const short*, long* const*) 0x00C6F3E0 -- the MMX multi-tap delay effect, modelled in
// portable C++: the 128-sample block is copied into the aligned history; with the cross-fade flag set the current taps ramp down
// (gain, gain - step, ...), the tap sets swap and the new taps ramp up from 0; otherwise every tap mixes at its constant gain;
// outputs 2 and 3 only when the master has four or more channels; the last 64 history bytes move in front of the buffer.
#include <cstdio>
#include <cstring>
struct SDelayTap { long m_delay; unsigned short m_gain; short m_step; };
struct SRecord { SDelayTap m_taps[6]; };
static unsigned long g_outputChannels = 2;
static void Ramp(const short* src, unsigned long count, long* dst, short initial, short step)   // CUnprovenMixKernel_00c6f1a0
{
    short gains[4];
    for (int i = 0; i < 4; ++i) gains[i] = (short)(initial + i * step);
    for (unsigned long block = 0; block < count * 2; ++block) {
        for (int lane = 0; lane < 4; ++lane) { int high = ((int)src[block * 4 + lane] * gains[lane]) >> 16; dst[block * 4 + lane] += high * 2; }
        for (int lane = 0; lane < 4; ++lane) gains[lane] = (short)(gains[lane] + step * 4);
    }
}
static void Scale(const short* src, unsigned long count, long* dst, short gain)                 // ScaleAccumulate16MMX
{
    for (unsigned long i = 0; i < count * 8; ++i) { int high = ((int)src[i] * gain) >> 16; dst[i] += high * 2; }
}
struct CDelay {
    unsigned char storage[0x40 + 0x100];
    unsigned char* m_pAlignedBuffer;
    SRecord m_first, m_second;
    SRecord* m_pCurrent; SRecord* m_pPrevious;
    bool m_crossfade;
    CDelay() { std::memset(this, 0, sizeof(*this)); m_pAlignedBuffer = storage + 0x40; m_pCurrent = &m_first; m_pPrevious = &m_second; }
    static void Up(const short* s, const SDelayTap* t, long* a) { if (t->m_step != 0) Ramp(s - t->m_delay, 16, a, 0, t->m_step); }
    static void Down(const short* s, const SDelayTap* t, long* a) { if (t->m_gain != 0) Ramp(s - t->m_delay, 16, a, (short)t->m_gain, (short)-t->m_step); }
    static void Mix(const short* s, unsigned long n, const SDelayTap* t, long* a) { if (t->m_gain != 0) Scale(s - t->m_delay, n, a, (short)t->m_gain); }
    void Process(const short* input, long* const* outputs)
    {
        std::memcpy(m_pAlignedBuffer, input, 128 * sizeof(short));
        const short* h = (const short*)m_pAlignedBuffer;
        int n = g_outputChannels >= 4 ? 4 : 2, i;
        if (m_crossfade) {
            m_crossfade = false;
            for (i = 0; i < n; ++i) Down(h, &m_pCurrent->m_taps[i], outputs[i]);
            SRecord* current = m_pCurrent; m_pCurrent = m_pPrevious; m_pPrevious = current;
            for (i = 0; i < n; ++i) Up(h, &m_pCurrent->m_taps[i], outputs[i]);
        } else {
            for (i = 0; i < n; ++i) Mix(h, 16, &m_pCurrent->m_taps[i], outputs[i]);
        }
        std::memmove(m_pAlignedBuffer - 0x40, m_pAlignedBuffer + 0xc0, 0x40);
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main()
{
    short in[128]; long o0[128], o1[128], o2[128], o3[128]; long* outs[4] = { o0, o1, o2, o3 };
    int i;
    for (i = 0; i < 128; ++i) in[i] = (short)(i * 256 - 16000);
    CDelay d;
    d.m_first.m_taps[0].m_gain = 0x4000; d.m_first.m_taps[0].m_delay = 0;      // half gain, no delay
    d.m_first.m_taps[1].m_gain = 0;                                             // muted tap: output untouched
    d.m_first.m_taps[2].m_gain = 0x2000; d.m_first.m_taps[2].m_delay = 8;
    std::memset(o0, 0, sizeof o0); std::memset(o1, 0, sizeof o1); std::memset(o2, 0, sizeof o2); std::memset(o3, 0, sizeof o3);
    d.Process(in, outs);
    check(o0[5] == ((in[5] * 0x4000) >> 16) * 2 && o0[127] == ((in[127] * 0x4000) >> 16) * 2, "constant gain mixes the block");
    check(o1[0] == 0 && o1[127] == 0, "a zero gain tap mixes nothing");
    check(o2[40] == 0, "two channels: outputs 2 and 3 untouched");
    check(std::memcmp(d.storage, in + 96, 0x40) == 0, "the last 64 bytes move in front of the buffer");
    check(d.m_pCurrent == &d.m_first, "no swap without the cross-fade flag");
    // cross-fade with four channels: old set ramps down from its gain, the sets swap, the new set ramps up from 0
    g_outputChannels = 4;
    d.m_first.m_taps[0].m_step = 0x40;
    d.m_second.m_taps[0].m_step = 0x20; d.m_second.m_taps[0].m_gain = 0x1000;
    d.m_second.m_taps[3].m_step = 0x10;
    d.m_crossfade = true;
    std::memset(o0, 0, sizeof o0); std::memset(o2, 0, sizeof o2); std::memset(o3, 0, sizeof o3);
    d.Process(in, outs);
    long want0 = ((in[3] * (short)(0x4000 - 3 * 0x40)) >> 16) * 2 + ((in[3] * (short)(3 * 0x20)) >> 16) * 2;
    check(o0[3] == want0, "ramp down of the old tap plus ramp up of the new one");
    check(o3[9] == ((in[9] * (short)(9 * 0x10)) >> 16) * 2, "four channels: output 3 ramps up");
    check(o2[20] == ((d.storage[0] | 1) ? (((short)((const short*)d.m_pAlignedBuffer)[20 - 8] * (short)(0x2000 - 0)) >> 16) * 2 : 0), "output 2 ramps the delayed old tap down with step 0");
    check(d.m_pCurrent == &d.m_second && d.m_pPrevious == &d.m_first && !d.m_crossfade, "tap sets swapped, flag cleared");
    if (fails) { std::printf("00c6f3e0_TEST FAIL\n"); return 1; }
    std::printf("00c6f3e0_TEST PASS\n");
    return 0;
}
