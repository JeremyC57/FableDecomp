// Behaviour test for the libpk SSE2 four-record gain mixer Process (00c6e410): under the lock, a cross-fade ramps each current record
// down (start gain, -step), swaps current / previous and ramps the new records up (0, +step); otherwise each record mixes at
// its constant Q15 gain. Outputs 2-3 are only touched with four or more output channels. The kernels are modelled in C
// (per-lane ramp of 8 lanes, high-word product doubled = (s*g)>>15 rounded down).
#include <cstdio>
struct GainRamp { unsigned short gain; short step; };
static int Mul(short s, int g) { int p = (int)s * (short)g; int hi = p >> 16; return hi * 2; }
static void Ramp(const short* src, unsigned long count, long* dst, short initial, short step)
{
    int lanes[8];
    for (int i = 0; i < 8; ++i) lanes[i] = (short)(initial + i * step);
    for (unsigned long b = 0; b < count * (8 / 8); ++b) {
        for (int i = 0; i < 8; ++i) dst[b * 8 + i] += Mul(src[b * 8 + i], lanes[i]);
        for (int i = 0; i < 8; ++i) { int n = lanes[i] + (short)(step * 8); lanes[i] = n > 32767 ? 32767 : (n < -32768 ? -32768 : n); }
    }
}
static void Constant(const short* src, unsigned long count, long* dst, short gain)
{
    for (unsigned long i = 0; i < count * 8; ++i) dst[i] += Mul(src[i], gain);
}
struct Mixer {
    bool crossfade; GainRamp first[6]; GainRamp second[6]; GainRamp* current; GainRamp* previous; unsigned long channels; int locks;
    void Process(const short* input, long* const* outputs)
    {
        ++locks;
        int n = channels >= 4 ? 4 : 2;
        if (crossfade) {
            crossfade = false;
            for (int k = 0; k < n; ++k) if (current[k].gain) Ramp(input, 16, outputs[k], current[k].gain, -current[k].step);
            GainRamp* c = current; current = previous; previous = c;
            for (int k = 0; k < n; ++k) if (current[k].step) Ramp(input, 16, outputs[k], 0, current[k].step);
        } else {
            for (int k = 0; k < n; ++k) if (current[k].gain) Constant(input, 16, outputs[k], current[k].gain);
        }
        --locks;
    }
};
int main()
{
    static short input[128]; static long out[4][128];
    for (int i = 0; i < 128; ++i) input[i] = 1000;
    long* outputs[4] = { out[0], out[1], out[2], out[3] };
    Mixer m = { false };
    for (int k = 0; k < 6; ++k) { m.first[k].gain = 16384; m.first[k].step = 0; m.second[k].gain = 0; m.second[k].step = 128; }
    m.current = m.first; m.previous = m.second; m.channels = 2; m.locks = 0;
    m.Process(input, outputs);
    if (out[0][0] != 500 || out[1][127] != 500 || out[2][0] != 0 || m.locks != 0) { std::printf("00c6e410_TEST FAIL\n"); return 1; }
    m.channels = 4; m.crossfade = true;
    m.Process(input, outputs);
    if (m.crossfade || m.current != m.second || m.previous != m.first || out[2][0] == 0 || out[3][1] <= out[3][0]) { std::printf("00c6e410_TEST FAIL\n"); return 1; }
    std::printf("00c6e410_TEST PASS\n");
    return 0;
}