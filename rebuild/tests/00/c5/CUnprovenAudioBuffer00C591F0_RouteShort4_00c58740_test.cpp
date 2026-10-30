// Behaviour test for 0x00C58740 RouteShort4(source, gains): four 128-sample short planes mixed through MixShortGain --
// mono: all into buffer 0 with gain >> 2; stereo: planes 0/2 into buffer 0 and 1/3 into buffer 1 with gain >> 1;
// otherwise plane i into buffer i at full gain.
#include <cstdio>

struct Mixer {
    int* Buffers[6]; unsigned int ChannelCount;
    void MixShortGain(const short* src, int* dst, unsigned short gain) { for (int i = 0; i < 128; ++i) dst[i] += src[i] * gain; }
    void RouteShort4(const short* source, const unsigned short* gains)
    {
        if (ChannelCount == 1) {
            for (int p = 0; p < 4; ++p) MixShortGain(source + 128 * p, Buffers[0], gains[p] >> 2);
        } else if (ChannelCount == 2) {
            for (int p = 0; p < 4; ++p) MixShortGain(source + 128 * p, Buffers[p & 1], gains[p] >> 1);
        } else {
            for (int p = 0; p < 4; ++p) MixShortGain(source + 128 * p, Buffers[p], gains[p]);
        }
    }
};

static int fail() { std::printf("00c58740_TEST FAIL\n"); return 1; }

int main()
{
    static short src[512]; static int buf[4][128];
    for (int i = 0; i < 512; ++i) src[i] = (short)((i / 128) + 1);     // plane p holds p+1
    unsigned short gains[4] = { 8, 16, 32, 64 };
    Mixer m; for (int b = 0; b < 4; ++b) m.Buffers[b] = buf[b]; m.Buffers[4] = m.Buffers[5] = 0;
    m.ChannelCount = 1; m.RouteShort4(src, gains);
    if (buf[0][5] != 1 * 2 + 2 * 4 + 3 * 8 + 4 * 16 || buf[1][5] != 0) return fail();
    for (int b = 0; b < 4; ++b) for (int i = 0; i < 128; ++i) buf[b][i] = 0;
    m.ChannelCount = 2; m.RouteShort4(src, gains);
    if (buf[0][127] != 1 * 4 + 3 * 16 || buf[1][0] != 2 * 8 + 4 * 32 || buf[2][0] != 0) return fail();
    for (int b = 0; b < 4; ++b) for (int i = 0; i < 128; ++i) buf[b][i] = 0;
    m.ChannelCount = 4; m.RouteShort4(src, gains);
    if (buf[0][0] != 8 || buf[1][0] != 32 || buf[2][0] != 96 || buf[3][0] != 256) return fail();
    std::printf("00c58740_TEST PASS\n");
    return 0;
}