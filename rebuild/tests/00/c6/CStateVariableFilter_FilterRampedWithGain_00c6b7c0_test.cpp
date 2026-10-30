// Behaviour test for 0x00C6B7C0 CStateVariableFilter::FilterRampedWithGain (see src/00c6b7c0.cpp): ramped SVF with gain output.
#include <cstdio>
#include <cmath>
struct Svf { float f, d, g, tf, td, tg, band, low; unsigned long pos; };
static long Trunc(float v) { return (long)v; }  /* round(v - 0.5), negatives stepped up: truncation toward zero */
static void Run(Svf* s, short* x, unsigned long n, unsigned long block)
{
    for (unsigned long i = 0; i < n; ++i) {
        float t = (float)(s->pos + i) / (float)block, in = (float)x[i], o = 1.0f - t;
        float f = t * s->tf + o * s->f, d = o * s->d + t * s->td;
        float high = (in - s->low) - d * s->band;
        s->band = high * f + s->band; s->low = s->band * f + s->low;
        float g = o * s->g + t * s->tg;
        long out = Trunc(g * high * d + in);
        if (out > 0x7fff) out = 0x7fff; else if (out < -0x8000) out = -0x8000;
        x[i] = (short)out;
    }
}
int main()
{
    Svf s = { 0.5f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0 };
    short x[4] = { 100, -100, 32767, -32768 };
    Run(&s, x, 4, 128);          /* zero gain: output is the input */
    if (x[0] != 100 || x[1] != -100 || x[2] != 32767 || x[3] != -32768) { std::printf("00c6b7c0_TEST FAIL\n"); return 1; }
    Svf h = { 0.5f, 1.0f, 2.0f, 0.5f, 1.0f, 2.0f, 0.0f, 0.0f, 0 };
    short y[1] = { 1000 };
    Run(&h, y, 1, 128);          /* high = 1000, out = 2*1000*1 + 1000 = 3000 */
    if (y[0] != 3000 || h.band != 500.0f || h.low != 250.0f) { std::printf("00c6b7c0_TEST FAIL\n"); return 1; }
    std::printf("00c6b7c0_TEST PASS\n"); return 0;
}